/**
 *
 * Simulated version of LPC214x environment
 *
 */

#ifndef __LPC214x_H
#define __LPC214x_H

#include <stdio.h>

// The interface to the environment is reading/writing values from addresses.
// To avoid annoying colleagues, we'll just leave their code untouched and
// hijack the read/writes by some dumb C++ tricks.

// A class to represent a single hijacked address
class BasicSlot {
public:
  BasicSlot() {}
  virtual ~BasicSlot() {}
  virtual unsigned long get_int() = 0;
  virtual void set_int(unsigned long x) = 0;
  virtual void set_raw(unsigned long x) {
    set_int(x);
  }
private:
  // make sure we never accidentally make copies, by making
  // copy constructor and assignment private
  BasicSlot(const BasicSlot& alt) {
  }
  const BasicSlot& operator=(const BasicSlot& alt) {
    return *this;
  }
};

// Give a hijacked address a name so we can log read/writes meaningfully
class SniffSlot : public BasicSlot {
public:
  char name[64];
  BasicSlot *fwd;
  unsigned long raw;

  SniffSlot() {
    name[0] = '\0';
    fwd = 0/*NULL*/;
    raw = 0;
  }

  operator unsigned long() { return get_int(); }

  const BasicSlot& operator = (unsigned long x) {
    set_int(x);
    return *this;
  }

  const BasicSlot& operator |= (unsigned long x) {
    *this = (*this) | x;
  }

  virtual unsigned long get_int();
  virtual void set_int(unsigned long x);
  virtual void set_raw(unsigned long x);
};

// Access a hijacked memory address
extern SniffSlot& define_address0(const char *name,int base,int offset,const char *fname,int line);
#define define_address(name,base,offset) define_address0(name,base,offset,__FILE__,__LINE__)

/* Vectored Interrupt Controller (VIC) */
#define VIC_BASE_ADDR (0)	/* 0xFFFFF000 */

#define VICIRQStatus define_address("VICIRQStatus",VIC_BASE_ADDR,0x000)
#define VICFIQStatus define_address("VICFIQStatus",VIC_BASE_ADDR,0x004)
#define VICRawIntr define_address("VICRawIntr",VIC_BASE_ADDR,0x008)
#define VICIntSelect define_address("VICIntSelect",VIC_BASE_ADDR,0x00C)
#define VICIntEnable define_address("VICIntEnable",VIC_BASE_ADDR,0x010)
#define VICIntEnClr define_address("VICIntEnClr",VIC_BASE_ADDR,0x014)
#define VICSoftInt define_address("VICSoftInt",VIC_BASE_ADDR,0x018)
#define VICSoftIntClr define_address("VICSoftIntClr",VIC_BASE_ADDR,0x01C)
#define VICProtection define_address("VICProtection",VIC_BASE_ADDR,0x020)
#define VICVectAddr define_address("VICVectAddr",VIC_BASE_ADDR,0x030)
#define VICDefVectAddr define_address("VICDefVectAddr",VIC_BASE_ADDR,0x034)
#define VICVectAddr0 define_address("VICVectAddr0",VIC_BASE_ADDR,0x100)
#define VICVectAddr1 define_address("VICVectAddr1",VIC_BASE_ADDR,0x104)
#define VICVectAddr2 define_address("VICVectAddr2",VIC_BASE_ADDR,0x108)
#define VICVectAddr3 define_address("VICVectAddr3",VIC_BASE_ADDR,0x10C)
#define VICVectAddr4 define_address("VICVectAddr4",VIC_BASE_ADDR,0x110)
#define VICVectAddr5 define_address("VICVectAddr5",VIC_BASE_ADDR,0x114)
#define VICVectAddr6 define_address("VICVectAddr6",VIC_BASE_ADDR,0x118)
#define VICVectAddr7 define_address("VICVectAddr7",VIC_BASE_ADDR,0x11C)
#define VICVectAddr8 define_address("VICVectAddr8",VIC_BASE_ADDR,0x120)
#define VICVectAddr9 define_address("VICVectAddr9",VIC_BASE_ADDR,0x124)
#define VICVectAddr10 define_address("VICVectAddr10",VIC_BASE_ADDR,0x128)
#define VICVectAddr11 define_address("VICVectAddr11",VIC_BASE_ADDR,0x12C)
#define VICVectAddr12 define_address("VICVectAddr12",VIC_BASE_ADDR,0x130)
#define VICVectAddr13 define_address("VICVectAddr13",VIC_BASE_ADDR,0x134)
#define VICVectAddr14 define_address("VICVectAddr14",VIC_BASE_ADDR,0x138)
#define VICVectAddr15 define_address("VICVectAddr15",VIC_BASE_ADDR,0x13C)
#define VICVectCntl0 define_address("VICVectCntl0",VIC_BASE_ADDR,0x200)
#define VICVectCntl1 define_address("VICVectCntl1",VIC_BASE_ADDR,0x204)
#define VICVectCntl2 define_address("VICVectCntl2",VIC_BASE_ADDR,0x208)
#define VICVectCntl3 define_address("VICVectCntl3",VIC_BASE_ADDR,0x20C)
#define VICVectCntl4 define_address("VICVectCntl4",VIC_BASE_ADDR,0x210)
#define VICVectCntl5 define_address("VICVectCntl5",VIC_BASE_ADDR,0x214)
#define VICVectCntl6 define_address("VICVectCntl6",VIC_BASE_ADDR,0x218)
#define VICVectCntl7 define_address("VICVectCntl7",VIC_BASE_ADDR,0x21C)
#define VICVectCntl8 define_address("VICVectCntl8",VIC_BASE_ADDR,0x220)
#define VICVectCntl9 define_address("VICVectCntl9",VIC_BASE_ADDR,0x224)
#define VICVectCntl10 define_address("VICVectCntl10",VIC_BASE_ADDR,0x228)
#define VICVectCntl11 define_address("VICVectCntl11",VIC_BASE_ADDR,0x22C)
#define VICVectCntl12 define_address("VICVectCntl12",VIC_BASE_ADDR,0x230)
#define VICVectCntl13 define_address("VICVectCntl13",VIC_BASE_ADDR,0x234)
#define VICVectCntl14 define_address("VICVectCntl14",VIC_BASE_ADDR,0x238)
#define VICVectCntl15 define_address("VICVectCntl15",VIC_BASE_ADDR,0x23C)

/* Pin Connect Block */
#define PINSEL_BASE_ADDR (0x300)	/* 0xE002C000 */
#define PINSEL0 define_address("PINSEL0",PINSEL_BASE_ADDR,0x00)
#define PINSEL1 define_address("PINSEL1",PINSEL_BASE_ADDR,0x04)
#define PINSEL2 define_address("PINSEL2",PINSEL_BASE_ADDR,0x14)

/* General Purpose Input/Output (GPIO) */
#define GPIO_BASE_ADDR	(0x400)	/* 0xE0028000 */
#define IOPIN0 define_address("IOPIN0",GPIO_BASE_ADDR,0x00)
#define IOSET0 define_address("IOSET0",GPIO_BASE_ADDR,0x04)
#define IODIR0 define_address("IODIR0",GPIO_BASE_ADDR,0x08)
#define IOCLR0 define_address("IOCLR0",GPIO_BASE_ADDR,0x0C)
#define IOPIN1 define_address("IOPIN1",GPIO_BASE_ADDR,0x10)
#define IOSET1 define_address("IOSET1",GPIO_BASE_ADDR,0x14)
#define IODIR1 define_address("IODIR1",GPIO_BASE_ADDR,0x18)
#define IOCLR1 define_address("IOCLR1",GPIO_BASE_ADDR,0x1C)

/* Fast I/O setup */
#define FIO_BASE_ADDR	(0x500)	/* 0x3FFFC000 */
#define FIO0DIR define_address("FIO0DIR",FIO_BASE_ADDR,0x00)
#define FIO0MASK define_address("FIO0MASK",FIO_BASE_ADDR,0x10)
#define FIO0PIN define_address("FIO0PIN",FIO_BASE_ADDR,0x14)
//#define FIO0SET        (get_state().dout_onoff[0][1])
//#define FIO0CLR        (get_state().dout_onoff[0][0])
#define FIO0SET define_address("FIO0SET",FIO_BASE_ADDR,0x18)
#define FIO0CLR define_address("FIO0CLR",FIO_BASE_ADDR,0x1C)
#define FIO1DIR define_address("FIO1DIR",FIO_BASE_ADDR,0x20)
#define FIO1MASK define_address("FIO1MASK",FIO_BASE_ADDR,0x30)
#define FIO1PIN define_address("FIO1PIN",FIO_BASE_ADDR,0x34)
#define FIO1SET define_address("FIO1SET",FIO_BASE_ADDR,0x38)
#define FIO1CLR define_address("FIO1CLR",FIO_BASE_ADDR,0x3C)

/* System Control Block(SCB) modules include Memory Accelerator Module,
Phase Locked Loop, VPB divider, Power Control, External Interrupt, 
Reset, and Code Security/Debugging */

#define SCB_BASE_ADDR	(0x600) /* 0xE01FC000 */

/* Memory Accelerator Module (MAM) */
#define MAMCR define_address("MAMCR",SCB_BASE_ADDR,0x000)
#define MAMTIM define_address("MAMTIM",SCB_BASE_ADDR,0x004)
#define MEMMAP define_address("MEMMAP",SCB_BASE_ADDR,0x040)

/* Phase Locked Loop (PLL) */
#define PLLCON define_address("PLLCON",SCB_BASE_ADDR,0x080)
#define PLLCFG define_address("PLLCFG",SCB_BASE_ADDR,0x084)
#define PLLSTAT define_address("PLLSTAT",SCB_BASE_ADDR,0x088)
#define PLLFEED define_address("PLLFEED",SCB_BASE_ADDR,0x08C)

/* PLL48 Registers */
#define PLL48CON define_address("PLL48CON",SCB_BASE_ADDR,0x0A0)
#define PLL48CFG define_address("PLL48CFG",SCB_BASE_ADDR,0x0A4)
#define PLL48STAT define_address("PLL48STAT",SCB_BASE_ADDR,0x0A8)
#define PLL48FEED define_address("PLL48FEED",SCB_BASE_ADDR,0x0AC)

/* Power Control */
#define PCON define_address("PCON",SCB_BASE_ADDR,0x0C0)
#define PCONP define_address("PCONP",SCB_BASE_ADDR,0x0C4)

/* VPB Divider */
#define VPBDIV define_address("VPBDIV",SCB_BASE_ADDR,0x100)

/* External Interrupts */
#define EXTINT define_address("EXTINT",SCB_BASE_ADDR,0x140)
#define INTWAKE define_address("INTWAKE",SCB_BASE_ADDR,0x144)
#define EXTMODE define_address("EXTMODE",SCB_BASE_ADDR,0x148)
#define EXTPOLAR define_address("EXTPOLAR",SCB_BASE_ADDR,0x14C)

/* Reset */
#define RSIR define_address("RSIR",SCB_BASE_ADDR,0x180)

/* System Controls and Status */
#define SCS define_address("SCS",SCB_BASE_ADDR,0x1A0)

/* Timer 0 */
#define TMR0_BASE_ADDR	(0x800)	/* 0xE0004000 */
//#define T0IR           (get_state().tick)
#define T0IR define_address("T0IR",TMR0_BASE_ADDR,0x00)
#define T0TCR define_address("T0TCR",TMR0_BASE_ADDR,0x04)
#define T0TC define_address("T0TC",TMR0_BASE_ADDR,0x08)
#define T0PR define_address("T0PR",TMR0_BASE_ADDR,0x0C)
#define T0PC define_address("T0PC",TMR0_BASE_ADDR,0x10)
#define T0MCR define_address("T0MCR",TMR0_BASE_ADDR,0x14)
#define T0MR0 define_address("T0MR0",TMR0_BASE_ADDR,0x18)
#define T0MR1 define_address("T0MR1",TMR0_BASE_ADDR,0x1C)
#define T0MR2 define_address("T0MR2",TMR0_BASE_ADDR,0x20)
#define T0MR3 define_address("T0MR3",TMR0_BASE_ADDR,0x24)
#define T0CCR define_address("T0CCR",TMR0_BASE_ADDR,0x28)
#define T0CR0 define_address("T0CR0",TMR0_BASE_ADDR,0x2C)
#define T0CR1 define_address("T0CR1",TMR0_BASE_ADDR,0x30)
#define T0CR2 define_address("T0CR2",TMR0_BASE_ADDR,0x34)
#define T0CR3 define_address("T0CR3",TMR0_BASE_ADDR,0x38)
#define T0EMR define_address("T0EMR",TMR0_BASE_ADDR,0x3C)
#define T0CTCR define_address("T0CTCR",TMR0_BASE_ADDR,0x70)

