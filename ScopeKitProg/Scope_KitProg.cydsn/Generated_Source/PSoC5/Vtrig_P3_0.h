/*******************************************************************************
* File Name: Vtrig_P3_0.h  
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

#if !defined(CY_PINS_Vtrig_P3_0_H) /* Pins Vtrig_P3_0_H */
#define CY_PINS_Vtrig_P3_0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Vtrig_P3_0_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Vtrig_P3_0__PORT == 15 && ((Vtrig_P3_0__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Vtrig_P3_0_Write(uint8 value) ;
void    Vtrig_P3_0_SetDriveMode(uint8 mode) ;
uint8   Vtrig_P3_0_ReadDataReg(void) ;
uint8   Vtrig_P3_0_Read(void) ;
uint8   Vtrig_P3_0_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Vtrig_P3_0_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Vtrig_P3_0_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Vtrig_P3_0_DM_RES_UP          PIN_DM_RES_UP
#define Vtrig_P3_0_DM_RES_DWN         PIN_DM_RES_DWN
#define Vtrig_P3_0_DM_OD_LO           PIN_DM_OD_LO
#define Vtrig_P3_0_DM_OD_HI           PIN_DM_OD_HI
#define Vtrig_P3_0_DM_STRONG          PIN_DM_STRONG
#define Vtrig_P3_0_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Vtrig_P3_0_MASK               Vtrig_P3_0__MASK
#define Vtrig_P3_0_SHIFT              Vtrig_P3_0__SHIFT
#define Vtrig_P3_0_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Vtrig_P3_0_PS                     (* (reg8 *) Vtrig_P3_0__PS)
/* Data Register */
#define Vtrig_P3_0_DR                     (* (reg8 *) Vtrig_P3_0__DR)
/* Port Number */
#define Vtrig_P3_0_PRT_NUM                (* (reg8 *) Vtrig_P3_0__PRT) 
/* Connect to Analog Globals */                                                  
#define Vtrig_P3_0_AG                     (* (reg8 *) Vtrig_P3_0__AG)                       
/* Analog MUX bux enable */
#define Vtrig_P3_0_AMUX                   (* (reg8 *) Vtrig_P3_0__AMUX) 
/* Bidirectional Enable */                                                        
#define Vtrig_P3_0_BIE                    (* (reg8 *) Vtrig_P3_0__BIE)
/* Bit-mask for Aliased Register Access */
#define Vtrig_P3_0_BIT_MASK               (* (reg8 *) Vtrig_P3_0__BIT_MASK)
/* Bypass Enable */
#define Vtrig_P3_0_BYP                    (* (reg8 *) Vtrig_P3_0__BYP)
/* Port wide control signals */                                                   
#define Vtrig_P3_0_CTL                    (* (reg8 *) Vtrig_P3_0__CTL)
/* Drive Modes */
#define Vtrig_P3_0_DM0                    (* (reg8 *) Vtrig_P3_0__DM0) 
#define Vtrig_P3_0_DM1                    (* (reg8 *) Vtrig_P3_0__DM1)
#define Vtrig_P3_0_DM2                    (* (reg8 *) Vtrig_P3_0__DM2) 
/* Input Buffer Disable Override */
#define Vtrig_P3_0_INP_DIS                (* (reg8 *) Vtrig_P3_0__INP_DIS)
/* LCD Common or Segment Drive */
#define Vtrig_P3_0_LCD_COM_SEG            (* (reg8 *) Vtrig_P3_0__LCD_COM_SEG)
/* Enable Segment LCD */
#define Vtrig_P3_0_LCD_EN                 (* (reg8 *) Vtrig_P3_0__LCD_EN)
/* Slew Rate Control */
#define Vtrig_P3_0_SLW                    (* (reg8 *) Vtrig_P3_0__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Vtrig_P3_0_PRTDSI__CAPS_SEL       (* (reg8 *) Vtrig_P3_0__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Vtrig_P3_0_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Vtrig_P3_0__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Vtrig_P3_0_PRTDSI__OE_SEL0        (* (reg8 *) Vtrig_P3_0__PRTDSI__OE_SEL0) 
#define Vtrig_P3_0_PRTDSI__OE_SEL1        (* (reg8 *) Vtrig_P3_0__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Vtrig_P3_0_PRTDSI__OUT_SEL0       (* (reg8 *) Vtrig_P3_0__PRTDSI__OUT_SEL0) 
#define Vtrig_P3_0_PRTDSI__OUT_SEL1       (* (reg8 *) Vtrig_P3_0__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Vtrig_P3_0_PRTDSI__SYNC_OUT       (* (reg8 *) Vtrig_P3_0__PRTDSI__SYNC_OUT) 


#if defined(Vtrig_P3_0__INTSTAT)  /* Interrupt Registers */

    #define Vtrig_P3_0_INTSTAT                (* (reg8 *) Vtrig_P3_0__INTSTAT)
    #define Vtrig_P3_0_SNAP                   (* (reg8 *) Vtrig_P3_0__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Vtrig_P3_0_H */


/* [] END OF FILE */
