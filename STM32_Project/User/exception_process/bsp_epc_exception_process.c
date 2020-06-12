#include "bsp_cmd_info_process.h"
#include "bsp_epc_exception_process.h"

extern uint8_t ROW_COUNT;
extern uint8_t *Exception_History_Array;
extern uint8_t *Exception_Current_Array;

// ������¼ Exception_History_Array ����ĳ���
uint16_t History_Array_Element_Count = 1;

// ��ʼ���������ʼ�ռ�
/*
***************************************************************************
*	�� �� ��: Exception_Array_Init
*	����˵��: �����豸��¼�����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Exception_Array_Init(void)
{
    Exception_History_Array = (uint8_t *)malloc(sizeof(uint8_t) * 1);
    Exception_Current_Array = (uint8_t *)malloc(sizeof(uint8_t) * 1);
    *Exception_Current_Array = (uint8_t)1;
    *Exception_History_Array = (uint8_t)0;
}

// �������ݼ�¼�����ݵ���ǰ�쳣��ѯ����
/*
***************************************************************************
*	�� �� ��: Exception_Copy_To_Current_Array
*	����˵��: ����ǰ���ݼ�¼���ѯ�������Ƶ���ǰ�쳣��ѯ����
*	��    ��: Data_Send_Record_Type
*	�� �� ֵ: ��
***************************************************************************
*/
void Exception_Copy_To_Current_Array(Data_Send_Record_Type Data_Send_Record_LinkList)
{
    uint8_t *current_pointer;
    Data_Send_Record_Type p = Data_Send_Record_LinkList->next;
    
    Exception_Current_Array = (uint8_t *)realloc(Exception_Current_Array, sizeof(uint8_t) * ROW_COUNT);
    current_pointer = Exception_Current_Array;
    
    while(p)
    {
        *current_pointer = p->EPC_String_Count;
        current_pointer++;  
        p = p->next;
    }
}

// �������ݼ�¼�����ݵ���ʷ�쳣��ѯ����
/*
***************************************************************************
*	�� �� ��: Exception_Copy_To_Current_Array
*	����˵��: ����ǰ���ݼ�¼���ѯ�������Ƶ���ʷ�쳣��ѯ����
*	��    ��: Data_Send_Record_Type
*	�� �� ֵ: ��
***************************************************************************
*/
void Exception_Copy_To_History_Array(Data_Send_Record_Type Data_Send_Record_LinkList)
{
    uint8_t *history_pointer;
    Data_Send_Record_Type p = Data_Send_Record_LinkList->next;
    
    Exception_History_Array = (uint8_t *)realloc(Exception_History_Array, sizeof(uint8_t) * ROW_COUNT);
    history_pointer = Exception_History_Array;
    
    while(p)
    {
        *history_pointer = p->EPC_String_Count;
        History_Array_Element_Count++;
        history_pointer++;  
        p = p->next;
    }
}

// ����쳣�豸���ݼ�¼
/*
***************************************************************************
*	�� �� ��: Screen_Exception_Data_Clear
*	����˵��: �����Ļ�쳣�豸��¼
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Screen_Exception_Data_Clear(void)
{
    Send_Head();
    Send_Data_Clear();
    Send_Frame_ID();
    Send_Widget_2_ID();
    Send_Tail();
}

// �����쳣�豸���ݼ�¼��Ϣ
/*
***************************************************************************
*	�� �� ��: Screen_Exception_Data_Add
*	����˵��: ����Ļ�����쳣�豸���ݼ�¼��Ϣ
*	��    ��: uint8_t
*	�� �� ֵ: ��
***************************************************************************
*/
void Screen_Exception_Data_Add(uint8_t data)
{
    Send_Head();
    Send_Data_Add();
    Send_Frame_ID();
    Send_Widget_2_ID();
    Send_Number(data);
    Send_Data_Split();
    Send_Tail();
}

// ��Ļ������
/*
***************************************************************************
*	�� �� ��: Screen_Buzzer
*	����˵��: ��Ļ����������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Screen_Buzzer(void)
{
    Send_Head();
    Send_Screen_Buzzer();
    Send_Tail();
}

// ��ѯ�豸�Ƿ��������쳣
/*
***************************************************************************
*	�� �� ��: Exception_Check
*	����˵��: �����ߵ�RFID ��Ϣ��ʾ����Ļ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Exception_Check(void)
{
    uint8_t i = 0;
    uint8_t offline_count = 0;
    uint8_t *history_pointer = Exception_History_Array;
    uint8_t *current_pointer = Exception_Current_Array;
    
    // ÿ���һ��Ҫ���һ���쳣�豸��
    Screen_Exception_Data_Clear();
    
    // ��������
    for(i = 0; i < History_Array_Element_Count; i++)
    {
        if(*history_pointer == *current_pointer)
        {
            Screen_Exception_Data_Add(i);
            offline_count++;
        }
        history_pointer++;
        current_pointer++;
    }
    
    // ���͵���Ļ
    Send_Online_EPC_Num(ROW_COUNT - offline_count);
    Send_Offline_EPC_Num(offline_count);
    
    // ��������
    if(offline_count > 0 && SCREEN_BUZZER_SWITCH)
    {
       Screen_Buzzer();
    }
    
    // Exception_History_Array ����ĳ�����������
    History_Array_Element_Count = 0;
}