/* Timer 1 */
#define TMR1_BASE_ADDR (0x900)		/* 0xE0008000 */
#define T1IR define_address("T1IR",TMR1_BASE_ADDR,0x00)
#define T1TCR define_address("T1TCR",TMR1_BASE_ADDR,0x04)
#define T1TC define_address("T1TC",TMR1_BASE_ADDR,0x08)
#define T1PR define_address("T1PR",TMR1_BASE_ADDR,0x0C)
#define T1PC define_address("T1PC",TMR1_BASE_ADDR,0x10)
#define T1MCR define_address("T1MCR",TMR1_BASE_ADDR,0x14)
#define T1MR0 define_address("T1MR0",TMR1_BASE_ADDR,0x18)
#define T1MR1 define_address("T1MR1",TMR1_BASE_ADDR,0x1C)
#define T1MR2 define_address("T1MR2",TMR1_BASE_ADDR,0x20)
#define T1MR3 define_address("T1MR3",TMR1_BASE_ADDR,0x24)
#define T1CCR define_address("T1CCR",TMR1_BASE_ADDR,0x28)
#define T1CR0 define_address("T1CR0",TMR1_BASE_ADDR,0x2C)
#define T1CR1 define_address("T1CR1",TMR1_BASE_ADDR,0x30)
#define T1CR2 define_address("T1CR2",TMR1_BASE_ADDR,0x34)
#define T1CR3 define_address("T1CR3",TMR1_BASE_ADDR,0x38)
#define T1EMR define_address("T1EMR",TMR1_BASE_ADDR,0x3C)
#define T1CTCR define_address("T1CTCR",TMR1_BASE_ADDR,0x70)

/* Pulse Width Modulator (PWM) */
#define PWM_BASE_ADDR  (0x1000) /* 0xE0014000 */
#define PWMIR define_address("PWMIR",PWM_BASE_ADDR,0x00)
#define PWMTCR define_address("PWMTCR",PWM_BASE_ADDR,0x04)
#define PWMTC define_address("PWMTC",PWM_BASE_ADDR,0x08)
#define PWMPR define_address("PWMPR",PWM_BASE_ADDR,0x0C)
#define PWMPC define_address("PWMPC",PWM_BASE_ADDR,0x10)
#define PWMMCR define_address("PWMMCR",PWM_BASE_ADDR,0x14)
#define PWMMR0 define_address("PWMMR0",PWM_BASE_ADDR,0x18)
#define PWMMR1 define_address("PWMMR1",PWM_BASE_ADDR,0x1C)
#define PWMMR2 define_address("PWMMR2",PWM_BASE_ADDR,0x20)
#define PWMMR3 define_address("PWMMR3",PWM_BASE_ADDR,0x24)
#define PWMMR4 define_address("PWMMR4",PWM_BASE_ADDR,0x40)
#define PWMMR5 define_address("PWMMR5",PWM_BASE_ADDR,0x44)
#define PWMMR6 define_address("PWMMR6",PWM_BASE_ADDR,0x48)
#define PWMEMR define_address("PWMEMR",PWM_BASE_ADDR,0x3C)
#define PWMPCR define_address("PWMPCR",PWM_BASE_ADDR,0x4C)
#define PWMLER define_address("PWMLER",PWM_BASE_ADDR,0x50)

