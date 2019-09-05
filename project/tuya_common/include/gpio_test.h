/***********************************************************
*  File: tuya_gpio_test.h
*  Author: lql
*  Date: 20180502
***********************************************************/

typedef BYTE_T BOARD_TYPE; 
#define BK_BOARD_WA1  0
#define BK_BOARD_WA2  1
#define BK_BOARD_WA3  2
#define BK_BOARD_WA2L 3
#define BK_BOARD_MAX  4


BOOL_T gpio_test_cb(BOARD_TYPE type);

BOOL_T gpio_test_all(VOID);