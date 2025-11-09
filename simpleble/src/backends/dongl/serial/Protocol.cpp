#include "Protocol.h"

using namespace SimpleBLE::Dongl::Serial;

Protocol::Protocol(const std::string& device_path) : ProtocolBase(device_path) {}

Protocol::~Protocol() {}

basic_WhoamiRsp Protocol::basic_whoami() {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_basic_tag;
    command.cmd.basic.which_cmd = basic_Command_whoami_tag;
    command.cmd.basic.cmd.whoami = basic_WhoamiCmd_init_default;

    dongl_Response response = exchange(command);
    return response.rsp.basic.rsp.whoami;
}

basic_ResetRsp Protocol::basic_reset() {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_basic_tag;
    command.cmd.basic.which_cmd = basic_Command_reset_tag;
    command.cmd.basic.cmd.reset = basic_ResetCmd_init_default;

    dongl_Response response = exchange(command);
    return response.rsp.basic.rsp.reset;
}

basic_DfuStartRsp Protocol::basic_dfu_start() {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_basic_tag;
    command.cmd.basic.which_cmd = basic_Command_dfu_start_tag;
    command.cmd.basic.cmd.dfu_start = basic_DfuStartCmd_init_default;

    dongl_Response response = exchange(command);
    return response.rsp.basic.rsp.dfu_start;
}

simpleble_InitRsp Protocol::simpleble_init() {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_simpleble_tag;
    command.cmd.simpleble.which_cmd = simpleble_Command_init_tag;
    command.cmd.simpleble.cmd.init = simpleble_InitCmd_init_default;

    dongl_Response response = exchange(command);
    return response.rsp.simpleble.rsp.init;
}

// GAP commands
sd_GapAddrSetRsp Protocol::sd_gap_addr_set(sd_GapAddrSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_addr_set_tag;
    command.cmd.softdevice.cmd.gap_addr_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_addr_set;
}

sd_GapAddrGetRsp Protocol::sd_gap_addr_get(sd_GapAddrGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_addr_get_tag;
    command.cmd.softdevice.cmd.gap_addr_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_addr_get;
}

sd_GapWhitelistSetRsp Protocol::sd_gap_whitelist_set(sd_GapWhitelistSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_whitelist_set_tag;
    command.cmd.softdevice.cmd.gap_whitelist_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_whitelist_set;
}

sd_GapDeviceIdentitiesSetRsp Protocol::sd_gap_device_identities_set(sd_GapDeviceIdentitiesSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_device_identities_set_tag;
    command.cmd.softdevice.cmd.gap_device_identities_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_device_identities_set;
}

sd_GapPrivacySetRsp Protocol::sd_gap_privacy_set(sd_GapPrivacySetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_privacy_set_tag;
    command.cmd.softdevice.cmd.gap_privacy_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_privacy_set;
}

sd_GapPrivacyGetRsp Protocol::sd_gap_privacy_get(sd_GapPrivacyGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_privacy_get_tag;
    command.cmd.softdevice.cmd.gap_privacy_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_privacy_get;
}

sd_GapAdvSetConfigureRsp Protocol::sd_gap_adv_set_configure(sd_GapAdvSetConfigureCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_adv_set_configure_tag;
    command.cmd.softdevice.cmd.gap_adv_set_configure = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_adv_set_configure;
}

sd_GapAdvStartRsp Protocol::sd_gap_adv_start(sd_GapAdvStartCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_adv_start_tag;
    command.cmd.softdevice.cmd.gap_adv_start = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_adv_start;
}

sd_GapAdvStopRsp Protocol::sd_gap_adv_stop(sd_GapAdvStopCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_adv_stop_tag;
    command.cmd.softdevice.cmd.gap_adv_stop = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_adv_stop;
}

sd_GapConnParamUpdateRsp Protocol::sd_gap_conn_param_update(sd_GapConnParamUpdateCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_conn_param_update_tag;
    command.cmd.softdevice.cmd.gap_conn_param_update = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_conn_param_update;
}

sd_GapDisconnectRsp Protocol::sd_gap_disconnect(sd_GapDisconnectCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_disconnect_tag;
    command.cmd.softdevice.cmd.gap_disconnect = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_disconnect;
}

sd_GapTxPowerSetRsp Protocol::sd_gap_tx_power_set(sd_GapTxPowerSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_tx_power_set_tag;
    command.cmd.softdevice.cmd.gap_tx_power_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_tx_power_set;
}

sd_GapAppearanceSetRsp Protocol::sd_gap_appearance_set(sd_GapAppearanceSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_appearance_set_tag;
    command.cmd.softdevice.cmd.gap_appearance_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_appearance_set;
}

