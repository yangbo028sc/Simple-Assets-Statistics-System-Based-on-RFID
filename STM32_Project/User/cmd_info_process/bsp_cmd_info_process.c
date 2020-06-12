#include "bsp_systick.h"
#include "bsp_cmd_info_process.h"

extern uint8_t ROW_COUNT;
extern uint8_t SCREEN_MODIFY_ROW;

/****************************************** ������� **********************************************/
/**************************************************************************************************/
// ����ͷ�ڵ�


/*
***************************************************************************
*	�� �� ��: Create_Data_Send_Record_Node
*	����˵��: ����һ�����ݼ�¼����ͷ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
Data_Send_Record_Type Create_Data_Send_Record_LinkList(void)
{
    Data_Send_Record_Type head = (Data_Send_Record_Type)malloc(sizeof(EPC_Node));
    if (head != NULL)
    {
        head->next = NULL;
        head->EPC_String_Count = 0;
    }
    return head;
}

// �����ڵ�
/*�������ļ� starup_stm32f10x_hd.s �� ���� 
  Heap_Size Ϊ EQU     0x0000F000 �ѿռ��
  �䲻��ᵼ�� malloc ����ռ�ʧ��*/

/*
***************************************************************************
*	�� �� ��: Create_Data_Send_Record_Node
*	����˵��: ����һ�����ݼ�¼������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
Data_Send_Record_Type Create_Data_Send_Record_Node(void)
{
    Data_Send_Record_Type node = (Data_Send_Record_Type) malloc(sizeof( EPC_Node));
    if (node != NULL)
    {
        node->next = NULL;
        node->EPC_String_Count = 0;
    }
    return node;
}

// ����ڵ�
void Data_Record_Insert_Node(char *data_string, Data_Send_Record_Type Data_Send_Record_LinkList)
{
    Data_Send_Record_Type p = Data_Send_Record_LinkList;       
    Data_Send_Record_Type Node = Create_Data_Send_Record_Node();        // ����һ���ڵ� ����ʼ��
           
    Node->EPC_String_Count = 1;                                         // ��������ݲ�������,Count = 1;
    strncpy(Node->EPC_String, data_string, DATA_BYTE_LENGTH);           // ���ַ�����ֵ�� Node.EPC_String
            
    // ����ڵ�
    while(p->next != NULL) 
    {
        p=p->next;
    }
    p->next = Node;
}


// ���Ҵ�EPC��Ϣ��Data_Send_Record.EPC_String�Ƿ����
// �����ڷ��ش��ڡ���¼�к� SCREEN_MODIFY_ROW
Data_Send_Record_Type EPC_Info_Is_Exist(char *data_string, Data_Send_Record_Type Data_Send_Record_LinkList)
{
    // �ڷ��ͱ������ʷ��¼
    Data_Send_Record_Type p = Data_Send_Record_LinkList;
    while(p)
    {
        p = p->next;
        if(strcmp(p->EPC_String, data_string) == 0)
        {
            return p;
        }
        SCREEN_MODIFY_ROW ++; // ��¼��Ҫ�޸ĵ�����
    }
    return NULL;
}
/**************************************************************************************************/
/**************************************************************************************************/

/* ���͵����ֶ��� 3λ�� ��ʾ */
void Send_Number(uint8_t temp)
{
    if (temp >= 1 && temp < 10)
    {
        Usart_Screen_Send_8_Bits( USART_Screen, 0x30); 
        Usart_Screen_Send_8_Bits( USART_Screen, 0x30); 
        Usart_Screen_Send_8_Bits( USART_Screen, temp % 10 + 0x30); 
    }
    else if (temp >= 10 && temp < 100)
    {
        Usart_Screen_Send_8_Bits( USART_Screen, 0x30); 
        Usart_Screen_Send_8_Bits( USART_Screen, (temp / 10) + 0x30);
        Usart_Screen_Send_8_Bits( USART_Screen, temp % 10 + 0x30); 
    }
    else
    {
        Usart_Screen_Send_8_Bits( USART_Screen, (temp / 100) + 0x30);
        Usart_Screen_Send_8_Bits( USART_Screen, ((temp- (temp / 100)*100) / 10) + 0x30);
        Usart_Screen_Send_8_Bits( USART_Screen, temp % 10 + 0x30); 
    }
}

// �л�����
void Screen_Switch(void)
{
    Send_Head();
    Send_Screen_Switch();
    Send_Frame_ID();
    Send_Tail();
}

// �����ʷ����
void Screen_Data_Clear(void)
{
    // ������ݼ�¼��
    Send_Head();
    Send_Data_Clear();
    Send_Frame_ID();
    Send_Widget_1_ID();
    Send_Tail();
    
    // ����쳣�豸��
    Send_Head();
    Send_Data_Clear();
    Send_Frame_ID();
    Send_Widget_2_ID();
    Send_Tail();
}

// �����豸�����ı�
/*
***************************************************************************
*	�� �� ��: Send_Total_EPC_Num
*	����˵��: ����Ļ���͵�ǰ��⵽��RFID��ǩ����
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Send_Total_EPC_Num(void)
{
    Send_Head();
    Send_Write_Text();
    Send_Frame_ID();
    Send_Text_Total_ID();
    Send_Number(ROW_COUNT);
    Send_Tail();
}


// ���������豸�����ı�
/*
***************************************************************************
*	�� �� ��: Send_Online_EPC_Num
*	����˵��: ����Ļ���ͷ��������豸�����ı�
*	��    ��: uint8_t
*	�� �� ֵ: ��
***************************************************************************
*/
void Send_Online_EPC_Num(uint8_t count)
{
    Send_Head();
    Send_Write_Text();
    Send_Frame_ID();
    Send_Text_Online_ID();
    Send_Number(count);
    Send_Tail();
}

