#ifndef _RAW_STORGE_H
#define _RAW_STORGE_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
	extern "C" {
#endif

/* 初始化raw存储模块 */
OPERATE_RET raw_storage_init(IN CONST CHAR_T *p_attr);

/* 导入raw分配cfg */
OPERATE_RET raw_storage_import_cfg(IN CONST CHAR_T *p_input);

/* 导出raw分配cfg */
OPERATE_RET raw_storage_export_cfg(OUT CHAR_T **p_p_output);

/* 释放导出的raw分配cfg字符串 */
OPERATE_RET raw_storage_free_export_cfg_str(IN CHAR_T *p_str);

/* 添加一个raw分配cfg */
OPERATE_RET raw_storage_add_cfg(IN CONST CHAR_T *p_key, IN CONST UINT_T req_size);

/* 查询一个raw分配cfg */
OPERATE_RET raw_storage_get_cfg(IN CONST CHAR_T *p_key, INOUT UINT_T *p_req_size);

/* 删除一个raw分配cfg */
OPERATE_RET raw_storage_delete_cfg(IN CONST CHAR_T *p_key, IN CONST BOOL_T fuzzy);

/* 写raw数据 */
OPERATE_RET raw_storage_write(IN CONST CHAR_T *p_key, IN CONST UINT_T write_len, IN CONST BYTE_T *write_data);

/* 读raw数据 */
OPERATE_RET raw_storage_read(IN CONST CHAR_T *p_key, IN CONST UINT_T read_len, INOUT BYTE_T *read_data);

/* 打印cfg数据 */
VOID raw_storage_dump_cfgs(VOID);

/* 去初始化 */
VOID raw_storage_uninit(VOID);


#ifdef __cplusplus
}
#endif
#endif

