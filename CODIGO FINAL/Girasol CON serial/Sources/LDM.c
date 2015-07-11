/*
 * LDM.c
 *
 *  Created on: Jun 1, 2014
 *      Author: Juan Pablo
 */

#include "LDM.h"
      
#include <stdint.h>

#define CPU_BUS_CLK_HZ                  50000000U /* Initial value of the bus clock frequency in Hz */
#define CPU_CORE_CLK_HZ                 100000000U /* Initial value of the core/system clock frequency in Hz.  */
#define CPU_XTAL_CLK_HZ                 50000000U /* Value of the external crystal or oscillator clock frequency in Hz */
#define CPU_INT_SLOW_CLK_HZ             32768U /* Value of the slow internal oscillator clock frequency in Hz  */
#define CPU_INT_FAST_CLK_HZ             4000000U /* Value of the fast internal oscillator clock frequency in Hz  */

#define CPU_CLOCK_CONFIG_NUMBER         0x01U /* Specifies number of defined clock configurations. */

#define CPU_BUS_CLK_HZ_CLOCK_CONFIG0    50000000U /* Value of the bus clock frequency in the clock configuration 0 in Hz. */
#define CPU_CORE_CLK_HZ_CLOCK_CONFIG0   100000000U /* Value of the core/system clock frequency in the clock configuration 0 in Hz. */

#define CPU_CLOCK_CONFIG_0              0x00U /* Clock configuration 0 identifier */
#define CPU_CORE_CLK_HZ_CONFIG_0        100000000UL /* Core clock frequency in clock configuration 0 */
#define CPU_BUS_CLK_HZ_CONFIG_0         50000000UL /* Bus clock frequency in clock configuration 0 */
#define CPU_FLEXBUS_CLK_HZ_CONFIG_0     50000000UL /* Flexbus clock frequency in clock configuration 0 */
#define CPU_FLASH_CLK_HZ_CONFIG_0       25000000UL /* FLASH clock frequency in clock configuration 0 */
#define CPU_USB_CLK_HZ_CONFIG_0         0UL /* USB clock frequency in clock configuration 0 */
#define CPU_PLL_FLL_CLK_HZ_CONFIG_0     100000000UL /* PLL/FLL clock frequency in clock configuration 0 */
#define CPU_MCGIR_CLK_HZ_CONFIG_0       1000000UL /* MCG internal reference clock frequency in clock configuration 0 */
#define CPU_OSCER_CLK_HZ_CONFIG_0       50000000UL /* System OSC external reference clock frequency in clock configuration 0 */
#define CPU_ERCLK32K_CLK_HZ_CONFIG_0    1000UL /* External reference clock 32k frequency in clock configuration 0 */
#define CPU_MCGFF_CLK_HZ_CONFIG_0       48828UL /* MCG fixed frequency clock */

typedef uint8_t LDD_TClockConfiguration;

typedef struct  {
  uint32_t cpu_core_clk_hz;            /* Core clock frequency in clock configuration */
  uint32_t cpu_bus_clk_hz;             /* Bus clock frequency in clock configuration */
  uint32_t cpu_flexbus_clk_hz;         /* Flexbus clock frequency in clock configuration */
  uint32_t cpu_flash_clk_hz;           /* FLASH clock frequency in clock configuration */
  uint32_t cpu_usb_clk_hz;             /* USB clock frequency in clock configuration */
  uint32_t cpu_pll_fll_clk_hz;         /* PLL/FLL clock frequency in clock configuration */
  uint32_t cpu_mcgir_clk_hz;           /* MCG internal reference clock frequency in clock configuration */
  uint32_t cpu_oscer_clk_hz;           /* System OSC external reference clock frequency in clock configuration */
  uint32_t cpu_erclk32k_clk_hz;        /* External reference clock 32k frequency in clock configuration */
  uint32_t cpu_mcgff_clk_hz;           /* MCG fixed frequency clock */
} TCpuClockConfiguration;

extern const TCpuClockConfiguration PE_CpuClockConfigurations[CPU_CLOCK_CONFIG_NUMBER];

#define CPU_CLOCK_SLOW_MASK                                0x10U     /* Mode uses slow internal reference clock */ 
#define CPU_CLOCK_FAST_MASK                                0x20U     /* Mode uses fast internal reference clock */ 
#define CPU_CLOCK_EXTERNAL_CLOCK_MASK                      0x40U     /* Mode uses external reference clock from external clock input */ 
#define CPU_CLOCK_EXTERNAL_CRYSTAL_MASK                    0x80U     /* Mode uses external reference clock from crystal/resonator reference connection */ 
#define CPU_CLOCK_RTC_OSC_MASK                             0x0100U   /* Mode uses RTC oscillator clock */ 
#define CPU_CLOCK_PLL_MASK                                 0x0200U   /* PLL module is enabled in other than PEE and PBE mode */ 
#define CPU_CLOCK_IRC48M_MASK                              0x0400U   /* Mode uses internal reference clock 48MHz */ 

#define CPU_MCG_MODE_FEI                                   0x00U     /* FEI mode ID */ 
#define CPU_MCG_MODE_FBI                                   0x01U     /* FBI mode ID */ 
#define CPU_MCG_MODE_BLPI                                  0x02U     /* BLPI mode ID */ 
#define CPU_MCG_MODE_FEE                                   0x03U     /* FEE mode ID */ 
#define CPU_MCG_MODE_FBE                                   0x04U     /* FBE mode ID */ 
#define CPU_MCG_MODE_BLPE                                  0x05U     /* BLPE mode ID */ 
#define CPU_MCG_MODE_PBE                                   0x06U     /* PBE mode ID */ 
#define CPU_MCG_MODE_PEE                                   0x07U     /* PEE mode ID */ 
#define CPU_MCG_MODES                                      0x08U     /* Number of available MCG modes */ 
#define CPU_MCG_MODE_INDEX_MASK                            0x0FU     /* Mask of bits where MCG mode ID is encoded */ 

