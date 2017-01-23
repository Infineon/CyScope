/*******************************************************************************
* File Name: P5_4_TriggerLut_Block.h  
* Version 1.90
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_P5_4_TriggerLut_Block_H) /* Pins P5_4_TriggerLut_Block_H */
#define CY_PINS_P5_4_TriggerLut_Block_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "P5_4_TriggerLut_Block_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 P5_4_TriggerLut_Block__PORT == 15 && ((P5_4_TriggerLut_Block__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    P5_4_TriggerLut_Block_Write(uint8 value) ;
void    P5_4_TriggerLut_Block_SetDriveMode(uint8 mode) ;
uint8   P5_4_TriggerLut_Block_ReadDataReg(void) ;
uint8   P5_4_TriggerLut_Block_Read(void) ;
uint8   P5_4_TriggerLut_Block_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define P5_4_TriggerLut_Block_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define P5_4_TriggerLut_Block_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define P5_4_TriggerLut_Block_DM_RES_UP          PIN_DM_RES_UP
#define P5_4_TriggerLut_Block_DM_RES_DWN         PIN_DM_RES_DWN
#define P5_4_TriggerLut_Block_DM_OD_LO           PIN_DM_OD_LO
#define P5_4_TriggerLut_Block_DM_OD_HI           PIN_DM_OD_HI
#define P5_4_TriggerLut_Block_DM_STRONG          PIN_DM_STRONG
#define P5_4_TriggerLut_Block_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define P5_4_TriggerLut_Block_MASK               P5_4_TriggerLut_Block__MASK
#define P5_4_TriggerLut_Block_SHIFT              P5_4_TriggerLut_Block__SHIFT
#define P5_4_TriggerLut_Block_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define P5_4_TriggerLut_Block_PS                     (* (reg8 *) P5_4_TriggerLut_Block__PS)
/* Data Register */
#define P5_4_TriggerLut_Block_DR                     (* (reg8 *) P5_4_TriggerLut_Block__DR)
/* Port Number */
#define P5_4_TriggerLut_Block_PRT_NUM                (* (reg8 *) P5_4_TriggerLut_Block__PRT) 
/* Connect to Analog Globals */                                                  
#define P5_4_TriggerLut_Block_AG                     (* (reg8 *) P5_4_TriggerLut_Block__AG)                       
/* Analog MUX bux enable */
#define P5_4_TriggerLut_Block_AMUX                   (* (reg8 *) P5_4_TriggerLut_Block__AMUX) 
/* Bidirectional Enable */                                                        
#define P5_4_TriggerLut_Block_BIE                    (* (reg8 *) P5_4_TriggerLut_Block__BIE)
/* Bit-mask for Aliased Register Access */
#define P5_4_TriggerLut_Block_BIT_MASK               (* (reg8 *) P5_4_TriggerLut_Block__BIT_MASK)
/* Bypass Enable */
#define P5_4_TriggerLut_Block_BYP                    (* (reg8 *) P5_4_TriggerLut_Block__BYP)
/* Port wide control signals */                                                   
#define P5_4_TriggerLut_Block_CTL                    (* (reg8 *) P5_4_TriggerLut_Block__CTL)
/* Drive Modes */
#define P5_4_TriggerLut_Block_DM0                    (* (reg8 *) P5_4_TriggerLut_Block__DM0) 
#define P5_4_TriggerLut_Block_DM1                    (* (reg8 *) P5_4_TriggerLut_Block__DM1)
#define P5_4_TriggerLut_Block_DM2                    (* (reg8 *) P5_4_TriggerLut_Block__DM2) 
/* Input Buffer Disable Override */
#define P5_4_TriggerLut_Block_INP_DIS                (* (reg8 *) P5_4_TriggerLut_Block__INP_DIS)
/* LCD Common or Segment Drive */
#define P5_4_TriggerLut_Block_LCD_COM_SEG            (* (reg8 *) P5_4_TriggerLut_Block__LCD_COM_SEG)
/* Enable Segment LCD */
#define P5_4_TriggerLut_Block_LCD_EN                 (* (reg8 *) P5_4_TriggerLut_Block__LCD_EN)
/* Slew Rate Control */
#define P5_4_TriggerLut_Block_SLW                    (* (reg8 *) P5_4_TriggerLut_Block__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define P5_4_TriggerLut_Block_PRTDSI__CAPS_SEL       (* (reg8 *) P5_4_TriggerLut_Block__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define P5_4_TriggerLut_Block_PRTDSI__DBL_SYNC_IN    (* (reg8 *) P5_4_TriggerLut_Block__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define P5_4_TriggerLut_Block_PRTDSI__OE_SEL0        (* (reg8 *) P5_4_TriggerLut_Block__PRTDSI__OE_SEL0) 
#define P5_4_TriggerLut_Block_PRTDSI__OE_SEL1        (* (reg8 *) P5_4_TriggerLut_Block__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define P5_4_TriggerLut_Block_PRTDSI__OUT_SEL0       (* (reg8 *) P5_4_TriggerLut_Block__PRTDSI__OUT_SEL0) 
#define P5_4_TriggerLut_Block_PRTDSI__OUT_SEL1       (* (reg8 *) P5_4_TriggerLut_Block__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define P5_4_TriggerLut_Block_PRTDSI__SYNC_OUT       (* (reg8 *) P5_4_TriggerLut_Block__PRTDSI__SYNC_OUT) 


#if defined(P5_4_TriggerLut_Block__INTSTAT)  /* Interrupt Registers */

    #define P5_4_TriggerLut_Block_INTSTAT                (* (reg8 *) P5_4_TriggerLut_Block__INTSTAT)
    #define P5_4_TriggerLut_Block_SNAP                   (* (reg8 *) P5_4_TriggerLut_Block__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_P5_4_TriggerLut_Block_H */


/* [] END OF FILE */