sd_GapAppearanceGetRsp Protocol::sd_gap_appearance_get(sd_GapAppearanceGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_appearance_get_tag;
    command.cmd.softdevice.cmd.gap_appearance_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_appearance_get;
}

sd_GapPpcpSetRsp Protocol::sd_gap_ppcp_set(sd_GapPpcpSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_ppcp_set_tag;
    command.cmd.softdevice.cmd.gap_ppcp_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_ppcp_set;
}

sd_GapPpcpGetRsp Protocol::sd_gap_ppcp_get(sd_GapPpcpGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_ppcp_get_tag;
    command.cmd.softdevice.cmd.gap_ppcp_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_ppcp_get;
}

sd_GapDeviceNameSetRsp Protocol::sd_gap_device_name_set(sd_GapDeviceNameSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_device_name_set_tag;
    command.cmd.softdevice.cmd.gap_device_name_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_device_name_set;
}

sd_GapDeviceNameGetRsp Protocol::sd_gap_device_name_get(sd_GapDeviceNameGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_device_name_get_tag;
    command.cmd.softdevice.cmd.gap_device_name_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_device_name_get;
}

sd_GapAuthenticateRsp Protocol::sd_gap_authenticate(sd_GapAuthenticateCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_authenticate_tag;
    command.cmd.softdevice.cmd.gap_authenticate = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_authenticate;
}

sd_GapSecParamsReplyRsp Protocol::sd_gap_sec_params_reply(sd_GapSecParamsReplyCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_sec_params_reply_tag;
    command.cmd.softdevice.cmd.gap_sec_params_reply = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_sec_params_reply;
}

sd_GapAuthKeyReplyRsp Protocol::sd_gap_auth_key_reply(sd_GapAuthKeyReplyCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_auth_key_reply_tag;
    command.cmd.softdevice.cmd.gap_auth_key_reply = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_auth_key_reply;
}

sd_GapLescDhkeyReplyRsp Protocol::sd_gap_lesc_dhkey_reply(sd_GapLescDhkeyReplyCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_lesc_dhkey_reply_tag;
    command.cmd.softdevice.cmd.gap_lesc_dhkey_reply = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_lesc_dhkey_reply;
}

sd_GapKeypressNotifyRsp Protocol::sd_gap_keypress_notify(sd_GapKeypressNotifyCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_keypress_notify_tag;
    command.cmd.softdevice.cmd.gap_keypress_notify = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_keypress_notify;
}

sd_GapLescOobDataGetRsp Protocol::sd_gap_lesc_oob_data_get(sd_GapLescOobDataGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_lesc_oob_data_get_tag;
    command.cmd.softdevice.cmd.gap_lesc_oob_data_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_lesc_oob_data_get;
}

sd_GapLescOobDataSetRsp Protocol::sd_gap_lesc_oob_data_set(sd_GapLescOobDataSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_lesc_oob_data_set_tag;
    command.cmd.softdevice.cmd.gap_lesc_oob_data_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_lesc_oob_data_set;
}

sd_GapEncryptRsp Protocol::sd_gap_encrypt(sd_GapEncryptCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_encrypt_tag;
    command.cmd.softdevice.cmd.gap_encrypt = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_encrypt;
}

sd_GapSecInfoReplyRsp Protocol::sd_gap_sec_info_reply(sd_GapSecInfoReplyCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_sec_info_reply_tag;
    command.cmd.softdevice.cmd.gap_sec_info_reply = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_sec_info_reply;
}

sd_GapConnSecGetRsp Protocol::sd_gap_conn_sec_get(sd_GapConnSecGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_conn_sec_get_tag;
    command.cmd.softdevice.cmd.gap_conn_sec_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_conn_sec_get;
}

sd_GapRssiStartRsp Protocol::sd_gap_rssi_start(sd_GapRssiStartCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_rssi_start_tag;
    command.cmd.softdevice.cmd.gap_rssi_start = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_rssi_start;
}

sd_GapRssiStopRsp Protocol::sd_gap_rssi_stop(sd_GapRssiStopCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_rssi_stop_tag;
    command.cmd.softdevice.cmd.gap_rssi_stop = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_rssi_stop;
}

sd_GapScanStartRsp Protocol::sd_gap_scan_start(sd_GapScanStartCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_scan_start_tag;
    command.cmd.softdevice.cmd.gap_scan_start = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_scan_start;
}

sd_GapScanStopRsp Protocol::sd_gap_scan_stop(sd_GapScanStopCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_scan_stop_tag;
    command.cmd.softdevice.cmd.gap_scan_stop = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_scan_stop;
}

sd_GapConnectRsp Protocol::sd_gap_connect(sd_GapConnectCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_connect_tag;
    command.cmd.softdevice.cmd.gap_connect = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_connect;
}

