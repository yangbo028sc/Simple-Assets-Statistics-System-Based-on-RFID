#include "bsp_queue.h"
#include "bsp_cmd_info_process.h"

/*
***************************************************************************
*	�� �� ��: Cmd_Queue_Init
*	����˵��: ��ʼ��EPC���ն���Cmd_Queue_Init
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Cmd_Queue_Init(Cmd_Queue_Type *Cmd_Queue)
{
    Cmd_Queue->Head = 0;
    Cmd_Queue->Tail = 0;
    Cmd_Queue->Current = 0;
}

// �����
/*
***************************************************************************
*	�� �� ��: Cmd_Queue_Push
*	����˵��: Ԫ�������
*	��    ��: Cmd_Queue_Type *, ElemType *
*	�� �� ֵ: ��
***************************************************************************
*/
void Cmd_Queue_Push(Cmd_Queue_Type *Cmd_Queue, ElemType *data)
{
    if((Cmd_Queue->Tail - Cmd_Queue->Head + 1) == CMD_QUEUE_MAX_SIZE)
    {
        Cmd_Queue->Tail = 0;
    }
    Cmd_Queue->Data[Cmd_Queue->Tail] = *data;    
    Cmd_Queue->Tail++;
}

// ������
/*
***************************************************************************
*	�� �� ��: Cmd_Queue_Pop
*	����˵��: Ԫ�س�����
*	��    ��: Cmd_Queue_Type *, ElemType *
*	�� �� ֵ: ��
***************************************************************************
*/
void Cmd_Queue_Pop(Cmd_Queue_Type *Cmd_Queue, ElemType *data)
{
    if((Cmd_Queue->Current +1) == CMD_QUEUE_MAX_SIZE)
    {
        Cmd_Queue->Current = 0;
    }
    *data = Cmd_Queue->Data[Cmd_Queue->Current];
    Cmd_Queue->Current++;

}

/* ����һ�� EPC ��Ϣ����������Ƿ���ȷ */
// EPC��Ϣǰ׺
char *EPC_INFO_PREFIX_1 = "222016333210018000";
char *EPC_INFO_PREFIX_2 = "222016333210019000";
char *EPC_INFO_PREFIX_3 = "222016333210020000";

// һ���ϴ��ֵ���ܹ�װ�½ϳ���EPC��Ϣ �˴�Ϊ64�ֽ�
#define  STRING_TEMP_LENGTH  64

/*
***************************************************************************
*	�� �� ��: Cmd_Queue_Return_And_Check_String
*	����˵��: EPC��Ϣ����
*	��    ��: Cmd_Queue_Type *Cmd_Queue, char *data_string
*	�� �� ֵ: uint8_t
***************************************************************************
*/
uint8_t Cmd_Queue_Return_And_Check_String(Cmd_Queue_Type *Cmd_Queue, char *data_string)
{
    uint8_t i = 0;
    uint8_t temp;
    char string_temp[STRING_TEMP_LENGTH];

    // ȡ��һ������
    while(1)
    {
        Cmd_Queue_Pop(Cmd_Queue, &temp);
        string_temp[i] = temp;
        i++;
        if(temp == '\n')
        {
            break;
        }
    }
    
    // �Ƚ��ַ���ǰ׺
    if((strncmp(string_temp, EPC_INFO_PREFIX_1, strlen(EPC_INFO_PREFIX_1)) == 0) || 
       (strncmp(string_temp, EPC_INFO_PREFIX_2, strlen(EPC_INFO_PREFIX_2)) == 0) ||
       (strncmp(string_temp, EPC_INFO_PREFIX_3, strlen(EPC_INFO_PREFIX_3)) == 0))
    {
        strncpy(data_string, string_temp, DATA_BYTE_LENGTH);
        return 1;
    }
    else
    {
        return 0;
    }
}


