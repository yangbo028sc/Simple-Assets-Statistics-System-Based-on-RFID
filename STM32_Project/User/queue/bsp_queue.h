#ifndef __BSP_QUEUE_H__
#define __BSP_QUEUE_H__

#include "stm32f10x.h"

#define  CMD_QUEUE_MAX_SIZE   512       // ���д�С    
typedef  uint8_t              ElemType; // ����Ԫ������   
typedef  uint8_t              pointer;  // ָ������

typedef struct{
    pointer Head;                       // ����ͷָ��
    pointer Tail;                       // ����βָ��
    pointer Current;                    // ��ǰ��������ָ�룺��ָ��֡ͷ
    ElemType Data[CMD_QUEUE_MAX_SIZE];  // ����
}Cmd_Queue_Type;

void Cmd_Queue_Init(Cmd_Queue_Type *Cmd_Queue);
void Cmd_Queue_Push(Cmd_Queue_Type *Queue, ElemType *data);
void Cmd_Queue_Pop(Cmd_Queue_Type *Queue, ElemType *data);
uint8_t Cmd_Queue_Return_And_Check_String(Cmd_Queue_Type *Cmd_Queue, char *data_string);

#endif /* __BSP_QUEUE_H__ */
