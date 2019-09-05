/***********************************************************
*  File: tuya_main.c
*  Author: nzy
*  Date: 20171012
***********************************************************/
#include "adapter_platform.h"
#include "tuya_iot_wifi_api.h"
#include "mf_test.h"
#include "tuya_uart.h"
#include "tuya_gpio.h"
#include "gw_intf.h"
#include "wf_basic_intf.h"
#include "BkDriverUart.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
#define TEST_SSID "tuya_mdev_test1"
#define WF_SSID_LEN 32
#define UG_PKG_HEAD 0x55aa55aa
#define UG_PKG_TAIL 0xaa55aa55
#define UG_START_ADDR 0x132000   //892k  0x101000------0x1FE000
#define OPRT_WR_FLASH_ERROR (-1005)
#define RT_IMG_WR_UNIT 512

/***********************************************************
*************************variable define********************
***********************************************************/
typedef enum {
    UGS_RECV_HEADER = 0,
    UGS_RECV_IMG_DATA,
    UGS_FINISH
}UG_STAT_E;
typedef struct
{
    u32 header_flag;//0xaa55aa55
    char sw_version[12];//sofrware version
    u32 bin_len;
    u32 bin_sum;
    u32 head_sum;//header_flag + sw_version + bin_len + bin_sum
    u32 tail_flag;//0x55aa55aa
}UPDATE_FILE_HDR_S;
typedef struct {
    UPDATE_FILE_HDR_S file_header;
    u32 flash_addr;
    u32 start_addr;
    u32 recv_data_cnt;
    UG_STAT_E stat;
}UG_PROC_S;

typedef VOID (*APP_PROD_CB)(BOOL_T flag, CHAR_T rssi);
STATIC APP_PROD_CB app_prod_test = NULL;
STATIC GW_WF_CFG_MTHD_SEL gwcm_mode = GWCM_OLD;
STATIC CHAR_T prod_ssid_name[WF_SSID_LEN + 1] = TEST_SSID;
static UG_PROC_S *ug_proc = NULL;

/***********************************************************
*************************function define********************
***********************************************************/
extern VOID app_init(VOID);
extern VOID pre_device_init(VOID);
extern OPERATE_RET device_init(VOID);
extern BOOL_T gpio_test(VOID);
extern VOID mf_user_callback(VOID);
extern void extended_app_waiting_for_launch(void);
extern TY_GPIO_PORT_E swith_ctl_port;
STATIC VOID __gw_ug_inform_cb(INOUT BOOL_T *handled, IN CONST FW_UG_S *fw);
STATIC OPERATE_RET __gw_upgrage_process_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len,IN CONST UINT_T offset,\
                                                      IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len, IN PVOID_T pri_data);
STATIC VOID __gw_upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data);

STATIC OPERATE_RET __mf_gw_upgrade_notify_cb(VOID);
STATIC VOID __mf_gw_ug_inform_cb(UINT_T file_size, UINT_T file_crc);

#define TY_UART TY_UART1
STATIC VOID __tuya_mf_uart_init(UINT_T baud,UINT_T bufsz)
{
    ty_uart_init(TY_UART,baud,TYWL_8B,TYP_NONE,TYS_STOPBIT1,bufsz,TRUE);
}
STATIC VOID __tuya_mf_uart_free(VOID)
{
    ty_uart_free(TY_UART);
}

STATIC VOID __tuya_mf_send(IN CONST BYTE_T *data,IN CONST UINT_T len)
{
    ty_uart_send_data(TY_UART,data,len);
}

