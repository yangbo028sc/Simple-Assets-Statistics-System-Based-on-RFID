#ifndef __BSP_CMD_INFO_PROCESS_H__
#define __BSP_CMD_INFO_PROCESS_H__

#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "bsp_usart_screen.h"
#include "bsp_usart_zigbee.h"

// ������ֵ(��ѯ����) uint8_t max = 255
#define EPC_String_Count_Max     255

#define DATA_BYTE_LENGTH          22        // UM202 ���������ݹ���22���ֽ�
#define EPC_INFO_LENGTH           20        // EPC ������Ч��Ϣ����
#define FRAME_TAIL_LENGTH         2         // ֡β����

/* �������� */
#define SCREEN_HEAD              0xEE        // ��Ļ֡ͷ
#define SCREEN_TAIL              0xFFFCFFFF  // ��Ļ֡β
#define SCREEN_SWITCH            0xB100      // ��Ļ�л������ݼ�¼ҳ������
#define SCREEN_DATA_ADD          0xB152      // �������ݼ�¼����
#define SCREEN_DATA_CLEAR        0xB153      // �����ʷ���ݼ�¼����
#define SCREEN_WRITE_TEXT        0xB110      // д�ı�����
#define SCREEN_DATA_MODIFY       0xB157      // �޸����ݼ�¼����
#define SCREEN_DATA_SPLIT        0x3B        // ���ݼ�¼֮��ķֺ�
#define SCREEN_FRAME_ID          0x0001      // ���� ID
#define SCREEN_WIDGET_1_ID       0x0001      // ���ݼ�¼�ؼ� ID
#define SCREEN_WIDGET_2_ID       0x0002      // ���ݼ�¼�ؼ� ID
#define SCREEN_TEXT_TOTAL_ID     0x0003      // �ı��ؼ� ID 3
#define SCREEN_TEXT_Online_ID    0x0004      // �ı��ؼ� ID 4
#define SCREEN_TEXT_Offline_ID   0x0005      // �ı��ؼ� ID 5
#define SCREEN_BUZZER            0x610E      // ��Ļ������ EE�� 61 Time ��FF FC FF FF ����˵���� Time (1 ���ֽ�): ������Ѷ���ʱ�䣬��λ 10ms

#define Send_Head()              Usart_Screen_Send_8_Bits(USART_Screen, SCREEN_HEAD)
#define Send_Tail()              Usart_Screen_Send_32_Bits(USART_Screen, SCREEN_TAIL)
#define Send_Data_Clear()        Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_DATA_CLEAR)
#define Send_Write_Text()        Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_WRITE_TEXT)
#define Send_Screen_Switch()     Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_SWITCH)
#define Send_Text_Total_ID()     Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_TEXT_TOTAL_ID)
#define Send_Text_Online_ID()    Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_TEXT_Online_ID)
#define Send_Text_Offline_ID()   Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_TEXT_Offline_ID)
#define Send_Data_Add()          Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_DATA_ADD)
#define Send_Data_Modify()       Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_DATA_MODIFY)
#define Send_Frame_ID()          Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_FRAME_ID)
#define Send_Widget_1_ID()       Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_WIDGET_1_ID )
#define Send_Widget_2_ID()       Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_WIDGET_2_ID )
#define Send_Data_Split()        Usart_Screen_Send_8_Bits(USART_Screen, SCREEN_DATA_SPLIT)
#define Send_Screen_Buzzer()     Usart_Screen_Send_16_Bits(USART_Screen, SCREEN_BUZZER)

typedef struct EPC_Node{
    uint8_t  EPC_String_Count;                 // ��ͬ EPC ��Ϣ�ķ��ʹ���  
    char     EPC_String[DATA_BYTE_LENGTH];     // ���ݷ��ͼ�¼
    struct   EPC_Node *next;
}EPC_Node, *Data_Send_Record_Type;

Data_Send_Record_Type Create_Data_Send_Record_LinkList(void);
Data_Send_Record_Type Create_Data_Send_Record_Node(void);
Data_Send_Record_Type EPC_Info_Is_Exist(char *data_string, Data_Send_Record_Type Data_Send_Record_LinkList);

void Screen_Init(void);
void Send_Total_EPC_Num(void);
void Send_Number(uint8_t temp);
void Send_Online_EPC_Num(uint8_t count);
void Send_Offline_EPC_Num(uint8_t count);

void Screen_Data_Add(char *data_string, Data_Send_Record_Type Data_Send_Record_LinkList);
void Screen_Data_Modify(char* data_string, Data_Send_Record_Type exist_node);

void Send_EPC_To_Zigbee(char *data_string);

#endif /* __BSP_CMD_INFO_PROCESS_H__ */
