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

#include "Elastos.Droid.Internal.h"
#include "elastos/droid/internal/telephony/imsphone/ImsPhoneCommandInterface.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Telephony {
namespace ImsPhone {

//=====================================================================
//                       ImsPhoneCommandInterface
//=====================================================================

ImsPhoneCommandInterface::ImsPhoneCommandInterface()
{
}

ECode ImsPhoneCommandInterface::constructor(
    /* [in] */ IContext* context)
{
    return BaseCommands::constructor(context);
}

ECode ImsPhoneCommandInterface::SetOnNITZTime(
    /* [in] */ IHandler* h,
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetIccCardStatus(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SupplyIccPin(
    /* [in] */ const String& pin,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SupplyIccPuk(
    /* [in] */ const String& puk,
    /* [in] */ const String& newPin,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SupplyIccPin2(
    /* [in] */ const String& pin,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SupplyIccPuk2(
    /* [in] */ const String& puk,
    /* [in] */ const String& newPin2,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ChangeIccPin(
    /* [in] */ const String& oldPin,
    /* [in] */ const String& newPin,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ChangeIccPin2(
    /* [in] */ const String& oldPin2,
    /* [in] */ const String& newPin2,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ChangeBarringPassword(
    /* [in] */ const String& facility,
    /* [in] */ const String& oldPwd,
    /* [in] */ const String& newPwd,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SupplyDepersonalization(
    /* [in] */ const String& netpin,
    /* [in] */ const String& type,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetCurrentCalls(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetPDPContextList(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetDataCallList(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::Dial(
    /* [in] */ const String& address,
    /* [in] */ Int32 clirMode,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::Dial(
    /* [in] */ const String& address,
    /* [in] */ Int32 clirMode,
    /* [in] */ IUUSInfo* uusInfo,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetIMSI(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetIMSIForApp(
    /* [in] */ const String& aid,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetIMEI(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetIMEISV(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::HangupConnection(
    /* [in] */ Int32 gsmIndex,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::HangupWaitingOrBackground(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::HangupForegroundResumeBackground(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SwitchWaitingOrHoldingAndActive(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::Conference(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetPreferredVoicePrivacy(
    /* [in] */ Boolean enable,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetPreferredVoicePrivacy(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SeparateConnection(
    /* [in] */ Int32 gsmIndex,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::AcceptCall(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::RejectCall(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ExplicitCallTransfer(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetLastCallFailCause(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetLastPdpFailCause(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetLastDataCallFailCause(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetMute(
    /* [in] */ Boolean enableMute,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetMute(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetSignalStrength(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetVoiceRegistrationState(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetDataRegistrationState(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetOperator(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendDtmf(
    /* [in] */ Char32 c,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::StartDtmf(
    /* [in] */ Char32 c,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::StopDtmf(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendBurstDtmf(
    /* [in] */ const String& dtmfString,
    /* [in] */ Int32 on,
    /* [in] */ Int32 off,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendSMS(
    /* [in] */ const String& smscPDU,
    /* [in] */ const String& pdu,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendSMSExpectMore(
    /* [in] */ const String& smscPDU,
    /* [in] */ const String& pdu,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendCdmaSms(
    /* [in] */ ArrayOf<Byte>* pdu,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendImsGsmSms(
    /* [in] */ const String& smscPDU,
    /* [in] */ const String& pdu,
    /* [in] */ Int32 retry,
    /* [in] */ Int32 messageRef,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendImsCdmaSms(
    /* [in] */ ArrayOf<Byte>* pdu,
    /* [in] */ Int32 retry,
    /* [in] */ Int32 messageRef,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetImsRegistrationState(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::DeleteSmsOnSim(
    /* [in] */ Int32 index,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::DeleteSmsOnRuim(
    /* [in] */ Int32 index,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::WriteSmsToSim(
    /* [in] */ Int32 status,
    /* [in] */ const String& smsc,
    /* [in] */ const String& pdu,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::WriteSmsToRuim(
    /* [in] */ Int32 status,
    /* [in] */ const String& pdu,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetupDataCall(
    /* [in] */ const String& radioTechnology,
    /* [in] */ const String& profile,
    /* [in] */ const String& apn,
    /* [in] */ const String& user,
    /* [in] */ const String& password,
    /* [in] */ const String& authType,
    /* [in] */ const String& protocol,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::DeactivateDataCall(
    /* [in] */ Int32 cid,
    /* [in] */ Int32 reason,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetRadioPower(
    /* [in] */ Boolean on,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetSuppServiceNotifications(
    /* [in] */ Boolean enable,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::AcknowledgeLastIncomingGsmSms(
    /* [in] */ Boolean success,
    /* [in] */ Int32 cause,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::AcknowledgeLastIncomingCdmaSms(
    /* [in] */ Boolean success,
    /* [in] */ Int32 cause,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::AcknowledgeIncomingGsmSmsWithPdu(
    /* [in] */ Boolean success,
    /* [in] */ const String& ackPdu,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::IccIO(
    /* [in] */ Int32 command,
    /* [in] */ Int32 fileid,
    /* [in] */ const String& path,
    /* [in] */ Int32 p1,
    /* [in] */ Int32 p2,
    /* [in] */ Int32 p3,
    /* [in] */ const String& data,
    /* [in] */ const String& pin2,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::IccIOForApp(
    /* [in] */ Int32 command,
    /* [in] */ Int32 fileid,
    /* [in] */ const String& path,
    /* [in] */ Int32 p1,
    /* [in] */ Int32 p2,
    /* [in] */ Int32 p3,
    /* [in] */ const String& data,
    /* [in] */ const String& pin2,
    /* [in] */ const String& aid,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetCLIR(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetCLIR(
    /* [in] */ Int32 clirMode,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::QueryCallWaiting(
    /* [in] */ Int32 serviceClass,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetCallWaiting(
    /* [in] */ Boolean enable,
    /* [in] */ Int32 serviceClass,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetNetworkSelectionModeAutomatic(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetNetworkSelectionModeManual(
    /* [in] */ const String& operatorNumeric,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetNetworkSelectionMode(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetAvailableNetworks(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetCallForward(
    /* [in] */ Int32 action,
    /* [in] */ Int32 cfReason,
    /* [in] */ Int32 serviceClass,
    /* [in] */ const String& number,
    /* [in] */ Int32 timeSeconds,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::QueryCallForwardStatus(
    /* [in] */ Int32 cfReason,
    /* [in] */ Int32 serviceClass,
    /* [in] */ const String& number,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::QueryCLIP(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetBasebandVersion(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::QueryFacilityLock(
    /* [in] */ const String& facility,
    /* [in] */ const String& password,
    /* [in] */ Int32 serviceClass,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::QueryFacilityLockForApp(
    /* [in] */ const String& facility,
    /* [in] */ const String& password,
    /* [in] */ Int32 serviceClass,
    /* [in] */ const String& appId,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetFacilityLock(
    /* [in] */ const String& facility,
    /* [in] */ Boolean lockState,
    /* [in] */ const String& password,
    /* [in] */ Int32 serviceClass,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetFacilityLockForApp(
    /* [in] */ const String& facility,
    /* [in] */ Boolean lockState,
    /* [in] */ const String& password,
    /* [in] */ Int32 serviceClass,
    /* [in] */ const String& appId,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendUSSD(
    /* [in] */ const String& ussdString,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::CancelPendingUssd(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ResetRadio(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::InvokeOemRilRequestRaw(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::InvokeOemRilRequestStrings(
    /* [in] */ ArrayOf<String>* strings,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetBandMode(
    /* [in] */ Int32 bandMode,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::QueryAvailableBandMode(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendTerminalResponse(
    /* [in] */ const String& contents,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendEnvelope(
    /* [in] */ const String& contents,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendEnvelopeWithStatus(
    /* [in] */ const String& contents,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::HandleCallSetupRequestFromSim(
    /* [in] */ Boolean accept,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetPreferredNetworkType(
    /* [in] */ Int32 networkType,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetPreferredNetworkType(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetNeighboringCids(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetLocationUpdates(
    /* [in] */ Boolean enable,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetSmscAddress(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetSmscAddress(
    /* [in] */ const String& address,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ReportSmsMemoryStatus(
    /* [in] */ Boolean available,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ReportStkServiceIsRunning(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetCdmaSubscriptionSource(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetGsmBroadcastConfig(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetGsmBroadcastConfig(
    /* [in] */ ArrayOf<ISmsBroadcastConfigInfo*>* config,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetGsmBroadcastActivation(
    /* [in] */ Boolean activate,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetDeviceIdentity(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetCDMASubscription(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetPhoneType(
    /* [in] */ Int32 phoneType)
{
    // ==================before translated======================
    // //Set by CDMAPhone and GSMPhone constructor
    return NOERROR;
}

ECode ImsPhoneCommandInterface::QueryCdmaRoamingPreference(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetCdmaRoamingPreference(
    /* [in] */ Int32 cdmaRoamingType,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetCdmaSubscriptionSource(
    /* [in] */ Int32 cdmaSubscription,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::QueryTTYMode(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetTTYMode(
    /* [in] */ Int32 ttyMode,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SendCDMAFeatureCode(
    /* [in] */ const String& FeatureCode,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetCdmaBroadcastConfig(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetCdmaBroadcastConfig(
    /* [in] */ ArrayOf<ICdmaSmsBroadcastConfigInfo*>* configs,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetCdmaBroadcastActivation(
    /* [in] */ Boolean activate,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ExitEmergencyCallbackMode(
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SupplyIccPinForApp(
    /* [in] */ const String& pin,
    /* [in] */ const String& aid,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SupplyIccPukForApp(
    /* [in] */ const String& puk,
    /* [in] */ const String& newPin,
    /* [in] */ const String& aid,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SupplyIccPin2ForApp(
    /* [in] */ const String& pin2,
    /* [in] */ const String& aid,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SupplyIccPuk2ForApp(
    /* [in] */ const String& puk2,
    /* [in] */ const String& newPin2,
    /* [in] */ const String& aid,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ChangeIccPinForApp(
    /* [in] */ const String& oldPin,
    /* [in] */ const String& newPin,
    /* [in] */ const String& aidPtr,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::ChangeIccPin2ForApp(
    /* [in] */ const String& oldPin2,
    /* [in] */ const String& newPin2,
    /* [in] */ const String& aidPtr,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::RequestIsimAuthentication(
    /* [in] */ const String& nonce,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::RequestIccSimAuthentication(
    /* [in] */ Int32 authContext,
    /* [in] */ const String& data,
    /* [in] */ const String& aid,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetVoiceRadioTechnology(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetCellInfoList(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetCellInfoListRate(
    /* [in] */ Int32 rateInMillis,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetInitialAttachApn(
    /* [in] */ const String& apn,
    /* [in] */ const String& protocol,
    /* [in] */ Int32 authType,
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::SetDataProfile(
    /* [in] */ ArrayOf<IDataProfile*>* dps,
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::NvReadItem(
    /* [in] */ Int32 itemID,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::NvWriteItem(
    /* [in] */ Int32 itemID,
    /* [in] */ const String& itemValue,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::NvWriteCdmaPrl(
    /* [in] */ ArrayOf<Byte>* preferredRoamingList,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::NvResetConfig(
    /* [in] */ Int32 resetType,
    /* [in] */ IMessage* response)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::GetHardwareConfig(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::RequestShutdown(
    /* [in] */ IMessage* result)
{
    return NOERROR;
}

ECode ImsPhoneCommandInterface::NeedsOldRilFeature(
    /* [in] */ const String& feature,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

} // namespace Imsphone
} // namespace Telephony
} // namespace Internal
} // namespace Droid
} // namespace Elastos