STATIC UINT_T __tuya_mf_recv(OUT BYTE_T *buf,IN CONST UINT_T len)
{
    return ty_uart_read_data(TY_UART,buf,len);
}
STATIC BOOL_T scan_test_ssid(VOID)
{
    //if(FALSE == get_new_prod_mode()) {
        //return false;
    //}

    OPERATE_RET op_ret = OPRT_OK;
    //special for GWCM_OLD_PROD.......only do prodtesting when in smartlink or ap mode
	//if(gwcm_mode == GWCM_OLD_PROD ) {
        op_ret = wd_gw_wsm_read(&(get_gw_cntl()->gw_wsm));
        if(get_gw_cntl()->gw_wsm.nc_tp >= GWNS_TY_SMARTCFG) {
            return false;
        }
	//}

    wf_wk_mode_set(WWM_STATION);
    AP_IF_S *ap = NULL;
    BOOL_T flag = TRUE;
    PR_NOTICE("current product ssid name:%s", prod_ssid_name);
	op_ret = wf_assign_ap_scan(prod_ssid_name, &ap);//lql
    wf_station_disconnect();
    if(OPRT_OK != op_ret) {
        PR_NOTICE("wf_assign_ap_scan failed(%d)",op_ret);
        return FALSE;
    }
    //check if has authorized
    op_ret = wd_gw_base_if_read(&(get_gw_cntl()->gw_base));
    if(OPRT_OK != op_ret) {
        PR_DEBUG("read flash err");
        flag = FALSE;
    }
    // gateway base info verify
    #if TY_SECURITY_CHIP
    if(!get_gw_cntl()->gw_base.has_auth) {
    #else
    if(0 == get_gw_cntl()->gw_base.auth_key[0] || \
       0 == get_gw_cntl()->gw_base.uuid[0]) {
    #endif
        PR_DEBUG("please write uuid and auth_key first");
        flag = FALSE;
    }

    if(app_prod_test) {
        app_prod_test(flag, ap->rssi);
    }
    return TRUE;
}

void app_cfg_set(IN CONST GW_WF_CFG_MTHD_SEL mthd, APP_PROD_CB callback)
{
    app_prod_test = callback;
    gwcm_mode = mthd;
}

/***********************************************************
*  Function: user_main 
*  Input: none
*  Output: none
*  Return: none
***********************************************************/
void user_main(void)
{
    OPERATE_RET op_ret = OPRT_OK;
    
    op_ret = tuya_iot_init();
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_init err:%d",op_ret);
        return;
    }
#if TY_SECURITY_CHIP
    SEChip_I2C_Init(); //加密芯片 I2C 接口初始化
#endif

    pre_device_init();
    tuya_iot_kv_flash_init(NULL);
	extended_app_waiting_for_launch();
#if 1
    // to add prodect test code
    mf_reg_gw_ug_cb(__mf_gw_ug_inform_cb, __gw_upgrage_process_cb, __mf_gw_upgrade_notify_cb);
    MF_IMPORT_INTF_S intf = {
        __tuya_mf_uart_init,
        __tuya_mf_uart_free,
        __tuya_mf_send,
        __tuya_mf_recv,
        gpio_test,
        mf_user_callback
    };
    op_ret = mf_init(&intf,APP_BIN_NAME,USER_SW_VER,TRUE);
    if(OPRT_OK != op_ret) {
        PR_ERR("mf_init err:%d",op_ret);
        return;
    }
    __tuya_mf_uart_free();
    PR_NOTICE("mf_init succ"); 
#endif


    // register gw upgrade inform callback
    iot_register_pre_gw_ug_cb(__gw_ug_inform_cb);

// for debug
#if 0
    WF_GW_PROD_INFO_S wf_prod_info = {
        "003tuyatestf7f149189","NeA8Wc7srpAZHEMuru867oblOLN2QCC5",NULL,NULL
    };
    op_ret = tuya_iot_set_wf_gw_prod_info(&wf_prod_info);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_set_wf_gw_prod_info err:%d",op_ret);
        return;
    }
#endif

#if 0
    DEBUG_GW_PROD_INFO_S debug_prod_info = {
        TRUE, NULL, NULL
    };
    op_ret = tuya_iot_set_wf_gw_debug_info(&debug_prod_info);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_set_wf_gw_prod_info err:%d",op_ret);
        return;
    }
#endif

    app_init();
    PR_DEBUG("gwcm_mode %d",gwcm_mode);
    if(gwcm_mode != GWCM_OLD) {
        PR_DEBUG("low_power select");
        if(true == scan_test_ssid()) {
            PR_DEBUG("prodtest");
            return;
        }
        PR_DEBUG("no tuya_mdev_test1!");
        op_ret = device_init();
        if(OPRT_OK != op_ret) {
            PR_ERR("device_init error:%d",op_ret);
            return;
        }
    }else {
        PR_DEBUG("device_init in");
        op_ret = device_init();
        if(OPRT_OK != op_ret) {
            PR_ERR("device_init err:%d",op_ret);
            return;
        }
    }
}

extern OPERATE_RET uni_flash_set_protect(IN CONST BOOL_T enable);
// mf gateway upgrade start 
VOID __mf_gw_ug_inform_cb(UINT_T file_size, UINT_T file_crc)
{
	#if 1
    ug_proc = Malloc(SIZEOF(UG_PROC_S));
    if(NULL == ug_proc) {
        PR_ERR("malloc err");
        return;
    }
    memset(ug_proc,0,SIZEOF(UG_PROC_S));
	#endif
}

// gateway upgrade start
STATIC VOID __gw_ug_inform_cb(INOUT BOOL_T *handled, IN CONST FW_UG_S *fw)
{
	OPERATE_RET op_ret = OPRT_OK;

//	image_seq_t seq;
	if(fw->tp != DEV_NM_ATH_SNGL)
    {
        *handled = FALSE;
        return;
    }
    *handled = TRUE;

	PR_DEBUG("ota_notify_data_begin enter");
	ug_proc = Malloc(SIZEOF(UG_PROC_S));
    if(NULL == ug_proc) {
        PR_ERR("malloc err");
        return;
    }
    memset(ug_proc,0,SIZEOF(UG_PROC_S));

    op_ret = tuya_iot_upgrade_gw(fw,__gw_upgrage_process_cb,__gw_upgrade_notify_cb,NULL);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_upgrade_gw err:%d",op_ret);
        return;
    }
}

