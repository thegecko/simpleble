#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include <chrono>
#include <condition_variable>
#include <mutex>

#include "Wire.h"
#include "ProtocolBase.h"

#include "protocol/d2h.pb.h"
#include "protocol/h2d.pb.h"

namespace SimpleBLE {
namespace Dongl {
namespace Serial {

class Protocol : public ProtocolBase {
  public:
    Protocol(const std::string& device_path);
    ~Protocol();

    basic_WhoamiRsp basic_whoami();
    basic_ResetRsp basic_reset();
    basic_DfuStartRsp basic_dfu_start();

    simpleble_InitRsp simpleble_init();

    // GAP commands
    sd_GapAddrSetRsp sd_gap_addr_set(sd_GapAddrSetCmd* cmd);
    sd_GapAddrGetRsp sd_gap_addr_get(sd_GapAddrGetCmd* cmd);
    sd_GapWhitelistSetRsp sd_gap_whitelist_set(sd_GapWhitelistSetCmd* cmd);
    sd_GapDeviceIdentitiesSetRsp sd_gap_device_identities_set(sd_GapDeviceIdentitiesSetCmd* cmd);
    sd_GapPrivacySetRsp sd_gap_privacy_set(sd_GapPrivacySetCmd* cmd);
    sd_GapPrivacyGetRsp sd_gap_privacy_get(sd_GapPrivacyGetCmd* cmd);
    sd_GapAdvSetConfigureRsp sd_gap_adv_set_configure(sd_GapAdvSetConfigureCmd* cmd);
    sd_GapAdvStartRsp sd_gap_adv_start(sd_GapAdvStartCmd* cmd);
    sd_GapAdvStopRsp sd_gap_adv_stop(sd_GapAdvStopCmd* cmd);
    sd_GapConnParamUpdateRsp sd_gap_conn_param_update(sd_GapConnParamUpdateCmd* cmd);
    sd_GapDisconnectRsp sd_gap_disconnect(sd_GapDisconnectCmd* cmd);
    sd_GapTxPowerSetRsp sd_gap_tx_power_set(sd_GapTxPowerSetCmd* cmd);
    sd_GapAppearanceSetRsp sd_gap_appearance_set(sd_GapAppearanceSetCmd* cmd);
    sd_GapAppearanceGetRsp sd_gap_appearance_get(sd_GapAppearanceGetCmd* cmd);
    sd_GapPpcpSetRsp sd_gap_ppcp_set(sd_GapPpcpSetCmd* cmd);
    sd_GapPpcpGetRsp sd_gap_ppcp_get(sd_GapPpcpGetCmd* cmd);
    sd_GapDeviceNameSetRsp sd_gap_device_name_set(sd_GapDeviceNameSetCmd* cmd);
    sd_GapDeviceNameGetRsp sd_gap_device_name_get(sd_GapDeviceNameGetCmd* cmd);
    sd_GapAuthenticateRsp sd_gap_authenticate(sd_GapAuthenticateCmd* cmd);
    sd_GapSecParamsReplyRsp sd_gap_sec_params_reply(sd_GapSecParamsReplyCmd* cmd);
    sd_GapAuthKeyReplyRsp sd_gap_auth_key_reply(sd_GapAuthKeyReplyCmd* cmd);
    sd_GapLescDhkeyReplyRsp sd_gap_lesc_dhkey_reply(sd_GapLescDhkeyReplyCmd* cmd);
    sd_GapKeypressNotifyRsp sd_gap_keypress_notify(sd_GapKeypressNotifyCmd* cmd);
    sd_GapLescOobDataGetRsp sd_gap_lesc_oob_data_get(sd_GapLescOobDataGetCmd* cmd);
    sd_GapLescOobDataSetRsp sd_gap_lesc_oob_data_set(sd_GapLescOobDataSetCmd* cmd);
    sd_GapEncryptRsp sd_gap_encrypt(sd_GapEncryptCmd* cmd);
    sd_GapSecInfoReplyRsp sd_gap_sec_info_reply(sd_GapSecInfoReplyCmd* cmd);
    sd_GapConnSecGetRsp sd_gap_conn_sec_get(sd_GapConnSecGetCmd* cmd);
    sd_GapRssiStartRsp sd_gap_rssi_start(sd_GapRssiStartCmd* cmd);
    sd_GapRssiStopRsp sd_gap_rssi_stop(sd_GapRssiStopCmd* cmd);
    sd_GapScanStartRsp sd_gap_scan_start(sd_GapScanStartCmd* cmd);
    sd_GapScanStopRsp sd_gap_scan_stop(sd_GapScanStopCmd* cmd);
    sd_GapConnectRsp sd_gap_connect(sd_GapConnectCmd* cmd);
    sd_GapConnectCancelRsp sd_gap_connect_cancel(sd_GapConnectCancelCmd* cmd);
    sd_GapRssiGetRsp sd_gap_rssi_get(sd_GapRssiGetCmd* cmd);
    sd_GapPhyUpdateRsp sd_gap_phy_update(sd_GapPhyUpdateCmd* cmd);
    sd_GapDataLengthUpdateRsp sd_gap_data_length_update(sd_GapDataLengthUpdateCmd* cmd);
    sd_GapQosChannelSurveyStartRsp sd_gap_qos_channel_survey_start(sd_GapQosChannelSurveyStartCmd* cmd);
    sd_GapQosChannelSurveyStopRsp sd_gap_qos_channel_survey_stop(sd_GapQosChannelSurveyStopCmd* cmd);
    sd_GapAdvAddrGetRsp sd_gap_adv_addr_get(sd_GapAdvAddrGetCmd* cmd);
    sd_GapNextConnEvtCounterGetRsp sd_gap_next_conn_evt_counter_get(sd_GapNextConnEvtCounterGetCmd* cmd);
    sd_GapConnEvtTriggerStartRsp sd_gap_conn_evt_trigger_start(sd_GapConnEvtTriggerStartCmd* cmd);
    sd_GapConnEvtTriggerStopRsp sd_gap_conn_evt_trigger_stop(sd_GapConnEvtTriggerStopCmd* cmd);

