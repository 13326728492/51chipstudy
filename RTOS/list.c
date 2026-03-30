#include "list.h"





void listInit(struct List* list,char* ListName)
{
    list->listName = ListName;
    list->head = NULL;
    list->tail = NULL;
    list->head->next = list->tail;

}

void listAppend(struct List* list,Node* node)
{
    node->next = NULL;
    if(list->head->next == NULL)
    {
        list->head->next = node;
        list->tail = node;
        
    }
    list->tail->next = node;
    list->tail = node;


}

void listPush(struct List* list,struct Node* node)
{
		struct Node* temp = list->head->next;
    node->next = NULL;
    
    list->head->next = node;
    node->next = temp;

}

struct Node* listPop(struct List* list)
{
    struct Node* ret = list->head->next;
    list->head->next = list->head->next->next;
    ret->next = NULL;

    return ret;
}


struct Node* NodeFind(struct List* plist,struct Node* node)
{
    struct Node* temp = plist->head->next;
    while(temp!=NULL)
    {
        if(temp == node)
        {
            return node;
        }
        temp=temp->next;
    }
    return 0;
}

BOOL ListEmpty(struct List* plist)
{
	return (plist->head->next == NULL)? 1 : 0;
}
//????
struct Node* listRemove(struct List* list,Node* node)
{
    struct Node* prevNode = list->head;
    struct Node* currentNode = list->head->next;
    while(currentNode != NULL&&prevNode->next->next != NULL)
    {
        if(currentNode == node){
            prevNode->next = currentNode->next;
            currentNode->next = NULL;
            return currentNode;
        }
        currentNode = currentNode -> next;
        prevNode = prevNode->next;
    }

    return NULL;
}


void SystemListInit()
{
    listInit(&AllList,"AllList");
    listInit(&ReadyList,"ReadyList");
    listInit(&BlockList,"BlockList");
}