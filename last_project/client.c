/*************************************************************************
	> File Name: cilent.c
	> Author:liangyunsong 
	> Mail:1820202749@qq.com 
	> Created Time: 2019年12月03日 星期二 20时44分40秒
 ************************************************************************/
#include "./common/head.h"
#include "./common/common.h"
#include "./common/linklist.h"
#define connectnum 2
#define MaxNum 6
char value[50] = {0};
char name[20] = {0};
int  ins,heartport;
char path[] = "./client.conf";

void sleeptime(){
    srand(time(0));
    double sleeptime = fabs((double)(rand()% connectnum)/ 100.0);
    printf("sleeptime :(%f)\n", sleeptime);
    sleep(sleeptime);
    return ;
}

void heartbeat(void){
    //配置文件中获取 Master端ip和sock 端口
    char *masterip = get_value(path, "Masterip");
    char *mastport_hb = get_value(path, "masterheartip");
    int mastport = strtoint(mastport_hb);
    printf("master ip 和 sock端口获取成功\n");

    char ip[15] = {'0'};
    int port = mastport;
    strcpy(ip, masterip);
    free(masterip);
    free(mastport_hb);

    while(1){
        //睡眠时间
        sleeptime();
        //链接请求
        int sockfd = sockclient(ip, port);
        if(sockfd != -1){
            printf("连接正确\n");
           // datatransmission(sockfd);
            
        }else{
            perror("connect false");
        }
        close(sockfd);
    }
}



/*发送数据字符串*/
int sendDataString(int sockFd, int dataSize, char *sendData) {
       
    int nowDataSize = dataSize;
    char *nowDataStrStarPos = &sendData[0];
    
    while (nowDataSize > 0) {
        int sendDataSize;
        nowDataSize = (int)strlen(nowDataStrStarPos);
        sendDataSize = (TRANS_MAX < nowDataSize ? TRANS_MAX: nowDataSize);
        
        if (send(sockFd, nowDataStrStarPos, sizeof(char) *sendDataSize, 0) < 0)  {
            perror("dataTransmission.c (send Data)");
            return -1;
        }
        nowDataStrStarPos += sendDataSize;
    }
    
    return 0;
}


/*获运行信息*/
char *getscriptruninfo(int Signifier) {
    if (Signifier < 100 || Signifier > 105) {
        perror("getScriptRunInfo() (Signifier error)");
        return NULL;
    }
    
    /* 从配置文件中获取日志文件所在位置 */
    
    char *logPath = get_value(path, "logpath");
    if (logPath == NULL) {
        perror("server.conf (error don't have logPath)");
        return NULL;
    }
    if (logPath[(int)strlen(logPath) - 1] == '/') {
        logPath[(int)strlen(logPath) - 1] = '\0';
    }
    
    /* 从配置文件中获取脚本所在位置  */
    
    char *ScriptPath = get_value(path, "scriptpath");
    if (ScriptPath == NULL) {
        perror("server.conf (error don't have ScriptPath)");
        return NULL;
    }
    if (ScriptPath[(int)strlen(ScriptPath) - 1] == '/') {
        ScriptPath[(int)strlen(ScriptPath) - 1] = '\0';
    }
    
    char Cmd[MAXBUFF] = {'0'};
    char tempData[MAXBUFF];
    
    /* 生成执行命令 */
    
    strcpy(Cmd, ScriptPath);
    strcat(Cmd, "/getLogHeadInfo.sh ");
    strcat(Cmd, logPath);
    switch (Signifier) {
        case 100 : {
            strcat(Cmd, "/cpu.log");
        } break;
        case 101 : {
            strcat(Cmd, "/disk.log");
        } break;
        case 102 : {
            strcat(Cmd, "/save.log");
        } break;
        case 103 : {
            strcat(Cmd, "/mem.log");
        } break;
        case 104 : {
            strcat(Cmd, "/system.log");
        } break;
        case 105 : {
            strcat(Cmd, "/user.log");
        } break;
    }
    
    if (logPath != NULL) {
        free(logPath);
    }
    if (ScriptPath != NULL) {
        free(ScriptPath);
    }
    
    /* 调用脚本执行命令 */
    
    FILE *fp = popen(Cmd, "r");
    fgets(tempData, sizeof(char) * MAXBUFF, fp);
    pclose(fp);
    printf("tempData <%s>\n", tempData);

    /* 将获得的数据返回 */

    char *retData = NULL;
    // char *retData = (char *)calloc(sizeof(char), (int)strlen(tempData));
    // strcpy(retData, tempData);
    retData = strdup(tempData);//
    if (retData == NULL || (int)strlen(retData) == 0 || (((int)strlen(retData) == 1) && (retData[0] == '\n'))) {
        if (retData != NULL) {
            free(retData);
        }
        retData = strdup("NULL");
    }
    
    return retData;
}