#define BUF_SIZE 4096
// mf gateway upgrade result notify
OPERATE_RET __mf_gw_upgrade_notify_cb(VOID)
{
	#if 1
    u32 ret = 0,i = 0,k = 0,rlen = 0,addr = 0;
    u32 flash_checksum=0;
    u8 *pTempbuf;
    pTempbuf = Malloc(BUF_SIZE);
    if(pTempbuf == NULL) {
        PR_ERR("Malloc failed!!");
    }
    uni_flash_set_protect(TRUE);
    for(i = 0; i < ug_proc->file_header.bin_len; i += BUF_SIZE) {
        rlen  = ((ug_proc->file_header.bin_len - i) >= BUF_SIZE) ? BUF_SIZE : (ug_proc->file_header.bin_len - i);
        addr = ug_proc->start_addr + i;
        uni_flash_read(addr, pTempbuf, rlen);
        for(k = 0; k < rlen; k++) {
            flash_checksum += pTempbuf[k];
        }
    }
    
    if(flash_checksum != ug_proc->file_header.bin_sum) {
        PR_ERR("verify_ota_checksum err  checksum(0x%x)  file_header.bin_sum(0x%x)",flash_checksum,ug_proc->file_header.bin_sum);
    	//uni_flash_erase(ug_proc->start_addr,ug_proc->file_header.bin_len);
    	Free(pTempbuf);
    	Free(ug_proc);
        return;
    }
    
    PR_NOTICE("the gateway upgrade success");
    Free(pTempbuf);
    Free(ug_proc);
    //os_printf("the gateway upgrade succes,now go to reset!!\r\n");
	#endif
    return OPRT_OK;
}

// gateway upgrade result notify
STATIC VOID __gw_upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    uni_flash_set_protect(TRUE);
    if(OPRT_OK == download_result) { // update success
        // verify 
        u32 ret = 0,i = 0,k = 0,rlen = 0,addr = 0;
        u32 flash_checksum=0;
        u8 *pTempbuf;
        pTempbuf = Malloc(BUF_SIZE);
        if(pTempbuf == NULL){
            PR_ERR("Malloc failed!!");
        }
        for(i = 0; i < ug_proc->file_header.bin_len; i += BUF_SIZE){
            rlen  = ((ug_proc->file_header.bin_len - i) >= BUF_SIZE) ? BUF_SIZE : (ug_proc->file_header.bin_len - i);
            addr = ug_proc->start_addr + i;
            uni_flash_read(addr, pTempbuf, rlen);
            for(k = 0; k < rlen; k++){
                flash_checksum += pTempbuf[k];
            }
        }
        if(flash_checksum != ug_proc->file_header.bin_sum){
            PR_ERR("verify_ota_checksum err  checksum(0x%x)  file_header.bin_sum(0x%x)",flash_checksum,ug_proc->file_header.bin_sum);
        	//uni_flash_erase(ug_proc->start_addr,ug_proc->file_header.bin_len);
        	Free(pTempbuf);
            return;
        }
        PR_NOTICE("the gateway upgrade success");
        Free(pTempbuf);
        //os_printf("the gateway upgrade succes,now go to reset!!\r\n");
        SystemReset();
        return;
    }else {
        PR_ERR("the gateway upgrade failed");
    }
}

