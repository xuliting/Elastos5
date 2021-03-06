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

#ifndef __ELASTOS_DROID_SERVER_NETWORKTIMEUPDATESERVICE_H__
#define __ELASTOS_DROID_SERVER_NETWORKTIMEUPDATESERVICE_H__


#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/database/ContentObserver.h"
#include "elastos/droid/content/BroadcastReceiver.h"


using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::App::IAlarmManager;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Utility::ITrustedTime;

namespace Elastos {
namespace Droid {
namespace Server {

class NetworkTimeUpdateService
    : public Object
{
private:
    class NitzReceiver : public BroadcastReceiver
    {
    public:
        NitzReceiver(
            /* [in] */ NetworkTimeUpdateService* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        TO_STRING_IMPL("NetworkTimeUpdateService::NitzReceiver: ")

    private:
        NetworkTimeUpdateService* mHost;
    };

    /** Receiver for ConnectivityManager events */
    class ConnectivityReceiver
        : public BroadcastReceiver
    {
    public:
        TO_STRING_IMPL("NetworkTimeUpdateService::ConnectivityReceiver")

        ConnectivityReceiver(
            /* [in] */ NetworkTimeUpdateService* owner);

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        NetworkTimeUpdateService* mHost;
    };

    /** Observer to watch for changes to the AUTO_TIME setting */
    class SettingsObserver
        : public ContentObserver
    {
    public:
        TO_STRING_IMPL("NetworkTimeUpdateService::SettingsObserver")

        CARAPI constructor(
            /* [in] */ IHandler* handler,
            /* [in] */ Int32 msg,
            /* [in] */ NetworkTimeUpdateService* owner);

        CARAPI Observe(
            /* [in] */ IContext* context);

        //@Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        Int32 mMsg;
        AutoPtr<IHandler> mHandler;
        NetworkTimeUpdateService* mHost;
    };

    class MyReceiver
        : public BroadcastReceiver
    {
    public:
        TO_STRING_IMPL("NetworkTimeUpdateService::MyReceiver")

        MyReceiver(
            /* [in] */ NetworkTimeUpdateService* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        NetworkTimeUpdateService* mHost;
    };

    /** Handler to do the network accesses on */
    class MyHandler
        : public Handler
    {
    public:
        TO_STRING_IMPL("NetworkTimeUpdateService::ConnectionHandler")

        MyHandler(
            /* [in] */ ILooper* l,
            /* [in] */ NetworkTimeUpdateService* host);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        NetworkTimeUpdateService* mHost;
    };

public:
    NetworkTimeUpdateService();

    CARAPI constructor(
        /* [in] */ IContext* context);

    /** Initialize the receivers and initiate the first NTP request */
    CARAPI SystemRunning();

private:
    CARAPI_(void) RegisterForTelephonyIntents();

    CARAPI_(void) RegisterForAlarms();

    CARAPI_(void) RegisterForConnectivityIntents();

    CARAPI_(void) OnPollNetworkTime(
        /* [in] */ Int32 event);

    /**
     * Cancel old alarm and starts a new one for the specified interval.
     *
     * @param interval when to trigger the alarm, starting from now.
     */
    CARAPI_(void) ResetAlarm(
        /* [in] */ Int64 interval);

    /**
     * Checks if the user prefers to automatically set the time.
     */
    CARAPI_(Boolean) IsAutomaticTimeRequested();

    CARAPI SendMessage(
        /* [in] */ Int32 event);
private:
    friend class MyHandler;
    friend class ConnectivityReceiver;

    static const String TAG;
    static const Boolean DBG;

    static const Int32 EVENT_AUTO_TIME_CHANGED;
    static const Int32 EVENT_POLL_NETWORK_TIME;
    static const Int32 EVENT_NETWORK_CONNECTED;

    static const String ACTION_POLL;
    static Int32 POLL_REQUEST;

    static const Int64 NOT_SET;
    Int64 mNitzTimeSetTime;
    // TODO: Have a way to look up the timezone we are in
    Int64 mNitzZoneSetTime;

    AutoPtr<IContext> mContext;
    AutoPtr<ITrustedTime> mTime;

    // NTP lookup is done on this thread and handler
    AutoPtr<MyHandler> mHandler;
    AutoPtr<IAlarmManager> mAlarmManager;
    AutoPtr<IPendingIntent> mPendingPollIntent;
    AutoPtr<SettingsObserver> mSettingsObserver;
    // The last time that we successfully fetched the NTP time.
    Int64 mLastNtpFetchTime;

    // Normal polling frequency
    Int64 mPollingIntervalMs;
    // Try-again polling interval, in case the network request failed
    Int64 mPollingIntervalShorterMs;
    // Number of times to try again
    Int32 mTryAgainTimesMax;
    // If the time difference is greater than this threshold, then update the time.
    Int32 mTimeErrorThresholdMs;
    // Keeps track of how many quick attempts were made to fetch NTP time.
    // During bootup, the network may not have been up yet, or it's taking time for the
    // connection to happen.
    Int32 mTryAgainCounter;

    AutoPtr<MyReceiver> mAlarmReceiver;
    AutoPtr<NitzReceiver> mNitzReceiver;

    AutoPtr<ConnectivityReceiver> mConnectivityReceiver;
};

}//namespace Server
}//namespace Droid
}//namespace Elastos

#endif //__ELASTOS_DROID_SERVER_NETWORKTIMEUPDATESERVICE_H__
