/*************************************************************************
	> File Name: Mater.c
	> Author:liangyunsong 
	> Mail:1820202749@qq.com 
	> Created Time: 2019年12月03日 星期二 18时33分10秒
 ************************************************************************/

#include "./common/head.h"
#include "./common/common.h"
#include "./common/linklist.h"
char value[50] = {0};
const char path[] = "./Mater.conf";


typedef struct args {
    LinkList **list;
    int listnum;
} args;

args arg;

void listen_list(void ){
    unsigned int s,f;
    s = ntohl(inet_addr(get_value(path, "startip")));
    f = ntohl(inet_addr(get_value(path, "endip")));
    //获取心跳端口
    char *cheartport = get_value(path, "heartport");
    int heartport = strtoint(cheartport);
    //获取本机 ip
    char *masterip = get_value(path, "masterip");
    //初始化链表
   // LinkList  *list = linkinit();
    printf("端口获取成功\n");
    struct sockaddr_in initaddr;
    initaddr.sin_family = AF_INET;
    initaddr.sin_addr.s_addr = inet_addr("0,0,0,0");
    initaddr.sin_port = htons(heartport);
    int sockll = socket_();
    for(unsigned int i = s; i <= f; i++){
        //不可操作的端口
        if((i & 255) == 255 || (i << 24) == 0) continue;
        initaddr.sin_addr.s_addr = htonl(i);
        if(shake_hand(initaddr) == 0){
           	LinkList *minLenList = arg.list[0];
            int nowConnectNum = arg.list[0]->length;
        	for (int i = 1; i < arg.listnum; i++) {
            	nowConnectNum += arg.list[i]->length;
            	minLenList = (arg.list[i]->length < minLenList->length ? arg.list[i] : minLenList);
        	} 
            printf("shake_hand success!\n");
            char *stre = inttostr(htonl(i)); 
            linkinsert(minLenList, stre, sockll);
            printf("%s 用户加入链表\n",stre);
			free(minLenList);
        }
    }
    close(sockll);
}

void *startlisten(void ){
    //配置文件 获取 ip
     
    char *masterip = get_value(path ,"masterip");
    if(masterip == NULL){
        perror("master.conf error about masterip\n");
        free(masterip);
        return NULL;
    }

    char *clientport1 = get_value(path ,"clientport");
    if(clientport1 == NULL){
        perror("master.conf error about clientport\n");
        free(clientport1);
        return NULL;
    }

    printf("clientport %s",clientport1);
    int clientport = strtoint(clientport1);
    printf("clientport %d\n",clientport);
    int sockfd ,sockson;
    struct sockaddr_in addrson;
    char ip[20] = {'\0'};
    printf("循环监听\n");
    sockfd = socket_create(clientport);
    socklen_t addrsonlen = sizeof(addrson);
    printf("while （1) \n");   
    while(1){
        LinkList *minlenlist = arg.list[0];
        int nowconnectnum = arg.list[0]->length;
        printf("linkList\n");
        printf("进入程序  %d\n",arg.listnum);
        for(int i = 1; i < arg.listnum; i++){
            nowconnectnum += arg.list[i]->length;   
            minlenlist = (arg.list[i]->length < minlenlist->length ? arg.list[i] : minlenlist);
        }
        printf("循环成功\n");
        sockson = accept(sockfd ,(struct sockaddr *)&addrson, &addrsonlen);
        if(sockson < 0 ){
            perror("accept\n");
            break;
        }
        printf("sockson = %d \n", sockson );
        printf("加入链表阶段");
        memset(ip, '\0',sizeof(ip));
        sockgetfromip(ip, (struct sockaddr_in *)&addrson);
        //将新建写入链表
        linkinsert(minlenlist, ip, sockson);
       }
    close(sockfd);
    return NULL;
}

