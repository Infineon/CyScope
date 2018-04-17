/*******************************************************************************
* File Name: Mixer_1.h  
* Version 2.0
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Mixer component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_MIXER_Mixer_1_H) 
#define CY_MIXER_Mixer_1_H 

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"


/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Mixer_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


#if(!CY_PSOC5A)
    #if(CYDEV_VARIABLE_VDDA == 1)
        #if (!defined(CY_LIB_SC_BST_CLK_EN))
            #error Component Mixer_v2_0 requires cy_boot v3.30 or later
        #endif /* (!defined(CY_LIB_SC_BST_CLK_EN)) */
    #endif /* CYDEV_VARIABLE_VDDA == 1 */
#endif /* (!CY_PSOC5A) */

/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct
{
    uint8   enableState;
    uint8   scCR0Reg;
    uint8   scCR1Reg;
    uint8   scCR2Reg;
}   Mixer_1_backupStruct;

/* Fixed configuration of SC Block only performed once */
extern uint8 Mixer_1_initVar;

/***************************************
*        Function Prototypes 
***************************************/

void Mixer_1_Start(void)                   ;
void Mixer_1_Stop(void)                    ;
void Mixer_1_SetPower(uint8 power)         ;
void Mixer_1_Sleep(void)                   ;
void Mixer_1_Wakeup(void)                  ;
void Mixer_1_SaveConfig(void)              ;
void Mixer_1_RestoreConfig(void)           ;
void Mixer_1_Init(void)                    ;
void Mixer_1_Enable(void)                  ;


/***************************************
*              Constants        
***************************************/

/* Constants for Mixer Type */
#define Mixer_1_CTMIXER                    (0x00u)
#define Mixer_1_DTMIXER                    (0x01u)

/* Constants for Local Oscillator Freq */
#define Mixer_1_LO_CLOCK_FREQ_100_KHz      (100u)
#define Mixer_1_UNKNOWN_EXTERNAL_LO        (-1)

/* Constants for SetPower function */
#define Mixer_1_MINPOWER                   (0x00u)
#define Mixer_1_LOWPOWER                   (0x01u)
#define Mixer_1_MEDPOWER                   (0x02u)
#define Mixer_1_HIGHPOWER                  (0x03u)

/* Constants for LO Source */
#define Mixer_1_LO_SOURCE_INTERNAL         (0x00u)
#define Mixer_1_LO_SOURCE_EXTERNAL         (0x01u)


/***************************************
*       Initial Paramater Values
***************************************/

#define Mixer_1_MIXER_TYPE                 (1u)
#define Mixer_1_INIT_POWER                 (3u)
#define Mixer_1_LO_SOURCE                  (1u)
#define Mixer_1_EXTERNAL_LO_CLK_FREQ       ((float)100)
#define Mixer_1_VddaVal                    (5u)

#if (Mixer_1_LO_SOURCE == Mixer_1_LO_SOURCE_INTERNAL)
    /* Internal LO clock frequency */
    #define Mixer_1_LO_CLOCK_FREQ          ((float)100)
#else
    #define Mixer_1_LO_CLOCK_FREQ          (((float)100 == \
                                                      Mixer_1_UNKNOWN_EXTERNAL_LO) ? \
                                                      ((float)100) : ((float)100))
#endif /* Mixer_1_LO_SOURCE == Mixer_1_LO_SOURCE_INTERNAL */


/***************************************
*              Registers        
***************************************/