/*开六个线程*/
int datatransmission(int sockFd) {
   // signal(SIGPIPE, SIG_IGN);
    
    /* 传输6个脚本的数据 */

    for (int i = 0; i < 6; i++) {
        /* 接收标识码  */
        
        int recvRet, dataType;
        recvRet = recv(sockFd, &dataType, sizeof(int), 0);
        if (recvRet == -1) {
            perror("dataTransmission.c (recvRet)");
            return -1;
        } else if (dataType < 100 || dataType > 105) {
            perror("recv dataType error");
            sendclose(sockFd);
            return 0;
        }
        
        /* 获取即将发送的字符串 */
        
      //  signal(SIGPIPE, SIG_DFL);
        char *sendData = getscriptruninfo(dataType);
        if (sendData == NULL) {
            perror("getScriptRunInfo() run error");
            return -1;
        }
     //   signal(SIGPIPE, SIG_IGN);
        
        /* 发送字符串长度 */
        
        int dataSize = (int)strlen(sendData) + 5;
        // if (send(sockFd, &dataSize, sizeof(int), 0) < 0) {
        //     perror("dataTransmission.c (send dataSize)");
        //     return -1;
        // }
        // 
        // sleep(0.5);
        
        /* 发送字符串 */
        
        // if (sendDataString(sockFd, dataSize, sendData) == -1) {
        //     return -1;
        // }
        printf("sendData : <%s>\n", sendData);
        if (send(sockFd, sendData, sizeof(char) * dataSize, 0) < 0)  {
            perror("dataTransmission.c (send Data)");
            return -1;
        }
        
        /* 释放sendData字符串空间 */
        
        if (sendData != NULL) {
            free(sendData);
        }
    }
    
    return 0;
}


//获取报警信息
void *getwarninginfo(){
    //获取 监听端口 和 ip
    char *localip = get_value(path, "Masterip");
    char *dataportgi = get_value(path, "masterdataport");
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
    }
    return NULL;
}

int runandsave(char *logfile, char *scriptfile, int sleeptime, int monitorwarning){
  while (1) {
        char Cmd[MAXBUFF] = {'0'};
        strcpy(Cmd, scriptfile);
        
        /* 运行其他不含有警报信息的脚本 */
        
        if (monitorwarning == 0) {
            
            /* 直接执行shell命令：xxx.sh >> xxx/xxx.log  */
            
            strcat(Cmd, " >> ");
            strcat(Cmd, logfile);
            
            FILE *pfp = popen(Cmd, "w");
            pclose(pfp);
        }
        
        /* 运行系统信息脚本，分析运行结果是否有警报信息 */
        
        if (monitorwarning == 1) {
            char runRes[MAXBUFF] = {'0'};
            
            /* 获取脚本运行结果 */
            
            FILE *pfp = popen(Cmd, "r");
            fgets(runRes, sizeof(runRes), pfp);
            pclose(pfp);
            
            /* 将脚本运行结果写入日志文件 */
            
            if (writerpilog(logfile, runRes) == -1) {
                perror("RunAndSave()(writerPiLog)");
                break;
            }
            
            /* 判断脚本运行结果中是否有警报信息 */
            
      /*      if (strstr(runRes, "warning") != NULL) {
                if (sendwarninginfo(runRes)) {
                    perror("sendWarningInfo error");
                    break;
                }
            }  */
        }
        sleep(sleeptime);
    }
    return 0;
}

