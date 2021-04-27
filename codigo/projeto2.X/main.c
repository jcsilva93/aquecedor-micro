/*
 * File:   main.c
 * Author: Jean Carlos da Silva       RA: 35212            Turma: T01
 *         Hugo Gomes Gandra          RA: 2016013845       Turma: T01  
 *
 * Created on 16 de Novembro de 2020, 13:31
 */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>

#pragma config FOSC=INTRC_NOCLKOUT
#pragma config WDTE=OFF
#pragma config PWRTE=OFF
#pragma config MCLRE=ON
#pragma config CP=OFF
#pragma config CPD=OFF
#pragma config BOREN=ON
#pragma config IESO=ON
#pragma config FCMEN=ON
#pragma config LVP=OFF
#pragma config BOR4V=BOR40V
#pragma config WRT=OFF

#define _XTAL_FREQ 4000000

//VARIAVÉIS
unsigned int seg = 0;
unsigned int min = 0;
unsigned int hora = 12;
unsigned int min_start = 0;
unsigned int hora_start = 12;
unsigned int min_stop = 0;
unsigned int hora_stop = 20;
unsigned int cont = 0;
unsigned int temp = 0;
unsigned int flag = 0;
unsigned int max_temp = 40;
unsigned int check_time = 0;
unsigned int aquecedor = 0;
unsigned int nivel = 0;
unsigned int ajustar = 0;

//FUNÇÕES LCD
void inicia_lcd();
void envia_lcd_dado(char valor);
void envia_lcd_comando(char RS);

void main(void){
    //SETUP
    TRISA = 0b00000111;
    TRISB = 0b00000011;
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    PORTAbits.RA3 = 0;
    PORTB = 0b00000000;
    ANSELH = 0;
    ANSEL = 0b00000001;

    ANSELbits.ANS0 = 1;
    ADCON0bits.ADON = 1;
    ADCON1bits.ADFM = 1;
    ADCON0bits.CHS0 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON0bits.ADCS0 = 1;
    ADCON0bits.ADCS1 = 1;
    
    OPTION_REGbits.INTEDG = 0;
    INTCONbits.INTE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;    
    PIE1bits.TMR1IE = 1;
    T1CONbits.TMR1ON = 1;
    T1CONbits.TMR1CS = 0;
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1CKPS1 = 1;
    TMR1 = 3036;
    
    inicia_lcd();
    envia_lcd_comando(0x80);

    while (1) {
        ADCON0bits.GO = 1;
        switch (flag) {
            case 0: //TELA INICIAL                  
                if (nivel == 0){
                    envia_lcd_comando(0x80);
                    printf("T:%d%cC  N:BAIXO ", temp, 0xDF);
                }else if (nivel == 1){
                    envia_lcd_comando(0x80);
                    printf("T:%d%cC  N:ALTO  ", temp, 0xDF);
                }
                if(check_time == 1){
                    envia_lcd_comando(0xc0);
                    printf("A:LIGADO            ");
                }else if (check_time == 0){
                    envia_lcd_comando(0xc0);
                    printf("A:DESLIGADO           ");
                }
                if (RB1 == 1){
                    flag++;
                    __delay_ms(250);
                }
                break;
            case 1: //TEMPERATURA DESEJADA                
                envia_lcd_comando(0x80);
                printf("   TEMPERATURA   ");
                envia_lcd_comando(0xc0);
                printf("      %d%cC       ",max_temp, 0xDF);
                if (RB2 == 1 && ajustar == 0){
                    __delay_ms(500);
                    ajustar = 1;
                }else if (RB2 == 1 && ajustar == 1){
                    ajustar = 0;
                    __delay_ms(500);
                }
                if (RB1 == 1 && ajustar == 0){
                    flag ++;
                    __delay_ms(250);
                }else if (RB1 == 1 && ajustar == 1){
                    __delay_ms(250);
                    max_temp = max_temp +5;
                    if(max_temp == 105){
                        max_temp = 10;
                    }                    
                }            
                break;
            case 2: //RELÓGIO                
                envia_lcd_comando(0x80);
                printf("     RELOGIO     ");
                envia_lcd_comando(0xc0);
                printf("    %02d:%02d:%02d    ", hora, min, seg);
                if (RB2 == 1 && ajustar == 0){
                    __delay_ms(500);
                    ajustar = 1;
                }else if (RB2 == 1 && ajustar == 1){
                    ajustar = 2;
                    __delay_ms(500);
                }else if (RB2 == 1 && ajustar == 2){
                    ajustar = 0;
                    __delay_ms(500);
                }
                if (RB1 == 1 && ajustar == 0){
                    flag++;
                    __delay_ms(250);
                }else if(RB1 == 1 && ajustar == 1){
                    min++;
                    if (min >= 60){
                        min = 0;
                    }
                    __delay_ms(250);
                }else if(RB1 == 1 && ajustar == 2){
                    hora++;
                    if (hora >= 24){
                        hora = 0;
                    }
                    __delay_ms(250);
                }             
                break;
            case 3: //START TIME                
                envia_lcd_comando(0x80);
                printf(" HORA DE INICIO ");
                envia_lcd_comando(0xc0);
                printf("     %02d:%02d    ", hora_start, min_start);
                if (RB2 == 1 && ajustar == 0){
                    __delay_ms(500);
                    ajustar = 1;
                }else if (RB2 == 1 && ajustar == 1){
                    ajustar = 2;
                    __delay_ms(500);
                }else if (RB2 == 1 && ajustar == 2){
                    ajustar = 0;
                    __delay_ms(500);
                }
                if (RB1 == 1 && ajustar == 0){
                    flag++;
                    __delay_ms(250);
                }else if(RB1 == 1 && ajustar == 1){
                    min_start++;
                    if(min_start >= 60){
                        min_start = 0;
                    }
                    __delay_ms(250);
                }else if(RB1 == 1 && ajustar == 2){
                    hora_start++;
                    if (hora_start >=24){
                        hora_start = 0;
                    }
                    __delay_ms(250);
                }                  
                break;
            case 4: //STOP TIME                
                envia_lcd_comando(0x80);
                printf(" HORA DE PARADA ");
                envia_lcd_comando(0xc0);
                printf("     %02d:%02d    ", hora_stop, min_stop);
                if (RB2 == 1 && ajustar == 0){
                    __delay_ms(500);
                    ajustar = 1;
                }else if (RB2 == 1 && ajustar == 1){
                    ajustar = 2;
                    __delay_ms(500);
                }else if (RB2 == 1 && ajustar == 2){
                    ajustar = 0;
                    __delay_ms(500);
                }
                if (RB1 == 1 && ajustar == 0){
                    flag = 0;
                    __delay_ms(250);
                }else if(RB1 == 1 && ajustar == 1){
                    min_stop++;
                    if(min_stop >= 60){
                        min_stop = 0;
                    }
                    __delay_ms(250);
                }else if(RB1 == 1 && ajustar == 2){
                    hora_stop++;
                    if (hora_stop >=24){
                        hora_stop = 0;
                    }
                    __delay_ms(250);
                }              
                break;
        }
    }
    return;
}

