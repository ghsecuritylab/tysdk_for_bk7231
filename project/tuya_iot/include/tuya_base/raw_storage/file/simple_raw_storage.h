#ifndef _SIMPLE_RAW_STORAGE_H_
#define _SIMPLE_RAW_STORAGE_H_

#include "adapter_platform.h"

#ifdef __cplusplus
extern "C" {
#endif


OPERATE_RET simple_raw_storage_init(IN CONST CHAR_T *p_path, OUT UINT_T *p_totalsize);

OPERATE_RET simple_raw_storage_deinit(VOID);

OPERATE_RET simple_raw_storage_write(IN CONST UINT_T write_offset, IN CONST UINT_T write_len, IN CONST BYTE_T *write_data);

OPERATE_RET simple_raw_storage_read(IN CONST UINT_T read_offset, IN CONST UINT_T read_len, INOUT BYTE_T *read_data);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*_SIMPLE_RAW_STORAGE_H_*/
