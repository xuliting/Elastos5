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

#ifndef __ELASTOS_DROID_DIALER_CALLLOG_DEFAULTVOICEMAILNOTIFIER_H__
#define __ELASTOS_DROID_DIALER_CALLLOG_DEFAULTVOICEMAILNOTIFIER_H__

#include "_Elastos.Droid.Dialer.h"
#include "elastos/droid/dialer/calllog/PhoneNumberDisplayHelper.h"

using Elastos::Droid::App::INotificationManager;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Dialer {
namespace CallLog {

/**
 * Implementation of {@link VoicemailNotifier} that shows a notification in the
 * status bar.
 */
class DefaultVoicemailNotifier
    : public Object
    , public IVoicemailNotifier
{
private:
    /** Information about a new voicemail. */
    class NewCall
        : public Object
        , public IDefaultVoicemailNotifierNewCall
    {
    public:
        NewCall(
            /* [in] */ IUri* callsUri,
            /* [in] */ IUri* voicemailUri,
            /* [in] */ const String& number,
            /* [in] */ Int32 numberPresentation)
            : mCallsUri(callsUri)
            , mVoicemailUri(voicemailUri)
            , mNumber(number)
            , mNumberPresentation(numberPresentation)
        {}

        CAR_INTERFACE_DECL()

    public:
        AutoPtr<IUri> mCallsUri;
        AutoPtr<IUri> mVoicemailUri;
        String mNumber;
        Int32 mNumberPresentation;
    };

    /**
     * Default implementation of {@link NewCallsQuery} that looks up the list of new calls to
     * notify about in the call log.
     */
    class DefaultNewCallsQuery
        : public Object
        , public IDefaultVoicemailNotifierNewCallsQuery
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Query(
            /* [out, callee] */ ArrayOf<IDefaultVoicemailNotifierNewCall*>** newCalls);

    private:
        DefaultNewCallsQuery(
            /* [in] */ IContentResolver* contentResolver)
            : mContentResolver(contentResolver)
        {}

         /** Returns an instance of {@link NewCall} created by using the values of the cursor. */
        CARAPI_(AutoPtr<NewCall>) CreateNewCallsFromCursor(
            /* [in] */ ICursor* cursor);

    private:
        static const AutoPtr<ArrayOf<String> > PROJECTION;
        static const Int32 ID_COLUMN_INDEX = 0;
        static const Int32 NUMBER_COLUMN_INDEX = 1;
        static const Int32 VOICEMAIL_URI_COLUMN_INDEX = 2;
        static const Int32 NUMBER_PRESENTATION_COLUMN_INDEX = 3;

        AutoPtr<IContentResolver> mContentResolver;

        friend class DefaultVoicemailNotifier;
    };

    /**
     * Default implementation of {@link NameLookupQuery} that looks up the name of a contact in the
     * contacts database.
     */
    class DefaultNameLookupQuery
        : public Object
        , public IDefaultVoicemailNotifierNameLookupQuery
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Query(
            /* [in] */ const String& number,
            /* [out] */ String* NewCalls);

    private:
        DefaultNameLookupQuery(
            /* [in] */ IContentResolver* contentResolver)
            : mContentResolver(contentResolver)
        {}

    private:
        static const AutoPtr<ArrayOf<String> > PROJECTION;
        static const Int32 DISPLAY_NAME_COLUMN_INDEX = 0;

        AutoPtr<IContentResolver> mContentResolver;

        friend class DefaultVoicemailNotifier;
    };

public:
    CAR_INTERFACE_DECL();

    /** Returns the singleton instance of the {@link DefaultVoicemailNotifier}. */
    static CARAPI_(AutoPtr<DefaultVoicemailNotifier>) GetInstance(
        /* [in] */ IContext* context);

    /** Updates the notification and notifies of the call with the given URI. */
    // @Override
    CARAPI UpdateNotification(
        /* [in] */ IUri* newCallUri);

    // @Override
    CARAPI ClearNotification();

    /** Create a new instance of {@link NewCallsQuery}. */
    static CARAPI_(AutoPtr<IDefaultVoicemailNotifierNewCallsQuery>) CreateNewCallsQuery(
        /* [in] */ IContentResolver* contentResolver);

    /** Create a new instance of {@link NameLookupQuery}. */
    static CARAPI_(AutoPtr<IDefaultVoicemailNotifierNameLookupQuery>) CreateNameLookupQuery(
        /* [in] */ IContentResolver* contentResolver);

    /**
     * Create a new PhoneNumberHelper.
     * <p>
     * This will cause some Disk I/O, at least the first time it is created, so it should not be
     * called from the main thread.
     */
    static CARAPI_(AutoPtr<PhoneNumberDisplayHelper>) CreatePhoneNumberHelper(
        /* [in] */ IContext* context);

private:
    DefaultVoicemailNotifier(
        /* [in] */ IContext* context,
        /* [in] */ INotificationManager* notificationManager,
        /* [in] */ IDefaultVoicemailNotifierNewCallsQuery* newCallsQuery,
        /* [in] */ IDefaultVoicemailNotifierNameLookupQuery* nameLookupQuery,
        /* [in] */ PhoneNumberDisplayHelper* phoneNumberHelper);

    /** Creates a pending intent that marks all new voicemails as old. */
    CARAPI_(AutoPtr<IPendingIntent>) CreateMarkNewVoicemailsAsOldIntent();

public:
    static const String TAG;

private:
    /** The tag used to identify notifications from this class. */
    static const String NOTIFICATION_TAG;
    /** The identifier of the notification of new voicemails. */
    static const Int32 NOTIFICATION_ID = 1;

    /** The singleton instance of {@link DefaultVoicemailNotifier}. */
    static AutoPtr<DefaultVoicemailNotifier> sInstance;

    AutoPtr<IContext> mContext;
    AutoPtr<INotificationManager> mNotificationManager;
    AutoPtr<IDefaultVoicemailNotifierNewCallsQuery> mNewCallsQuery;
    AutoPtr<IDefaultVoicemailNotifierNameLookupQuery> mNameLookupQuery;
    AutoPtr<PhoneNumberDisplayHelper> mPhoneNumberHelper;
};

} // CallLog
} // Dialer
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_DIALER_CALLLOG_DEFAULTVOICEMAILNOTIFIER_H__