sd_GapConnectCancelRsp Protocol::sd_gap_connect_cancel(sd_GapConnectCancelCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_connect_cancel_tag;
    command.cmd.softdevice.cmd.gap_connect_cancel = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_connect_cancel;
}

sd_GapRssiGetRsp Protocol::sd_gap_rssi_get(sd_GapRssiGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_rssi_get_tag;
    command.cmd.softdevice.cmd.gap_rssi_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_rssi_get;
}

sd_GapPhyUpdateRsp Protocol::sd_gap_phy_update(sd_GapPhyUpdateCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_phy_update_tag;
    command.cmd.softdevice.cmd.gap_phy_update = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_phy_update;
}

sd_GapDataLengthUpdateRsp Protocol::sd_gap_data_length_update(sd_GapDataLengthUpdateCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_data_length_update_tag;
    command.cmd.softdevice.cmd.gap_data_length_update = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_data_length_update;
}

sd_GapQosChannelSurveyStartRsp Protocol::sd_gap_qos_channel_survey_start(sd_GapQosChannelSurveyStartCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_qos_channel_survey_start_tag;
    command.cmd.softdevice.cmd.gap_qos_channel_survey_start = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_qos_channel_survey_start;
}

sd_GapQosChannelSurveyStopRsp Protocol::sd_gap_qos_channel_survey_stop(sd_GapQosChannelSurveyStopCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_qos_channel_survey_stop_tag;
    command.cmd.softdevice.cmd.gap_qos_channel_survey_stop = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_qos_channel_survey_stop;
}

sd_GapAdvAddrGetRsp Protocol::sd_gap_adv_addr_get(sd_GapAdvAddrGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_adv_addr_get_tag;
    command.cmd.softdevice.cmd.gap_adv_addr_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_adv_addr_get;
}

sd_GapNextConnEvtCounterGetRsp Protocol::sd_gap_next_conn_evt_counter_get(sd_GapNextConnEvtCounterGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_next_conn_evt_counter_get_tag;
    command.cmd.softdevice.cmd.gap_next_conn_evt_counter_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_next_conn_evt_counter_get;
}

sd_GapConnEvtTriggerStartRsp Protocol::sd_gap_conn_evt_trigger_start(sd_GapConnEvtTriggerStartCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_conn_evt_trigger_start_tag;
    command.cmd.softdevice.cmd.gap_conn_evt_trigger_start = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_conn_evt_trigger_start;
}

sd_GapConnEvtTriggerStopRsp Protocol::sd_gap_conn_evt_trigger_stop(sd_GapConnEvtTriggerStopCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gap_conn_evt_trigger_stop_tag;
    command.cmd.softdevice.cmd.gap_conn_evt_trigger_stop = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gap_conn_evt_trigger_stop;
}

// GATTC commands
sd_GattcPrimaryServicesDiscoverRsp Protocol::sd_gattc_primary_services_discover(sd_GattcPrimaryServicesDiscoverCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_primary_services_discover_tag;
    command.cmd.softdevice.cmd.gattc_primary_services_discover = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_primary_services_discover;
}

sd_GattcRelationshipsDiscoverRsp Protocol::sd_gattc_relationships_discover(sd_GattcRelationshipsDiscoverCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_relationships_discover_tag;
    command.cmd.softdevice.cmd.gattc_relationships_discover = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_relationships_discover;
}

sd_GattcCharacteristicsDiscoverRsp Protocol::sd_gattc_characteristics_discover(sd_GattcCharacteristicsDiscoverCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_characteristics_discover_tag;
    command.cmd.softdevice.cmd.gattc_characteristics_discover = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_characteristics_discover;
}

sd_GattcDescriptorsDiscoverRsp Protocol::sd_gattc_descriptors_discover(sd_GattcDescriptorsDiscoverCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_descriptors_discover_tag;
    command.cmd.softdevice.cmd.gattc_descriptors_discover = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_descriptors_discover;
}

sd_GattcAttrInfoDiscoverRsp Protocol::sd_gattc_attr_info_discover(sd_GattcAttrInfoDiscoverCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_attr_info_discover_tag;
    command.cmd.softdevice.cmd.gattc_attr_info_discover = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_attr_info_discover;
}

sd_GattcCharValueByUuidReadRsp Protocol::sd_gattc_char_value_by_uuid_read(sd_GattcCharValueByUuidReadCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_char_value_by_uuid_read_tag;
    command.cmd.softdevice.cmd.gattc_char_value_by_uuid_read = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_char_value_by_uuid_read;
}

sd_GattcReadRsp Protocol::sd_gattc_read(sd_GattcReadCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_read_tag;
    command.cmd.softdevice.cmd.gattc_read = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_read;
}

