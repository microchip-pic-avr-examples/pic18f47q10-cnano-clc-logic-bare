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

/* Code Macros */
/*  Define reload values PR2 & PR4 registers */
#define TIMER2PR 0x9F 
#define TIMER4PR 0x4F 
/*  Define reload values for PWM3DCH & PWM3DCL registers   */
#define PWM3H 0x4F 
#define PWM3L 0xC0 
/*  Define reload values for PWM4DCH & PWM4DCL registers   */
#define PWM4H 0x27 
#define PWM4L 0xC0 
/*  Define NULL values for the CLC use */
#define CLCNULL 0x00 
/*  Define register values for PPS pin-mapping   */
#define PPS_CONFIG_RA2_PWM3_OUT 0x07 
#define PPS_CONFIG_RA3_PWM4_OUT 0x08
#define PPS_CONFIG_RB0_CLC3_OUT 0x1A
#define PPS_CONFIG_RC2_CLC1_OUT 0x18
#define PPS_CONFIG_RC3_CLC2_OUT 0x19

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
    OSCCON1 = _OSCCON1_NOSC1_MASK
            | _OSCCON1_NOSC2_MASK;
    /* HFFRQ 64_MHz; */
    OSCFRQ = _OSCFRQ_FRQ3_MASK;
}

static void PORT_init(void)
{
    /* PORT RA2 and RA3 output driver enabled */
    TRISA &= ~(_TRISA_TRISA2_MASK | _TRISA_TRISA3_MASK);
    /* PORT RB0  output driver enabled */
    TRISB &= ~_TRISB_TRISB0_MASK; 
    /* PORT RC2 and RC3 output driver enabled */
    TRISC &= ~(_TRISC_TRISC2_MASK | _TRISC_TRISC3_MASK); 
}

static void TMR2_init(void)
{
    /* Set TMR2 to generate a pulse every 10us (Frequency = 100kHz) */ 
    /* Timer 2 clock source is FOSC/4 */
    T2CLKCON |= _T2CLKCON_CS0_MASK; 
    /* Load period values */
    T2PR = TIMER2PR;
    /* Enable Timer2 */
    T2CON = _T2CON_ON_MASK; 
}

static void TMR4_init(void)
{
    /*  Set TMR4 to generate a pulse every 5us (Frequency = 200kHz)*/
    /*  TIMER4 is also set to start at the same time as TIMER2 */
    /* Timer 4 clock source is FOSC/4 */
    T4CLKCON |= _T4CLKCON_CS0_MASK;
    /* Timer4 Resets at rising TMR4_ers*/
    T4HLT |= _T4HLT_MODE2_MASK; 
    /* Timer 4 reset source is TMR2_postscaled; */
    T4RST |= _T4RST_RSEL0_MASK;
    /* Load period values */
    T4PR = TIMER4PR;
    /* Enable Timer4 */
    T4CON = _T4CON_ON_MASK; 
}

static void PWM3_init(void)
 {
    /* Set the PWM3 to 50% duty-cycle with TIMER2 as source */
    /* Enable PWM3*/
    PWM3CON = _PWM3CON_PWM3EN_MASK;
    /* Load duty-cycle values */
    PWM3DCH = PWM3H;
    PWM3DCL = PWM3L; 
    /* Clear PWM3 pulse source */
    CCPTMRS &= ~(_CCPTMRS_P3TSEL0_MASK | _CCPTMRS_P3TSEL1_MASK);
    /* Select TIMER2 as pulse source */
    CCPTMRS |= _CCPTMRS_P3TSEL0_MASK; 
 }

static void PWM4_init(void)
 {
    /*  Set the PWM4 to 50% duty-cycle with TIMER4 as source */
    /* Enable PWM4*/
    PWM4CON = _PWM4CON_PWM4EN_MASK;
    /* Load duty-cycle values */
    PWM4DCH = PWM4H; 
    PWM4DCL = PWM4L;
    /* Clear PWM4 pulse source */
    CCPTMRS &= ~(_CCPTMRS_P4TSEL0_MASK | _CCPTMRS_P4TSEL1_MASK);
    /*  Select TIMER4 as pulse source */
    CCPTMRS |= _CCPTMRS_P4TSEL1_MASK; 
 }
 
static void CLC1_init(void)
{
    /*  Set the CLC1 to implement AND-OR Logic with the focus on OR */
    /*  Clear the output polarity register */
    CLC1POL = CLCNULL;
    /* Configure PWM3_OUT as input for first OR Gate */
    CLC1SEL0 = _CLC1SEL0_D1S1_MASK 
             | _CLC1SEL0_D1S3_MASK 
             | _CLC1SEL0_D1S4_MASK; 
    /* Configure PWM3_OUT as input for second OR Gate */
    CLC1SEL1 = _CLC1SEL1_D2S1_MASK 
             | _CLC1SEL1_D2S3_MASK 
             | _CLC1SEL1_D2S4_MASK;
    /* Configure PWM4_OUT as input for third OR Gate */
    CLC1SEL2 = _CLC1SEL2_D3S0_MASK 
             | _CLC1SEL2_D3S1_MASK 
             | _CLC1SEL2_D3S3_MASK 
             | _CLC1SEL2_D3S4_MASK;
    /* Configure PWM4_OUT as input for forth OR Gate */
    CLC1SEL3 = _CLC1SEL3_D4S0_MASK 
             | _CLC1SEL3_D4S1_MASK 
             | _CLC1SEL3_D4S3_MASK 
             | _CLC1SEL3_D4S4_MASK; 
    /* All 4 inputs are not inverted*/
    CLC1GLS0 = _CLC1GLS0_LC1G1D1T_MASK; 
    CLC1GLS1 = _CLC1GLS1_LC1G2D2T_MASK;
    CLC1GLS2 = _CLC1GLS2_LC1G3D3T_MASK; 
    CLC1GLS3 = _CLC1GLS3_LC1G4D4T_MASK;
    /* CLC1 enabled; MODE AND-OR*/
    CLC1CON = _CLC1CON_EN_MASK ; 
}
 
static void CLC2_init(void)
{
    /* Set the CLC2 to implement 4-input AND Logic with the focus on 2-input AND */
    /*  Clear the output polarity register */
    CLC2POL = CLCNULL;
    /* Configure PWM3_OUT as input for first OR Gate */
    CLC2SEL0 = _CLC2SEL0_D1S1_MASK 
             | _CLC2SEL0_D1S3_MASK 
             | _CLC2SEL0_D1S4_MASK;
    /* Configure PWM3_OUT as input for second OR Gate */
    CLC2SEL1 = _CLC2SEL1_D2S1_MASK 
             | _CLC2SEL1_D2S3_MASK
             | _CLC2SEL1_D2S4_MASK;
    /* Configure PWM4_OUT as input for third OR Gate */
    CLC2SEL2 = _CLC2SEL2_D3S0_MASK 
             | _CLC2SEL2_D3S1_MASK 
             | _CLC2SEL2_D3S3_MASK 
             | _CLC2SEL2_D3S4_MASK;
    /* Configure PWM4_OUT as input for forth OR Gate */
    CLC2SEL3 = _CLC2SEL3_D4S0_MASK 
             | _CLC2SEL3_D4S1_MASK 
             | _CLC2SEL3_D4S3_MASK 
             | _CLC2SEL3_D4S4_MASK;
    /* All 4 inputs are not inverted*/
    CLC2GLS0 = _CLC2GLS0_LC2G1D1T_MASK; 
    CLC2GLS1 = _CLC2GLS1_LC2G2D2T_MASK;
    CLC2GLS2 = _CLC2GLS2_LC2G3D3T_MASK;
    CLC2GLS3 = _CLC2GLS3_LC2G4D4T_MASK;
    /* CLC2 enabled; MODE 4-input AND*/
    CLC2CON = _CLC2CON_EN_MASK 
            |_CLC2CON_MODE1_MASK;
}
 
static void CLC3_init(void)
{
    /* Set the CLC3 to implement OR-XOR Logic with the focus on 2-input XOR */
    /*  Clear the output polarity register */
    CLC3POL = CLCNULL;
    /* Configure PWM3_OUT as input for first OR Gate */
    CLC3SEL0 = _CLC3SEL0_D1S1_MASK 
             | _CLC3SEL0_D1S3_MASK 
             | _CLC3SEL0_D1S4_MASK; 
    /* Configure PWM3_OUT as input for second OR Gate */
    CLC3SEL1 = _CLC3SEL1_D2S1_MASK 
             | _CLC3SEL1_D2S3_MASK 
             | _CLC3SEL1_D2S4_MASK; 
    /* Configure PWM4_OUT as input for third OR Gate */
    CLC3SEL2 = _CLC3SEL2_D3S0_MASK 
             | _CLC3SEL2_D3S1_MASK 
             | _CLC3SEL2_D3S3_MASK 
             | _CLC3SEL2_D3S4_MASK;   
    /* Configure PWM4_OUT as input for forth OR Gate */
    CLC3SEL3 = _CLC3SEL3_D4S0_MASK 
             | _CLC3SEL3_D4S1_MASK 
             | _CLC3SEL3_D4S3_MASK 
             | _CLC3SEL3_D4S4_MASK;  
    /* All 4 inputs are not inverted*/
    CLC3GLS0 = _CLC3GLS0_LC3G1D1T_MASK; 
    CLC3GLS1 = _CLC3GLS1_LC3G2D2T_MASK; 
    CLC3GLS2 = _CLC3GLS2_LC3G3D3T_MASK; 
    CLC3GLS3 = _CLC3GLS3_LC3G4D4T_MASK; 
    /* CLC3 enabled; MODE OR-XOR*/
    CLC3CON = _CLC3CON_EN_MASK
            |_CLC3CON_MODE0_MASK;
}

static void PPS_init(void)
{
    /*Configure RA2 for PWM3 output*/
    RA2PPS = PPS_CONFIG_RA2_PWM3_OUT;
    /*Configure RA3 for PWM4 output*/  
    RA3PPS = PPS_CONFIG_RA3_PWM4_OUT;
    /*Configure RB0 for CLC3 output*/
    RB0PPS = PPS_CONFIG_RB0_CLC3_OUT;
    /*Configure RC2 for CLC1 output*/
    RC2PPS = PPS_CONFIG_RC2_CLC1_OUT;
    /*Configure RC3 for CLC2 output*/
    RC3PPS = PPS_CONFIG_RC3_CLC2_OUT;     
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
