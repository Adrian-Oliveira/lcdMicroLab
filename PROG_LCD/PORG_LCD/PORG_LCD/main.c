#define F_CPU 1000000UL

#include <util/delay.h>
#include <avr/io.h>
#include "lcd.h"
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


#define set_bit(Y,bit_x) (Y|=(1<<bit_x))
#define clr_bit(Y,bit_x) (Y&=~(1<<bit_x))
#define tst_bit(Y,bit_x) (Y&(1<<bit_x))
#define cpl_bit(Y,bit_x) (Y^=(1<<bit_x))

unsigned char EEMEM valor_inicial[4] = {0x33, 0x22, 0x11, 0x55};

const unsigned char teclado[4][4] PROGMEM = {{'1', '2', '3', 'A'},
												{'4', '5', '6', 'B'},
												{'7', '8', '9', 'C'},
												{'F', '0', 'E', 'D'}};

const unsigned char USER1[6] PROGMEM ={'A','B','C','1','2','3'};

const unsigned char USER2[6] PROGMEM ={'B','C','D','2','3','4'};

const unsigned char USER3[6] PROGMEM ={'C','D','F','3','4','5'};


int main(void)
{
	
    /* Replace with your application code */
	lcd_init(LCD_DISP_ON_CURSOR); 
 
	DDRD=0x00;
	PORTD=0xFF;
	
	DDRC=0xFF;
	PORTC=0x00;
	
	DDRB=0x0F;
	PORTB=0xFF;
	                            
	MCUCR = 0x00;
	
	GICR = (1<<INT0);
	sei();
	
	unsigned char l,t='r',i,j,s[3]={'r','r','r'},f='0',fl='0',sh[3]={'s','s','s'};
	
	
	
while(1)
{
	s[0]='r';
	s[1]='r';
	s[2]='r';
	f='0';
	fl='0';
	while (fl=='0') 
	{
	
		
		lcd_puts("Login:");
		
		
		
		
		
	while (f=='0')
			{
				for(j=0;j<4;j++)
				{
				
				
					clr_bit(PORTB,j);
					_delay_ms(10);
					for(i=0;i<4;i++)
					{
						l=i+4;
						if(!tst_bit(PINB,l))
						{
							t = pgm_read_byte(&teclado[i][j]);
							lcd_putc(t);
							if ((t!='r')&(s[0]=='r')){
								s[0]=t;
								t='r';
								}
							while(!tst_bit(PINB,l));
							if ((t!='r')&(s[0]!='r')&(s[1]=='r')&(s[2]=='r')){
								s[1]=t;
								t='r';
								}
							while(!tst_bit(PINB,l));
							if ((t!='r')&(s[0]!='r')&(s[1]!='r')&(s[2]=='r'))
								{
									s[2]=t;
									f='1';
								}
								
							while(!tst_bit(PINB,l));
						}
						l=0;
					
						t='r';
					}
					set_bit(PORTB,j);
				}
			
			};
			
			if((s[0]==USER1[0])&&(s[1]==USER1[1])&&(s[2]==USER1[2]))
				fl='1';
			if((s[0]==USER2[0])&&(s[1]==USER2[1])&&(s[2]==USER2[2]))
				fl='2';
			if((s[0]==USER3[0])&&(s[1]==USER3[1])&&(s[2]==USER3[2]))
				fl='3';
			if(fl=='0')
				{
					lcd_gotoxy(0,1);
					lcd_puts("login invalido");
					
					
				}
				
			
	 };
	 
	lcd_clrscr();
	lcd_puts("senha:");
	if(fl=='1'){sh[0]=USER1[3];sh[1]=USER1[4];sh[2]=USER1[5];}
	if(fl=='2'){sh[0]=USER2[3];sh[1]=USER2[4];sh[2]=USER2[5];}
	if(fl=='3'){
		sh[0]=USER3[3];
		sh[1]=USER3[4];
		sh[2]=USER3[5];}
	s[0]='r';
	s[1]='r';
	s[2]='r';
	f='0';
		
		while (f=='0')
		{
			for(j=0;j<4;j++)
			{
				
				
				clr_bit(PORTB,j);
				_delay_ms(10);
				for(i=0;i<4;i++)
				{
					l=i+4;
					if(!tst_bit(PINB,l))
					{
						t = pgm_read_byte(&teclado[i][j]);
						lcd_putc(t);
						if ((t!='r')&(s[0]=='r')){
							s[0]=t;
							t='r';
						}
						while(!tst_bit(PINB,l));
						if ((t!='r')&(s[0]!='r')&(s[1]=='r')&(s[2]=='r')){
							s[1]=t;
							t='r';
						}
						while(!tst_bit(PINB,l));
						if ((t!='r')&(s[0]!='r')&(s[1]!='r')&(s[2]=='r'))
						{
							s[2]=t;
							f='1';
						}
						
						while(!tst_bit(PINB,l));
					}
					l=0;
					
					t='r';
				}
				set_bit(PORTB,j);
			}
			
		};
		
		lcd_clrscr();
		if((s[0]==sh[0])&&(s[1]==sh[1])&&(s[2]==sh[2]))
		{
			lcd_gotoxy(0,1);
			switch (fl)
			{
				case '1':
					lcd_puts("ola USER1");
					break;
				case '2':
					lcd_puts("ola USER2");
					break;
				case '3':
					lcd_puts("ola USER3");
					break;
			}
			lcd_gotoxy(0,1);
			lcd_puts("Porta aberta");
			set_bit(PORTC,PC2);
			set_bit(PORTC,PC1);
			set_bit(PORTC,PC0);
			_delay_ms(5000);
			clr_bit(PORTC,PC2);
		}
		
		else
		{
			lcd_puts("senha invalida");
			_delay_ms(3000);
		}
		lcd_clrscr();
	
	


};
	  
                                 
		
	return 0;
}

ISR(INT0_vect)
{
	lcd_clrscr();
	lcd_puts("FOGO PORTA ABERTA");
	set_bit(PORTC,PC2);
	set_bit(PORTC,PC3);
	_delay_ms(1000);
	
	
}