typedef uint16_t CPU_TClockGenMode;
typedef uint8_t CPU_TClockPowerMode;

typedef struct {
  uint8_t MCG_C1_value;
  uint8_t MCG_C2_value;
  uint8_t MCG_C4_value;
  uint8_t MCG_C5_value;
  uint8_t MCG_C6_value;
  uint8_t MCG_SC_value;
  uint8_t OSC_CR_value;
} CPU_TClockGenRegs;

typedef struct {
  uint32_t SIM_SOPT1_value;
  uint32_t SIM_SOPT2_value;
  uint32_t SIM_CLKDIV1_value;
} CPU_TClockSysRegs;

typedef struct {
  CPU_TClockGenMode Mode;
  CPU_TClockPowerMode PowerMode;
  CPU_TClockGenRegs GenRegs;
  CPU_TClockSysRegs SysRegs;
  uint32_t BusClock;
} CPU_TClockConfigDescriptor;

#define CPU_MCG_MODE_CONFIG_0                              (CPU_MCG_MODE_PEE | CPU_CLOCK_EXTERNAL_CLOCK_MASK | CPU_CLOCK_FAST_MASK) /* Clock generator mode */
#define CPU_CLOCK_PMODE_CONFIG_0                           0U /* RUN power mode */

#define CPU_MCG_C1_CONFIG_0                                0x2AU /* MCG_C1 */
#define CPU_MCG_C2_CONFIG_0                                0x21U /* MCG_C2 */
#define CPU_MCG_C4_CONFIG_0                                0x00U /* MCG_C4 */
#define CPU_MCG_C5_CONFIG_0                                0x0FU /* MCG_C5 */
#define CPU_MCG_C6_CONFIG_0                                0x48U /* MCG_C6 */
#define CPU_MCG_SC_CONFIG_0                                0x04U /* MCG_SC */
#define CPU_OSC_CR_CONFIG_0                                0x80U /* OSC_CR */
#define CPU_SIM_SOPT1_CONFIG_0                             0x000C0000UL /* SIM_SOPT1 */
#define CPU_SIM_SOPT2_CONFIG_0                             0x00010000UL /* SIM_SOPT2 */
#define CPU_SIM_CLKDIV1_CONFIG_0                           0x01130000UL /* SIM_CLKDIV1 */


/* Global variables */
LDD_TClockConfiguration ClockConfigurationID = CPU_CLOCK_CONFIG_0; /* Active clock configuration */

static const CPU_TClockGenMode ClockGenModeMatrix[8][8] = {
/* This matrix defines which mode is next in the MCG Mode state diagram in transitioning from the
   current mode to a target mode*/
  { CPU_MCG_MODE_FEI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FEE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE  }, /* FEI */
  { CPU_MCG_MODE_FEI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_BLPI, CPU_MCG_MODE_FEE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE  }, /* FBI */
  { CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_BLPI, CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI  }, /* BLPI */
  { CPU_MCG_MODE_FEI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FEE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE  }, /* FEE */
  { CPU_MCG_MODE_FEI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FEE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_BLPE, CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PBE  }, /* FBE */
  { CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_BLPE, CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PBE  }, /* BLPE */
  { CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_BLPE, CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PEE  }, /* PBE */
  { CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PBE,  CPU_MCG_MODE_PEE  }  /* PEE */
};

/* Clock generator default state in FEI mode 
   Clock source:                        Slow internal reference, disabled in the STOP mode
   Clock source frequency:              32.768 kHz (Property: Clock settings\Clock sources\Internal oscillator\Slow internal reference clock)
   FLL:                                 Enabled, engaged
   FLL factor:                          640
   Internal reference clock (MCGIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Enabled
*/
/* MCG_C1: CLKS=0,IREFS=1,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FEI_MCG_C1                             0x04U     /* MCG_C1 value in FEI default state */ 
/* MCG_C2: LOCRE0=1 */
#define CPU_DEFAULT_FEI_MCG_C2                             0x80U     /* MCG_C2 value in FEI default state */ 
/* MCG_C4: DMX32=0,DRST_DRS=0 */
#define CPU_DEFAULT_FEI_MCG_C4                             0x00U     /* MCG_C4 value in FEI default state */ 
/* MCG_C5: PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=0 */
#define CPU_DEFAULT_FEI_MCG_C5                             0x00U     /* MCG_C5 value in FEI default state */ 
/* MCG_C6: LOLIE0=0,CME0=0,VDIV0=0 */
#define CPU_DEFAULT_FEI_MCG_C6                             0x00U     /* MCG_C6 value in FEI default state */ 
/* MCG_C6: VDIV0|=2 */
#define CPU_DEFAULT_FEI_MCG_SC                             0x02U     /* MCG_SC value in FEI default state */ 
/* OSC_CR: ERCLKEN=0,EREFSTEN=0 */
#define CPU_DEFAULT_FEI_OSC_CR                             0x00U     /* OSC_CR value in FEI default state */ 

