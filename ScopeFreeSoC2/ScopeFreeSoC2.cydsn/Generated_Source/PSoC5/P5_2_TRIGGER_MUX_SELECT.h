/*******************************************************************************
* File Name: P5_2_TRIGGER_MUX_SELECT.h  
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

#if !defined(CY_PINS_P5_2_TRIGGER_MUX_SELECT_H) /* Pins P5_2_TRIGGER_MUX_SELECT_H */
#define CY_PINS_P5_2_TRIGGER_MUX_SELECT_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "P5_2_TRIGGER_MUX_SELECT_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 P5_2_TRIGGER_MUX_SELECT__PORT == 15 && ((P5_2_TRIGGER_MUX_SELECT__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    P5_2_TRIGGER_MUX_SELECT_Write(uint8 value) ;
void    P5_2_TRIGGER_MUX_SELECT_SetDriveMode(uint8 mode) ;
uint8   P5_2_TRIGGER_MUX_SELECT_ReadDataReg(void) ;
uint8   P5_2_TRIGGER_MUX_SELECT_Read(void) ;
uint8   P5_2_TRIGGER_MUX_SELECT_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define P5_2_TRIGGER_MUX_SELECT_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define P5_2_TRIGGER_MUX_SELECT_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define P5_2_TRIGGER_MUX_SELECT_DM_RES_UP          PIN_DM_RES_UP
#define P5_2_TRIGGER_MUX_SELECT_DM_RES_DWN         PIN_DM_RES_DWN
#define P5_2_TRIGGER_MUX_SELECT_DM_OD_LO           PIN_DM_OD_LO
#define P5_2_TRIGGER_MUX_SELECT_DM_OD_HI           PIN_DM_OD_HI
#define P5_2_TRIGGER_MUX_SELECT_DM_STRONG          PIN_DM_STRONG
#define P5_2_TRIGGER_MUX_SELECT_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define P5_2_TRIGGER_MUX_SELECT_MASK               P5_2_TRIGGER_MUX_SELECT__MASK
#define P5_2_TRIGGER_MUX_SELECT_SHIFT              P5_2_TRIGGER_MUX_SELECT__SHIFT
#define P5_2_TRIGGER_MUX_SELECT_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define P5_2_TRIGGER_MUX_SELECT_PS                     (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__PS)
/* Data Register */
#define P5_2_TRIGGER_MUX_SELECT_DR                     (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__DR)
/* Port Number */
#define P5_2_TRIGGER_MUX_SELECT_PRT_NUM                (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__PRT) 
/* Connect to Analog Globals */                                                  
#define P5_2_TRIGGER_MUX_SELECT_AG                     (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__AG)                       
/* Analog MUX bux enable */
#define P5_2_TRIGGER_MUX_SELECT_AMUX                   (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__AMUX) 
/* Bidirectional Enable */                                                        
#define P5_2_TRIGGER_MUX_SELECT_BIE                    (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__BIE)
/* Bit-mask for Aliased Register Access */
#define P5_2_TRIGGER_MUX_SELECT_BIT_MASK               (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__BIT_MASK)
/* Bypass Enable */
#define P5_2_TRIGGER_MUX_SELECT_BYP                    (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__BYP)
/* Port wide control signals */                                                   
#define P5_2_TRIGGER_MUX_SELECT_CTL                    (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__CTL)
/* Drive Modes */
#define P5_2_TRIGGER_MUX_SELECT_DM0                    (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__DM0) 
#define P5_2_TRIGGER_MUX_SELECT_DM1                    (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__DM1)
#define P5_2_TRIGGER_MUX_SELECT_DM2                    (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__DM2) 
/* Input Buffer Disable Override */
#define P5_2_TRIGGER_MUX_SELECT_INP_DIS                (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__INP_DIS)
/* LCD Common or Segment Drive */
#define P5_2_TRIGGER_MUX_SELECT_LCD_COM_SEG            (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__LCD_COM_SEG)
/* Enable Segment LCD */
#define P5_2_TRIGGER_MUX_SELECT_LCD_EN                 (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__LCD_EN)
/* Slew Rate Control */
#define P5_2_TRIGGER_MUX_SELECT_SLW                    (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define P5_2_TRIGGER_MUX_SELECT_PRTDSI__CAPS_SEL       (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define P5_2_TRIGGER_MUX_SELECT_PRTDSI__DBL_SYNC_IN    (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define P5_2_TRIGGER_MUX_SELECT_PRTDSI__OE_SEL0        (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__PRTDSI__OE_SEL0) 
#define P5_2_TRIGGER_MUX_SELECT_PRTDSI__OE_SEL1        (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define P5_2_TRIGGER_MUX_SELECT_PRTDSI__OUT_SEL0       (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__PRTDSI__OUT_SEL0) 
#define P5_2_TRIGGER_MUX_SELECT_PRTDSI__OUT_SEL1       (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define P5_2_TRIGGER_MUX_SELECT_PRTDSI__SYNC_OUT       (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__PRTDSI__SYNC_OUT) 


#if defined(P5_2_TRIGGER_MUX_SELECT__INTSTAT)  /* Interrupt Registers */

    #define P5_2_TRIGGER_MUX_SELECT_INTSTAT                (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__INTSTAT)
    #define P5_2_TRIGGER_MUX_SELECT_SNAP                   (* (reg8 *) P5_2_TRIGGER_MUX_SELECT__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_P5_2_TRIGGER_MUX_SELECT_H */


/* [] END OF FILE */
