/*
tuya_iot_com_api.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef __TUYA_IOT_COM_API_H
#define __TUYA_IOT_COM_API_H

#include "tuya_cloud_types.h"
#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
*  Function: tuya_iot_get_sdk_info 
*  Desc:     get tuya_iot_sdk build info
*  Return:   sdk info
***********************************************************/
CHAR_T *tuya_iot_get_sdk_info(VOID);

/***********************************************************
*  Function: tuya_iot_init_params
*  Desc:     init tuya_iot_sdk
*  Input:    fs_storge_path: filesystem read write storge path
*            (if os have no fs,then fs_storge_path is invalid)
*  Input:    p_param: custom init params
*  Return:  OPRT_OK: success  Other: fail
***********************************************************/
#define tuya_iot_init(VOID) tuya_iot_init_params(VOID)
OPERATE_RET tuya_iot_init_params(VOID);


/***********************************************************
*  Function: tuya_iot_kv_flash_init
*  Desc:     init kv flash
*  Input:    fs_storge_path: filesystem read write storge path
*            (if os have no fs,then fs_storge_path is invalid)
*  Input:    p_param: custom init params
*  Return:  OPRT_OK: success  Other: fail
***********************************************************/
#define tuya_iot_kv_flash_init(fs_storge_path) tuya_iot_kv_flash_init_param(fs_storge_path,NULL)
OPERATE_RET tuya_iot_kv_flash_init_param(IN CONST CHAR_T *fs_storge_path, IN CONST TY_INIT_PARAMS_S *p_param);
/***********************************************************
*  Function: tuya_iot_oem_set
*  Desc:     set oem mode
*  Input:    oem
*  Note: must call first
***********************************************************/
VOID tuya_iot_oem_set(IN CONST BOOL_T oem);

/***********************************************************
*  Function: tuya_iot_upgrade_gw
*  Desc:     upgrage gateway firmware
*  Input:    fw: firmware info
*  Input:    get_file_cb: download content storage callback
*  Input:    upgrd_nofity_cb: firmware download finish callback
*  Input:    pri_data: private param of get_file_cb && upgrd_nofity_cb
*  Input:    notify: notify app the upgrade status
*  Input:    download_buf_size: max download buffer size when downloading
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
#define tuya_iot_upgrade_gw(fw, get_file_cb, upgrd_nofity_cb, pri_data) \
    tuya_iot_upgrade_gw_notify(fw, get_file_cb, upgrd_nofity_cb, pri_data, TRUE, 0)
OPERATE_RET tuya_iot_upgrade_gw_notify(IN CONST FW_UG_S *fw,
                                       IN CONST GET_FILE_DATA_CB get_file_cb,\
                                       IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,\
                                       IN CONST PVOID_T pri_data,\
                                       BOOL_T notify, UINT_T download_buf_size);

/***********************************************************
*  Function: tuya_iot_gw_module_update
*  Desc:     update gw module version
*  Input:    type: modult type
*  Input:    ver: version
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET tuya_iot_gw_version_update(IN GW_PERMIT_DEV_TP_T type, IN CONST CHAR_T *ver);


/***********************************************************
*  Function: tuya_iot_upgrade_dev
*  Desc:     upgrage sub-device/soc/mcu firmware
*  Input:    devid: if upgrade sub-device, then devid = sub-device_id
*                   if upgrade soc/mcu, then devid = NULL
*  Input:    fw: firmware info
*  Input:    get_file_cb: download content storage callback
*  Input:    upgrd_nofity_cb: firmware download finish callback
*  Input:    pri_data: private param of get_file_cb && upgrd_nofity_cb
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
#define tuya_iot_upgrade_dev(devid, fw, get_file_cb, upgrd_nofity_cb, pri_data) \
    tuya_iot_upgrade_dev_notify(devid, fw, get_file_cb, upgrd_nofity_cb, pri_data, TRUE, 0)
OPERATE_RET tuya_iot_upgrade_dev_notify(IN CONST CHAR_T *devid,
                                        IN CONST FW_UG_S *fw, \
                                        IN CONST GET_FILE_DATA_CB get_file_cb,\
                                        IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,\
                                        IN CONST PVOID_T pri_data,\
                                        BOOL_T notify, UINT_T download_buf_size);

/***********************************************************
*  Function: tuya_iot_get_dev_if
*  Desc:     get sub-device info
*  Input:    devid: if devid = sub-device_id, then return the sub-dev info
*                   if devid = NULL, then return the gateway info
*  Return:   (DEV_DESC_IF_S *): success  NULL: fail
***********************************************************/
DEV_DESC_IF_S *tuya_iot_get_dev_if(IN CONST CHAR_T *dev_id);

/***********************************************************
*  Function: tuya_iot_dev_traversal
*  Desc:     Traversing all devices under the gateway,
*            except the gateway device.should access a data token first.
*  Input:    token
*  Input:    iterator
*  Return:   (DEV_DESC_IF_S *) (NULL means end)
***********************************************************/
DEV_DESC_IF_S *tuya_iot_dev_traversal(INOUT VOID **iterator);

