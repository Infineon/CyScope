/*******************************************************************************
* File Name: P2_0_SOC.h  
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

#if !defined(CY_PINS_P2_0_SOC_H) /* Pins P2_0_SOC_H */
#define CY_PINS_P2_0_SOC_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "P2_0_SOC_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 P2_0_SOC__PORT == 15 && ((P2_0_SOC__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    P2_0_SOC_Write(uint8 value) ;
void    P2_0_SOC_SetDriveMode(uint8 mode) ;
uint8   P2_0_SOC_ReadDataReg(void) ;
uint8   P2_0_SOC_Read(void) ;
uint8   P2_0_SOC_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define P2_0_SOC_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define P2_0_SOC_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define P2_0_SOC_DM_RES_UP          PIN_DM_RES_UP
#define P2_0_SOC_DM_RES_DWN         PIN_DM_RES_DWN
#define P2_0_SOC_DM_OD_LO           PIN_DM_OD_LO
#define P2_0_SOC_DM_OD_HI           PIN_DM_OD_HI
#define P2_0_SOC_DM_STRONG          PIN_DM_STRONG
#define P2_0_SOC_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define P2_0_SOC_MASK               P2_0_SOC__MASK
#define P2_0_SOC_SHIFT              P2_0_SOC__SHIFT
#define P2_0_SOC_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define P2_0_SOC_PS                     (* (reg8 *) P2_0_SOC__PS)
/* Data Register */
#define P2_0_SOC_DR                     (* (reg8 *) P2_0_SOC__DR)
/* Port Number */
#define P2_0_SOC_PRT_NUM                (* (reg8 *) P2_0_SOC__PRT) 
/* Connect to Analog Globals */                                                  
#define P2_0_SOC_AG                     (* (reg8 *) P2_0_SOC__AG)                       
/* Analog MUX bux enable */
#define P2_0_SOC_AMUX                   (* (reg8 *) P2_0_SOC__AMUX) 
/* Bidirectional Enable */                                                        
#define P2_0_SOC_BIE                    (* (reg8 *) P2_0_SOC__BIE)
/* Bit-mask for Aliased Register Access */
#define P2_0_SOC_BIT_MASK               (* (reg8 *) P2_0_SOC__BIT_MASK)
/* Bypass Enable */
#define P2_0_SOC_BYP                    (* (reg8 *) P2_0_SOC__BYP)
/* Port wide control signals */                                                   
#define P2_0_SOC_CTL                    (* (reg8 *) P2_0_SOC__CTL)
/* Drive Modes */
#define P2_0_SOC_DM0                    (* (reg8 *) P2_0_SOC__DM0) 
#define P2_0_SOC_DM1                    (* (reg8 *) P2_0_SOC__DM1)
#define P2_0_SOC_DM2                    (* (reg8 *) P2_0_SOC__DM2) 
/* Input Buffer Disable Override */
#define P2_0_SOC_INP_DIS                (* (reg8 *) P2_0_SOC__INP_DIS)
/* LCD Common or Segment Drive */
#define P2_0_SOC_LCD_COM_SEG            (* (reg8 *) P2_0_SOC__LCD_COM_SEG)
/* Enable Segment LCD */
#define P2_0_SOC_LCD_EN                 (* (reg8 *) P2_0_SOC__LCD_EN)
/* Slew Rate Control */
#define P2_0_SOC_SLW                    (* (reg8 *) P2_0_SOC__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define P2_0_SOC_PRTDSI__CAPS_SEL       (* (reg8 *) P2_0_SOC__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define P2_0_SOC_PRTDSI__DBL_SYNC_IN    (* (reg8 *) P2_0_SOC__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define P2_0_SOC_PRTDSI__OE_SEL0        (* (reg8 *) P2_0_SOC__PRTDSI__OE_SEL0) 
#define P2_0_SOC_PRTDSI__OE_SEL1        (* (reg8 *) P2_0_SOC__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define P2_0_SOC_PRTDSI__OUT_SEL0       (* (reg8 *) P2_0_SOC__PRTDSI__OUT_SEL0) 
#define P2_0_SOC_PRTDSI__OUT_SEL1       (* (reg8 *) P2_0_SOC__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define P2_0_SOC_PRTDSI__SYNC_OUT       (* (reg8 *) P2_0_SOC__PRTDSI__SYNC_OUT) 


#if defined(P2_0_SOC__INTSTAT)  /* Interrupt Registers */

    #define P2_0_SOC_INTSTAT                (* (reg8 *) P2_0_SOC__INTSTAT)
    #define P2_0_SOC_SNAP                   (* (reg8 *) P2_0_SOC__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_P2_0_SOC_H */


/* [] END OF FILE */
