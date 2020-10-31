
#include "chrQueue.h"
//#include "Includes.h"
chrQueue createChrQueue(volatile unsigned char *port,unsigned char mask){
    chrQueue nQueue;
    nQueue.port =  port;
    nQueue.mask = mask;
    return nQueue;
}
void pushChr(chrQueue *qPtr,char chr){
    if(isChrQueueEmpty(qPtr)){
        
        qPtr->headPtr = qPtr->rearPtr = (cQueue *)malloc(sizeof(cQueue));
        qPtr->rearPtr->chr = chr;
        *qPtr->port |= qPtr->mask;                           // Start sending messages in the queue
    }
    else{
        qPtr->rearPtr->nextChr = (cQueue *)malloc(sizeof(cQueue));
        qPtr->rearPtr = qPtr->rearPtr->nextChr;
        qPtr->rearPtr->chr = chr;
    }
}
void popChr(chrQueue *qPtr){
    if(isChrQueueEmpty(qPtr))
        return;
    if(qPtr->headPtr == qPtr->rearPtr){
        free(qPtr->headPtr);
        qPtr->headPtr = (cQueue *)0 ;
        qPtr->rearPtr = (cQueue *)0;
    }
    else{
        cQueue *tempPtr = qPtr->headPtr;
        qPtr->headPtr = qPtr->headPtr->nextChr;
        free(tempPtr);
    }   
}
unsigned char isChrQueueEmpty(chrQueue *qPtr){
    if(qPtr->headPtr == (cQueue *)0 && qPtr->rearPtr == (cQueue *)0)
        return 1;
    return 0;
}
