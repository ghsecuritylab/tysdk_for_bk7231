/***********************************************************
*  File: tuya_gpio_test.c
*  Author: lql
*  Date: 20180502
***********************************************************/

#include "adapter_platform.h"
#include "tuya_gpio.h"
#include "uni_log.h"
#include "gpio_test.h"


typedef struct
{
    INT_T   ionum;
    TY_GPIO_PORT_E iopin[8];
}CTRL_GROUP;

typedef struct
{
    INT_T group_num;
    CTRL_GROUP *group;
}GPIO_TEST_TABLE;

STATIC CTRL_GROUP groups_wa1[] = 
{
#if 1
    // group 0
    {
        .ionum  = 3,
        .iopin = {TY_GPIOA_7, TY_GPIOA_18, TY_GPIOA_19}
    },

     // group 1
    {
        .ionum  = 2,
        .iopin = {TY_GPIOA_6, TY_GPIOA_9}
    },

     // group 2
    {
        .ionum  = 2,
        .iopin = {TY_GPIOA_4, TY_GPIOA_8}
    },

     // group 3
    {
        .ionum  = 0,
        .iopin = {}
    },
#endif

};

STATIC CTRL_GROUP groups_wa2[] = 
{
#if 1
    // group 0
    {
        .ionum  = 2,
        .iopin = {TY_GPIOA_6, TY_GPIOA_18}
    },

     // group 1
    {
        .ionum  = 2,
        .iopin = {TY_GPIOA_8, TY_GPIOA_19}
    },

     // group 2
    {
        .ionum  = 2,
        .iopin = {TY_GPIOA_4, TY_GPIOA_7}
    },

     // group 3
    {
        .ionum  = 0,
        .iopin = {}
    },
#endif

};

STATIC CTRL_GROUP groups_wa3[] = 
{
#if 1
    // group 0
    {
        .ionum  = 3,
        .iopin = {TY_GPIOA_8, TY_GPIOA_18, TY_GPIOA_28}
    },

     // group 1
    {
        .ionum  = 3,
        .iopin = {TY_GPIOA_6, TY_GPIOA_7, TY_GPIOA_9}
    },

     // group 2
    {
        .ionum  = 2,
        .iopin = {TY_GPIOA_4, TY_GPIOA_19}
    },

     // group 3
    {
        .ionum  = 0,
        .iopin = {}
    },
#endif
};
STATIC CTRL_GROUP groups_wa2l[] = 
{
    // group 0
    {
        .ionum  = 2,
        .iopin = {TY_GPIOA_6, TY_GPIOA_18}
    },

     // group 1
    {
        .ionum  = 3,
        .iopin = {TY_GPIOA_8, TY_GPIOA_19}
    },

     // group 2
    {
        .ionum  = 0,
        .iopin = {}
    },

     // group 3
    {
        .ionum  = 0,
        .iopin = {}
    },
};



STATIC GPIO_TEST_TABLE gpio_test_table = {
    .group_num = 4,
    .group = NULL
};

BOOL_T gpio_test_cb(BOARD_TYPE type)
{
    if((type < BK_BOARD_WA1) || (type >= BK_BOARD_MAX)) {
        PR_ERR("board type is out of range[%d]",type);
        return FALSE;
    }

    if(type == BK_BOARD_WA1) {
        gpio_test_table.group = groups_wa1;
    }else if(type == BK_BOARD_WA2) {
        gpio_test_table.group = groups_wa2;
    }else if(type == BK_BOARD_WA3) {
        gpio_test_table.group = groups_wa3;
    }else if(type == BK_BOARD_WA2L) {
        gpio_test_table.group = groups_wa2l;
    }

    INT_T idx,i,j;
    for(idx = 0; idx < gpio_test_table.group_num; idx++) {
        for(i = 0; i < gpio_test_table.group[idx].ionum; i++) {
            //set io direction
            
            for(j = 0; j < gpio_test_table.group[idx].ionum; j++) {
                if(i== j) {
                    tuya_gpio_inout_set(gpio_test_table.group[idx].iopin[j],FALSE);
                }else {
                    tuya_gpio_inout_set(gpio_test_table.group[idx].iopin[j],TRUE);
                }
            }
            // write 1
            tuya_gpio_write(gpio_test_table.group[idx].iopin[i],TRUE);
            for(j = 0; j < gpio_test_table.group[idx].ionum; j++) {
                if(i!= j) {
                    if(tuya_gpio_read(gpio_test_table.group[idx].iopin[j]) != 1) {
                        PR_ERR("[%d]gpio test err_high i = %d,j = %d",idx,i,j);
                        return FALSE;
                    }
                }
            }

            // write 0
            tuya_gpio_write(gpio_test_table.group[idx].iopin[i],FALSE);
            for(j = 0; j < gpio_test_table.group[idx].ionum; j++) {
                if(i!= j) {
                    if(tuya_gpio_read(gpio_test_table.group[idx].iopin[j]) != 0) {
                        PR_ERR("[%d]gpio test err_lowi = %d,j = %d",idx,i,j);
                        return FALSE;
                    }
                }
            }
            
        }
    }

    return TRUE;
}

BOOL_T gpio_test_all(VOID)
{
    return  gpio_test_cb(BK_BOARD_WA1) || gpio_test_cb(BK_BOARD_WA2) || gpio_test_cb(BK_BOARD_WA3) || gpio_test_cb(BK_BOARD_WA2L);
}