#define DEV_DESC_TRAVERSAL_BEGIN(dev_if) \
do { \
    DEV_DESC_IF_S * dev_if = NULL; \
    VOID *iterator = NULL; \
    do { \
        dev_if = (DEV_DESC_IF_S *)tuya_iot_dev_traversal(&iterator); \
        if(dev_if) {

#define DEV_DESC_TRAVERSAL_FINISH(dev_if) \
        } \
    }while(dev_if); \
}while(0);

#define DEV_DESC_TRAVERSAL_BREAK(dev_if)     break;
#define DEV_DESC_TRAVERSAL_CONTINUE(dev_if)  continue;

/***********************************************************
*  Function: tuya_iot_get_dp_desc
*  Desc:     get dp info of a sub-device and a dp id
*  Input:    dev_id: sub-device id
*  Input:    dpid: dp id
*  Return:   (DP_DESC_IF_S *): success  NULL: fail
***********************************************************/
DP_DESC_IF_S *tuya_iot_get_dp_desc(IN CONST CHAR_T *dev_id,IN CONST BYTE_T dpid);

/***********************************************************
*  Function: tuya_iot_get_dp_prop_value
*  Desc:     get dp value of a sub-device and a dp id
*  Input:    dev_id: sub-device id
*  Input:    dpid: dp id
*  Return:   (DP_PROP_VALUE_U *): success  NULL: fail
***********************************************************/
DP_PROP_VALUE_U *tuya_iot_get_dp_prop_value(IN CONST CHAR_T *dev_id,IN CONST BYTE_T dpid);

