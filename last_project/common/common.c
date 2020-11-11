/*************************************************************************
	> File Name: common.c
	> Author:liangyunsong 
	> Mail:1820202749@qq.com 
	> Created Time: 2019年12月03日 星期二 18时34分49秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "common.h" 
#include "linklist.h"
//类型转换

char *inttostr(int inte){
    char *stre;
    int i = 0;
    while(inte >= 10){
        stre[i] = (inte % 10) + '0';
        inte = inte / 10;
    }
    if(inte)
    stre[i + 1] = inte % 10;
    return  stre;
}


int strtoint(char *str){
    if (!strcmp(str , "" )){
        printf("出错 返回 %s",str);
        return -1;
    }
    int ret = 0;
    for(int i = 0; str[i]; i++){
        if(str[i] < '0' || str[i] >'9'){
            continue;
        }
        ret = ret * 10 +(str[i] - '0');
    }
    return ret;

}

char *get_value(const char *confPath, const char *needVar) {
    FILE *fp = fopen(confPath, "r");

    if (fp == NULL) {
        printf("confPath:(%s)\n", confPath);
        perror("Get_conf.c (Get_conf->(12) 'fopen') ");
        return NULL;
    }

    char line[1024] = "";
    char *varValue = (char *)calloc(sizeof(char), MAXBUFF);
    
    while (fgets(line, MAXBUFF, fp) != NULL) {
       
        //Skip annotations.
        if (line[0] == '#') {
            continue;
        }
        
        // Determine whether the variable needed is in the configuration file.
        if (strstr(line, needVar) == NULL) {
            continue;
        }
        
        /* Determine whether the variable name is the variable required. */

        if (!strcmp(strtok(line, "="), needVar)) {
            
            /* Cut value of the variable needed. */
            
            strcpy(varValue, strtok(NULL, "="));
        }
    }
    
    if (varValue[(int)strlen(varValue) - 1] == ' ' || varValue[(int)strlen(varValue) - 1] == '\n' ) {
        varValue[(int)strlen(varValue) - 1] = '\0';
    }
    
    fclose(fp);
    char *retStr=strdup(varValue);
    free(varValue);

    return (!strcmp(retStr, "") ? NULL : retStr);
}



//脚本中信息获取
/*char *get_value(char *path, char *key, char *ans) {
    FILE *fp = NULL;
    if (key == NULL || ans == NULL || path == NULL) {
        printf("error in argument");
        return NULL;
    }

    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        return NULL;
    }

    char *line = NULL;
    size_t len;
    ssize_t nread;
    char *substr = NULL;

   // int i = 0;
    while ((nread = getline(&line, &len, fp)) != -1) {
        if ((substr = strstr(line, key)) == NULL) {
            continue;
        }
        if (line[strlen(key)] == '=') {
            strncpy(ans, &substr[strlen(key) + 1], nread - strlen(key) - 1);
            *(ans + strlen(ans) - 1) = '\0';
            printf("ans =  %s\n",ans);
            break;
        }
  }
    free(line);
    fclose(fp);
    if (substr == NULL) {
        return NULL;
    }
    return ans;
}*/
//套接字的创建

int socket_() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket_()");
        return -1;
    }
    return sockfd;
}
//server 端套接字的创建

int socket_create(int port) {
    int socketfd;
    struct sockaddr_in sock_addr;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
        perror("bind");
        close(socketfd);
        printf("asdas \n");
        return -1;
    }

    if (listen(socketfd, 20) < 0) {
        perror("listen");
        close(socketfd);
        return -1;
    }
    printf("sicketfd Created\n");
    return socketfd;
}
//服务端套接字的创建
int sockserver(char *ip, int port){
    struct sockaddr_in saddr;
    //初始化
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(ip);
    saddr.sin_port = htons(port);
    //套接字 创建
    //int socketfd = socket_();
    int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (bind(socketfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        perror("bind");
        close(socketfd);
        return -1;
    }
    if (listen(socketfd, 20) < 0) {
        perror("listen");
        close(socketfd);
        return -1;
    }
    printf("sicketfd Created\n");
    return socketfd;

}
//套接字连接
int sockclient(char *ip, int port){
	struct sockaddr_in caddr;
	memset(&caddr, 0, sizeof(caddr));
	caddr.sin_family = AF_INET;
    caddr.sin_port = htons(port);
    int sockfd = socket_();
    //ip 的值 给 sin_addr 
     caddr.sin_addr.s_addr = inet_addr(ip);
    if(inet_pton(AF_INET, ip, &caddr.sin_addr) <0){
        close(sockfd);
        printf("inet_ntop错误\n");
        return -1;
    }
    if(connect(sockfd, (struct sockaddr*)&caddr, sizeof(caddr)) <0 ){
        perror("connect");
        close(sockfd);
        return -1;
    }
    return sockfd;
}
//ip的获取
int sockgetfromip(char *fromip, struct sockaddr_in *sonaddr){
    if(inet_ntop(AF_INET,(void *)&(sonaddr->sin_addr),fromip,16) == NULL){
        return -1;

    }
    return 0;
}
//套接字关闭消息发送
int sendclose(int socked){
    int sendtag = CLOSE_NOW;
    if(send(socked, &sendtag, sizeof(int) , 0)<0){
        perror("sendclose");
        return -1;
    }
    return 0;
}
//文件写入 脚本文件

int writerpilog(const char *logfile, char *loginfo){
    FILE *fp = fopen(logfile, "a");

    if(fp == NULL){
        printf("logfile : %s, ",logfile);
        perror("write_pi_log() : ");
        return -1;
    }
    if(fseek(fp, 0, SEEK_END) < 0 ){
        fclose(fp);
        return -1;
    }
    if(fwrite(loginfo,sizeof(char), strlen(loginfo), fp) <= 0){
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}


int shake_hand(struct sockaddr_in host) {
    int sockfd;
    struct timeval tm;
    fd_set wfds;
    if ((sockfd = socket_()) < 0) {
        return -1;
    }
    unsigned long len = 1;
    ioctl(sockfd, FIONBIO, &len);
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    tm.tv_sec = 0;
    tm.tv_usec = 10000;
    int error = -1;
    int len_t = sizeof(int);
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) <0) {
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
            getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&error, (socklen_t *)&len_t);
            if (error != 0) {
                close(sockfd);
                return -1;
            }
            printf("已连接完成 \n");
            

        } else {
            close(sockfd);
            return -1;
        }
    }
    close(sockfd);
    return 0;

}