void *monitorhealth(void *arg){
    char *scriptfile, *logfile;
    int sleeptime, monitorwaring;
    /*从配置文件中获取日志文件 xxx.log文件*/
    printf("进入日志文件的获取\n");
    char *logpath = get_value(path, "logpath");
    if(logpath == NULL){
        perror("server.conf (error don't have logpath)");
        return NULL;
    }
    /*获取脚本所在位置*/
    char *scriptpath = get_value(path, "scriptpath");
    if(scriptpath == NULL){
        perror("client.conf error (don't have scriptpath)");
        return NULL;
    }
    printf("路径信息获取成功\n");

    if(scriptpath[(int)strlen(scriptpath) - 1] == '/'){
        scriptpath[(int)strlen(scriptpath) - 1] == '\0';
    }

    logfile = (char *)calloc(sizeof(char), ((int)strlen(logpath) + 30));
    scriptfile = (char *)calloc(sizeof(char),((int)strlen(scriptfile) + 30));
    strcpy(logfile, logpath);
    strcpy(scriptfile, scriptpath);
    monitorwaring = 0;
    printf("数据类型进行匹配\n");
    int datatype = *((int *)arg);
    switch(datatype){
        case 100: {
                    sleeptime = 5;
                    strcat(logfile,"/cpu.log");
                    strcat(scriptfile,"/cpu.sh");
        }break;
        case 101: {
                    sleeptime = 60;
                    strcat(logfile,"/disk.log");
                    strcat(scriptfile,"/disk.sh");
        }break;
        case 102: {
                    sleeptime = 30;
                    strcat(logfile,"/save.log");
                    strcat(scriptfile,"/save.sh");
        }break;
        case 103: {
                    sleeptime = 5;
                    strcat(logfile,"/mem.log");
                    strcat(scriptfile,"/mem.sh");
        }break;
        case 104: {
                    sleeptime = 60;
                    strcat(logfile,"/system.log");
                    strcat(scriptfile,"/system.sh");
        }break;
        case 105: {
                    sleeptime = 60;
                    strcat(logfile,"/user.log");
                    strcat(scriptfile,"/user,sh");
                    
        }break;
    }
    if(runandsave(logfile, scriptfile, sleeptime, monitorwaring) == -1){
        return NULL;
    }
    return NULL;

}


int clientconnect(){
     /* 创建指定数量线程，将存储服务器信息的链表作为参数传递给dataTransmission函数 */
    
    pthread_t thread[MaxNum];
    printf("线程创建\n");
    int num = 100;
    for (int i = 0; i < MaxNum; i++) {
         num += i;
        if (pthread_create(&thread[i], NULL, monitorhealth, (void *)&num)) {
            perror("pthread_create dataTransmission");
            return 1;
        }
        printf("线程创建成功 \n");
        num = 100;
    }
    
     for (int i = 0; i < MaxNum; i++) {
        pthread_join(thread[i], NULL);
    }
    //  pthread_join(listenThread, NULL);
   //   pthread_join(warningThread, NULL);
    
      pthread_exit(NULL);  
}
int main() {

    printf("程序开始\n");

  /*  heartport = atoi(get_value(path,"clientlistenport"));
    int sockfd;
    printf("信息读取完成\n");
    char *masterheartip  = get_value(path, "Masterip");//点分十进制
    printf("%s %d\n",masterheartip,heartport);
  //  sockfd = sockserver(masterheartip,heartport);
    while (1) {
        printf("进入连接状态\n");
        if((sockfd = sockclient(masterheartip,heartport)) < 0){
            printf("%d \n ",sockfd );
            printf("创建错误\n");
            continue ;
        }
        printf("连接成功\n");
        close(sockfd);
        break; 
    }
    printf("进入心跳连接测试\n");
    heartbeat();*/
    printf("文件日志系统运行\n");
    clientconnect();


    return 0;
}