/* Clock generator default state in FBI mode
   Clock source:                        Slow internal reference, disabled in the STOP mode
   Clock source frequency:              32.768 kHz (Property: Clock settings\Clock sources\Internal oscillator\Slow internal reference clock)
   FLL:                                 Enabled, bypassed
   FLL factor:                          640
   Internal reference clock (MCGIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Enabled
*/
/* MCG_C1: CLKS|=1,IREFS=1,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FBI_MCG_C1                             0x44U     /* MCG_C1 value in FBI default state */ 
/* MCG_C2: LOCRE0=1 */
#define CPU_DEFAULT_FBI_MCG_C2                             0x80U     /* MCG_C2 value in FBI default state */ 
/* MCG_C4: DMX32=0,DRST_DRS=0 */
#define CPU_DEFAULT_FBI_MCG_C4                             0x00U     /* MCG_C4 value in FBI default state */ 
/* MCG_C5: PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=0 */
#define CPU_DEFAULT_FBI_MCG_C5                             0x00U     /* MCG_C5 value in FBI default state */ 
/* MCG_C6: LOLIE0=0,CME0=0,VDIV0=0 */
#define CPU_DEFAULT_FBI_MCG_C6                             0x00U     /* MCG_C6 value in FBI default state */ 
/* MCG_C6: VDIV0|=2 */
#define CPU_DEFAULT_FBI_MCG_SC                             0x02U     /* MCG_SC value in FBI default state */ 
/* OSC_CR: ERCLKEN=0,EREFSTEN=0 */
#define CPU_DEFAULT_FBI_OSC_CR                             0x00U     /* OSC_CR value in FBI default state */ 

/* Clock generator default state in BLPI mode
   Clock source:                        Slow internal reference, disabled in the STOP mode
   Clock source frequency:              32.768 kHz (Property: Clock settings\Clock sources\Internal oscillator\Slow internal reference clock)
   FLL:                                 Disabled
   Internal reference clock (MCGIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Enabled
*/
/* MCG_C1: CLKS|=1,IREFS=1,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_BLPI_MCG_C1                            0x44U     /* MCG_C1 value in BLPI default state */ 
/* MCG_C2: LOCRE0=1,LP=1 */
#define CPU_DEFAULT_BLPI_MCG_C2                            0x82U     /* MCG_C2 value in BLPI default state */ 
/* MCG_C4:  */
#define CPU_DEFAULT_BLPI_MCG_C4                            0x00U     /* MCG_C4 value in BLPI default state */ 
/* MCG_C5: PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=0 */
#define CPU_DEFAULT_BLPI_MCG_C5                            0x00U     /* MCG_C5 value in BLPI default state */ 
/* MCG_C6: LOLIE0=0,CME0=0,VDIV0=0 */
#define CPU_DEFAULT_BLPI_MCG_C6                            0x00U     /* MCG_C6 value in BLPI default state */ 
/* MCG_C6: VDIV0|=2 */
#define CPU_DEFAULT_BLPI_MCG_SC                            0x02U     /* MCG_SC value in BLPI default state */ 
/* OSC_CR: ERCLKEN=0,EREFSTEN=0 */
#define CPU_DEFAULT_BLPI_OSC_CR                            0x00U     /* OSC_CR value in BLPI default state */ 

/* Clock generator default state in FEE mode
   Clock source:                        External reference clock (Property: Clock settings\Clock sources\System oscillator 0\Clock source)
   Clock source frequency:              50 MHz (Property: Clock settings\Clock sources\System oscillator 0\Clock source\Clock frequency)
   External frequency range:            Very high
   FLL external reference divider:      1280
   FLL:                                 Enabled, engaged
   FLL factor:                          640
   Internal reference clock (MCGIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Enabled
*/
/* MCG_C1: CLKS=0,FRDIV|=6,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FEE_MCG_C1                             0x30U     /* MCG_C1 value in FEE default state */ 
/* MCG_C2: LOCRE0=1,RANGE|=2 */
#define CPU_DEFAULT_FEE_MCG_C2                             0xA0U     /* MCG_C2 value in FEE default state */ 
/* MCG_C4: DMX32=0,DRST_DRS=0 */
#define CPU_DEFAULT_FEE_MCG_C4                             0x00U     /* MCG_C4 value in FEE default state */ 
/* MCG_C5: PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=0 */
#define CPU_DEFAULT_FEE_MCG_C5                             0x00U     /* MCG_C5 value in FEE default state */ 
/* MCG_C6: LOLIE0=0,CME0=0,VDIV0=0 */
#define CPU_DEFAULT_FEE_MCG_C6                             0x00U     /* MCG_C6 value in FEE default state */ 
/* MCG_C6: VDIV0|=2 */
#define CPU_DEFAULT_FEE_MCG_SC                             0x02U     /* MCG_SC value in FEE default state */ 
/* OSC_CR: ERCLKEN=0,EREFSTEN=0 */
#define CPU_DEFAULT_FEE_OSC_CR                             0x00U     /* OSC_CR value in FEE default state */ 

/* Clock generator default state in FBE mode
   Clock source:                        External reference clock (Property: Clock settings\Clock sources\System oscillator 0\Clock source)
   Clock source frequency:              50 MHz (Property: Clock settings\Clock sources\System oscillator 0\Clock source\Clock frequency)
   External frequency range:            Very high
   FLL external reference divider:      1280
   FLL:                                 Enabled, bypassed
   FLL factor:                          640
   Internal reference clock (MCGIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Enabled
*/
/* MCG_C1: CLKS|=2,FRDIV|=6,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FBE_MCG_C1                             0xB0U     /* MCG_C1 value in FBE default state */ 
/* MCG_C2: LOCRE0=1,RANGE|=2 */
#define CPU_DEFAULT_FBE_MCG_C2                             0xA0U     /* MCG_C2 value in FBE default state */ 
/* MCG_C4: DMX32=0,DRST_DRS=0 */
#define CPU_DEFAULT_FBE_MCG_C4                             0x00U     /* MCG_C4 value in FBE default state */ 
/* MCG_C5: PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=0 */
#define CPU_DEFAULT_FBE_MCG_C5                             0x00U     /* MCG_C5 value in FBE default state */ 
/* MCG_C6: LOLIE0=0,CME0=0,VDIV0=0 */
#define CPU_DEFAULT_FBE_MCG_C6                             0x00U     /* MCG_C6 value in FBE default state */ 
/* MCG_C6: VDIV0|=2 */
#define CPU_DEFAULT_FBE_MCG_SC                             0x02U     /* MCG_SC value in FBE default state */ 
/* OSC_CR: ERCLKEN=0,EREFSTEN=0 */
#define CPU_DEFAULT_FBE_OSC_CR                             0x00U     /* OSC_CR value in FBE default state */ 

/* Clock generator default state in BLPE mode
   Clock source:                        External reference clock (Property: Clock settings\Clock sources\System oscillator 0\Clock source)
   Clock source frequency:              50 MHz (Property: Clock settings\Clock sources\System oscillator 0\Clock source\Clock frequency)
   External frequency range:            Very high
   FLL external reference divider:      1280
   FLL:                                 Disabled
   Internal reference clock (MCGIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Enabled
*/
/* MCG_C1: CLKS|=1,FRDIV|=6,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_BLPE_MCG_C1                            0x70U     /* MCG_C1 value in BLPE default state */ 
/* MCG_C2: LOCRE0=1,RANGE|=2,LP=1 */
#define CPU_DEFAULT_BLPE_MCG_C2                            0xA2U     /* MCG_C2 value in BLPE default state */ 
/* MCG_C4:  */
#define CPU_DEFAULT_BLPE_MCG_C4                            0x00U     /* MCG_C4 value in BLPE default state */ 
/* MCG_C5: PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=0 */
#define CPU_DEFAULT_BLPE_MCG_C5                            0x00U     /* MCG_C5 value in BLPE default state */ 
/* MCG_C6: LOLIE0=0,CME0=0,VDIV0=0 */
#define CPU_DEFAULT_BLPE_MCG_C6                            0x00U     /* MCG_C6 value in BLPE default state */ 
/* MCG_C6: VDIV0|=2 */
#define CPU_DEFAULT_BLPE_MCG_SC                            0x02U     /* MCG_SC value in BLPE default state */ 
/* OSC_CR: ERCLKEN=0,EREFSTEN=0 */
#define CPU_DEFAULT_BLPE_OSC_CR                            0x00U     /* OSC_CR value in BLPE default state */ 
/* Clock generator default state in PEE mode
   Clock source:                        External reference clock (Property: Clock settings\Clock sources\System oscillator 0\Clock source)
   Clock source frequency:              50 MHz (Property: Clock settings\Clock sources\System oscillator 0\Clock source\Clock frequency)
	 PLL External Reference Divider       %EXPE(12 + 1)
	 PLL Multiplier                       48
   PLL:                                 Enabled, engaged
   Internal reference clock (MCGIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Enabled
*/
/* MCG_C1: CLKS=0,FRDIV|=6,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_PEE_MCG_C1                             0x30U     /* MCG_C1 value in PEE default state */ 
/* MCG_C2: LOCRE0=1,RANGE|=2 */
#define CPU_DEFAULT_PEE_MCG_C2                             0xA0U     /* MCG_C2 value in PEE default state */ 
/* MCG_C4: DMX32=0,DRST_DRS=0 */
#define CPU_DEFAULT_PEE_MCG_C4                             0x00U     /* MCG_C4 value in PEE default state */ 
/* MCG_C5: PLLCLKEN0=0,PLLSTEN0=0,PRDIV0|=0x0C */
#define CPU_DEFAULT_PEE_MCG_C5                             0x0CU     /* MCG_C5 value in PEE default state */ 
/* MCG_C6: PLLS=1,CME0=0,VDIV0|=0x18 */
#define CPU_DEFAULT_PEE_MCG_C6                             0x58U     /* MCG_C6 value in PEE default state */ 
/* MCG_C6: VDIV0|=2 */
#define CPU_DEFAULT_PEE_MCG_SC                             0x02U     /* MCG_SC value in PEE default state */ 
/* OSC_CR: ERCLKEN=0,EREFSTEN=0 */
#define CPU_DEFAULT_PEE_OSC_CR                             0x00U     /* OSC_CR value in PEE default state */ 
/* Clock generator default state in PBE mode
   Clock source:                        External reference clock (Property: Clock settings\Clock sources\System oscillator 0\Clock source)
   Clock source frequency:              50 MHz (Property: Clock settings\Clock sources\System oscillator 0\Clock source\Clock frequency)
	 PLL External Reference Divider       %EXPE(12 + 1)
	 PLL Multiplier                       48
   PLL:                                 Enabled, bypassed
   Internal reference clock (MCGIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Enabled
*/
/* MCG_C1: CLKS|=2,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_PBE_MCG_C1                             0x80U     /* MCG_C1 value in PBE default state */ 
/* MCG_C2: LOCRE0=1,RANGE|=2 */
#define CPU_DEFAULT_PBE_MCG_C2                             0xA0U     /* MCG_C2 value in PBE default state */ 
/* MCG_C4: DMX32=0,DRST_DRS=0 */
#define CPU_DEFAULT_PBE_MCG_C4                             0x00U     /* MCG_C4 value in PBE default state */ 
/* MCG_C5: PLLCLKEN0=0,PLLSTEN0=0,PRDIV0|=0x0C */
#define CPU_DEFAULT_PBE_MCG_C5                             0x0CU     /* MCG_C5 value in PBE default state */ 
/* MCG_C6: PLLS=1,CME0=0,VDIV0|=0x18 */
#define CPU_DEFAULT_PBE_MCG_C6                             0x58U     /* MCG_C6 value in PBE default state */ 
/* MCG_C6: VDIV0|=2 */
#define CPU_DEFAULT_PBE_MCG_SC                             0x02U     /* MCG_SC value in PBE default state */ 
/* OSC_CR: ERCLKEN=0,EREFSTEN=0 */
#define CPU_DEFAULT_PBE_OSC_CR                             0x00U     /* OSC_CR value in PBE default state */ 


static const CPU_TClockGenRegs CPU_DefaultFEI = {
  CPU_DEFAULT_FEI_MCG_C1,CPU_DEFAULT_FEI_MCG_C2,CPU_DEFAULT_FEI_MCG_C4,CPU_DEFAULT_FEI_MCG_C5,CPU_DEFAULT_FEI_MCG_C6,CPU_DEFAULT_FEI_MCG_SC,CPU_DEFAULT_FEI_OSC_CR
};
static const CPU_TClockGenRegs CPU_DefaultFBI = {
  CPU_DEFAULT_FBI_MCG_C1,CPU_DEFAULT_FBI_MCG_C2,CPU_DEFAULT_FBI_MCG_C4,CPU_DEFAULT_FBI_MCG_C5,CPU_DEFAULT_FBI_MCG_C6,CPU_DEFAULT_FBI_MCG_SC,CPU_DEFAULT_FBI_OSC_CR
};
static const CPU_TClockGenRegs CPU_DefaultBLPI = {
  CPU_DEFAULT_BLPI_MCG_C1,CPU_DEFAULT_BLPI_MCG_C2,CPU_DEFAULT_BLPI_MCG_C4,CPU_DEFAULT_BLPI_MCG_C5,CPU_DEFAULT_BLPI_MCG_C6,CPU_DEFAULT_BLPI_MCG_SC,CPU_DEFAULT_BLPI_OSC_CR
};
static const CPU_TClockGenRegs CPU_DefaultFEE = {
  CPU_DEFAULT_FEE_MCG_C1,CPU_DEFAULT_FEE_MCG_C2,CPU_DEFAULT_FEE_MCG_C4,CPU_DEFAULT_FEE_MCG_C5,CPU_DEFAULT_FEE_MCG_C6,CPU_DEFAULT_FEE_MCG_SC,CPU_DEFAULT_FEE_OSC_CR
};
static const CPU_TClockGenRegs CPU_DefaultFBE = {
  CPU_DEFAULT_FBE_MCG_C1,CPU_DEFAULT_FBE_MCG_C2,CPU_DEFAULT_FBE_MCG_C4,CPU_DEFAULT_FBE_MCG_C5,CPU_DEFAULT_FBE_MCG_C6,CPU_DEFAULT_FBE_MCG_SC,CPU_DEFAULT_FBE_OSC_CR
};
static const CPU_TClockGenRegs CPU_DefaultBLPE = {
  CPU_DEFAULT_BLPE_MCG_C1,CPU_DEFAULT_BLPE_MCG_C2,CPU_DEFAULT_BLPE_MCG_C4,CPU_DEFAULT_BLPE_MCG_C5,CPU_DEFAULT_BLPE_MCG_C6,CPU_DEFAULT_BLPE_MCG_SC,CPU_DEFAULT_BLPE_OSC_CR
};
static const CPU_TClockGenRegs CPU_DefaultPBE = {
  CPU_DEFAULT_PBE_MCG_C1,CPU_DEFAULT_PBE_MCG_C2,CPU_DEFAULT_PBE_MCG_C4,CPU_DEFAULT_PBE_MCG_C5,CPU_DEFAULT_PBE_MCG_C6,CPU_DEFAULT_PBE_MCG_SC,CPU_DEFAULT_PBE_OSC_CR
};
static const CPU_TClockGenRegs CPU_DefaultPEE = {
  CPU_DEFAULT_PEE_MCG_C1,CPU_DEFAULT_PEE_MCG_C2,CPU_DEFAULT_PEE_MCG_C4,CPU_DEFAULT_PEE_MCG_C5,CPU_DEFAULT_PEE_MCG_C6,CPU_DEFAULT_PEE_MCG_SC,CPU_DEFAULT_PEE_OSC_CR
};

static const CPU_TClockGenRegs *const CPU_ClockGenModeRegs[CPU_MCG_MODES] = {
  &CPU_DefaultFEI,
  &CPU_DefaultFBI,
  &CPU_DefaultBLPI,
  &CPU_DefaultFEE,
  &CPU_DefaultFBE,
  &CPU_DefaultBLPE,
  &CPU_DefaultPBE,
  &CPU_DefaultPEE
};

static const CPU_TClockConfigDescriptor CPU_ClockConfigDescriptors[CPU_CLOCK_CONFIG_NUMBER] = {
  /* Clock configuration 0 */
  {
    CPU_MCG_MODE_CONFIG_0,             /* Clock generator mode */
    CPU_CLOCK_PMODE_CONFIG_0,          /* Very low power mode enable/disable */
    {                                  /* MCG registers */
      CPU_MCG_C1_CONFIG_0,CPU_MCG_C2_CONFIG_0,CPU_MCG_C4_CONFIG_0,CPU_MCG_C5_CONFIG_0,CPU_MCG_C6_CONFIG_0,CPU_MCG_SC_CONFIG_0,CPU_OSC_CR_CONFIG_0,
    },
    {                                  /* SIM registers */
      CPU_SIM_SOPT1_CONFIG_0,CPU_SIM_SOPT2_CONFIG_0,CPU_SIM_CLKDIV1_CONFIG_0,
    },
    CPU_BUS_CLK_HZ_CONFIG_0            /* Bus frequency */
  }
};

CPU_TClockGenMode CPU_GetClockGenMode(void)
{
  switch (MCG_C1  & MCG_C1_CLKS_MASK) {
    case  0x00U:
      /* Output of FLL or PLL is selected */
      if ((MCG_C6 & MCG_C6_PLLS_MASK) == 0x00U) {
        /* Output of FLL is selected */
        if ((MCG_C1 & MCG_C1_IREFS_MASK) == 0x00U) {
          /* External reference clock is selected */
          return CPU_MCG_MODE_FEE;
        } else {
          /* Internal reference clock is selected */
          return CPU_MCG_MODE_FEI;
        }
      } else {
        /* Output of FLL is selected */
	      return CPU_MCG_MODE_PEE;
	    }  
    case 0x40U:
      /* Internal reference clock is selected */
      if ((MCG_C2 & MCG_C2_LP_MASK) == 0x00U) {
        /* Low power mode is disabled */
        return CPU_MCG_MODE_FBI;
      } else {
        /* Low power mode is enabled */
        return CPU_MCG_MODE_BLPI;
      }
    case 0x80U:
      /* External reference clock is selected */
      if ((MCG_C6 & MCG_C6_PLLS_MASK) == 0x00U) {
        /* FLL or PLL is selected */
        if ((MCG_C2 & MCG_C2_LP_MASK) == 0x00U) {
          /* FLL is selected */
          return CPU_MCG_MODE_FBE;
        } else {
          /* Low power mode is enabled */
          return CPU_MCG_MODE_BLPE;
        }
      } else {
        /* PLL is selected */
        return CPU_MCG_MODE_PBE;
      }
    default:
      return (CPU_TClockGenMode)0x00U;
  }
}

