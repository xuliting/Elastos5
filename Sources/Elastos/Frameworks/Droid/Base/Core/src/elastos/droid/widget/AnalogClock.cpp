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

#include "Elastos.Droid.Widget.h"
#include <Elastos.CoreLibrary.Utility.h>
#include "elastos/droid/content/CIntentFilter.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/text/format/CDateUtils.h"
#include "elastos/droid/widget/AnalogClock.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Content::EIID_IBroadcastReceiver;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Text::Format::CDateUtils;
using Elastos::Droid::Text::Format::IDateUtils;
using Elastos::Core::CString;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Utility::ITimeZone;
using Elastos::Utility::ITimeZoneHelper;
using namespace Elastos::Core;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(AnalogClock, View, IAnalogClock)

AnalogClock::IntentBroadcastReceiver::IntentBroadcastReceiver(
    /* [in] */ AnalogClock* host)
    : mHost(host)
{
}

ECode AnalogClock::IntentBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action == IIntent::ACTION_TIMEZONE_CHANGED) {
        String tz;
        intent->GetStringExtra(String("time-zone"), &tz);
        AutoPtr<ITimeZoneHelper> helper;
        AutoPtr<ITimeZone> itz;

        CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&helper);
        helper->GetTimeZone(tz, (ITimeZone**)&itz);
        String id;
        itz->GetID(&id);
        mHost->mCalendar = NULL;
        CTime::New(id, (ITime**)&(mHost->mCalendar));
    }

    mHost->OnTimeChanged();
    mHost->Invalidate();

    return NOERROR;
}

ECode AnalogClock::IntentBroadcastReceiver::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = String("AnalogClock::IntentBroadcastReceiver: ");
    (*info).AppendFormat("%p", this);
    return NOERROR;
}

AnalogClock::AnalogClock()
    : mDialWidth(0)
    , mDialHeight(0)
    , mAttached(FALSE)
    , mMinutes(0.0f)
    , mHour(0.0f)
    , mChanged(FALSE)
{
    mIntentReceiver = new IntentBroadcastReceiver(this);
    CHandler::New((IHandler**)&mHandler);
}

ECode AnalogClock::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode AnalogClock::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode AnalogClock::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return constructor(context, attrs, defStyle, 0);
}

ECode AnalogClock::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 defStyleRes)
{
    View::constructor(context, attrs, defStyle, defStyleRes);
    AutoPtr<IResources> r;
    context->GetResources((IResources**)&r);

    AutoPtr<ArrayOf<Int32> > attrIds = TO_ATTRS_ARRAYOF(R::styleable::AnalogClock);
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyle, defStyleRes, (ITypedArray**)&a);
    a->GetDrawable(R::styleable::AnalogClock_dial, (IDrawable**)&mDial);
    if (mDial == NULL) {
        context->GetDrawable(R::drawable::clock_dial, (IDrawable**)&mDial);
    }

    a->GetDrawable(R::styleable::AnalogClock_hand_hour, (IDrawable**)&mHourHand);
    if (mHourHand == NULL) {
        context->GetDrawable(R::drawable::clock_hand_hour, (IDrawable**)&mHourHand);
    }

    a->GetDrawable(R::styleable::AnalogClock_hand_minute, (IDrawable**)&mMinuteHand);
    if (mMinuteHand == NULL) {
        context->GetDrawable(R::drawable::clock_hand_minute, (IDrawable**)&mMinuteHand);
    }

    CTime::New((ITime**)&mCalendar);
    mDial->GetIntrinsicWidth(&mDialWidth);
    mDial->GetIntrinsicHeight(&mDialHeight);
    return NOERROR;
}

ECode AnalogClock::OnAttachedToWindow()
{
    View::OnAttachedToWindow();

    if (!mAttached) {
        mAttached = TRUE;
        AutoPtr<IIntentFilter> filter;
        CIntentFilter::New((IIntentFilter**)&filter);

        filter->AddAction(IIntent::ACTION_TIME_TICK);
        filter->AddAction(IIntent::ACTION_TIME_CHANGED);
        filter->AddAction(IIntent::ACTION_TIMEZONE_CHANGED);
        AutoPtr<IIntent> rst;

        AutoPtr<IContext> context;
        GetContext((IContext**)&context);

        AutoPtr<IUserHandle> userHandle;
        Process::MyUserHandle((IUserHandle**)&userHandle);
        context->RegisterReceiverAsUser(mIntentReceiver, userHandle, filter, String(""), mHandler, (IIntent**)&rst);
    }

    if(mCalendar)
        mCalendar = NULL;
    CTime::New((ITime**)&mCalendar);
    OnTimeChanged();
    return NOERROR;
}

