#include "bsp_led.h"
#include "bsp_queue.h"
#include "bsp_systick.h"
#include "bsp_usart_um202.h"
#include "bsp_usart_screen.h"
#include "bsp_usart_zigbee.h"
#include "bsp_cmd_info_process.h"
#include "bsp_epc_exception_process.h"

uint8_t ROW_COUNT = 0;                              // ���ݼ�¼������
uint32_t while_count = 0;                           // ������ while ������
uint8_t INTERRUPT_END = 0;                          // ���� 1 �����жϱ�־λ
uint8_t SCREEN_MODIFY_ROW = 0;                      // ������Ļ���ݼ�¼��
char data_string[DATA_BYTE_LENGTH];                 // ��ǰ EPC ��Ϣ����

Cmd_Queue_Type Cmd_Queue;                           // ������б�������ʼ��
uint8_t *Exception_History_Array;                   // ��ѯ�����豸��ʷ��¼
uint8_t *Exception_Current_Array;                   // ��ѯ�����豸��ǰ��¼
Data_Send_Record_Type exist_node;                   // ��Ų�����Ϣ ���ݷ��ͼ�¼���ʼ��
Data_Send_Record_Type Data_Send_Record_LinkList;    // ���ݷ��ͼ�¼����

void Device_Init(void);
void EPC_Check_And_Send_To_Screen(void);
void EPC_Check_Exception_And_Send_To_Screen(void);

/*
***************************************************************************
*	�� �� ��: main
*	����˵��: ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
int main(void)
{   
    Device_Init();    
    while(1)
    {
        if(INTERRUPT_END)
        {            
            EPC_Check_And_Send_To_Screen();             
            INTERRUPT_END = 0;                                                            // ����жϱ�־
        }
        EPC_Check_Exception_And_Send_To_Screen();
    }
}




/* �������,SysTick,LED,USART1,USART2
   USART3,Screen,�쳣EPC��ѯ��ʼ��
*/

/*
***************************************************************************
*	�� �� ��: Device_Init
*	����˵��: ��ʼ��������ϵͳӲ��
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Device_Init(void)
{
    Cmd_Queue_Init(&Cmd_Queue);  
    exist_node = Create_Data_Send_Record_Node();  
    Data_Send_Record_LinkList = Create_Data_Send_Record_LinkList();    

    LED_GPIO_Config();
    USART_UM202_Config();
    USART_Screen_Config();
    USART_Zigbee_Config();
    
    //SysTick_Init();
    Screen_Init();
    Exception_Array_Init();
}

/* ����һ�� EPC ��Ϣ 
   ��鲢�������Ļ
*/
/*
***************************************************************************
*	�� �� ��: EPC_Check_And_Send_To_Screen
*	����˵��: EPC��Ϣ���غͷ�����ʾ
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void EPC_Check_And_Send_To_Screen()
{
    if(Cmd_Queue_Return_And_Check_String(&Cmd_Queue, data_string))
    {
        LED_G_ON;                                                                   // �жϷ�����EPC��Ϣ�Ϸ��̵���   
        
        exist_node = EPC_Info_Is_Exist(data_string, Data_Send_Record_LinkList);     // ��ѯ����EPC��Ϣ�Ƿ��Ѿ������
        if (exist_node)
        {                    
            Screen_Data_Modify(data_string, exist_node);                            // �޸�һ�����ݼ�¼
            SCREEN_MODIFY_ROW = 0; 
            // �޸�����������0
        }
        else
        {                    
            Screen_Data_Add(data_string, Data_Send_Record_LinkList);                // ����һ�����ݼ�¼                    
            Send_Total_EPC_Num();                                                   // ��������EPC�豸��
            SCREEN_MODIFY_ROW = 0;                                                  // �޸�����������0
            
            // �����ݷ��� ZIGBEE                                                    // �����Ҫ�����ݷ��͵��ⲿ�豸���� USART3
            // Send_EPC_To_Zigbee(data_string);   
                         
        }
        LED_G_OFF;  
                 
    }           
}

/* ��鶪ʧ��EPC */
/*
***************************************************************************
*	�� �� ��: EPC_Check_Exception_And_Send_To_Screen
*	����˵��: ������ߵ�RFID EPC,������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void EPC_Check_Exception_And_Send_To_Screen(void)
{
       Exception_Copy_To_Current_Array(Data_Send_Record_LinkList);
       if(while_count % EPC_EXCEPTION_TOLERANCE == 0)
       {
           Exception_Check();
           Exception_Copy_To_History_Array(Data_Send_Record_LinkList);
       } 
       while_count++;
}