sd_GattcCharValuesReadRsp Protocol::sd_gattc_char_values_read(sd_GattcCharValuesReadCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_char_values_read_tag;
    command.cmd.softdevice.cmd.gattc_char_values_read = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_char_values_read;
}

sd_GattcWriteRsp Protocol::sd_gattc_write(sd_GattcWriteCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_write_tag;
    command.cmd.softdevice.cmd.gattc_write = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_write;
}

sd_GattcHvConfirmRsp Protocol::sd_gattc_hv_confirm(sd_GattcHvConfirmCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_hv_confirm_tag;
    command.cmd.softdevice.cmd.gattc_hv_confirm = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_hv_confirm;
}

sd_GattcExchangeMtuRequestRsp Protocol::sd_gattc_exchange_mtu_request(sd_GattcExchangeMtuRequestCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gattc_exchange_mtu_request_tag;
    command.cmd.softdevice.cmd.gattc_exchange_mtu_request = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gattc_exchange_mtu_request;
}

// GATTS commands
sd_GattsServiceAddRsp Protocol::sd_gatts_service_add(sd_GattsServiceAddCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_service_add_tag;
    command.cmd.softdevice.cmd.gatts_service_add = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_service_add;
}

sd_GattsIncludeAddRsp Protocol::sd_gatts_include_add(sd_GattsIncludeAddCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_include_add_tag;
    command.cmd.softdevice.cmd.gatts_include_add = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_include_add;
}

sd_GattsCharacteristicAddRsp Protocol::sd_gatts_characteristic_add(sd_GattsCharacteristicAddCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_characteristic_add_tag;
    command.cmd.softdevice.cmd.gatts_characteristic_add = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_characteristic_add;
}

sd_GattsDescriptorAddRsp Protocol::sd_gatts_descriptor_add(sd_GattsDescriptorAddCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_descriptor_add_tag;
    command.cmd.softdevice.cmd.gatts_descriptor_add = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_descriptor_add;
}

sd_GattsValueSetRsp Protocol::sd_gatts_value_set(sd_GattsValueSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_value_set_tag;
    command.cmd.softdevice.cmd.gatts_value_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_value_set;
}

sd_GattsValueGetRsp Protocol::sd_gatts_value_get(sd_GattsValueGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_value_get_tag;
    command.cmd.softdevice.cmd.gatts_value_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_value_get;
}

sd_GattsHvxRsp Protocol::sd_gatts_hvx(sd_GattsHvxCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_hvx_tag;
    command.cmd.softdevice.cmd.gatts_hvx = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_hvx;
}

sd_GattsServiceChangedRsp Protocol::sd_gatts_service_changed(sd_GattsServiceChangedCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_service_changed_tag;
    command.cmd.softdevice.cmd.gatts_service_changed = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_service_changed;
}

sd_GattsRwAuthorizeReplyRsp Protocol::sd_gatts_rw_authorize_reply(sd_GattsRwAuthorizeReplyCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_rw_authorize_reply_tag;
    command.cmd.softdevice.cmd.gatts_rw_authorize_reply = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_rw_authorize_reply;
}

sd_GattsSysAttrSetRsp Protocol::sd_gatts_sys_attr_set(sd_GattsSysAttrSetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_sys_attr_set_tag;
    command.cmd.softdevice.cmd.gatts_sys_attr_set = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_sys_attr_set;
}

sd_GattsSysAttrGetRsp Protocol::sd_gatts_sys_attr_get(sd_GattsSysAttrGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_sys_attr_get_tag;
    command.cmd.softdevice.cmd.gatts_sys_attr_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_sys_attr_get;
}

sd_GattsInitialUserHandleGetRsp Protocol::sd_gatts_initial_user_handle_get(sd_GattsInitialUserHandleGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_initial_user_handle_get_tag;
    command.cmd.softdevice.cmd.gatts_initial_user_handle_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_initial_user_handle_get;
}

sd_GattsAttrGetRsp Protocol::sd_gatts_attr_get(sd_GattsAttrGetCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_attr_get_tag;
    command.cmd.softdevice.cmd.gatts_attr_get = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_attr_get;
}

sd_GattsExchangeMtuReplyRsp Protocol::sd_gatts_exchange_mtu_reply(sd_GattsExchangeMtuReplyCmd* cmd) {
    dongl_Command command = dongl_Command_init_zero;
    command.which_cmd = dongl_Command_softdevice_tag;
    command.cmd.softdevice.which_cmd = sd_Command_gatts_exchange_mtu_reply_tag;
    command.cmd.softdevice.cmd.gatts_exchange_mtu_reply = *cmd;

    dongl_Response response = exchange(command);
    return response.rsp.softdevice.rsp.gatts_exchange_mtu_reply;
}