int find_min (int *sum, int ins) {
    int sub = 0;
    for (int i = 0; i < ins; i++) {
        if (*(sum + i) < *(sum + sub)) {
            sub = i;
        }
        return sub;
    }
}

bool check_online (LinkedList *head, struct sockaddr_in client, int ins) {
    for (int i = 0; i < ins; i++) {
        Node *p = head[i];
        while (p->next != NULL) {
            if (p->next->addr.sin_addr.s_addr == client.sin_addr.s_addr) {
                return false;
            }
            p = p->next;
        }
    }
    return true;
}
int list_add(LinkedList p,struct sockaddr_in *list, int *ind){
	while(p->next != NULL){
	memcpy(&list[*ind], &p->next->addr, sizeof(p->next->addr));
    *ind+=1;
     p=p->next;  
    }
    return 1;
}
/*int accessible(LinkedList head , char *name){
    Node *p = head;
    while(p->next !=NULL){
        if( shake_try(p->addr,name) !=0){
            p->heart += 1;
            if(p->heart == 3){
                dele(head,p);
                D(RED_HL([DELLETE])"%s\n",p->name);
                p->heart = 0;
            }
        }
        p = p->next;
    }
    return 0;

}
*/
/*
int shake(struct sockaddr_in host, char *name, struct sockaddr_in *list, int list_len,int number) { // 交换用户列表
    int sockfd;
    struct timeval tm;
    fd_set wfds;
    if ((sockfd = socket_()) < 0) { //创建套接字
        return -1;
    }
    unsigned long len = 1;
    ioctl(sockfd, FIONBIO, &len); //改为非阻塞IO
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    tm.tv_sec = 0;
    tm.tv_usec = 10000;
    int error = -1;
    int len_t = sizeof(int);
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) < 0) {
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, 
                       &error, (socklen_t*)&len_t);
            if (error != 0) { //失败
                close(sockfd); 
                return -1;
            }
            len= 0;
            Msg *msg = (Msg *)malloc(sizeof(Msg));
            msg->kind = number;
            msg->len = list_len;
            strcpy(msg->name, name);
            len = 0;
            ioctl(sockfd, FIONBIO, &len); //改为非阻塞
            send(sockfd, msg, sizeof(Msg), 0); // 发送2类报文
            send(sockfd, list, sizeof(list), 0); //发送用户列表
            //printf("%d\n", __LINE__);
            free(msg);
        } else {
            close(sockfd);
            return -1;
        }
    } 
    close(sockfd);
    return 0;
}
*/
int shake_try(struct sockaddr_in host, char *name) {
    int sockfd;
    struct timeval tm;
    fd_set wfds;

    if ((sockfd = socket_()) < 0) {
        return -1;
    }
    
    unsigned long len = 1;
    ioctl(sockfd, FIONBIO, &len); 
    
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    tm.tv_sec = 0;
    tm.tv_usec = 10000;
    int error = -1;
    int len_t = sizeof(int);
    
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) < 0) {
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&len_t);
            if (error != 0) {
                close(sockfd);
                return -1;
            }
            Msg *msg = (Msg *)malloc(sizeof(Msg));
            msg->kind = 1;
            msg->len = 0;
            strcpy(msg->name, name);
            len = 0;
          //  ioctl(sockfd, FIONBIO, &len);  //改为阻塞
            send(sockfd, msg, sizeof(Msg), 0); // 发送0类报文
            free(msg);
        } else {
            close(sockfd);
            return -1;
        }
    } 
    close(sockfd);
    return 0;
}


int shake_echg(struct sockaddr_in host, char *name, struct sockaddr_in *list, int list_len) { // 交换用户列表
    int sockfd;
    struct timeval tm;
    fd_set wfds;
    if ((sockfd = socket_()) < 0) { //创建套接字
        return -1;
    }
    unsigned long len = 1;
    ioctl(sockfd, FIONBIO, &len); //改为非阻塞IO
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    tm.tv_sec = 0;
    tm.tv_usec = 10000;
    int error = -1;
    int len_t = sizeof(int);
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) < 0) {
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR,&error,(socklen_t *)&len_t); 
            if (error != 0) { //失败
                close(sockfd); 
                return -1;
            }
            len= 0;
            Msg *msg = (Msg *)malloc(sizeof(Msg));
            msg->kind = 2;
            msg->len = list_len;
            strcpy(msg->name, name);
            len = 0;
          //  ioctl(sockfd, FIONBIO, &len); //改为非阻塞
            send(sockfd, msg, sizeof(Msg), 0); // 发送2类报文
            send(sockfd, list, sizeof(list), 0); //发送用户列表
            //printf("%d\n", __LINE__);
            free(msg);
        } else {
            close(sockfd);
            return -1;
        }
    } 
    close(sockfd);
    return 0;
}