pthread_t createconnect(LinkList **list, int num) {
     pthread_t thread;
     arg.list = list;
     arg.listnum = num;

    if (pthread_create(&thread, NULL, (void *)startlisten, NULL)) {
                perror("listenGetIP : \033[1;31mcreate thread error\033[0m""]]");
        return -1;
    }
    return thread;
}


//获取报警信息
void *getWarninginfo(){
    //获取 监听端口 和 ip
    char *localip = get_value(path, "masterip");
    char *dataportgi = get_value(path, "dataport");
    int dataport = strtoint(dataportgi);
    free(dataportgi);

    int sockfd, sockson;
    struct sockaddr_in addrson;
    char ip[20] = {'\0'};
    char warning[1024] = {'\0'};

    sockfd = sockserver(localip, dataport);
    socklen_t addrsonlen = sizeof(addrson);
    free(localip);

    while(1){
        sockson = accept(sockfd, (struct sockaddr *)&addrson, &addrsonlen);
        if(sockson < 0) {
            perror("getWarninginfo accept error\n");
            return NULL;

        }
        memset(ip, '\0', sizeof(ip));
        sockgetfromip(ip, (struct sockaddr_in *)&addrson); //转换了addrson的类型
        memset(warning, '\0', sizeof(warning));
        if(recv(sockson, warning, 1024, 0) == -1){
            perror("getWarninginfo recv\n");
            continue;
        }
        printf("getWarninginfo接受完毕\n");
        char *logpath = get_value(path, "logpath");
        if(logpath == NULL){
            perror("master.conf error don't have logpath");
            continue;
        }
        /*
        if(logpath[(int)strlen(logpath) - 1] == '/'){
            logpath[(int)strlen(logpath) - 1] = '\0';
        }
        char *logpath1 = (char *)malloc(sizeof(char) * ((int)strlen(logpath) + (int)strlen(ip) + 1 + 5));
        strcpy(logpath1, logpath);
        strcat(logpath1, "/");
        strcat(logpath1, ip);
        strcat(logpath1, "/warning.log");
        free(logpath);
        if(writerpilog(logpath1, warning) == 1){
            perror("getWarninginfo (write error )");
            return NULL;
        }
        */
    }
    return NULL;
}

int masterconnect() {

    /* Get threadNum from master.conf */
   char *threadNum =  get_value(path,"ins");
    printf(" threadNum   %s",threadNum);
    if (threadNum == NULL) {
        perror("master.conf error (don't have threadNum)");
        return 1;
    }
    int MaxNum = strtoint(threadNum);
  
    printf("maxnum %d\n",MaxNum);
    LinkList *list[MaxNum];

    for (int i = 0; i < MaxNum; i++) {
        list[i] = linkinit();
    }
    /*遍历所有用户 存在的 即加入链表list*/
    listen_list();

    
    /* 开启子线程，在子线程中开启socket服务，监控上线服务器，存储连接套接字。*/
    
    pthread_t listenthread = createconnect(list, MaxNum);
    if (listenthread  == -1)  {
        exit(1);
    }
    
    /* 紧急警报监听 */
    
    pthread_t warningthread;
    if (pthread_create(&warningthread, NULL, getWarninginfo, NULL)) {
        perror("masterConnect.c create thread getWarningInfo:");
        return 1;
    }   
    /* 创建指定数量线程，将存储服务器信息的链表作为参数传递给dataTransmission函数 */
    
   /* pthread_t thread[MaxNum];
    for (int i = 0; i < MaxNum; i++) {
        if (pthread_create(&thread[i], NULL, dataTransmission, (void *)list[i])) {
            perror("pthread_create dataTransmission");
            return 1;
        }
    }*/
    
  /*  for (int i = 0; i < MaxNum; i++) {
        pthread_join(thread[i], NULL);
    }*/
    pthread_join(listenthread, NULL);
    pthread_join(warningthread, NULL);
    
    pthread_exit(NULL); 
    return 0;
}

int main() {

   printf("程序开始\n");

//获取信息
	masterconnect();
    while(1){
        
    }
    return 0;
}
