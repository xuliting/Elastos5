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

#include "elastos/droid/systemui/statusbar/KeyguardIndicationController.h"
#include "elastos/droid/systemui/keyguard/KeyguardUpdateMonitor.h"
#include "R.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Internal.h"
#include "Elastos.Droid.View.h"
#include <elastos/droid/os/ServiceManager.h>
#include <elastos/droid/text/TextUtils.h>
#include <elastos/droid/text/format/Formatter.h>
#include <elastos/core/CoreUtils.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::SystemUI::Keyguard::KeyguardUpdateMonitor;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::IBatteryStats;
using Elastos::Droid::Os::IBatteryManager;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::Format::Formatter;
using Elastos::Droid::View::IView;
using Elastos::Core::CoreUtils;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {

const String KeyguardIndicationController::TAG("KeyguardIndicationController");
const Int32 KeyguardIndicationController::MSG_HIDE_TRANSIENT = 1;

ECode KeyguardIndicationController::UpdateMonitorCallback::OnRefreshBatteryInfo(
    /* [in] */ IKeyguardUpdateMonitorBatteryStatus* status)
{
    Int32 bs;
    status->GetStatus(&bs);
    mHost->mPowerPluggedIn = bs == IBatteryManager::BATTERY_STATUS_CHARGING
        || bs == IBatteryManager::BATTERY_STATUS_FULL;
    status->IsCharged(&mHost->mPowerCharged);
    mHost->UpdateIndication();
    return NOERROR;
}

KeyguardIndicationController::ControllerBroadcastReceiver::ControllerBroadcastReceiver(
    /* [in] */ KeyguardIndicationController* host)
    : mHost(host)
{}

ECode KeyguardIndicationController::ControllerBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    if (mHost->mVisible) {
        mHost->UpdateIndication();
    }
    return NOERROR;
}

KeyguardIndicationController::ControllerHandler::ControllerHandler(
    /* [in] */ KeyguardIndicationController* host)
    : mHost(host)
{
    Handler::constructor();
}

ECode KeyguardIndicationController::ControllerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what = 0;
    msg->GetWhat(&what);
    if (what == MSG_HIDE_TRANSIENT && mHost->mTransientIndication != NULL) {
        mHost->mTransientIndication = NULL;
        mHost->UpdateIndication();
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(KeyguardIndicationController, Object, IKeyguardIndicationController)


KeyguardIndicationController::KeyguardIndicationController()
    : mVisible(FALSE)
    , mPowerPluggedIn(FALSE)
    , mPowerCharged(FALSE)
{}

ECode KeyguardIndicationController::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IKeyguardIndicationTextView* textView)
{
    mReceiver = new ControllerBroadcastReceiver(this);
    mHandler = new ControllerHandler(this);
    mContext = context;
    mTextView = textView;

    AutoPtr<IInterface> obj = ServiceManager::GetService(IBatteryStats::SERVICE_NAME);
    mBatteryInfo = IIBatteryStats::Probe(obj);
    mUpdateMonitor = new UpdateMonitorCallback(this);

    KeyguardUpdateMonitor::GetInstance(context)->RegisterCallback(mUpdateMonitor);
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> user;
    helper->GetOWNER((IUserHandle**)&user);
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New(IIntent::ACTION_TIME_TICK, (IIntentFilter**)&filter);
    AutoPtr<IIntent> i;
    return context->RegisterReceiverAsUser(mReceiver, user, filter, String(NULL), NULL, (IIntent**)&i);
}

ECode KeyguardIndicationController::SetVisible(
    /* [in] */ Boolean visible)
{
    mVisible = visible;
    IView::Probe(mTextView)->SetVisibility(visible ? IView::VISIBLE : IView::GONE);
    if (visible) {
        HideTransientIndication();
        UpdateIndication();
    }
    return NOERROR;
}

ECode KeyguardIndicationController::SetRestingIndication(
    /* [in] */ const String& restingIndication)
{
    mRestingIndication = restingIndication;
    UpdateIndication();
    return NOERROR;
}

ECode KeyguardIndicationController::HideTransientIndicationDelayed(
    /* [in] */ Int64 delayMs)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_HIDE_TRANSIENT, (IMessage**)&msg);
    Boolean tmp = 0;
    mHandler->SendMessageDelayed(msg, delayMs, &tmp);
    return NOERROR;
}

ECode KeyguardIndicationController::ShowTransientIndication(
    /* [in] */ Int32 transientIndication)
{
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    String str;
    res->GetString(transientIndication, &str);
    ShowTransientIndication(str);
    return NOERROR;
}

ECode KeyguardIndicationController::ShowTransientIndication(
    /* [in] */ const String& transientIndication)
{
    mTransientIndication = transientIndication;
    mHandler->RemoveMessages(MSG_HIDE_TRANSIENT);
    UpdateIndication();
    return NOERROR;
}

ECode KeyguardIndicationController::HideTransientIndication()
{
    if (mTransientIndication != NULL) {
        mTransientIndication = NULL;
        mHandler->RemoveMessages(MSG_HIDE_TRANSIENT);
        UpdateIndication();
    }
    return NOERROR;
}

void KeyguardIndicationController::UpdateIndication()
{
    if (mVisible) {
        AutoPtr<ICharSequence> value = CoreUtils::Convert(ComputeIndication());
        mTextView->SwitchIndication(value);
    }
}

String KeyguardIndicationController::ComputeIndication()
{
    if (!TextUtils::IsEmpty(mTransientIndication)) {
        return mTransientIndication;
    }
    if (mPowerPluggedIn) {
        return ComputePowerIndication();
    }
    return mRestingIndication;
}

String KeyguardIndicationController::ComputePowerIndication()
{
    String str;
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    if (mPowerCharged) {
        res->GetString(R::string::keyguard_charged, &str);
        return str;
    }

    // Try fetching charging time from battery stats.
    // try {
    Int64 chargingTimeRemaining = 0;
    mBatteryInfo->ComputeChargeTimeRemaining(&chargingTimeRemaining);
    if (chargingTimeRemaining > 0) {
        String chargingTimeFormatted =
                Formatter::FormatShortElapsedTime(mContext, chargingTimeRemaining);

        AutoPtr<ICharSequence> value;
        CString::New(chargingTimeFormatted, (ICharSequence**)&value);
        AutoPtr<ArrayOf<IInterface*> > values = ArrayOf<IInterface*>::Alloc(1);
        values->Set(0, value);
        res->GetString(R::string::keyguard_indication_charging_time, values, &str);
        return str;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Error calling IBatteryStats: ", e);
    // }

    // Fall back to simple charging label.
    res->GetString(R::string::keyguard_plugged_in, &str);
    return str;
}

} // namespace StatusBar
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos
