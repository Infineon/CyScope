/*******************************************************************************
* File Name: Modulator_Analog_Out_P3_4.h  
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

#if !defined(CY_PINS_Modulator_Analog_Out_P3_4_H) /* Pins Modulator_Analog_Out_P3_4_H */
#define CY_PINS_Modulator_Analog_Out_P3_4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Modulator_Analog_Out_P3_4_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Modulator_Analog_Out_P3_4__PORT == 15 && ((Modulator_Analog_Out_P3_4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Modulator_Analog_Out_P3_4_Write(uint8 value) ;
void    Modulator_Analog_Out_P3_4_SetDriveMode(uint8 mode) ;
uint8   Modulator_Analog_Out_P3_4_ReadDataReg(void) ;
uint8   Modulator_Analog_Out_P3_4_Read(void) ;
uint8   Modulator_Analog_Out_P3_4_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Modulator_Analog_Out_P3_4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Modulator_Analog_Out_P3_4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Modulator_Analog_Out_P3_4_DM_RES_UP          PIN_DM_RES_UP
#define Modulator_Analog_Out_P3_4_DM_RES_DWN         PIN_DM_RES_DWN
#define Modulator_Analog_Out_P3_4_DM_OD_LO           PIN_DM_OD_LO
#define Modulator_Analog_Out_P3_4_DM_OD_HI           PIN_DM_OD_HI
#define Modulator_Analog_Out_P3_4_DM_STRONG          PIN_DM_STRONG
#define Modulator_Analog_Out_P3_4_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Modulator_Analog_Out_P3_4_MASK               Modulator_Analog_Out_P3_4__MASK
#define Modulator_Analog_Out_P3_4_SHIFT              Modulator_Analog_Out_P3_4__SHIFT
#define Modulator_Analog_Out_P3_4_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Modulator_Analog_Out_P3_4_PS                     (* (reg8 *) Modulator_Analog_Out_P3_4__PS)
/* Data Register */
#define Modulator_Analog_Out_P3_4_DR                     (* (reg8 *) Modulator_Analog_Out_P3_4__DR)
/* Port Number */
#define Modulator_Analog_Out_P3_4_PRT_NUM                (* (reg8 *) Modulator_Analog_Out_P3_4__PRT) 
/* Connect to Analog Globals */                                                  
#define Modulator_Analog_Out_P3_4_AG                     (* (reg8 *) Modulator_Analog_Out_P3_4__AG)                       
/* Analog MUX bux enable */
#define Modulator_Analog_Out_P3_4_AMUX                   (* (reg8 *) Modulator_Analog_Out_P3_4__AMUX) 
/* Bidirectional Enable */                                                        
#define Modulator_Analog_Out_P3_4_BIE                    (* (reg8 *) Modulator_Analog_Out_P3_4__BIE)
/* Bit-mask for Aliased Register Access */
#define Modulator_Analog_Out_P3_4_BIT_MASK               (* (reg8 *) Modulator_Analog_Out_P3_4__BIT_MASK)
/* Bypass Enable */
#define Modulator_Analog_Out_P3_4_BYP                    (* (reg8 *) Modulator_Analog_Out_P3_4__BYP)
/* Port wide control signals */                                                   
#define Modulator_Analog_Out_P3_4_CTL                    (* (reg8 *) Modulator_Analog_Out_P3_4__CTL)
/* Drive Modes */
#define Modulator_Analog_Out_P3_4_DM0                    (* (reg8 *) Modulator_Analog_Out_P3_4__DM0) 
#define Modulator_Analog_Out_P3_4_DM1                    (* (reg8 *) Modulator_Analog_Out_P3_4__DM1)
#define Modulator_Analog_Out_P3_4_DM2                    (* (reg8 *) Modulator_Analog_Out_P3_4__DM2) 
/* Input Buffer Disable Override */
#define Modulator_Analog_Out_P3_4_INP_DIS                (* (reg8 *) Modulator_Analog_Out_P3_4__INP_DIS)
/* LCD Common or Segment Drive */
#define Modulator_Analog_Out_P3_4_LCD_COM_SEG            (* (reg8 *) Modulator_Analog_Out_P3_4__LCD_COM_SEG)
/* Enable Segment LCD */
#define Modulator_Analog_Out_P3_4_LCD_EN                 (* (reg8 *) Modulator_Analog_Out_P3_4__LCD_EN)
/* Slew Rate Control */
#define Modulator_Analog_Out_P3_4_SLW                    (* (reg8 *) Modulator_Analog_Out_P3_4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Modulator_Analog_Out_P3_4_PRTDSI__CAPS_SEL       (* (reg8 *) Modulator_Analog_Out_P3_4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Modulator_Analog_Out_P3_4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Modulator_Analog_Out_P3_4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Modulator_Analog_Out_P3_4_PRTDSI__OE_SEL0        (* (reg8 *) Modulator_Analog_Out_P3_4__PRTDSI__OE_SEL0) 
#define Modulator_Analog_Out_P3_4_PRTDSI__OE_SEL1        (* (reg8 *) Modulator_Analog_Out_P3_4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Modulator_Analog_Out_P3_4_PRTDSI__OUT_SEL0       (* (reg8 *) Modulator_Analog_Out_P3_4__PRTDSI__OUT_SEL0) 
#define Modulator_Analog_Out_P3_4_PRTDSI__OUT_SEL1       (* (reg8 *) Modulator_Analog_Out_P3_4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Modulator_Analog_Out_P3_4_PRTDSI__SYNC_OUT       (* (reg8 *) Modulator_Analog_Out_P3_4__PRTDSI__SYNC_OUT) 


#if defined(Modulator_Analog_Out_P3_4__INTSTAT)  /* Interrupt Registers */

    #define Modulator_Analog_Out_P3_4_INTSTAT                (* (reg8 *) Modulator_Analog_Out_P3_4__INTSTAT)
    #define Modulator_Analog_Out_P3_4_SNAP                   (* (reg8 *) Modulator_Analog_Out_P3_4__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Modulator_Analog_Out_P3_4_H */


/* [] END OF FILE */