/***********************************************************
*  Function: tuya_iot_gw_bind_dev_attr
*  Desc:     bind a sub-device to gateway
*  Input:    tp: sub-device type
*  Input:    uddd: sub-device detail type
*  Input:    id: sub-device id
*  Input:    pk: sub-device product key
*  Input:    ver: sub-device version
*  Input:    attr: sub-device mcu versions
*  Input:    attr_num: sub-device mcu versions
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
#define tuya_iot_gw_bind_dev(tp, uddd, id, pk, ver) \
    tuya_iot_gw_bind_dev_attr(tp, uddd, id, pk, ver, NULL, 0)
OPERATE_RET tuya_iot_gw_bind_dev_attr(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST USER_DEV_DTL_DEF_T uddd,\
                                      IN CONST CHAR_T *id,IN CONST CHAR_T *pk,IN CONST CHAR_T *ver, \
                                      IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num);

/***********************************************************
*  Function: tuya_iot_gw_subdevice_update
*  Desc:     update sub-device version
*  Input:    id: sub-device id
*  Input:    ver: sub-device version
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
#define tuya_iot_gw_subdevice_update(id, ver) \
    tuya_iot_gw_subdevice_update_versions(id, ver, NULL, 0)
OPERATE_RET tuya_iot_gw_subdevice_update_versions(IN CONST CHAR_T *id,IN CONST CHAR_T *ver,
                                                  IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num);

/***********************************************************
*  Function: tuya_iot_dev_upgd_progress_rept
*  Input: percent id tp
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_dev_upgd_progress_rept(IN CONST UINT_T percent, IN CONST CHAR_T *devid, IN CONST DEV_TYPE_T tp);

/***********************************************************
*  Function: tuya_iot_gw_unbind_dev
*  Desc:     unbind a sub-device from gateway
*  Input:    id: sub-device id
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET tuya_iot_gw_unbind_dev(IN CONST CHAR_T *id);

/***********************************************************
*  Function: tuya_iot_dev_online_stat_update
*  Desc:     update sub-device online state
*  Input:    dev_id: sub-device id
*  Input:    online: online state
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET tuya_iot_dev_online_stat_update(IN CONST CHAR_T *dev_id,IN CONST BOOL_T online);

/***********************************************************
*  Function: tuya_iot_sys_mag_hb_init
*  Desc:     enable gateway hearbeat check mechanism.if enabled,
*            gateway will check all the sub-devices every 3000 ms,
*            if sub-device not send a heartbeat to gateway during
*            its heartbeat timeout time. the gateway will make
*            the subdevice offline,and notify user by hb_send_cb
*            for at most 3 times.
*  Input:    hb_send_cb: sub-device_id
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET tuya_iot_sys_mag_hb_init(IN CONST DEV_HEARTBEAT_SEND_CB hb_send_cb);

/***********************************************************
*  Function: tuya_iot_set_dev_hb_timeout_cfg
*  Desc:     set sub-device heartbeat timeout param.
*            if hb_timeout_time==0xffffffff, this sub-device
*            will skip the heartbeat check, always online
*  Input:    devid: sub-device_id
*  Input:    hb_timeout_time: heartbeat timeout
*  Input:    max_resend_times: max resend times
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
#define tuya_iot_set_dev_hb_timeout(dev_id, hb_timeout_time) \
    tuya_iot_set_dev_hb_timeout_cfg(dev_id, hb_timeout_time, 2)
OPERATE_RET tuya_iot_set_dev_hb_timeout_cfg(IN CONST CHAR_T *dev_id,IN CONST TIME_S hb_timeout_time, IN CONST UINT_T max_resend_times);

/***********************************************************
*  Function: tuya_iot_fresh_dev_hb
*  Desc:     gateway recv a sub-device heartbeat info
*  Input:    devid: sub-device_id
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET tuya_iot_fresh_dev_hb(IN CONST CHAR_T *dev_id);

/***********************************************************
*  Function: dev_report_dp_json_async
*  Desc:     report dp info a-synced.
*  Input:    devid: if sub-device, then devid = sub-device_id
*                   if gateway/soc/mcu, then devid = NULL
*  Input:    dp_data: dp array header
*  Input:    cnt    : dp array count
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt);

/***********************************************************
*  Function: dev_report_dp_raw_sync
*  Desc:     report dp raw info synced.
*  Input:    devid: if sub-device, then devid = sub-device_id
*                   if gateway/soc/mcu, then devid = NULL
*  Input:    dpid: raw dp id
*  Input:    data: raw data
*  Input:    len : len of raw data
*  Input:    timeout: function blocks until timeout seconds
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
#define dev_report_dp_raw_sync(dev_id, dpid, data, len, timeout) \
    dev_report_dp_raw_sync_extend(dev_id, dpid, data, len, timeout, TRUE)
OPERATE_RET dev_report_dp_raw_sync_extend(IN CONST CHAR_T *dev_id,IN CONST BYTE_T dpid,\
                                                      IN CONST BYTE_T *data,IN CONST UINT_T len,\
                                                      IN CONST UINT_T timeout, IN CONST BOOL_T enable_auto_retrans);

/***********************************************************
*  Function: dev_report_dp_stat_sync
*  Desc:     report dp status info synced.
*            if time_stamp==0, time_stamp = time of msg arrival of the server
*  Input:    devid: if sub-device, then devid = sub-device_id
*                   if gateway/soc/mcu, then devid = NULL
*  Input:    dp_data: dp status array header
*  Input:    cnt    : dp status array count
*  Input:    timeout: function blocks until timeout seconds
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
#define dev_report_dp_stat_sync(dev_id, dp_data, cnt, timeout) \
    dev_report_dp_stat_sync_extend(dev_id, dp_data, cnt, timeout, TRUE)
OPERATE_RET dev_report_dp_stat_sync_extend(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,\
                                    IN CONST UINT_T cnt,IN CONST UINT_T timeout, IN CONST BOOL_T enable_auto_retrans);

/***********************************************************
*  Function: tuya_iot_get_wakeup_data
*  Desc:     get mqtt wakeup string pattern
*  Output:   wakeup_data_arr: wakeup string arr
*  Output:   p_len: wakeup string len
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET tuya_iot_get_wakeup_data(INOUT BYTE_T *wakeup_data_arr, INOUT UINT_T *p_len);

/***********************************************************
*  Function: tuya_iot_get_heartbeat_data
*  Desc:     get mqtt heartbeat string pattern
*  Output:   heartbeat_data_arr: heartbeat string arr
*  Output:   p_len: heartbeat string len
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET tuya_iot_get_heartbeat_data(INOUT BYTE_T *heartbeat_data_arr, INOUT UINT_T *p_len);

/***********************************************************
*  Function: tuya_iot_book_wakeup_topic
*  Desc:     unscribe mqtt msg topic and subscribe wakeup topic
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET tuya_iot_book_wakeup_topic(VOID);

/***********************************************************
*  Function: tuya_iot_get_mqtt_socket_fd
*  Desc:     get curr mqtt socket fd
*  Output:   mqtt socket fd
***********************************************************/
INT_T tuya_iot_get_mqtt_socket_fd(VOID);

/***********************************************************
*  Function: tuya_iot_send_custom_mqtt_msg
*  Desc:     send a custom protocol mqtt msg
*  Input:    protocol: mqtt protocol
*  Input:    p_data: mqtt msg
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET tuya_iot_send_custom_mqtt_msg(IN CONST UINT_T protocol, IN CONST BYTE_T *p_data);
/***********************************************************
*  Function: tuya_iot_media_init
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_media_init(VOID);

/***********************************************************
*  Function: tuya_iot_media_data_report
*  Input: dt_body->media data
*         timeout->need report time
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_media_data_report(IN CONST FLOW_BODY_ST *dt_body,IN CONST UINT_T timeout);

/***********************************************************
*  Function: tuya_iot_get_gw_id 
*  Input: none
*  Output: none
*  Return: (CHAR_T *)->device id
***********************************************************/
CHAR_T *tuya_iot_get_gw_id(VOID);

#ifdef __cplusplus
}
#endif

#endif