ECode AnalogClock::OnDetachedFromWindow()
{
    View::OnDetachedFromWindow();
    if (mAttached && mIntentReceiver) {
        AutoPtr<IContext> context;
        GetContext((IContext**)&context);
        context->UnregisterReceiver(mIntentReceiver);
        mAttached = FALSE;
    }
    return NOERROR;
}

ECode AnalogClock::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthMode = View::MeasureSpec::GetMode(widthMeasureSpec);
    Int32 widthSize =  View::MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightMode = View::MeasureSpec::GetMode(heightMeasureSpec);
    Int32 heightSize =  View::MeasureSpec::GetSize(heightMeasureSpec);

    Float hScale = 1.0f;
    Float vScale = 1.0f;

    if (widthMode != View::MeasureSpec::UNSPECIFIED && widthSize < mDialWidth) {
        hScale = (Float) widthSize / (Float) mDialWidth;
    }

    if (heightMode != View::MeasureSpec::UNSPECIFIED && heightSize < mDialHeight) {
        vScale = (Float )heightSize / (Float) mDialHeight;
    }

    Float scale = Elastos::Core::Math::Min(hScale, vScale);

    SetMeasuredDimension(ResolveSizeAndState((Int32) (mDialWidth * scale), widthMeasureSpec, 0),
        ResolveSizeAndState((Int32) (mDialHeight * scale), heightMeasureSpec, 0));
    return NOERROR;
}

ECode AnalogClock::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    View::OnSizeChanged(w, h, oldw, oldh);
    mChanged = TRUE;
    return NOERROR;
}

void AnalogClock::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    View::OnDraw(canvas);

    Boolean changed = mChanged;
    if (changed) {
        mChanged = FALSE;
    }

    Int32 availableWidth = mRight - mLeft;
    Int32 availableHeight = mBottom - mTop;

    Int32 x = availableWidth / 2;
    Int32 y = availableHeight / 2;

    AutoPtr<IDrawable> dial = mDial;
    Int32 w;
    dial->GetIntrinsicWidth(&w);
    Int32 h;
    dial->GetIntrinsicHeight(&h);

    Boolean scaled = FALSE;

    Int32 res;
    if (availableWidth < w || availableHeight < h) {
        scaled = TRUE;
        Float scale = Elastos::Core::Math::Min((Float) availableWidth / (Float) w,
            (Float) availableHeight / (Float) h);
        canvas->Save(&res);
        canvas->Scale(scale, scale, x, y);
    }

    if (changed) {
        dial->SetBounds(x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
    }
    dial->Draw(canvas);

    canvas->Save(&res);
    canvas->Rotate(mHour / 12.0f * 360.0f, x, y);
    AutoPtr<IDrawable> hourHand = mHourHand;
    if (changed) {
        hourHand->GetIntrinsicWidth(&w);
        hourHand->GetIntrinsicHeight(&h);
        hourHand->SetBounds(x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
    }
    hourHand->Draw(canvas);
    canvas->Restore();

    canvas->Save(&res);
    canvas->Rotate(mMinutes / 60.0f * 360.0f, x, y);

    AutoPtr<IDrawable> minuteHand = mMinuteHand;
    if (changed) {
        minuteHand->GetIntrinsicWidth(&w);
        minuteHand->GetIntrinsicHeight(&h);
        minuteHand->SetBounds(x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
    }
    minuteHand->Draw(canvas);
    canvas->Restore();

    if (scaled) {
        canvas->Restore();
    }
}

void AnalogClock::OnTimeChanged()
{
    mCalendar->SetToNow();

    Int32 hour = ((CTime*)mCalendar.Get())->mHour;
    Int32 minute = ((CTime*)mCalendar.Get())->mMinute;
    Int32 second = ((CTime*)mCalendar.Get())->mSecond;

    mMinutes = minute + second / 60.0f;
    mHour = hour + mMinutes / 60.0f;
    mChanged = TRUE;

    UpdateContentDescription(mCalendar);
}

void AnalogClock::UpdateContentDescription(
    /* [in] */ ITime* time)
{
    AutoPtr<IDateUtils> utils;
    CDateUtils::AcquireSingleton((IDateUtils**)&utils);
    Int32 flags = IDateUtils::FORMAT_SHOW_TIME | IDateUtils::FORMAT_24HOUR;
    String contentDescription;
    Int64 millis;
    time->ToMillis(FALSE, &millis);
    utils->FormatDateTime(mContext,
            millis, flags, &contentDescription);
    AutoPtr<ICharSequence> text;
    CString::New(contentDescription, (ICharSequence**)&text);
    SetContentDescription(text);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos

