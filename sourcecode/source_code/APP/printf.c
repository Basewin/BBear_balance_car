#include"printf.h"


// ��������
int fputc(int ch, FILE *f)
{
   USART_SendData(USART1, (unsigned char) ch);// USART1 ���Ի��� USART2 ��
   while (!(USART1->SR & USART_FLAG_TXE));
   return (ch);
}
// ��������
int GetKey (void)  
{
   while (!(USART1->SR & USART_FLAG_RXNE));
   return ((int)(USART1->DR & 0x1FF));
}
