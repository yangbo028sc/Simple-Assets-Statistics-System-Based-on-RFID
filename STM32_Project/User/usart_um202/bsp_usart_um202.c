#include "bsp_usart_um202.h"

static void USART_UM202_NVIC_Configuration(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    
    // Ƕ�������жϿ�������ѡ����2
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    // ѡ���ж�Դ USART
    NVIC_InitStructure.NVIC_IRQChannel = USART_UM202_IRQ;
    
    // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    
    // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    // ʹ���ж�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // ��ʼ������ NVIC
    NVIC_Init(&NVIC_InitStructure);
    
}


/*
***************************************************************************
*	�� �� ��: USART_UM202_Config
*	����˵��: USART1�������ü������ж�ʹ��
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void USART_UM202_Config(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    USART_InitTypeDef   USART_InitStructure;

    // �򿪶�Ӧ GPIO ʱ��
    USART_UM202_GPIO_APBxClkCmd(USART_UM202_GPIO_CLK, ENABLE);
    
    // �򿪶�Ӧ USART ʱ��
    USART_UM202_APBxClkCmd(USART_UM202_CLK, ENABLE);
    
    // ���� USART TX ����ģʽ
    GPIO_InitStructure.GPIO_Pin = USART_UM202_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART_UM202_TX_GPIO_PORT, &GPIO_InitStructure);
    
    
    // ���� USART RX ����ģʽ
    GPIO_InitStructure.GPIO_Pin = USART_UM202_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART_UM202_RX_GPIO_PORT, &GPIO_InitStructure);
    
    // ���� USART ģʽ
    // ������
    USART_InitStructure.USART_BaudRate = USART_UM202_BAUDRATE;
    
    // ����λ����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    
    // һλֹͣλ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    
    // ����żУ��
    USART_InitStructure.USART_Parity = USART_Parity_No;
    
    // ��Ӳ��������
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    // �շ�ģʽ
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART_UM202, &USART_InitStructure);
    
    // �������Ҫʹ���ж�
    if(USART_UM202_INTERRUPT)
    {
        // �����ж����ȼ�����
        USART_UM202_NVIC_Configuration();
    
        // ʹ�ܴ��ڽ����ж�
        USART_ITConfig(USART_UM202, USART_IT_RXNE, ENABLE);
    }
    
    // ʹ�ܴ���
    USART_Cmd(USART_UM202, ENABLE);
}

void Usart_UM202_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx,ch);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*
***************************************************************************
*	�� �� ��: Usart_UM202_Send_8_Bits
*	����˵��: ͨ����Ӧ���ڷ������ŷ��� 8bits ����
*	��    ��: USART_TypeDef * , uint8_t 
*	�� �� ֵ: ��
***************************************************************************
*/
void Usart_UM202_Send_8_Bits( USART_TypeDef * pUSARTx, uint8_t data)
{
    USART_SendData(pUSARTx, data);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*
***************************************************************************
*	�� �� ��: Usart_UM202_Send_16_Bits
*	����˵��: ͨ����Ӧ���ڷ������ŷ��� 16bits ����
*	��    ��: USART_TypeDef * , uint16_t
*	�� �� ֵ: ��
***************************************************************************
*/
void Usart_UM202_Send_16_Bits( USART_TypeDef * pUSARTx, uint16_t data)
{
	uint8_t temp_h, temp_l;
	
	temp_h = (data&0XFF00)>>8;
	temp_l = data&0XFF;
	
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*
***************************************************************************
*	�� �� ��: Usart_UM202_Send_32_Bits
*	����˵��: ͨ����Ӧ���ڷ������ŷ��� 32bits ����
*	��    ��: USART_TypeDef * , uint32_t
*	�� �� ֵ: ��
***************************************************************************
*/
void Usart_UM202_Send_32_Bits( USART_TypeDef * pUSARTx, uint32_t data)
{
    uint16_t temp_h16;
    uint16_t temp_l16;
    temp_h16 = data>>16;
    temp_l16 = data&0xFFFF;
    Usart_UM202_Send_16_Bits(pUSARTx, temp_h16);
    Usart_UM202_Send_16_Bits(pUSARTx, temp_l16);
}

