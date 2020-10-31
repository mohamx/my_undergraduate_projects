/* 
 * File:   Queue.h
 * Author: MHmed Hatem
 *
 * Created on September 26, 2017, 2:24 PM
 */

#ifndef CHRQUEUE_H
#define	CHRQUEUE_H

#define _PORT_DATATYPE  volatile unsigned char
#define _MASK_DATATYPE  unsigned char

struct c_Queue{
    char chr;
    struct c_Queue *nextChr;
};
typedef struct c_Queue cQueue;

struct chr_Queue{
    cQueue *headPtr;
    cQueue *rearPtr;
    _PORT_DATATYPE *port;
    _MASK_DATATYPE mask;
};
typedef struct chr_Queue chrQueue;

extern chrQueue *uartTxChrQueue;
extern chrQueue *sspTxChrQueue;

chrQueue createChrQueue(volatile unsigned char *port,unsigned char mask);
void pushChr(chrQueue *qPtr,char chr);
void popChr(chrQueue *qPtr);
unsigned char isChrQueueEmpty(chrQueue *qPtr);

#endif	/* CHRQUEUE_H */

