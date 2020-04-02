/**
* \file main.c
*
* \brief Main source file.
*
    (c) 2020 Microchip Technology Inc. and its subsidiaries.
	
    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.
    
	THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.
    
	IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

/* PIC18F47Q10 Configuration Bit Settings */
#pragma config MCLRE = EXTMCLR    /* Master Clear Enable bit->MCLR pin (RE3) is MCLR */
#pragma config WDTE = OFF       /* WDT operating mode->WDT Disabled */
#pragma config LVP = ON       /* Low voltage programming enabled */

#include <xc.h>

/* Code Function Declarations */
static void CLK_init(void);
static void PORT_init(void);
static void TMR2_init(void);
static void TMR4_init(void);
static void PWM3_init(void);
static void PWM4_init(void);
static void CLC1_init(void);
static void CLC2_init(void);
static void CLC3_init(void);
static void PPS_init(void);

static void CLK_init(void)
{
    /* HFINTOSC Oscillator */
    OSCCON1bits.NOSC = 6 ;
    /* HFFRQ 64_MHz; */
    OSCFRQ = 0x08;
}

static void PORT_init(void)
{
    /* PORT RA2 and RA3 output driver enabled */
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    /* PORT RB0  output driver enabled */
    TRISBbits.TRISB0 = 0; 
    /* PORT RC2 and RC3 output driver enabled */
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
}

static void TMR2_init(void)
{
    /* Set TMR2 to generate a pulse every 10us (Frequency = 100kHz) */ 
    /* Timer 2 clock source is FOSC/4 */
    T2CLKCONbits.CS = 1;
    /* Load period values */
    T2PR = 0x9F;
    /* Enable Timer2 */
    T2CON = 0x80;
}

static void TMR4_init(void)
{
    /*  Set TMR4 to generate a pulse every 5us (Frequency = 200kHz)*/
    /*  TIMER4 is also set to start at the same time as TIMER2 */
    /* Timer 4 clock source is FOSC/4 */
    T4CLKCONbits.CS = 1;
    /* Timer4 Resets at rising TMR4_ers*/
    T4HLTbits.MODE = 4; 
    /* Timer 4 reset source is TMR2_postscaled; */
    T4RSTbits.RSEL = 1;
    /* Load period values */
    T4PR = 0x4F;
    /* Enable Timer4 */
    T4CON = 0x80;
}

static void PWM3_init(void)
 {
    /* Set the PWM3 to 50% duty-cycle with TIMER2 as source */
    /* Enable PWM3*/
    PWM3CON = 0x80;
    /* Load duty-cycle values */
    PWM3DCH = 0x4F;
    PWM3DCL = 0xC0; 
    /* Select TIMER2 as pulse source */
    CCPTMRS = 0x10; 
 }

static void PWM4_init(void)
 {
    /*  Set the PWM4 to 50% duty-cycle with TIMER4 as source */
    /* Enable PWM4*/
    PWM4CON = 0x80;
    /* Load duty-cycle values */
    PWM4DCH = 0x27; 
    PWM4DCL = 0xC0;
    /*  Select TIMER4 as pulse source */
    CCPTMRSbits.P4TSEL = 2; 
 }
 
static void CLC1_init(void)
{
    /*  Set the CLC1 to implement AND-OR Logic with the focus on OR */
    /*  Clear the output polarity register */
    CLC1POL = 0x00;
    /* Configure PWM3_OUT as input for first OR Gate */
    CLC1SEL0 = 0x1A; 
    /* Configure PWM3_OUT as input for second OR Gate */
    CLC1SEL1 = 0x1A;
    /* Configure PWM4_OUT as input for third OR Gate */
    CLC1SEL2 = 0x1B;
    /* Configure PWM4_OUT as input for forth OR Gate */
    CLC1SEL3 = 0x1B; 
    /* All 4 inputs are not inverted*/
    CLC1GLS0 = 0x02; 
    CLC1GLS1 = 0x08;
    CLC1GLS2 = 0x20; 
    CLC1GLS3 = 0x80;
    /* CLC1 enabled; MODE AND-OR*/
    CLC1CON = 0x80 ; 
}
 
static void CLC2_init(void)
{
    /* Set the CLC2 to implement 4-input AND Logic with the focus on 2-input AND */
    /*  Clear the output polarity register */
    CLC2POL = 0x00;
    /* Configure PWM3_OUT as input for first OR Gate */
    CLC2SEL0 = 0x1A;
    /* Configure PWM3_OUT as input for second OR Gate */
    CLC2SEL1 = 0x1A;
    /* Configure PWM4_OUT as input for third OR Gate */
    CLC2SEL2 = 0x1B;
    /* Configure PWM4_OUT as input for forth OR Gate */
    CLC2SEL3 = 0x1B;
    /* All 4 inputs are not inverted*/
    CLC2GLS0 = 0x02; 
    CLC2GLS1 = 0x08;
    CLC2GLS2 = 0x20;
    CLC2GLS3 = 0x80;
    /* CLC2 enabled */
    CLC2CONbits.EN = 1; 
    /* MODE 4-input AND */
    CLC2CONbits.MODE = 2;
}
 
static void CLC3_init(void)
{
    /* Set the CLC3 to implement OR-XOR Logic with the focus on 2-input XOR */
    /*  Clear the output polarity register */
    CLC3POL = 0x00;
    /* Configure PWM3_OUT as input for first OR Gate */
    CLC3SEL0 = 0x1A; 
    /* Configure PWM3_OUT as input for second OR Gate */
    CLC3SEL1 = 0x1A; 
    /* Configure PWM4_OUT as input for third OR Gate */
    CLC3SEL2 = 0x1B;   
    /* Configure PWM4_OUT as input for forth OR Gate */
    CLC3SEL3 = 0x1B;  
    /* All 4 inputs are not inverted*/
    CLC3GLS0 = 0x02; 
    CLC3GLS1 = 0x08; 
    CLC3GLS2 = 0x20; 
    CLC3GLS3 = 0x80; 
    /* CLC3 enabled */
    CLC3CONbits.EN = 1; 
    /* MODE OR-XOR */
    CLC3CONbits.MODE = 1;
}

static void PPS_init(void)
{
    /*Configure RA2 for PWM3 output*/
    RA2PPS = 0x07;
    /*Configure RA3 for PWM4 output*/  
    RA3PPS = 0x08;
    /*Configure RB0 for CLC3 output*/
    RB0PPS = 0x1A;
    /*Configure RC2 for CLC1 output*/
    RC2PPS = 0x18;
    /*Configure RC3 for CLC2 output*/
    RC3PPS = 0x19;     
}
 
void main(void) {
    
    CLK_init();
    PORT_init();
    TMR2_init();
    TMR4_init();
    PWM3_init();
    PWM4_init();
    CLC1_init();
    CLC2_init();
    CLC3_init();
    PPS_init();
    
    while (1)
    {
        ;
    }
}
