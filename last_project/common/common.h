/*************************************************************************
	> File Name: common.h
	> Author:liangyunsong 
	> Mail:1820202749@qq.com 
	> Created Time: 2019年12月03日 星期二 18时34分55秒
 ************************************************************************/

#include "head.h"
#include "linklist.h"
#ifndef _COMMON_H
#define _COMMON_H
typedef struct Msg{
	int kind;
	int len;
 	char name[20];


}Msg;

int strtoint(char *);

//char *get_value(char *path, char *key, char *const ans);
//server 端
char *inttostr(int);

char *get_value(const char *confPath,const char *needVar);

int socket_create(int port);

int socket_();

int sockserver(char *ip, int port );

int sockclient(char *ip, int port);

int sockgetfromip(char *, struct sockaddr_in *);

int sendclose(int );

int writerpilog(const char *, char *);

int shake_hand(struct sockaddr_in host);

int find_min(int *sum, int ins);

bool check_online(LinkedList *head, struct sockaddr_in cilent, int ins);

int  accessible(LinkedList head, char *name);

int shake_try(struct sockaddr_in host, char *);

int list_add(LinkedList p, struct sockaddr_in *list, int *ind);

int shake_echg(struct sockaddr_in host, char *name, struct sockaddr_in *list, int list_len);

void listen_list(void);

void *getwarninginfo(void);

//client 端

void heartbeat(void );

int datatransmission(int /*sockfd*/);

void *monitorhealth(void *arg);

char *getscriptruninfo(int /*signifier*/);

int serverconnect();

void *getinfo();

int sendwarninginfo(char *);

int sendDataString(int /*sockfd*/  ,int /*datasize*/ ,char * /*senddata*/);



#endif