void CPU_SetClockConfigGenMode(LDD_TClockConfiguration ConfigID)
{
  CPU_TClockGenMode NextMode;
  CPU_TClockGenRegs NextModeRegs;
  CPU_TClockGenMode TargetMode = CPU_ClockConfigDescriptors[ConfigID].Mode;
  
  /* External clock source pin routing */
  if ((TargetMode & (CPU_CLOCK_EXTERNAL_CLOCK_MASK | CPU_CLOCK_EXTERNAL_CRYSTAL_MASK)) != 0) { /* Is external clock source used in targeted clock configuration? */
    /* If yes, initialize the EXTAL pin routing */
    SIM_SCGC5 |= (uint32_t)SIM_SCGC5_PORTA_MASK; /* Enable EXTAL/XTAL pins clock gate */
    /* PORTA_PCR18: ISF=0,MUX=0 */
    PORTA_PCR18 &= (uint32_t)~(uint32_t)(
                    PORT_PCR_ISF_MASK |
                    PORT_PCR_MUX(0x07)
                   );
    if ((TargetMode & CPU_CLOCK_EXTERNAL_CRYSTAL_MASK) != 0) { /* Is external crystal/resonator used in targeted clock configuration? */
      /* If yes, initialize also XTAL pin routing */
      /* PORTA_PCR19: ISF=0,MUX=0 */
      PORTA_PCR19 &= (uint32_t)~(uint32_t)(
                      PORT_PCR_ISF_MASK |
                      PORT_PCR_MUX(0x07)
                     );
    }
  }
  
  if ((MCG_SC & MCG_SC_FCRDIV_MASK) != CPU_ClockConfigDescriptors[ConfigID].GenRegs.MCG_SC_value) { /* Check if it is necessary to update Fast Clock Internal Reference Divider */ 
    if ((MCG_C2 & MCG_C2_IRCS_MASK) == 0x00U) {
      MCG_SC = (MCG_SC & (uint8_t)(~(MCG_SC_FCRDIV_MASK))) | CPU_ClockConfigDescriptors[ConfigID].GenRegs.MCG_SC_value;
    } else {
      if ((MCG_C1 & (MCG_C1_IREFS_MASK || MCG_C1_IRCLKEN_MASK)) != 0x00U) { /* Is internal reference clock active? */      
        MCG_C2 &= (uint8_t)(~(MCG_C2_IRCS_MASK)); /* Disable the fast internal clock */
        while((MCG_S & MCG_S_IRCST_MASK) != 0x00U) { /* Check that the source internal reference clock is slow clock. */
        }
        MCG_SC = (MCG_SC & (uint8_t)(~(MCG_SC_FCRDIV_MASK))) | CPU_ClockConfigDescriptors[ConfigID].GenRegs.MCG_SC_value;    
        MCG_C2 |= MCG_C2_IRCS_MASK;    /* Re-enable the fast internal clock */
        while((MCG_S & MCG_S_IRCST_MASK) == 0x00U) { /* Check that the source internal reference clock is fast clock. */
        }
      } else {
        MCG_C2 &= (uint8_t)(~(MCG_C2_IRCS_MASK)); /* Disable the fast internal clock */
        MCG_SC = (MCG_SC & (uint8_t)(~(MCG_SC_FCRDIV_MASK))) | CPU_ClockConfigDescriptors[ConfigID].GenRegs.MCG_SC_value;    
        MCG_C2 |= MCG_C2_IRCS_MASK;    /* Re-enable the fast internal clock */
      }
    }
  }      
  NextMode = CPU_GetClockGenMode(); /* Identify the currently active MCG mode */
  do {
    NextMode = ClockGenModeMatrix[NextMode & CPU_MCG_MODE_INDEX_MASK][TargetMode & CPU_MCG_MODE_INDEX_MASK];
    if (NextMode == (TargetMode & CPU_MCG_MODE_INDEX_MASK)) {
      NextModeRegs = CPU_ClockConfigDescriptors[ConfigID].GenRegs;
    } else {
      NextModeRegs = *CPU_ClockGenModeRegs[NextMode & CPU_MCG_MODE_INDEX_MASK];
    }
    switch (NextMode & CPU_MCG_MODE_INDEX_MASK) {
      case CPU_MCG_MODE_FEI:
      case CPU_MCG_MODE_FBI:
        MCG_C1 = NextModeRegs.MCG_C1_value; /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) */
        while((MCG_S & MCG_S_IREFST_MASK) == 0x00U) { /* Check that the source of the FLL reference clock is the internal reference clock. */
        }
        MCG_C2 = ((NextModeRegs.MCG_C2_value) & (uint8_t)(~(MCG_C2_FCFTRIM_MASK))) | (MCG_C2 & MCG_C2_FCFTRIM_MASK); /* Set C2 (freq. range, ext. and int. reference selection etc.; trim values not changed) */
        MCG_C4 = ((NextModeRegs.MCG_C4_value) & (uint8_t)(~(MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK))) | (MCG_C4 & (MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK)); /* Set C4 (FLL output; trim values not changed) */
        OSC_CR = NextModeRegs.OSC_CR_value; /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */      	
        if (TargetMode & CPU_CLOCK_RTC_OSC_MASK) {
          MCG_C7 = MCG_C7_OSCSEL(1);   /* Select RTC oscillator as MCG clock source */
        } else if (TargetMode & CPU_CLOCK_IRC48M_MASK) {
          MCG_C7 = MCG_C7_OSCSEL(2);   /* Select IRC 48MHz as MCG clock source */
        } else {
          MCG_C7 = 0;                  /* Select system oscillator as MCG clock source */
        }
        MCG_C5 = NextModeRegs.MCG_C5_value; /* Set C5 (PLL settings, PLL reference divider etc.) */
        if (TargetMode & CPU_CLOCK_PLL_MASK) {
          MCG_C5 |= MCG_C5_PLLCLKEN0_MASK;
        }
        MCG_C6 = NextModeRegs.MCG_C6_value; /* Set C6 (PLL select, VCO divider etc.) */
        break;
      case CPU_MCG_MODE_BLPI:
        MCG_C1 = NextModeRegs.MCG_C1_value; /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) */
        while((MCG_S & MCG_S_IREFST_MASK) == 0x00U) { /* Check that the source of the FLL reference clock is the internal reference clock. */
        }
        MCG_C2 = ((NextModeRegs.MCG_C2_value) & (uint8_t)(~(MCG_C2_FCFTRIM_MASK))) | (MCG_C2 & MCG_C2_FCFTRIM_MASK); /* Set C2 (freq. range, ext. and int. reference selection etc.; trim values not changed) */
        OSC_CR = NextModeRegs.OSC_CR_value; /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */
        if (TargetMode & CPU_CLOCK_FAST_MASK) {
          while((MCG_S & MCG_S_IRCST_MASK) == 0x00U) { /* Check that the fast external reference clock is selected. */
      	  }
      	}      	
        break;
      case CPU_MCG_MODE_FEE:
      case CPU_MCG_MODE_FBE:
        MCG_C2 = ((NextModeRegs.MCG_C2_value) & (uint8_t)(~(MCG_C2_FCFTRIM_MASK))) | (MCG_C2 & MCG_C2_FCFTRIM_MASK); /* Set C2 (freq. range, ext. and int. reference selection etc.; trim values not changed) */
        OSC_CR = NextModeRegs.OSC_CR_value; /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */      	
        if (TargetMode & CPU_CLOCK_RTC_OSC_MASK) {
          MCG_C7 = MCG_C7_OSCSEL(1);   /* Select RTC oscillator as MCG clock source */
        } else if (TargetMode & CPU_CLOCK_IRC48M_MASK) {
          MCG_C7 = MCG_C7_OSCSEL(2);   /* Select IRC 48MHz as MCG clock source */
        } else {
          MCG_C7 = 0;                  /* Select system oscillator as MCG clock source */
        }
        MCG_C1 = NextModeRegs.MCG_C1_value; /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) */
        if ((TargetMode & CPU_CLOCK_EXTERNAL_CRYSTAL_MASK) != 0) {
          while((MCG_S & MCG_S_OSCINIT0_MASK) == 0x00U) { /* Check that the oscillator is running */
          }
        }
        while((MCG_S & MCG_S_IREFST_MASK) != 0x00U) { /* Check that the source of the FLL reference clock is the external reference clock. */
        }
        MCG_C4 = ((NextModeRegs.MCG_C4_value) & (uint8_t)(~(MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK))) | (MCG_C4 & (MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK)); /* Set C4 (FLL output; trim values not changed) */
        MCG_C5 = NextModeRegs.MCG_C5_value; /* Set C5 (PLL settings, PLL reference divider etc.) */
        if (TargetMode & CPU_CLOCK_PLL_MASK) {
          MCG_C5 |= MCG_C5_PLLCLKEN0_MASK;
        }
        MCG_C6 = NextModeRegs.MCG_C6_value; /* Set C6 (PLL select, VCO divider etc.) */
        break;
      case CPU_MCG_MODE_BLPE:
        MCG_C1 = NextModeRegs.MCG_C1_value; /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) */
        MCG_C2 = ((NextModeRegs.MCG_C2_value) & (uint8_t)(~(MCG_C2_FCFTRIM_MASK))) | (MCG_C2 & MCG_C2_FCFTRIM_MASK); /* Set C2 (freq. range, ext. and int. reference selection etc.; trim values not changed) */
        OSC_CR = NextModeRegs.OSC_CR_value; /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */      	
        if (TargetMode & CPU_CLOCK_RTC_OSC_MASK) {
          MCG_C7 = MCG_C7_OSCSEL(1);   /* Select RTC oscillator as MCG clock source */
        } else if (TargetMode & CPU_CLOCK_IRC48M_MASK) {
          MCG_C7 = MCG_C7_OSCSEL(2);   /* Select IRC 48MHz as MCG clock source */
        } else {
          MCG_C7 = 0;                  /* Select system oscillator as MCG clock source */
        }
        if ((TargetMode & CPU_CLOCK_EXTERNAL_CRYSTAL_MASK) != 0) {
          while((MCG_S & MCG_S_OSCINIT0_MASK) == 0x00U) { /* Check that the oscillator is running */
          }
        }
        break;
      case CPU_MCG_MODE_PEE:
      case CPU_MCG_MODE_PBE:
        OSC_CR = NextModeRegs.OSC_CR_value; /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */
        if (TargetMode & CPU_CLOCK_RTC_OSC_MASK) {
          MCG_C7 = MCG_C7_OSCSEL(1);   /* Select RTC oscillator as MCG clock source */
        } else if (TargetMode & CPU_CLOCK_IRC48M_MASK) {
          MCG_C7 = MCG_C7_OSCSEL(2);   /* Select IRC 48MHz as MCG clock source */
        } else {
          MCG_C7 = 0;                  /* Select system oscillator as MCG clock source */
        }
        MCG_C1 = NextModeRegs.MCG_C1_value; /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) */
        MCG_C2 = ((NextModeRegs.MCG_C2_value) & (uint8_t)(~(MCG_C2_FCFTRIM_MASK))) | (MCG_C2 & MCG_C2_FCFTRIM_MASK); /* Set C2 (freq. range, ext. and int. reference selection etc.; trim values not changed) */
        if ((TargetMode & CPU_MCG_MODE_INDEX_MASK) == CPU_MCG_MODE_PEE) {
          MCG_C5 = CPU_ClockConfigDescriptors[ConfigID].GenRegs.MCG_C5_value; /* Set C5 (PLL settings, PLL reference divider etc.) */
          MCG_C6 = CPU_ClockConfigDescriptors[ConfigID].GenRegs.MCG_C6_value; /* Set C6 (PLL select, VCO divider etc.) */
        } else {
          MCG_C5 = NextModeRegs.MCG_C5_value; /* Set C5 (PLL settings, PLL reference divider etc.) */
          MCG_C6 = NextModeRegs.MCG_C6_value; /* Set C6 (PLL select, VCO divider etc.) */
        }
        while((MCG_S & MCG_S_LOCK0_MASK) == 0x00U) { /* Wait until PLL is locked*/
        }
        break;
      default:
        break;            
    }
    switch (NextMode) {
      case CPU_MCG_MODE_FEI:
      case CPU_MCG_MODE_FEE:
        while((MCG_S & 0x0CU) != 0x00U) { /* Wait until output of the FLL is selected */
        }
        break;
      case CPU_MCG_MODE_FBI:
      case CPU_MCG_MODE_BLPI:
        while((MCG_S & 0x0CU) != 0x04U) { /* Wait until internal reference clock is selected as MCG output */
        }
        break;
      case CPU_MCG_MODE_FBE:
      case CPU_MCG_MODE_BLPE:
      case CPU_MCG_MODE_PBE:
        while((MCG_S & 0x0CU) != 0x08U) { /* Wait until external reference clock is selected as MCG output */
        }
        break;
      case CPU_MCG_MODE_PEE:
        while((MCG_S & 0x0CU) != 0x0CU) { /* Wait until output of the PLL is selected */
        }
        break;
    }
  } while (NextMode != (TargetMode & CPU_MCG_MODE_INDEX_MASK)); /* Loop until the target MCG mode is set */
}