/* SC Block Configuration Registers */
#define Mixer_1_CR0_REG                    (* (reg8 *) Mixer_1_SC__CR0 )
#define Mixer_1_CR0_PTR                    (  (reg8 *) Mixer_1_SC__CR0 )
#define Mixer_1_CR1_REG                    (* (reg8 *) Mixer_1_SC__CR1 )
#define Mixer_1_CR1_PTR                    (  (reg8 *) Mixer_1_SC__CR1 )
#define Mixer_1_CR2_REG                    (* (reg8 *) Mixer_1_SC__CR2 )
#define Mixer_1_CR2_PTR                    (  (reg8 *) Mixer_1_SC__CR2 )
/* SC Block clk control */
#define Mixer_1_CLK_REG                    (* (reg8 *) Mixer_1_SC__CLK )  
#define Mixer_1_CLK_PTR                    (  (reg8 *) Mixer_1_SC__CLK )
/* SC Boost clk Control */
#define Mixer_1_BSTCLK_REG                 (* (reg8 *) Mixer_1_SC__BST )  
#define Mixer_1_BSTCLK_PTR                 (  (reg8 *) Mixer_1_SC__BST )
/* Power manager */
#define Mixer_1_PM_ACT_CFG_REG             (* (reg8 *) Mixer_1_SC__PM_ACT_CFG )  
#define Mixer_1_PM_ACT_CFG_PTR             (  (reg8 *) Mixer_1_SC__PM_ACT_CFG )
/* Power manager */
#define Mixer_1_PM_STBY_CFG_REG            (* (reg8 *) Mixer_1_SC__PM_STBY_CFG )  
#define Mixer_1_PM_STBY_CFG_PTR            (  (reg8 *) Mixer_1_SC__PM_STBY_CFG )
/* Pump clock selectin register */
#define Mixer_1_PUMP_CR1_REG               (* (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)   
#define Mixer_1_PUMP_CR1_PTR               (  (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)
/* Pump Register for SC block */
#define Mixer_1_SC_MISC_REG                (* (reg8 *) CYDEV_ANAIF_RT_SC_MISC)
#define Mixer_1_SC_MISC_PTR                (  (reg8 *) CYDEV_ANAIF_RT_SC_MISC)
/* PM_ACT_CFG (Active Power Mode CFG Register) mask */ 
/* Power enable mask */
#define Mixer_1_ACT_PWR_EN                 Mixer_1_SC__PM_ACT_MSK 
/* PM_STBY_CFG (Alternative Active Power Mode CFG Register) mask */ 
/* Power enable mask */
#define Mixer_1_STBY_PWR_EN                Mixer_1_SC__PM_STBY_MSK 
/* LO oscillator clock register defines */
#if(Mixer_1_LO_SOURCE == Mixer_1_LO_SOURCE_INTERNAL)
    #define Mixer_1_PWRMGR_ACLK_REG        (* (reg8 *) Mixer_1_aclk__PM_ACT_CFG ) 
    #define Mixer_1_PWRMGR_ACLK_PTR        (  (reg8 *) Mixer_1_aclk__PM_ACT_CFG ) 

    #define Mixer_1_STBY_PWRMGR_ACLK_REG   (* (reg8 *) Mixer_1_aclk__PM_STBY_CFG )
    #define Mixer_1_STBY_PWRMGR_ACLK_PTR   (  (reg8 *) Mixer_1_aclk__PM_STBY_CFG )
#endif /* Mixer_1_LO_SOURCE == Mixer_1_LO_SOURCE_INTERNAL */

/* Below defines are to retain backward compatibility. These are depricated 
   and should not be used in future designs */

/* SC Block Configuration Registers */
#define Mixer_1_CR0                        Mixer_1_CR0_REG
#define Mixer_1_CR1                        Mixer_1_CR1_REG
#define Mixer_1_CR2                        Mixer_1_CR2_REG
/* SC Block clk control */
#define Mixer_1_CLK                        Mixer_1_CLK_REG     
/* SC Boost clk Control */
#define Mixer_1_BSTCLK                     Mixer_1_BSTCLK_REG  
/* Power manager */
#define Mixer_1_PM_ACT_CFG                 Mixer_1_PM_ACT_CFG_REG  
/* Power manager */
#define Mixer_1_PM_STBY_CFG                Mixer_1_PM_STBY_CFG_REG  
/* Pump clock selectin register */
#define Mixer_1_PUMP_CR1                   Mixer_1_PUMP_CR1_REG   
/* Pump Register for SC block */
#define Mixer_1_SC_MISC                    Mixer_1_SC_MISC_REG


/***************************************
*        Register Constants        
***************************************/

/* ANIF.PUMP.CR1 Constants */
#define Mixer_1_PUMP_CR1_SC_CLKSEL         (0x80u)

/* SC_MISC constants */
#define Mixer_1_PUMP_FORCE                 (0x20u)
#define Mixer_1_PUMP_AUTO                  (0x10u)
#define Mixer_1_DIFF_PGA_1_3               (0x02u)
#define Mixer_1_DIFF_PGA_0_2               (0x01u)

/* CR0 SC Block Control Register 0 definitions */

/* Bit Field SC_MODE_ENUM - SCxx_CR0[3:1] */
#define Mixer_1_MODE_CTMIXER               ((uint8)(0x02u << 1))
#define Mixer_1_MODE_DTMIXER               ((uint8)(0x03u << 1))

/* CR1 SC Block Control Register 1 definitions */

/* Bit Field  SC_DRIVE_ENUM - SCxx_CR1[1:0] */
#define Mixer_1_DRIVE_MASK                 (0x03u)
#define Mixer_1_DRIVE_280UA                (0x00u)
#define Mixer_1_DRIVE_420UA                (0x01u)
#define Mixer_1_DRIVE_530UA                (0x02u)
#define Mixer_1_DRIVE_650UA                (0x03u)

/* Bit Field  SC_COMP_ENUM - SCxx_CR1[3:2] */
#define Mixer_1_COMP_MASK                  ((uint8)(0x03u << 2))
#define Mixer_1_COMP_3P0PF                 ((uint8)(0x00u << 2))
#define Mixer_1_COMP_3P6PF                 ((uint8)(0x01u << 2))
#define Mixer_1_COMP_4P35PF                ((uint8)(0x02u << 2))
#define Mixer_1_COMP_5P1PF                 ((uint8)(0x03u << 2))