void __interrupt() Interrupt(void) {
    
    if (PIR1bits.TMR1IF) {
        //INCREMENTA RELÓGIO
        if (cont > 2) {
            seg++;
            cont = 0;
            if (seg == 60) {
                seg = 0;
                min++;
            }
            if (min == 60) {
                min = 0;
                hora++;
            }
            if (hora == 24) {
                hora = 0;
            }
        }
        cont++;
        
        //CONVERTE SENSOR
        unsigned int voltage = (unsigned int) (ADRESH << 8) | (ADRESL);
        temp = voltage * 125 * 1/256;
        
        //CHECA TEMPERATURA +/- 5º E HORA DE INICIAR
        if (min == min_start && hora == hora_start){
                check_time = 1;
            }else if (min == min_stop && hora == hora_stop){
                check_time = 0;
            }
        if (check_time == 1){
            if(temp >= max_temp +5){
                PORTAbits.RA3 = 0;
                aquecedor = 0;
            }else if(temp <= max_temp -5){
                PORTAbits.RA3 = 1;
                aquecedor = 1;
            }
        }else{
            PORTAbits.RA3 = 0;
            check_time = 0;
        }
        
        //CHECA NIVEL
        if (PORTAbits.RA1 == 1){
            nivel = 1;
        }else if (PORTAbits.RA1 == 0){
            nivel = 0;
            check_time = 0;
            PORTAbits.RA3 = 0;
        }      
        PIR1bits.TMR1IF = 0;
        TMR1 = 3036;
    }
}
void inicia_lcd(){
    PORTCbits.RC0 = 0;
    PORTCbits.RC1 = 0;
    
    __delay_ms(15);
    envia_lcd_comando(0x30);
    __delay_ms(5);
    envia_lcd_comando(0x30);
    __delay_us(100);
    envia_lcd_comando(0x30);
    __delay_us(40);
    
    envia_lcd_comando(0x38);
    __delay_us(40);
    envia_lcd_comando(0x01);
    __delay_ms(2);
    envia_lcd_comando(0x0C);
    __delay_us(40);
    envia_lcd_comando(0x06);
    __delay_us(40);
}
void envia_lcd_comando(char RS){
    PORTCbits.RC0 = 0;
    __delay_us(100);
    PORTD = RS;
    PORTCbits.RC1 = 1;
    PORTCbits.RC1 = 0;
}
void envia_lcd_dado(char valor){
    PORTCbits.RC0 = 1;
    __delay_us(100);
    PORTD = valor;
    PORTCbits.RC1 = 1;
    PORTCbits.RC1 = 0;
}
void putch(char p){
    envia_lcd_dado(p);
}
