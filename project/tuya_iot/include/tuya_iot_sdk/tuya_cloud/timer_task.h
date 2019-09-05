/***********************************************************
*  File: timer_task.h
***********************************************************/
#ifndef _TIMER_TASK_H
#define _TIMER_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

#define TIMER_TASK_ARR_DB_KEY  "timer_arr"

OPERATE_RET start_update_timer_task(VOID);
OPERATE_RET timer_task_init(UINT_T increase_unit);
#if 0
OPERATE_RET timer_task_release(void);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /*_TIMER_TASK_H*/

