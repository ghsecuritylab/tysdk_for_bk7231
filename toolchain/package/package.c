/***********************************************************
*  File: package.c 
*  Author: lql
*  Date: 20171107
***********************************************************/
#define __PACKAGE_GLOBALS
#include "package.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <sys/types.h> // open()
#include <fcntl.h>  
#include <unistd.h>
#include <errno.h>

/***********************************************************
*************************micro define***********************
***********************************************************/
#define DEBUG 1
#define BUF_SIZE 4096
/***********************************************************
*************************variable define********************
***********************************************************/


/***********************************************************
*************************function define********************
***********************************************************/
// argv[0]:exe name
// argv[argc-1]:package file name
int main(int argc,char *argv[]) 
{
    unsigned char buf[BUF_SIZE];
    unsigned char *tmp_buf;  
    int j = 0;
    unsigned int sum = 0;
    int ret = 0;
    unsigned int read_size = 0;
    unsigned int ug_pkg_desc_len;
    int fd = 0;
    int ug_pkg_fd = 0;
    UG_PKG_FH_S *pkg_fh = NULL;
    int file_size;
    char SW_VER[12];
    if(argc < 4) {
        printf("the input parameter is not enough\n");
        return 1;
    }
    ug_pkg_desc_len = sizeof(UG_PKG_FH_S);
    pkg_fh = malloc(ug_pkg_desc_len);
    if(NULL == pkg_fh) {
        printf("malloc error\n");
        return 2;
    }
    memset(pkg_fh,0,ug_pkg_desc_len);
     
    pkg_fh->head_flag = HTONL(UG_PKG_HEAD);
    pkg_fh->tail_flag = HTONL(UG_PKG_TAIL);
    strcpy(pkg_fh->sw_ver,argv[3]);
#if DEBUG
    printf("sw_version:%s\n",argv[3]);
#endif
    ug_pkg_fd = open(argv[2],O_CREAT | O_TRUNC | O_WRONLY, 0640);
    if(ug_pkg_fd < 0) {
        printf("open error line:%d.\n",__LINE__);
        goto ERR_EXIT;
    }
	fd = open(argv[1],O_RDONLY);
	if(fd < 0) {
		printf("open error line:%d.\n",__LINE__);
		goto ERR_EXIT;
	}
	file_size = lseek( fd, 0, SEEK_END );
	if(file_size <= 0) {            
		printf("lseek error line:%d.\n",__LINE__);
		goto ERR_EXIT;
	}
	#if DEBUG
	else {
		printf("%s file_size:0x%x\n",argv[1],file_size);
	}
	#endif

	pkg_fh->bin_len = HTONL(file_size);
	
	lseek(fd,0,SEEK_SET);
	lseek(ug_pkg_fd,ug_pkg_desc_len,SEEK_SET);
	while(read_size < file_size) {
		ret = read( fd, buf, BUF_SIZE);
		if(ret < 0) {
			printf("read error line:%d.\n",__LINE__);
			goto ERR_EXIT;
		}else if(0 == ret){ // 结尾
			break;
		}

		ret = write(ug_pkg_fd,buf,ret);
		if(ret < 0) {
			printf("write error line:%d.\n",__LINE__);
			goto ERR_EXIT;
		}

		for(j = 0;j < ret;j++) {
			sum += buf[j];
		}
		read_size += ret;
	}
	pkg_fh->bin_sum = HTONL(sum);
	#if DEBUG
	printf("%s sum:0x%x\n",argv[1],sum);
	#endif
	close(fd);
	fd = 0;
    sum = 0; 
    tmp_buf = (unsigned char*)pkg_fh;
    for(j = 0;j < ug_pkg_desc_len - 8;j++) {
        sum += tmp_buf[j];
    }
    pkg_fh->header_sum = HTONL(sum);
    #if DEBUG
    printf("header_sum:0x%x\n",sum);
    #endif
   
    lseek(ug_pkg_fd,0,SEEK_SET);
    ret = write(ug_pkg_fd,tmp_buf,ug_pkg_desc_len);
    if(ret < 0) {
        printf("write error line:%d.\n",__LINE__);
        goto ERR_EXIT;
    }
    
    close(ug_pkg_fd),ug_pkg_fd = 0;
    free(pkg_fh);
    pkg_fh = NULL;
    tmp_buf = NULL;
    return 0;
    
ERR_EXIT:
    if(pkg_fh) {
        free(pkg_fh);
    }

    if(ug_pkg_fd) {
        close(ug_pkg_fd);
    }

    if(fd) {
        close(fd);
    }

    return 3;
} 



















