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

#ifndef __ELASTOS_DROID_WEBKIT_FINDACTIONMODECALLBACK_H__
#define __ELASTOS_DROID_WEBKIT_FINDACTIONMODECALLBACK_H__

#include "Elastos.Droid.Text.h"
#include "Elastos.Droid.View.h"
#include "Elastos.Droid.Webkit.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::ITextWatcher;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Widget::IEditText;
using Elastos::Droid::Widget::ITextView;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebView;

class ECO_PUBLIC FindActionModeCallback
    : public Object
    , public IActionModeCallback
    , public ITextWatcher
{
private:
    class InnerListener
        : public Object
        , public IViewOnClickListener
        , public IWebViewFindListener
    {
    public:
        CAR_INTERFACE_DECL()

        InnerListener(
            /* [in] */ FindActionModeCallback* host);

        CARAPI OnClick(
            /* [in] */ IView* v);

        CARAPI OnFindResultReceived(
            /* [in] */ Int32 activeMatchOrdinal,
            /* [in] */ Int32 numberOfMatches,
            /* [in] */ Boolean isDoneCounting);

    private:
        FindActionModeCallback* mHost;
    };
public:
    class NoAction
        : public Object
        , public IActionModeCallback
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI OnCreateActionMode(
            /* [in] */ IActionMode* mode,
            /* [in] */ IMenu* menu,
            /* [out] */ Boolean* result);

        CARAPI OnPrepareActionMode(
            /* [in] */ IActionMode* mode,
            /* [in] */ IMenu* menu,
            /* [out] */ Boolean* result);

        CARAPI OnActionItemClicked(
            /* [in] */ IActionMode* mode,
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* result);

        CARAPI OnDestroyActionMode(
            /* [in] */ IActionMode* mode);

        CARAPI ToString(
            /* [out] */ String* info);
    };

public:
    CAR_INTERFACE_DECL()

    FindActionModeCallback();

    CARAPI constructor(
        /* [in] */ IContext* context);

    virtual CARAPI_(void) Finish();

    /*
     * Place text in the text field so it can be searched for.  Need to press
     * the find next or find previous button to find all of the matches.
     */
    virtual CARAPI_(void) SetText(
        /* [in] */ const String& text);

    /*
     * Set the WebView to search.  Must be non null, and set before calling
     * startActionMode.
     */
    virtual CARAPI SetWebView(
        /* [in] */ CWebView* webView);

    virtual CARAPI OnFindResultReceived(
        /* [in] */ Int32 activeMatchOrdinal,
        /* [in] */ Int32 numberOfMatches,
        /* [in] */ Boolean isDoneCounting);

    /*
     * Highlight all the instances of the string from mEditText in mWebView.
     */
    virtual CARAPI_(void) FindAll();

    virtual CARAPI_(void) ShowSoftInput();

    virtual CARAPI_(void) UpdateMatchCount(
        /* [in] */ Int32 matchIndex,
        /* [in] */ Int32 matchCount,
        /* [in] */ Boolean isEmptyFind);

    // OnClickListener implementation

    virtual CARAPI OnClick(
        /* [in] */ IView* v);

    // ActionMode.Callback implementation

    CARAPI OnCreateActionMode(
        /* [in] */ IActionMode* mode,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    CARAPI OnDestroyActionMode(
        /* [in] */ IActionMode* mode);

    CARAPI OnPrepareActionMode(
        /* [in] */ IActionMode* mode,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    CARAPI OnActionItemClicked(
        /* [in] */ IActionMode* mode,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* result);

    // TextWatcher implementation

    CARAPI BeforeTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 after);

    CARAPI OnTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 count);

    CARAPI AfterTextChanged(
        /* [in] */ IEditable* s);

    virtual CARAPI_(Int32) GetActionModeGlobalBottom();

    CARAPI ToString(
        /* [out] */ String* info);

private:
    /*
     * Move the highlight to the next match.
     * @param next If true, find the next match further down in the document.
     *             If false, find the previous match, up in the document.
     */
    CARAPI_(void) FindNext(
        /* [in] */ Boolean next);

    /*
     * Update the string which tells the user how many matches were found, and
     * which match is currently highlighted.
     */
    CARAPI_(void) UpdateMatchesString();

private:
    AutoPtr<IRect> mGlobalVisibleRect;
    AutoPtr<IPoint> mGlobalVisibleOffset;
    AutoPtr<IView> mCustomView;
    AutoPtr<IEditText> mEditText;
    AutoPtr<ITextView> mMatches;
    AutoPtr<CWebView> mWebView;
    AutoPtr<IInputMethodManager> mInput;
    AutoPtr<IResources> mResources;
    Boolean mMatchesFound;
    Int32 mNumberOfMatches;
    Int32 mActiveMatchIndex;
    AutoPtr<IActionMode> mActionMode;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_FINDACTIONMODECALLBACK_H__
