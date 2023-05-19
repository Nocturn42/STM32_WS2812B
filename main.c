#include "stm32f4xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "math.h"
#include "stm32f4_serial.h"
#include "stm32f4_gpio.h"
#include "stm32f4_delay.h"
#include "WS2812B.h"


char mode;
int color;
int tmp[3]= {} ;
int *ptr_tmp = tmp;
int r,g,b;
char buff [100] = "";
char buff2 [100] = "";
bool led1 = 1;
bool led2 = 1;
bool led3 = 1;
bool led4 = 1;
bool led5 = 1;
bool led6 = 1;
bool led7 = 1;
bool led8 = 1;
uint32_t color1 = 0x000000;
uint32_t color2 = 0x000000;
uint32_t color3 = 0x000000;
uint32_t color4 = 0x000000;
uint32_t color5 = 0x000000;
uint32_t color6 = 0x000000;
uint32_t color7 = 0x000000;
uint32_t color8 = 0x000000;
uint32_t color_tmp = 0x000000;

static const unsigned char BitReverseTable256[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};


void newHSV2RGB(int H, double S, double V, int *a)
{
	double h = H;
	double Hi = floor(H/60);
	double f = h/60 - Hi;
	double p = V*(1-S);
	double q = V*((1-f)*S);
	double t = V*(1-(1-f)*S);
//	printf("%f, %f, %f\n",f,q,t);
	if (Hi==0){
	*a = V * 255;
	*(a+1) = t * 255;
	*(a+2) = p * 255;
	}
	else if(Hi==1){
	*a = q * 255;
	*(a+1) = V * 255;
	*(a+2) = p * 255;
	}
	else if(Hi==2){
	*a = p * 255;
	*(a+1) = V * 255;
	*(a+2) = t * 255;
	}
	else if(Hi==3){
	*a = p * 255;
	*(a+1) = q * 255;
	*(a+2) = V * 255;
	}
	else if(Hi==4){
	*a = t * 255;
	*(a+1) = p * 255;
	*(a+2) = V * 255;
	}
	else if(Hi==5){
	*a = V * 255;
	*(a+1) = p * 255;
	*(a+2) = q * 255;
	}
}

void LED(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4, uint32_t color5, uint32_t color6, uint32_t color7, uint32_t color8)
{
		ws281x_sendBit(color1);
		ws281x_sendBit(color2);
		ws281x_sendBit(color3);
		ws281x_sendBit(color4);
		ws281x_sendBit(color5);
		ws281x_sendBit(color6);
		ws281x_sendBit(color7);
		ws281x_sendBit(color8);
		ws281x_Reset();
}

void still()
{
	while(1){
	newHSV2RGB(color, 1, 0.5,ptr_tmp);
	r = *ptr_tmp;
	g = *(ptr_tmp+1);
	b = *(ptr_tmp+2);
	color1 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color2 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color3 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color4 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color5 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color6 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color7 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color8 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color_tmp = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
		if (led1==0)
		color1 = 0;
	else if(led1==1)
		color1 = color_tmp;
	if (led2==0)
		color2 = 0;
	else if(led2==1)
		color2 = color_tmp;
	if (led3==0)
		color3 = 0;
	else if(led3==1)
		color3 = color_tmp;
	if (led4==0)
		color4 = 0;
	else if(led4==1)
		color4 = color_tmp;
	if (led5==0)
		color5 = 0;
	else if(led5==1)
		color5 = color_tmp;
	if (led6==0)
		color6 = 0;
	else if(led6==1)
		color6 = color_tmp;
	if (led7==0)
		color7 = 0;
	else if(led7==1)
		color7 = color_tmp;
	if (led8==0)
		color8 = 0;
	else if(led8==1)
		color8 = color_tmp;
  LED(color1, color2, color3, color4, color5, color6, color7, color8);
		if(mode!='A')
			break;
	}
	
}

void breath()
{
	double v=0;
	bool change = 0;
	while(1){
	if (change==0)
		v = v+0.01;
	else
		v = v-0.01;
	if(v>=0.8)
		change = 1;
	if(v<=0)
		change = 0;
  newHSV2RGB(color, 1, v,ptr_tmp);
	r = *ptr_tmp;
	g = *(ptr_tmp+1);
	b = *(ptr_tmp+2);
	color1 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color2 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color3 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color4 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color5 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color6 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color7 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color8 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color_tmp = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	if (led1==0)
		color1 = 0;
	else if(led1==1)
		color1 = color_tmp;
	if (led2==0)
		color2 = 0;
	else if(led2==1)
		color2 = color_tmp;
	if (led3==0)
		color3 = 0;
	else if(led3==1)
		color3 = color_tmp;
	if (led4==0)
		color4 = 0;
	else if(led4==1)
		color4 = color_tmp;
	if (led5==0)
		color5 = 0;
	else if(led5==1)
		color5 = color_tmp;
	if (led6==0)
		color6 = 0;
	else if(led6==1)
		color6 = color_tmp;
	if (led7==0)
		color7 = 0;
	else if(led7==1)
		color7 = color_tmp;
	if (led8==0)
		color8 = 0;
	else if(led8==1)
		color8 = color_tmp;
	LED(color1, color2, color3, color4, color5, color6, color7, color8);
	delayMs(25);
	if(mode!='B')
			break;
 }
 
}

void meteor()
{
	while(1){
  newHSV2RGB(color, 1, 0.7,ptr_tmp);
	//printf("%d,%d,%d\n",*ptr_tmp, *(ptr_tmp+1),*(ptr_tmp+2));
	r = *ptr_tmp;
	g = *(ptr_tmp+1);
	b = *(ptr_tmp+2);
	color1 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color2 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color3 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color4 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color5 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color6 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color7 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color8 = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	color_tmp = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	if (led1==0)
		color1 = 0;
	else if(led1==1)
		color1 = color_tmp;
	if (led2==0)
		color2 = 0;
	else if(led2==1)
		color2 = color_tmp;
	if (led3==0)
		color3 = 0;
	else if(led3==1)
		color3 = color_tmp;
	if (led4==0)
		color4 = 0;
	else if(led4==1)
		color4 = color_tmp;
	if (led5==0)
		color5 = 0;
	else if(led5==1)
		color5 = color_tmp;
	if (led6==0)
		color6 = 0;
	else if(led6==1)
		color6 = color_tmp;
	if (led7==0)
		color7 = 0;
	else if(led7==1)
		color7 = color_tmp;
	if (led8==0)
		color8 = 0;
	else if(led8==1)
		color8 = color_tmp;
	LED(color1, 0, 0, 0, 0, 0, 0, 0);
	delayMs(50);
	LED(0, color2, 0, 0, 0, 0, 0, 0);
	delayMs(50);
	LED(0, 0, color3, 0, 0, 0, 0, 0);
	delayMs(50);
	LED(0, 0, 0, color4, 0, 0, 0, 0);
	delayMs(50);
	LED(0, 0, 0, 0, color5, 0, 0, 0);
	delayMs(50);
	LED(0, 0, 0, 0, 0, color6, 0, 0);
	delayMs(50);
	LED(0, 0, 0, 0, 0, 0, color7, 0);
	delayMs(50);
	LED(0, 0, 0, 0, 0, 0, 0, color8);
	delayMs(50);
		if(mode!='C')
			break;
 }
}


