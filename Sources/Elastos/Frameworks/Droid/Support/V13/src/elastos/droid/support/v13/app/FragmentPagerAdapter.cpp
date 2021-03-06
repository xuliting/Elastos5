//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#include "elastos/droid/support/v13/app/FragmentPagerAdapter.h"
#include "elastos/droid/support/v13/app/FragmentCompat.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Support {
namespace V13 {
namespace App {

const String FragmentPagerAdapter::TAG("FragmentPagerAdapter");
const Boolean FragmentPagerAdapter::DEBUG = TRUE;

ECode FragmentPagerAdapter::StartUpdate(
    /* [in] */ IViewGroup* container)
{
    return NOERROR;
}

ECode FragmentPagerAdapter::InstantiateItem(
    /* [in] */ IViewGroup* container,
    /* [in] */ Int32 position,
    /* [out] */ IInterface** newPage)
{
    VALIDATE_NOT_NULL(newPage);
    if (mCurTransaction == NULL) {
        mFragmentManager->BeginTransaction((IFragmentTransaction**)&mCurTransaction);
    }

    Int64 itemId = GetItemId(position);

    // Do we already have this fragment?
    Int32 viewId;
    IView::Probe(container)->GetId(&viewId);
    String name = MakeFragmentName(viewId, itemId);
    AutoPtr<IFragment> fragment;
    mFragmentManager->FindFragmentByTag(name, (IFragment**)&fragment);
    if (fragment != NULL) {
        if (DEBUG) Logger::V(TAG, "Attaching item #%d: f=%s", itemId, TO_CSTR(fragment));
        mCurTransaction->Attach(fragment);
    }
    else {
        GetItem(position, (IFragment**)&fragment);
        if (DEBUG) Logger::V(TAG, "Adding item #%d: f=%s", itemId, TO_CSTR(fragment));
        IView::Probe(container)->GetId(&viewId);
        mCurTransaction->Add(viewId, fragment,
                MakeFragmentName(viewId, itemId));
    }
    if (fragment != mCurrentPrimaryItem) {
        FragmentCompat::SetMenuVisibility(fragment, FALSE);
        FragmentCompat::SetUserVisibleHint(fragment, FALSE);
    }

    *newPage = fragment;
    REFCOUNT_ADD(*newPage);
    return NOERROR;
}

// @Override
ECode FragmentPagerAdapter::DestroyItem(
    /* [in] */ IViewGroup* container,
    /* [in] */ Int32 position,
    /* [in] */ IInterface* object)
{
    if (mCurTransaction == NULL) {
        mFragmentManager->BeginTransaction((IFragmentTransaction**)&mCurTransaction);
    }
    if (DEBUG) {
        AutoPtr<IView> v;
        IFragment::Probe(object)->GetView((IView**)&v);
        Logger::V(TAG, "Detaching item #%d: f=%s v=%s", GetItemId(position),
                TO_CSTR(object), TO_CSTR(v));
    }
    mCurTransaction->Detach(IFragment::Probe(object));
    return NOERROR;
}

// @Override
ECode FragmentPagerAdapter::SetPrimaryItem(
    /* [in] */ IViewGroup* container,
    /* [in] */ Int32 position,
    /* [in] */ IInterface* object)
{
    AutoPtr<IFragment> fragment = IFragment::Probe(object);
    if (fragment != mCurrentPrimaryItem) {
        if (mCurrentPrimaryItem != NULL) {
            FragmentCompat::SetMenuVisibility(mCurrentPrimaryItem, FALSE);
            FragmentCompat::SetUserVisibleHint(mCurrentPrimaryItem, FALSE);
        }
        if (fragment != NULL) {
            FragmentCompat::SetMenuVisibility(fragment, TRUE);
            FragmentCompat::SetUserVisibleHint(fragment, TRUE);
        }
        mCurrentPrimaryItem = fragment;
    }
    return NOERROR;
}

// @Override
ECode FragmentPagerAdapter::FinishUpdate(
    /* [in] */ IViewGroup* container)
{
    if (mCurTransaction != NULL) {
        Int32 id;
        mCurTransaction->CommitAllowingStateLoss(&id);
        mCurTransaction = NULL;
        Boolean result;
        mFragmentManager->ExecutePendingTransactions(&result);
    }
    return NOERROR;
}

// @Override
ECode FragmentPagerAdapter::IsViewFromObject(
    /* [in] */ IView* view,
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IView> v;
    IFragment::Probe(object)->GetView((IView**)&v);
    *result = v.Get() == view;
    return NOERROR;
}

// @Override
ECode FragmentPagerAdapter::SaveState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state);
    *state = NULL;
    return NOERROR;
}

// @Override
ECode FragmentPagerAdapter::RestoreState(
    /* [in] */ IParcelable* state,
    /* [in] */ IClassLoader* loader)
{
    return NOERROR;
}

Int64 FragmentPagerAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    return position;
}

String FragmentPagerAdapter::MakeFragmentName(
    /* [in] */ Int32 viewId,
    /* [in] */ Int64 id)
{
    StringBuilder sb("elastos:switcher:");
    sb += viewId;
    sb += ":";
    sb += id;
    return sb.ToString();
}

} // namespace App
} // namespace V13
} // namespace Support
} // namespace Droid
} // namespace Elastos