void __LDM_init (void)
{
	SCB_SHPR3 &= SCB_SHPR3_PRI_15(0xF0);
	__asm ("msr basepri, %[input]"::[input] "r" (0xF0):);
	
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x00) |
				  SIM_CLKDIV1_OUTDIV2(0x01) |
				  SIM_CLKDIV1_OUTDIV3(0x04) |
				  SIM_CLKDIV1_OUTDIV4(0x04); 
	CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_0);
	SIM_CLKDIV1 = (uint32_t)CPU_ClockConfigDescriptors[CPU_CLOCK_CONFIG_0].SysRegs.SIM_CLKDIV1_value;
	SIM_SOPT1 = (uint32_t)((SIM_SOPT1 & (uint32_t)~(uint32_t)SIM_SOPT1_OSC32KSEL_MASK) | (uint32_t)CPU_ClockConfigDescriptors[CPU_CLOCK_CONFIG_0].SysRegs.SIM_SOPT1_value);
	SIM_SOPT2 = (uint32_t)((SIM_SOPT2 & (uint32_t)~(uint32_t)SIM_SOPT2_PLLFLLSEL_MASK) | (uint32_t)CPU_ClockConfigDescriptors[CPU_CLOCK_CONFIG_0].SysRegs.SIM_SOPT2_value);
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK |
			     SIM_SCGC5_PORTB_MASK |
			     SIM_SCGC5_PORTC_MASK |
			     SIM_SCGC5_PORTD_MASK |
			     SIM_SCGC5_PORTE_MASK;
}

#define CPU_FLASH_CONFIG_FIELD \
               /* NV_BACKKEY3: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY2: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY1: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY0: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY7: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY6: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY5: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY4: KEY=0xFF */ \
               0xFFU, \
               /* NV_FPROT3: PROT=0xFF */ \
               0xFFU, \
               /* NV_FPROT2: PROT=0xFF */ \
               0xFFU, \
               /* NV_FPROT1: PROT=0xFF */ \
               0xFFU, \
               /* NV_FPROT0: PROT=0xFF */ \
               0xFFU, \
               /* NV_FSEC: KEYEN=1,MEEN=3,FSLACC=3,SEC=2 */ \
               0x7EU, \
               /* NV_FOPT: ??=1,??=1,??=1,??=1,??=1,??=1,EZPORT_DIS=1,LPBOOT=1 */ \
               0xFFU, \
               /* NV_FEPROT: EPROT=0xFF */ \
               0xFFU, \
               /* NV_FDPROT: DPROT=0xFF */ \
               0xFFU

__attribute__ ((section (".cfmconfig"))) const uint8_t _cfm[0x10] = {CPU_FLASH_CONFIG_FIELD};
  