/* Bit Field  SC_DIV2_ENUM - SCxx_CR1[4] */
#define Mixer_1_DIV2                       ((uint8)(0x01u << 4))
#define Mixer_1_DIV2_DISABLE               ((uint8)(0x00u << 4))
#define Mixer_1_DIV2_ENABLE                ((uint8)(0x01u << 4))

/* Bit Field  SC_GAIN_ENUM - SCxx_CR1[5] */
#define Mixer_1_GAIN                       ((uint8)(0x01u << 5))
#define Mixer_1_GAIN_0DB                   ((uint8)(0x00u << 5))
#define Mixer_1_GAIN_6DB                   ((uint8)(0x01u << 5))

/* CR2 SC Block Control Register 2 definitions */

/* Bit Field  SC_BIAS_CONTROL_ENUM - SCxx_CR2[0] */
#define Mixer_1_BIAS                       (0x01u)
#define Mixer_1_BIAS_NORMAL                (0x00u)
#define Mixer_1_BIAS_LOW                   (0x01u)

/* Bit Field  SC_R20_40B_ENUM - SCxx_CR2[1] */
#define Mixer_1_R20_40B                    ((uint8)(0x01u << 1))
#define Mixer_1_R20_40B_40K                ((uint8)(0x00u << 1))
#define Mixer_1_R20_40B_20K                ((uint8)(0x01u << 1))

/* Bit Field  SC_REDC_ENUM  - SCxx_CR2[3:2] */
#define Mixer_1_REDC_MASK                  ((uint8)(0x03u << 2))
#define Mixer_1_REDC_00                    ((uint8)(0x00u << 2))
#define Mixer_1_REDC_01                    ((uint8)(0x01u << 2))
#define Mixer_1_REDC_10                    ((uint8)(0x02u << 2))
#define Mixer_1_REDC_11                    ((uint8)(0x03u << 2))

/* Bit Field  SC_RVAL_ENUM  - SCxx_CR2[6:4] */
#define Mixer_1_RVAL_MASK                  ((uint8)(0x07u << 4))
#define Mixer_1_RVAL_20K                   ((uint8)(0x00u << 4))
#define Mixer_1_RVAL_30K                   ((uint8)(0x01u << 4))
#define Mixer_1_RVAL_40K                   ((uint8)(0x02u << 4))
#define Mixer_1_RVAL_80K                   ((uint8)(0x03u << 4))
#define Mixer_1_RVAL_120K                  ((uint8)(0x04u << 4))
#define Mixer_1_RVAL_250K                  ((uint8)(0x05u << 4))
#define Mixer_1_RVAL_500K                  ((uint8)(0x06u << 4))
#define Mixer_1_RVAL_1000K                 ((uint8)(0x07u << 4))

/* Bit Field  SC_PGA_GNDVREF_ENUM - SCxx_CR2[7] */
#define Mixer_1_GNDVREF                    ((uint8)(0x01u << 7))
#define Mixer_1_GNDVREF_DI                 ((uint8)(0x00u << 7))
#define Mixer_1_GNDVREF_E                  ((uint8)(0x01u << 7)) 

/* SC Block Clock Control SCx.CLk */
#define Mixer_1_DYN_CNTL_EN                ((uint8)(0x01u << 5))
#define Mixer_1_BYPASS_SYNC                ((uint8)(0x01u << 4))
#define Mixer_1_CLK_EN                     ((uint8)(0x01u << 3))

/* SC Block Boost Clock Selection Register - Boost Clock Enable  SCxx_BST[3]  */
#define Mixer_1_BST_CLK_EN                 ((uint8)(0x01u << 3))
#define Mixer_1_BST_CLK_INDEX_MASK         (0x07u)
#define Mixer_1_PM_ACT_CFG_MASK            (0x0Fu)

/* internal LO oscillator Clock enable */
#if(Mixer_1_LO_SOURCE == Mixer_1_LO_SOURCE_INTERNAL)
    #define Mixer_1_ACT_PWR_ACLK_EN        Mixer_1_aclk__PM_ACT_MSK 
    #define Mixer_1_STBY_PWR_ACLK_EN       Mixer_1_aclk__PM_STBY_MSK 
#endif /* Mixer_1_LO_SOURCE == Mixer_1_LO_SOURCE_INTERNAL */

/* Constant for VDDA Threshold */
#define Mixer_1_CYDEV_VDDA_MV              (CYDEV_VDDA_MV)
#define Mixer_1_MINIMUM_VDDA_THRESHOLD_MV  (2700u)

#endif /* CY_MIXER_Mixer_1_H */


/* [] END OF FILE */