/* Universal Asynchronous Receiver Transmitter 0 (UART0) */
#define UART0_BASE_ADDR	(0x1100)	/* 0xE000C000 */
#define U0RBR define_address("U0RBR",UART0_BASE_ADDR,0x00)
#define U0THR define_address("U0THR",UART0_BASE_ADDR,0x00)
#define U0DLL define_address("U0DLL",UART0_BASE_ADDR,0x00)
#define U0DLM define_address("U0DLM",UART0_BASE_ADDR,0x04)
#define U0IER define_address("U0IER",UART0_BASE_ADDR,0x04)
#define U0IIR define_address("U0IIR",UART0_BASE_ADDR,0x08)
#define U0FCR define_address("U0FCR",UART0_BASE_ADDR,0x08)
#define U0LCR define_address("U0LCR",UART0_BASE_ADDR,0x0C)
#define U0MCR define_address("U0MCR",UART0_BASE_ADDR,0x10)
#define U0LSR define_address("U0LSR",UART0_BASE_ADDR,0x14)
#define U0MSR define_address("U0MSR",UART0_BASE_ADDR,0x18)
#define U0SCR define_address("U0SCR",UART0_BASE_ADDR,0x1C)
#define U0ACR define_address("U0ACR",UART0_BASE_ADDR,0x20)
#define U0FDR define_address("U0FDR",UART0_BASE_ADDR,0x28)
#define U0TER define_address("U0TER",UART0_BASE_ADDR,0x30)

/* Universal Asynchronous Receiver Transmitter 1 (UART1) */
#define UART1_BASE_ADDR (0x1200)		/*0xE0010000*/
#define U1RBR define_address("U1RBR",UART1_BASE_ADDR,0x00)
#define U1THR define_address("U1THR",UART1_BASE_ADDR,0x00)
#define U1DLL define_address("U1DLL",UART1_BASE_ADDR,0x00)
#define U1DLM define_address("U1DLM",UART1_BASE_ADDR,0x04)
#define U1IER define_address("U1IER",UART1_BASE_ADDR,0x04)
#define U1IIR define_address("U1IIR",UART1_BASE_ADDR,0x08)
#define U1FCR define_address("U1FCR",UART1_BASE_ADDR,0x08)
#define U1LCR define_address("U1LCR",UART1_BASE_ADDR,0x0C)
#define U1MCR define_address("U1MCR",UART1_BASE_ADDR,0x10)
#define U1LSR define_address("U1LSR",UART1_BASE_ADDR,0x14)
#define U1MSR define_address("U1MSR",UART1_BASE_ADDR,0x18)
#define U1SCR define_address("U1SCR",UART1_BASE_ADDR,0x1C)
#define U1ACR define_address("U1ACR",UART1_BASE_ADDR,0x20)
#define U1FDR define_address("U1FDR",UART1_BASE_ADDR,0x28)
#define U1TER define_address("U1TER",UART1_BASE_ADDR,0x30)

/* I2C Interface 0 */
#define I2C0_BASE_ADDR		(0x1200) /*0xE001C000*/
#define I20CONSET define_address("I20CONSET",I2C0_BASE_ADDR,0x00)
#define I20STAT define_address("I20STAT",I2C0_BASE_ADDR,0x04)
#define I20DAT define_address("I20DAT",I2C0_BASE_ADDR,0x08)
#define I20ADR define_address("I20ADR",I2C0_BASE_ADDR,0x0C)
#define I20SCLH define_address("I20SCLH",I2C0_BASE_ADDR,0x10)
#define I20SCLL define_address("I20SCLL",I2C0_BASE_ADDR,0x14)
#define I20CONCLR define_address("I20CONCLR",I2C0_BASE_ADDR,0x18)

/* I2C Interface 1 */
#define I2C1_BASE_ADDR		(0x1300) /*0xE005C000*/
#define I21CONSET define_address("I21CONSET",I2C1_BASE_ADDR,0x00)
#define I21STAT define_address("I21STAT",I2C1_BASE_ADDR,0x04)
#define I21DAT define_address("I21DAT",I2C1_BASE_ADDR,0x08)
#define I21ADR define_address("I21ADR",I2C1_BASE_ADDR,0x0C)
#define I21SCLH define_address("I21SCLH",I2C1_BASE_ADDR,0x10)
#define I21SCLL define_address("I21SCLL",I2C1_BASE_ADDR,0x14)
#define I21CONCLR define_address("I21CONCLR",I2C1_BASE_ADDR,0x18)

/* SPI0 (Serial Peripheral Interface 0) */
#define SPI0_BASE_ADDR		(0x1400) /*0xE0020000*/
#define S0SPCR define_address("S0SPCR",SPI0_BASE_ADDR,0x00)
#define S0SPSR define_address("S0SPSR",SPI0_BASE_ADDR,0x04)
#define S0SPDR define_address("S0SPDR",SPI0_BASE_ADDR,0x08)
#define S0SPCCR define_address("S0SPCCR",SPI0_BASE_ADDR,0x0C)
#define S0SPINT define_address("S0SPINT",SPI0_BASE_ADDR,0x1C)

/* SSP Controller */
#define SSP_BASE_ADDR		(0x1500) /*0xE0068000*/
#define SSPCR0 define_address("SSPCR0",SSP_BASE_ADDR,0x00)
#define SSPCR1 define_address("SSPCR1",SSP_BASE_ADDR,0x04)
#define SSPDR define_address("SSPDR",SSP_BASE_ADDR,0x08)
#define SSPSR define_address("SSPSR",SSP_BASE_ADDR,0x0C)
#define SSPCPSR define_address("SSPCPSR",SSP_BASE_ADDR,0x10)
#define SSPIMSC define_address("SSPIMSC",SSP_BASE_ADDR,0x14)
#define SSPRIS define_address("SSPRIS",SSP_BASE_ADDR,0x18)
#define SSPMIS define_address("SSPMIS",SSP_BASE_ADDR,0x1C)
#define SSPICR define_address("SSPICR",SSP_BASE_ADDR,0x20)

/* Real Time Clock */
#define RTC_BASE_ADDR		(0x1600) /*0xE0024000*/
#define ILR define_address("ILR",RTC_BASE_ADDR,0x00)
#define CTC define_address("CTC",RTC_BASE_ADDR,0x04)
#define CCR define_address("CCR",RTC_BASE_ADDR,0x08)
#define CIIR define_address("CIIR",RTC_BASE_ADDR,0x0C)
#define AMR define_address("AMR",RTC_BASE_ADDR,0x10)
#define CTIME0 define_address("CTIME0",RTC_BASE_ADDR,0x14)
#define CTIME1 define_address("CTIME1",RTC_BASE_ADDR,0x18)
#define CTIME2 define_address("CTIME2",RTC_BASE_ADDR,0x1C)
#define SEC define_address("SEC",RTC_BASE_ADDR,0x20)
#define MIN define_address("MIN",RTC_BASE_ADDR,0x24)
#define HOUR define_address("HOUR",RTC_BASE_ADDR,0x28)
#define DOM define_address("DOM",RTC_BASE_ADDR,0x2C)
#define DOW define_address("DOW",RTC_BASE_ADDR,0x30)
#define DOY define_address("DOY",RTC_BASE_ADDR,0x34)
#define MONTH define_address("MONTH",RTC_BASE_ADDR,0x38)
#define YEAR define_address("YEAR",RTC_BASE_ADDR,0x3C)
#define ALSEC define_address("ALSEC",RTC_BASE_ADDR,0x60)
#define ALMIN define_address("ALMIN",RTC_BASE_ADDR,0x64)
#define ALHOUR define_address("ALHOUR",RTC_BASE_ADDR,0x68)
#define ALDOM define_address("ALDOM",RTC_BASE_ADDR,0x6C)
#define ALDOW define_address("ALDOW",RTC_BASE_ADDR,0x70)
#define ALDOY define_address("ALDOY",RTC_BASE_ADDR,0x74)
#define ALMON define_address("ALMON",RTC_BASE_ADDR,0x78)
#define ALYEAR define_address("ALYEAR",RTC_BASE_ADDR,0x7C)
#define PREINT define_address("PREINT",RTC_BASE_ADDR,0x80)
#define PREFRAC define_address("PREFRAC",RTC_BASE_ADDR,0x84)

