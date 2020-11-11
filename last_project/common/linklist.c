/*************************************************************************
	> File Name: linklist.c
	> Author:liangyunsong 
	> Mail:1820202749@qq.com 
	> Created Time: 2019年12月03日 星期二 18时35分22秒
 ************************************************************************/

#include "linklist.h"

LinkList *linkinit(){
    LinkList *p = (LinkList *)malloc(sizeof(LinkList));
    p->head.next = NULL;
    p->length = 0;
    return p;
}

LinkNode *linkgetnewnode(char *ip, int sockfd){
    LinkNode *p = (LinkNode *)malloc(sizeof(LinkNode));
    p->ip = strdup(ip);
    p->sockfd = sockfd;
    p->next = NULL;
    return p;
}

void linkinsert(LinkList *l, char *ip, int sockfd){
    LinkNode *p = &(l->head);
    while(p->next){
        p = p->next;
    }
    LinkNode *insert_node = linkgetnewnode(ip,sockfd);
    insert_node->next = p->next;
    p->next = insert_node;
    l->length += 1;
    return ;


}


void linkerase(LinkList *l, LinkNode *needDelete){
    LinkNode *p = &(l->head);
    while(p->next){
        if(p->next == needDelete){
            break;
        }
    }
    if(p->next == NULL){
        return ;
    }
    LinkNode *delete_node = p->next;
    p->next = p->next->next;
    free(delete_node->ip);

    close(delete_node->sockfd);
    free(delete_node);
    l->length -= 1;
    return ;
}


/*int insert(LinkedList head, Node *node) {
    Node *p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
    return 0;

}

int dele(LinkedList head, Node *node) {
    Node *p = head;
    Node *q = head;
    p->len--;
    while (p->next != NULL && p->next != node) {
        q = p;
        p = p->next;
    }
    q->next = p->next;
    free(p);
    return 0;
}

*/