    // GATTC commands
    sd_GattcPrimaryServicesDiscoverRsp sd_gattc_primary_services_discover(sd_GattcPrimaryServicesDiscoverCmd* cmd);
    sd_GattcRelationshipsDiscoverRsp sd_gattc_relationships_discover(sd_GattcRelationshipsDiscoverCmd* cmd);
    sd_GattcCharacteristicsDiscoverRsp sd_gattc_characteristics_discover(sd_GattcCharacteristicsDiscoverCmd* cmd);
    sd_GattcDescriptorsDiscoverRsp sd_gattc_descriptors_discover(sd_GattcDescriptorsDiscoverCmd* cmd);
    sd_GattcAttrInfoDiscoverRsp sd_gattc_attr_info_discover(sd_GattcAttrInfoDiscoverCmd* cmd);
    sd_GattcCharValueByUuidReadRsp sd_gattc_char_value_by_uuid_read(sd_GattcCharValueByUuidReadCmd* cmd);
    sd_GattcReadRsp sd_gattc_read(sd_GattcReadCmd* cmd);
    sd_GattcCharValuesReadRsp sd_gattc_char_values_read(sd_GattcCharValuesReadCmd* cmd);
    sd_GattcWriteRsp sd_gattc_write(sd_GattcWriteCmd* cmd);
    sd_GattcHvConfirmRsp sd_gattc_hv_confirm(sd_GattcHvConfirmCmd* cmd);
    sd_GattcExchangeMtuRequestRsp sd_gattc_exchange_mtu_request(sd_GattcExchangeMtuRequestCmd* cmd);

    // GATTS commands
    sd_GattsServiceAddRsp sd_gatts_service_add(sd_GattsServiceAddCmd* cmd);
    sd_GattsIncludeAddRsp sd_gatts_include_add(sd_GattsIncludeAddCmd* cmd);
    sd_GattsCharacteristicAddRsp sd_gatts_characteristic_add(sd_GattsCharacteristicAddCmd* cmd);
    sd_GattsDescriptorAddRsp sd_gatts_descriptor_add(sd_GattsDescriptorAddCmd* cmd);
    sd_GattsValueSetRsp sd_gatts_value_set(sd_GattsValueSetCmd* cmd);
    sd_GattsValueGetRsp sd_gatts_value_get(sd_GattsValueGetCmd* cmd);
    sd_GattsHvxRsp sd_gatts_hvx(sd_GattsHvxCmd* cmd);
    sd_GattsServiceChangedRsp sd_gatts_service_changed(sd_GattsServiceChangedCmd* cmd);
    sd_GattsRwAuthorizeReplyRsp sd_gatts_rw_authorize_reply(sd_GattsRwAuthorizeReplyCmd* cmd);
    sd_GattsSysAttrSetRsp sd_gatts_sys_attr_set(sd_GattsSysAttrSetCmd* cmd);
    sd_GattsSysAttrGetRsp sd_gatts_sys_attr_get(sd_GattsSysAttrGetCmd* cmd);
    sd_GattsInitialUserHandleGetRsp sd_gatts_initial_user_handle_get(sd_GattsInitialUserHandleGetCmd* cmd);
    sd_GattsAttrGetRsp sd_gatts_attr_get(sd_GattsAttrGetCmd* cmd);
    sd_GattsExchangeMtuReplyRsp sd_gatts_exchange_mtu_reply(sd_GattsExchangeMtuReplyCmd* cmd);


};

}  // namespace Serial
}  // namespace Dongl
}  // namespace SimpleBLE
