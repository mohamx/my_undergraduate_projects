/* 
 * File:   msgQueue.h
 * Author: MHmed Hatem
 *
 * Created on September 26, 2017, 2:24 PM
 */

#ifndef MSGQUEUE_H
#define	MSGQUEUE_H

#define _PORT_DATATYPE  volatile unsigned char
#define _MASK_DATATYPE   unsigned char

struct m_Queue{
    char *msgPtr;
    struct m_Queue *nextMsg;
};
typedef struct m_Queue mQueue;

struct msg_Queue{
    mQueue *headPtr;
    mQueue *rearPtr;
    _PORT_DATATYPE *port;
    _MASK_DATATYPE mask;
};
typedef struct msg_Queue msgQueue;

extern msgQueue *uartTxMsgQueue;

msgQueue createMsgQueue(volatile unsigned char *port, unsigned char mask);
void pushMsg(msgQueue *qPtr,char *msgPtr);
void popMsg(msgQueue *qPtr);
unsigned char isMsgQueueEmpty(msgQueue *qPtr);

#endif	/* MSGQUEUE_H */