// gateway upgrade process
STATIC OPERATE_RET __gw_upgrage_process_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len,IN CONST UINT_T offset,\
                                                      IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    u32 sum_tmp = 0,i=0;
    u32 write_len = 0;
    switch(ug_proc->stat) {
        case UGS_RECV_HEADER: {
            if(len < SIZEOF(UPDATE_FILE_HDR_S)) {
                *remain_len = len;
                break;
            }
            //memcpy((unsigned char*)&ug_proc->file_header,data,SIZEOF(UPDATE_FILE_HDR_S));
            
            ug_proc->file_header.tail_flag = (data[28]<<24)|(data[29]<<16)|(data[30]<<8)|data[31];
            ug_proc->file_header.head_sum = (data[24]<<24)|(data[25]<<16)|(data[26]<<8)|data[27];
            ug_proc->file_header.bin_sum = (data[20]<<24)|(data[21]<<16)|(data[22]<<8)|data[23];
            ug_proc->file_header.bin_len = (data[16]<<24)|(data[17]<<16)|(data[18]<<8)|data[19];
            for(i=0;i<12;i++) {
                ug_proc->file_header.sw_version[i] = data[4 + i];
            }
            
            ug_proc->file_header.header_flag = (data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3];
            
            for(i = 0;i < SIZEOF(UPDATE_FILE_HDR_S) - 8;i++) {
                sum_tmp += data[i];
            } 
            PR_NOTICE("header_flag(0x%x) tail_flag(0x%x) head_sum(0x%x-0x%x) bin_sum(0x%x)",ug_proc->file_header.header_flag,ug_proc->file_header.tail_flag,ug_proc->file_header.head_sum,sum_tmp,ug_proc->file_header.bin_sum);
            //os_printf("sw_version:%s  bin_len = 0x%x   bin_sum = 0x%x\r\n",ug_proc->file_header.sw_version, ug_proc->file_header.bin_len,ug_proc->file_header.bin_sum);
            if((ug_proc->file_header.header_flag !=  UG_PKG_HEAD) || (ug_proc->file_header.tail_flag !=  UG_PKG_TAIL) || (ug_proc->file_header.head_sum != sum_tmp )) {
                memset(&ug_proc->file_header, 0, SIZEOF(UPDATE_FILE_HDR_S));
                PR_ERR("bin_file data header err: header_flag(0x%x) tail_flag(0x%x) bin_sum(0x%x) get_sum(0x%x)",ug_proc->file_header.header_flag,ug_proc->file_header.tail_flag,ug_proc->file_header.head_sum,sum_tmp);
                return OPRT_COM_ERROR;
            }
            
            os_printf("sw_ver:%s\r\n",ug_proc->file_header.sw_version);
            os_printf("get right bin_file_header!!!\r\n");
            ug_proc->start_addr = UG_START_ADDR;
            ug_proc->flash_addr = ug_proc->start_addr;
            ug_proc->stat = UGS_RECV_IMG_DATA;
            ug_proc->recv_data_cnt = 0;
            *remain_len = len - SIZEOF(UPDATE_FILE_HDR_S);
            uni_flash_set_protect(FALSE);
            uni_flash_erase(ug_proc->start_addr,ug_proc->file_header.bin_len);
        	os_printf("erase success  remain_len: %d  file size: %d!!!!!\r\n",*remain_len,ug_proc->file_header.bin_len);
        } 
        break;
        
        case UGS_RECV_IMG_DATA: {    //dont have set lock for flash! 
            //PR_DEBUG_RAW("ug_proc->recv_data_cnt : %d,len : %d\r\n",ug_proc->recv_data_cnt,len);
            *remain_len = len;
            if((len < RT_IMG_WR_UNIT) && (ug_proc->recv_data_cnt <= (ug_proc->file_header.bin_len - RT_IMG_WR_UNIT))) {
                break;
            }
            write_len = len;
            while(write_len >= RT_IMG_WR_UNIT) {
                if(uni_flash_write(ug_proc->flash_addr, &data[len - write_len], RT_IMG_WR_UNIT)) {
                    PR_ERR("Write sector failed");
                    return OPRT_WR_FLASH_ERROR;
                }
                ug_proc->flash_addr += RT_IMG_WR_UNIT;
                ug_proc->recv_data_cnt += RT_IMG_WR_UNIT;
                write_len -= RT_IMG_WR_UNIT; 
                *remain_len = write_len;
            }
            if((ug_proc->recv_data_cnt > (ug_proc->file_header.bin_len - RT_IMG_WR_UNIT)) \
				&& (write_len >= (ug_proc->file_header.bin_len - ug_proc->recv_data_cnt))) {//last 512 (write directly when get data )
                if(uni_flash_write(ug_proc->flash_addr, &data[len - write_len], write_len)) {
                    PR_ERR("Write sector failed");
                    return OPRT_WR_FLASH_ERROR;
                }
                os_printf("\r\nwrite success!!!\r\n");
                ug_proc->flash_addr += write_len;
                ug_proc->recv_data_cnt += write_len;
                write_len = 0;
                *remain_len = 0;
            }
            //os_printf("rcv_cnt:%d，remain_len:%d\r\n",ug_proc->recv_data_cnt,*remain_len);
            if(ug_proc->recv_data_cnt >= ug_proc->file_header.bin_len) {
                ug_proc->stat = UGS_FINISH;
                break;
            }
        }
        break;

        case UGS_FINISH: {
            *remain_len = 0;
        }
        break;
    }

    return OPRT_OK;
}


void set_prod_ssid(CHAR_T *ssid)
{
    if (strlen(ssid) > WF_SSID_LEN) {
        PR_ERR("ssid len over max value 32");
        return;
    }
    memset(prod_ssid_name, 0, sizeof(prod_ssid_name));
    strncpy(prod_ssid_name, ssid, WF_SSID_LEN);
    return;
}








