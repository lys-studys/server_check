/*************************************************************************
	> File Name: head.h
	> Author:liangyunsong 
	> Mail:1820202749@qq.com 
	> Created Time: 2019年12月03日 星期二 18时36分44秒
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>	
#include <complex.h> 
#include <ctype.h>
#include <errno.h>	
#include <fenv.h> 
#include <float.h>	
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>	
#include <locale.h>	
#include <math.h>	
#include <setjmp.h>	
#include <signal.h>	
#include <stdalign.h>
#include <stdarg.h>	
#include <stdatomic.h>
#include <stdbool.h> 
#include <stddef.h>	
#include <stdint.h> 
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>	
#include <tgmath.h>
#include <sys/ioctl.h>
#include <pthread.h>
typedef struct Node {
    struct sockaddr_in addr;
    char name[20];
    struct Node *next;
    int heart;
    int len;
} Node, *LinkedList;




#define MAXBUFF 4096

#define CPU_INFO 100
#define DISK_INFO 101
#define MALIPS_INFO 102
#define MEM_INFO 103
#define SYS_INFO 104
#define USER_INFO 105
#define DATANULL 106
#define CLOSE_NOW 107

#define CPU_WARN 200
#define DISK_WARN 201
#define MALIPS_WARN 202
#define MEM_WARN 203
#define SYS_WARN 204
#define USER_WARN 205

/* 配置文件路径 */

#define CONF_SERVER "/etc/SHMS-server/server.conf"
#define CONF_MASTER "/etc/SHMS-master/master.conf"

/* 程序运行日志路径 */
#define RUN_LOG_SERVER "/var/log/SHMS-server/SHMS-Server.log"
#define RUN_LOG_MASTER "/var/log/SHMS-master/SHMS-Master.log"

/* 每次最大发送长度 */

#define TRANS_MAX 1024


/*
typedef struct Msg{
    
    int type;
    int size;
    char name[20];
}Msg;
*/
#ifndef _DEBUG
#define D(fmt, args...) printf(fmt, ##args)
#else
#define D(fmt, args...)
#endif


#define COLOR(a, b) "\033[" #b "m" #a "\033[0m"
#define COLOR_HL(a, b) "\033[1m\033[1m\033[" #b "m" #a "\033[0m"

#define GREEN(a) COLOR(a, 32)
#define RED(a) COLOR(a, 31)
#define BLUE(a) COLOR(a, 34)
#define YELLOW(a) COLOR(a, 33)

#define GREEN_HL(a) COLOR_HL(a, 32)
#define RED_HL(a) COLOR_HL(a, 31)
#define BLUE_HL(a) COLOR_HL(a, 34)
#define YELLOW_HL(a) COLOR_HL(a, 33)


#endif
