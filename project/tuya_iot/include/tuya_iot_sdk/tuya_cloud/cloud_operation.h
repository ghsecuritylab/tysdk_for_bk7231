#ifndef _CLOUD_OPERATION_H_
#define _CLOUD_OPERATION_H_

#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"
//#include "tuya_ipc_media.h"

#define VALUE_LEN       128
#define TOKEN_LEN_MAX   2048
#define FILE_NAME_LEN   64

typedef struct {
    CHAR_T provider[VALUE_LEN];
    CHAR_T ak[VALUE_LEN];
    CHAR_T sk[VALUE_LEN];
    CHAR_T expiration[VALUE_LEN];
    CHAR_T bucket[VALUE_LEN];
    CHAR_T endpoint[VALUE_LEN];
    CHAR_T region[VALUE_LEN];
    CHAR_T media_path[VALUE_LEN];
    CHAR_T content_type[VALUE_LEN];
    CHAR_T log_path[VALUE_LEN];
    CHAR_T detect_path[VALUE_LEN];
    CHAR_T object[VALUE_LEN*2];
    CHAR_T token[TOKEN_LEN_MAX];
    CHAR_T store_mode[VALUE_LEN];
    UINT_T position;
}CLOUD_CONFIG_S;

typedef enum{
    UPLOAD_PATH_MEDIA = 0,
    UPLOAD_PATH_LOG,
    UPLOAD_PATH_DETECT,
    UPLOAD_PATH_MAX,
}CLOUD_UPLOAD_PATH_E;

typedef enum{
    UPLOAD_METHOD_HTTP = 0, /* http */
    UPLOAD_METHOD_HTTPS, /* https */
    UPLOAD_METHOD_MAX,
}CLOUD_UPLOAD_METHOD_E;


OPERATE_RET cloud_get_storage_config(INOUT CLOUD_CONFIG_S *config, IN CONST BOOL_T withvideostorage);
//OPERATE_RET cloud_storage_start_notify(IN INT_T start_time, IN IPC_MEDIA_INFO_S *media_setting, IN UINT_T slice_duration, IN CHAR_T *md5, OUT CHAR_T *new_key);
//OPERATE_RET cloud_storage_stop_notify(IN INT_T start_time,IN INT_T end_time);
//OPERATE_RET cloud_get_storage_encrypt_key(INOUT CHAR_T *encrypt_key);
OPERATE_RET cloud_put_data_to_file(IN CLOUD_CONFIG_S* config,
                                          IN CHAR_T *method,
                                          IN CLOUD_UPLOAD_PATH_E path,
                                          IN CHAR_T *file,
                                          IN CHAR_T* content_type,
                                          IN CHAR_T *data,
                                          IN INT_T len);

OPERATE_RET post_file_to_oss(IN CLOUD_CONFIG_S* config,
                             IN CLOUD_UPLOAD_METHOD_E method,
                             IN CLOUD_UPLOAD_PATH_E path,
                             IN CHAR_T *cFileLocalname,
                             CHAR_T *cFileUploadname,
                             IN CHAR_T* content_type,
                             OUT CHAR_T* logSuf);

OPERATE_RET cloud_append_post_data_to_file(IN CLOUD_CONFIG_S* config,
                                             IN CHAR_T *method,
                                             IN CLOUD_UPLOAD_PATH_E path,
                                             IN CHAR_T *file,
                                             IN CHAR_T* content_type,
                                             IN CHAR_T *data,
                                             IN INT_T len,
                                             IN INT_T position,
                                             IN INT_T sque);

#endif

