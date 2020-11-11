/*************************************************************************
	> File Name: linklist.h
	> Author:liangyunsong 
	> Mail:1820202749@qq.com 
	> Created Time: 2019年12月03日 星期二 18时35分30秒
 ************************************************************************/
#include "head.h"
#ifndef _LINKLIST_H
#define _LINKLIST_H
typedef struct LinkNode {
    char *ip;
    int sockfd;
    struct LinkNode *next;
} LinkNode;
typedef struct LinkList {
    LinkNode head;
    int length;
} LinkList;

LinkList *linkinit();

//typedef struct LinkedList{
//    
//}LinkedList;

LinkNode *linkgetnewnode(char *,int /*port*/);

void linkinsert(LinkList *,char *, int /*port*/);

void linkerase(LinkList *, LinkNode *);

int linksearch(LinkList *,int );

void linkClear(LinkList *);

//int insert(LinkedList head, Node *node);

//int dele(LinkedList head,Node *node);

#endif