void rainbow()
{
	int H = 0;
	while(1){
	int H1 = H + 20;
	int H2 = H + 40;
	int H3 = H + 60;
	int H4 = H + 80;
	int H5 = H + 100;
  int H6 = H + 120;	
	int H7 = H + 140;
	int H8 = H + 160;
	H++;
	if (H>=360)
		H=0;
  H1++;
	if(H1>=360)
		H1 = H1-360;
	H2++;
	if(H2>=360)
		H2 = H2-360;
	H3++;
	if(H3>=360)
		H3 = H3-360;
	H4++;
	if(H4>=360)
		H4 = H4-360;
	H5++;
	if(H5>=360)
		H5 = H5-360;
	H6++;
	if(H6>=360)
		H6 = H6-360;
	H7++;
	if(H7>=360)
		H7 = H7-360;
	H8++;
	if(H8>=360)
		H8 = H8-360;
	printf("%d\n",H);
	newHSV2RGB(H, 1, 0.7,ptr_tmp);
	int r = *ptr_tmp;
	int g = *(ptr_tmp+1);
	int b = *(ptr_tmp+2);
  newHSV2RGB(H1, 1, 0.7,ptr_tmp);
	int r1 = *ptr_tmp;
	int g1 = *(ptr_tmp+1);
	int b1 = *(ptr_tmp+2);
	newHSV2RGB(H2, 1, 0.7,ptr_tmp);
	int r2 = *ptr_tmp;
	int g2 = *(ptr_tmp+1);
	int b2 = *(ptr_tmp+2);
	newHSV2RGB(H3, 1, 0.7,ptr_tmp);
	int r3 = *ptr_tmp;
	int g3 = *(ptr_tmp+1);
	int b3 = *(ptr_tmp+2);
	newHSV2RGB(H4, 1, 0.7,ptr_tmp);
	int r4 = *ptr_tmp;
	int g4 = *(ptr_tmp+1);
	int b4 = *(ptr_tmp+2);
	newHSV2RGB(H5, 1, 0.7,ptr_tmp);
	int r5 = *ptr_tmp;
	int g5 = *(ptr_tmp+1);
	int b5 = *(ptr_tmp+2);
	newHSV2RGB(H6, 1, 0.7,ptr_tmp);
	int r6 = *ptr_tmp;
	int g6 = *(ptr_tmp+1);
	int b6 = *(ptr_tmp+2);
	newHSV2RGB(H7, 1, 0.7,ptr_tmp);
	int r7 = *ptr_tmp;
	int g7 = *(ptr_tmp+1);
	int b7 = *(ptr_tmp+2);
	newHSV2RGB(H8, 1, 0.7,ptr_tmp);
	int r8 = *ptr_tmp;
	int g8 = *(ptr_tmp+1);
	int b8 = *(ptr_tmp+2);
	printf("%d,%d,%d\n",r,g,b);
	printf("%d,%d,%d\n",r8,g8,b8);
	//printf("%d,%d,%d\n",*ptr_tmp, *(ptr_tmp+1),*(ptr_tmp+2));
	color1 = BitReverseTable256[b1]<<16|BitReverseTable256[r1]<<8|BitReverseTable256[g1];
	color2 = BitReverseTable256[b2]<<16|BitReverseTable256[r2]<<8|BitReverseTable256[g2];
	color3 = BitReverseTable256[b3]<<16|BitReverseTable256[r3]<<8|BitReverseTable256[g3];
	color4 = BitReverseTable256[b4]<<16|BitReverseTable256[r4]<<8|BitReverseTable256[g4];
	color5 = BitReverseTable256[b5]<<16|BitReverseTable256[r5]<<8|BitReverseTable256[g5];
	color6 = BitReverseTable256[b6]<<16|BitReverseTable256[r6]<<8|BitReverseTable256[g6];
	color7 = BitReverseTable256[b7]<<16|BitReverseTable256[r7]<<8|BitReverseTable256[g7];
	color8 = BitReverseTable256[b8]<<16|BitReverseTable256[r8]<<8|BitReverseTable256[g8];
	color_tmp = BitReverseTable256[b]<<16|BitReverseTable256[r]<<8|BitReverseTable256[g];
	if (led1==0)
		color1 = 0;
	else if(led1==1)
		color1 = color1;
	if (led2==0)
		color2 = 0;
	else if(led2==1)
		color2 = color2;
	if (led3==0)
		color3 = 0;
	else if(led3==1)
		color3 = color3;
	if (led4==0)
		color4 = 0;
	else if(led4==1)
		color4 = color4;
	if (led5==0)
		color5 = 0;
	else if(led5==1)
		color5 = color5;
	if (led6==0)
		color6 = 0;
	else if(led6==1)
		color6 = color6;
	if (led7==0)
		color7 = 0;
	else if(led7==1)
		color7 = color7;
	if (led8==0)
		color8 = 0;
	else if(led8==1)
		color8 = color8;
	LED(color1, color2, color3, color4, color5, color6, color7, color8);
	delayMs(25);
	if(mode!='D')
			break;
 }
 }
 

