/***********************************************************
*  File: action.h
*  Author: nzy
*  Date: 20180718
***********************************************************/
#ifndef __ACTION_H
    #define __ACTION_H

    #include "tuya_cloud_types.h"
    #include "tuya_cloud_com_defs.h"
    #include "mem_pool.h"
    #include "cJSON.h"

#ifdef __cplusplus
	extern "C" {
#endif
    
#ifdef  __ACTION_GLOBALS
    #define __ACTION_EXT
#else
    #define __ACTION_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
#define RULE_ID_LEN 20

typedef BYTE_T ACTION_TP_T;
#define AT_DP       1 // dp issue
#define AT_RULE     2 // rule trigger
#define AT_DELAY    3 // delay event

// dp issue
typedef struct {
    CHAR_T *mlk_dp_cmd; // {"cid":"xxx",dps:{"1":true,"2":100}}
}AT_DP_ISSUE_S;

// rule trigger
typedef struct {
    CHAR_T id[RULE_ID_LEN+1]; //后续改成指针，外部复制
}AT_RULE_TRIGGER_S;

// delay event
typedef struct {
    UINT_T msecond;
}AT_DELAY_S;

typedef union {
    AT_DP_ISSUE_S dp_isu;
    AT_RULE_TRIGGER_S rule_trig;
    AT_DELAY_S delay;
}ACTION_U;

typedef struct {
    ACTION_TP_T ac_tp;
    ACTION_U ac;
}ACTION_S;

// action set 
typedef struct s_action {
    USHORT_T num;
    ACTION_S action[0];
}ACTION_SET_S;

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: actions_parse
*  Input: root
*  Output: cond_set 
*  Return: OPERATE_RET
***********************************************************/
__ACTION_EXT \
OPERATE_RET actions_parse(IN cJSON *root,OUT ACTION_SET_S **act_set);

/***********************************************************
*  Function: free_action_set
*  Input: action_set
*  Output: action_set 
*  Return: OPERATE_RET
***********************************************************/
__ACTION_EXT \
VOID free_action_set(INOUT ACTION_SET_S *action_set);

__ACTION_EXT \
VOID actions_dump_print(ACTION_SET_S *p_act_set);
#ifdef __cplusplus
}
#endif
#endif

