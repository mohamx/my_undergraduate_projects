
#include "msgQueue.h"
//#include "Includes.h"

msgQueue createMsgQueue(volatile unsigned char *port, unsigned char mask){
    msgQueue nQueue;
    nQueue.port =  port;
    nQueue.mask = mask;
    return nQueue;
}
void pushMsg(msgQueue *qPtr,char *msgPtr){
    if(isMsgQueueEmpty(qPtr)){        
        qPtr->headPtr = qPtr->rearPtr = (mQueue*)malloc(sizeof(mQueue));
        (qPtr->rearPtr)->msgPtr = msgPtr;
        *qPtr->port |= qPtr->mask;                           // Start sending messages in the queue
    }
    else{
        qPtr->rearPtr->nextMsg = (mQueue*)malloc(sizeof(mQueue));
        qPtr->rearPtr = qPtr->rearPtr->nextMsg;
        qPtr->rearPtr->msgPtr = msgPtr;
    }
}
void popMsg(msgQueue *qPtr){
    if(isMsgQueueEmpty(qPtr))
        return;
    if(qPtr->headPtr == qPtr->rearPtr){
        free(qPtr->headPtr);
        qPtr->headPtr = (mQueue *)0;
        qPtr->rearPtr = (mQueue *)0;
    }
    else{
        mQueue *tempPtr;
        tempPtr = qPtr->headPtr;
        qPtr->headPtr = qPtr->headPtr->nextMsg;
        free(tempPtr);
    }   
}
unsigned char isMsgQueueEmpty(msgQueue *qPtr){
    if(qPtr->headPtr == (mQueue *)0 && qPtr->rearPtr == (mQueue *)0)
        return 1;
    return 0;
}