/* A/D Converter 0 (AD0) */
#define AD0_BASE_ADDR		(0x1700) /*0xE0034000*/
#define AD0CR define_address("AD0CR",AD0_BASE_ADDR,0x00)
#define AD0GDR define_address("AD0GDR",AD0_BASE_ADDR,0x04)
#define AD0STAT define_address("AD0STAT",AD0_BASE_ADDR,0x30)
#define AD0INTEN define_address("AD0INTEN",AD0_BASE_ADDR,0x0C)
#define AD0DR0 define_address("AD0DR0",AD0_BASE_ADDR,0x10)
#define AD0DR1 define_address("AD0DR1",AD0_BASE_ADDR,0x14)
#define AD0DR2 define_address("AD0DR2",AD0_BASE_ADDR,0x18)
#define AD0DR3 define_address("AD0DR3",AD0_BASE_ADDR,0x1C)
#define AD0DR4 define_address("AD0DR4",AD0_BASE_ADDR,0x20)
#define AD0DR5 define_address("AD0DR5",AD0_BASE_ADDR,0x24)
#define AD0DR6 define_address("AD0DR6",AD0_BASE_ADDR,0x28)
#define AD0DR7 define_address("AD0DR7",AD0_BASE_ADDR,0x2C)

#define ADGSR define_address("ADGSR",AD0_BASE_ADDR,0x08)
/* A/D Converter 1 (AD1) */
#define AD1_BASE_ADDR		(0x1800) /*0xE0060000*/
#define AD1CR define_address("AD1CR",AD1_BASE_ADDR,0x00)
#define AD1GDR define_address("AD1GDR",AD1_BASE_ADDR,0x04)
#define AD1STAT define_address("AD1STAT",AD1_BASE_ADDR,0x30)
#define AD1INTEN define_address("AD1INTEN",AD1_BASE_ADDR,0x0C)
#define AD1DR0 define_address("AD1DR0",AD1_BASE_ADDR,0x10)
#define AD1DR1 define_address("AD1DR1",AD1_BASE_ADDR,0x14)
#define AD1DR2 define_address("AD1DR2",AD1_BASE_ADDR,0x18)
#define AD1DR3 define_address("AD1DR3",AD1_BASE_ADDR,0x1C)
#define AD1DR4 define_address("AD1DR4",AD1_BASE_ADDR,0x20)
#define AD1DR5 define_address("AD1DR5",AD1_BASE_ADDR,0x24)
#define AD1DR6 define_address("AD1DR6",AD1_BASE_ADDR,0x28)
#define AD1DR7 define_address("AD1DR7",AD1_BASE_ADDR,0x2C)

/* D/A Converter */
#define DAC_BASE_ADDR		(0x1900) /*0xE006C000*/
#define DACR define_address("DACR",DAC_BASE_ADDR,0x00)

/* Watchdog */
#define WDG_BASE_ADDR		(0x1A00) /*0xE0000000*/
#define WDMOD define_address("WDMOD",WDG_BASE_ADDR,0x00)
#define WDTC define_address("WDTC",WDG_BASE_ADDR,0x04)
#define WDFEED define_address("WDFEED",WDG_BASE_ADDR,0x08)
#define WDTV define_address("WDTV",WDG_BASE_ADDR,0x0C)

/* USB Controller */
#define USB_BASE_ADDR		(0x1B00) /*0xE0090000*/			/* USB Base Address */
/* Device Interrupt Registers */
#define DEV_INT_STAT define_address("DEV_INT_STAT",USB_BASE_ADDR,0x00)
#define DEV_INT_EN define_address("DEV_INT_EN",USB_BASE_ADDR,0x04)
#define DEV_INT_CLR define_address("DEV_INT_CLR",USB_BASE_ADDR,0x08)
#define DEV_INT_SET define_address("DEV_INT_SET",USB_BASE_ADDR,0x0C)
#define DEV_INT_PRIO define_address("DEV_INT_PRIO",USB_BASE_ADDR,0x2C)

