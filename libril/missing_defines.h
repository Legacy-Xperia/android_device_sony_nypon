#ifndef MISSING_DEFINES_H
#define MISSING_DEFINES_H 1

#include <sys/cdefs.h>
#include <arpa/inet.h>

#define LOGE ALOGE
#define LOGI ALOGI
#define LOGD ALOGD
#define LOGW ALOGW

#define RIL_LastDataCallActivateFailCause RIL_DataCallFailCause

extern int ifc_set_mask(const char *name, in_addr_t mask);

#if RIL_VERSION >= 6
#define RIL_REQUEST_REGISTRATION_STATE RIL_REQUEST_VOICE_REGISTRATION_STATE
#define RIL_REQUEST_GPRS_REGISTRATION_STATE RIL_REQUEST_DATA_REGISTRATION_STATE
#define RIL_UNSOL_RESPONSE_NETWORK_STATE_CHANGED RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED
#define RIL_CardStatus RIL_CardStatus_v6
#define RIL_SignalStrength RIL_SignalStrength_v6
#define RIL_Data_Call_Response RIL_Data_Call_Response_v4
#define RIL_SIM_IO RIL_SIM_IO_v6
#endif

#define dispatchDataCall dummyDispatch
#define responseSetupDataCall dummyResponse
#define responseCellInfoList dummyResponse
#define dispatchVoiceRadioTech dummyDispatch
#define responseStringsNetworks dummyResponse
#define dispatchCdmaSubscriptionSource dummyDispatch
#define dispatchSetInitialAttachApn dummyDispatch
#define dispatchImsSms dummyDispatch

#endif
