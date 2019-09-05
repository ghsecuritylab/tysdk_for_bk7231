/***********************************************************
*  File: package.h 
*  Author: nzy
*  Date: 20150915
***********************************************************/
#ifndef _PACKAGE_H
    #define _PACKAGE_H

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  __PACKAGE_GLOBALS
    #define __PACKAGE_EXT
#else
    #define __PACKAGE_EXT extern
#endif


#define HTONL(X) DWORD_SWAP(X)
#define DWORD_SWAP(X)(((X)&0xff)<<24) + \
                     (((X)&0xff00)<<8) + \
                     (((X)&0xff0000)>>8) + \
                     (((X)&0xff000000)>>24)
/***********************************************************
*************************micro define***********************
***********************************************************/
#pragma pack(1)

#define SW_VER_LMT 11
#define UG_PKG_HEAD 0x55aa55aa
#define UG_PKG_TAIL 0xaa55aa55
//define SW_VER USER_SW_VER //"1.0.0"   //
    
typedef struct { // big-end
    unsigned int head_flag; // 0x55aa55aa
    unsigned char sw_ver[SW_VER_LMT+1];
    unsigned int bin_len;
	unsigned int bin_sum;
	unsigned int header_sum;
	unsigned int tail_flag; //0xaa55aa55
}UG_PKG_FH_S;

#pragma pack()

// package structure: UG_PKG_FH_S + UG_PKG_TAIL_S + actual file

/***********************************************************
*************************variable define********************
***********************************************************/


/***********************************************************
*************************function define********************
***********************************************************/

#ifdef __cplusplus
}
#endif
#endif