void My_Usart3_Printf(char *string){
    while(*string){        
        USART_SendData(USART3, (unsigned short int) *string++);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
}


char * My_Usart3_ReadLine(){    
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){         
        char c = USART_ReceiveData(USART3);         
        if(c != '\r' && c != '\n' && c!= ' '){
            sprintf (buff, "%s%c", buff, c);						
						return buff;
					}
					else{
						
            memcpy(buff2,buff, strlen(buff)); // ? buff ??? buff2
            memset(buff, 0, strlen(buff)); // ?? buff ???????      
						printf("%s\n",buff2);
            return buff2;
					}
        
    }
    return "";
}

int main(void)
	{
	serialBegin(SERIAL2, 115200);
// Baudrate must be 38400 with HC-05 AT mode
	serialBegin(SERIAL3, 9600);
	ws281x_init();
	delayInit(16);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART2_IRQn);
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  NVIC_EnableIRQ(USART3_IRQn);
	
	while(1)
		{
			if(mode=='A'){
				 still();
			}
			if(mode=='B'){
				 breath();
			}
			if(mode=='C'){
				 meteor();
			}
			if(mode=='D'){
				 rainbow();
			}
			
	}
	
	
}

void USART3_IRQHandler(){
 
    
    char * data = My_Usart3_ReadLine();	  	  
    if(!strcmp(data, "A")){
			mode = 'A';
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "B")){
			 mode = 'B';
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "C")){
			 mode = 'C';
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "D")){
			 mode = 'D';
			printf("rainbow\n");
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    }
		
		if(!strcmp(data, "H")){
			 led1 = 1;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "h")){
			 led1 = 0;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "I")){
			 led2 = 1;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "i")){
			led2 = 0;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "J")){
			led3 = 1;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "j")){
			led3 = 0;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "K")){
			led4 = 1;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "k")){
			led4 = 0;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "L")){
			led5 = 1;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "l")){
			led5 = 0;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "M")){
			led6 = 1;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "m")){
			led6 = 0;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "N")){
			led7 = 1;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "n")){
			led7 = 0;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "O")){
			led8 = 1;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, "o")){
			led8 = 0;
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
    } 
		if(!strcmp(data, " ")){
			memset(buff, 0, strlen(buff));
			memset(buff2, 0, strlen(buff2));
			memset(data, 0, strlen(data));
    } 
		else{
		int as,check_ascii;
				for(as=0;data[as]!='\0';as++)
				{
						check_ascii = data[as];
						if( (check_ascii >= 48) || (check_ascii <= 57) )
						{
							color = atoi(data);
							//printf("%d",color);
							if(color >360){ 
								color = 360;
								memset(buff, 0, strlen(buff));
			          memset(buff2, 0, strlen(buff2));
								memset(data, 0, strlen(data));
							}
							
				}
			}  
}
}
	  
void USART2_IRQHandler() 
	{
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
 {
  char c = (char) USART_ReceiveData(USART2);
  serialWrite(SERIAL3, &c);
 }
};
	