// ���������豸�����ı�
/*
***************************************************************************
*	�� �� ��: Send_Offline_EPC_Num
*	����˵��: ����Ļ���ͷ��������豸�����ı�
*	��    ��: uint8_t
*	�� �� ֵ: ��
***************************************************************************
*/
void Send_Offline_EPC_Num(uint8_t count)
{
    Send_Head();
    Send_Write_Text();
    Send_Frame_ID();
    Send_Text_Offline_ID();
    Send_Number(count);
    Send_Tail();
}

// ������Ƶ
// EE 94 33 3A 2F 53 6F 75 6E 64 73 2F 30 2E 77 61 76 FF FC FF FF 
/*
***************************************************************************
*	�� �� ��: Screen_Start_Music
*	����˵��: ��ʾ��������Ƶ
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Screen_Start_Music(void)
{
    Send_Head();      
    Usart_Screen_Send_32_Bits(USART_Screen, 0x94333A2F);
    Usart_Screen_Send_32_Bits(USART_Screen, 0x536F756E);
    Usart_Screen_Send_32_Bits(USART_Screen, 0x64732F30);
    Usart_Screen_Send_32_Bits(USART_Screen, 0x2E776176);
    Send_Tail();
}

// �豸����������Ļ�ĳ�ʼ����ʾ
/*
***************************************************************************
*	�� �� ��: Screen_Init
*	����˵��: ��ʼ����Ļ�������������ֵ�
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Screen_Init(void)
{    
    Screen_Start_Music();       // ��Ļ�������ֲ���
    Screen_Switch();            // �л����� 
    Screen_Data_Clear();        // �����ʷ����
    Send_Total_EPC_Num();       // EPC�豸��������Ļ
    Send_Online_EPC_Num(0);     // EPC�����豸������Ļ
    Send_Offline_EPC_Num(0);    // EPC�����豸������Ļ
}

/**************************************************************************************************/

// ��zigbee��������
void Send_EPC_To_Zigbee(char *data_string)
{
    uint8_t i = 0;
    for(i = 0; i < (DATA_BYTE_LENGTH - FRAME_TAIL_LENGTH); i++)
    {
        Usart_Zigbee_Send_8_Bits(USART_Zigbee, data_string[i]);
    }
}

/**************************************************************************************************/

// ����� �������ݵ���ž��� Data_Send_Record.Row_Count
void Send_Number_Add(void)
{
    Send_Number(ROW_COUNT);
}

// �� EPC
void Send_EPC(char *data_string)
{
    uint8_t i = 0;
    for ( i = 0; i < (DATA_BYTE_LENGTH - FRAME_TAIL_LENGTH); i++)
    {    
        Usart_Screen_Send_8_Bits(USART_Screen, data_string[i]);
    }
}

// �������ݵ�ͳ�ƴ��� 1
void Send_Count_Add(void)
{
    Send_Number(1);
}
  
// ��Ļ�޸����� �޸�������16λ��
void Send_Row_Modify()
{
    Usart_Screen_Send_16_Bits( USART_Screen, SCREEN_MODIFY_ROW); 
}

// ��������
void Send_Number_Modify(void)
{
    Send_Number(SCREEN_MODIFY_ROW);
}

// �޸����ݵ�ͳ�ƴ���Ӧ�ô�Data_Send_Record�ж�ȡ
void Send_Count_Modify(EPC_Node *exist_pointer)
{
    exist_pointer->EPC_String_Count++;  
    Send_Number(exist_pointer->EPC_String_Count);
}

// �������ݼ�¼����Ϣ
/*
***************************************************************************
*	�� �� ��: Screen_Data_Add
*	����˵��: ����Ļ����һ��EPC��ʾ����
*	��    ��: char * ,Data_Send_Record_Type
*	�� �� ֵ: ��
***************************************************************************
*/
void Screen_Data_Add(char *data_string, Data_Send_Record_Type Data_Send_Record_LinkList)
{
    Send_Head();                // ��֡ͷ
    Send_Data_Add();            // ��������������
    Send_Frame_ID();            // ������ID
    Send_Widget_1_ID();         // ���ؼ�ID
    
    Send_Number_Add();          // �����
    Send_Data_Split();
    Send_EPC(data_string);      // �� EPC
    Send_Data_Split();
    Send_Count_Add();           // ��������
    Send_Data_Split();
    
    Send_Tail();                // ��֡β

    ROW_COUNT++;                // ���������� 1
    Data_Record_Insert_Node(data_string, Data_Send_Record_LinkList);    // ��Data_Send_Record�������������ݵķ��ʹ�����EPC_INFO
}

// �޸����ݼ�¼����Ϣ
/*
***************************************************************************
*	�� �� ��: Screen_Data_Modify
*	����˵��: �޸�һ��EPC��ʾ����
*	��    ��: char *, EPC_Node*
*	�� �� ֵ: ��
***************************************************************************
*/
void Screen_Data_Modify(char* data_string, EPC_Node* exist_node)
{
    Send_Head();                // ��֡ͷ   
    Send_Data_Modify();         // ���޸���������
    Send_Frame_ID();            // ������ID
    Send_Widget_1_ID();         // ���ؼ�ID
    Send_Row_Modify();          // ���޸ĵ�����
    
    Send_Number_Modify();       // ���޸ĵ����
    Send_Data_Split();
    Send_EPC(data_string);      // �� EPC
    Send_Data_Split();
    Send_Count_Modify(exist_node);  // ��������
    Send_Data_Split();

    Send_Tail();                // ��֡β
}

/**************************************************************************************************/