/* Endpoint Interrupt Registers */
#define EP_INT_STAT define_address("EP_INT_STAT",USB_BASE_ADDR,0x30)
#define EP_INT_EN define_address("EP_INT_EN",USB_BASE_ADDR,0x34)
#define EP_INT_CLR define_address("EP_INT_CLR",USB_BASE_ADDR,0x38)
#define EP_INT_SET define_address("EP_INT_SET",USB_BASE_ADDR,0x3C)
#define EP_INT_PRIO define_address("EP_INT_PRIO",USB_BASE_ADDR,0x40)

/* Endpoint Realization Registers */
#define REALIZE_EP define_address("REALIZE_EP",USB_BASE_ADDR,0x44)
#define EP_INDEX define_address("EP_INDEX",USB_BASE_ADDR,0x48)
#define MAXPACKET_SIZE define_address("MAXPACKET_SIZE",USB_BASE_ADDR,0x4C)

/* Command Reagisters */
#define CMD_CODE define_address("CMD_CODE",USB_BASE_ADDR,0x10)
#define CMD_DATA define_address("CMD_DATA",USB_BASE_ADDR,0x14)

/* Data Transfer Registers */
#define RX_DATA define_address("RX_DATA",USB_BASE_ADDR,0x18)
#define TX_DATA define_address("TX_DATA",USB_BASE_ADDR,0x1C)
#define RX_PLENGTH define_address("RX_PLENGTH",USB_BASE_ADDR,0x20)
#define TX_PLENGTH define_address("TX_PLENGTH",USB_BASE_ADDR,0x24)
#define USB_CTRL define_address("USB_CTRL",USB_BASE_ADDR,0x28)

/* DMA Registers */
#define DMA_REQ_STAT define_address("DMA_REQ_STAT",USB_BASE_ADDR,0x50)
#define DMA_REQ_CLR define_address("DMA_REQ_CLR",USB_BASE_ADDR,0x54)
#define DMA_REQ_SET define_address("DMA_REQ_SET",USB_BASE_ADDR,0x58)
#define UDCA_HEAD define_address("UDCA_HEAD",USB_BASE_ADDR,0x80)
#define EP_DMA_STAT define_address("EP_DMA_STAT",USB_BASE_ADDR,0x84)
#define EP_DMA_EN define_address("EP_DMA_EN",USB_BASE_ADDR,0x88)
#define EP_DMA_DIS define_address("EP_DMA_DIS",USB_BASE_ADDR,0x8C)
#define DMA_INT_STAT define_address("DMA_INT_STAT",USB_BASE_ADDR,0x90)
#define DMA_INT_EN define_address("DMA_INT_EN",USB_BASE_ADDR,0x94)
#define EOT_INT_STAT define_address("EOT_INT_STAT",USB_BASE_ADDR,0xA0)
#define EOT_INT_CLR define_address("EOT_INT_CLR",USB_BASE_ADDR,0xA4)
#define EOT_INT_SET define_address("EOT_INT_SET",USB_BASE_ADDR,0xA8)
#define NDD_REQ_INT_STAT define_address("NDD_REQ_INT_STAT",USB_BASE_ADDR,0xAC)
#define NDD_REQ_INT_CLR define_address("NDD_REQ_INT_CLR",USB_BASE_ADDR,0xB0)
#define NDD_REQ_INT_SET define_address("NDD_REQ_INT_SET",USB_BASE_ADDR,0xB4)
#define SYS_ERR_INT_STAT define_address("SYS_ERR_INT_STAT",USB_BASE_ADDR,0xB8)
#define SYS_ERR_INT_CLR define_address("SYS_ERR_INT_CLR",USB_BASE_ADDR,0xBC)
#define SYS_ERR_INT_SET define_address("SYS_ERR_INT_SET",USB_BASE_ADDR,0xC0)
#define MODULE_ID define_address("MODULE_ID",USB_BASE_ADDR,0xFC)

#endif  // __LPC214x_H

