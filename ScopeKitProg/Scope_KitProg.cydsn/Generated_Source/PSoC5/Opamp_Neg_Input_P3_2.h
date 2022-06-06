/*******************************************************************************
* File Name: Opamp_Neg_Input_P3_2.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Opamp_Neg_Input_P3_2_H) /* Pins Opamp_Neg_Input_P3_2_H */
#define CY_PINS_Opamp_Neg_Input_P3_2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Opamp_Neg_Input_P3_2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Opamp_Neg_Input_P3_2__PORT == 15 && ((Opamp_Neg_Input_P3_2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Opamp_Neg_Input_P3_2_Write(uint8 value);
void    Opamp_Neg_Input_P3_2_SetDriveMode(uint8 mode);
uint8   Opamp_Neg_Input_P3_2_ReadDataReg(void);
uint8   Opamp_Neg_Input_P3_2_Read(void);
void    Opamp_Neg_Input_P3_2_SetInterruptMode(uint16 position, uint16 mode);
uint8   Opamp_Neg_Input_P3_2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Opamp_Neg_Input_P3_2_SetDriveMode() function.
     *  @{
     */
        #define Opamp_Neg_Input_P3_2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Opamp_Neg_Input_P3_2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Opamp_Neg_Input_P3_2_DM_RES_UP          PIN_DM_RES_UP
        #define Opamp_Neg_Input_P3_2_DM_RES_DWN         PIN_DM_RES_DWN
        #define Opamp_Neg_Input_P3_2_DM_OD_LO           PIN_DM_OD_LO
        #define Opamp_Neg_Input_P3_2_DM_OD_HI           PIN_DM_OD_HI
        #define Opamp_Neg_Input_P3_2_DM_STRONG          PIN_DM_STRONG
        #define Opamp_Neg_Input_P3_2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Opamp_Neg_Input_P3_2_MASK               Opamp_Neg_Input_P3_2__MASK
#define Opamp_Neg_Input_P3_2_SHIFT              Opamp_Neg_Input_P3_2__SHIFT
#define Opamp_Neg_Input_P3_2_WIDTH              1u

/* Interrupt constants */
#if defined(Opamp_Neg_Input_P3_2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Opamp_Neg_Input_P3_2_SetInterruptMode() function.
     *  @{
     */
        #define Opamp_Neg_Input_P3_2_INTR_NONE      (uint16)(0x0000u)
        #define Opamp_Neg_Input_P3_2_INTR_RISING    (uint16)(0x0001u)
        #define Opamp_Neg_Input_P3_2_INTR_FALLING   (uint16)(0x0002u)
        #define Opamp_Neg_Input_P3_2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Opamp_Neg_Input_P3_2_INTR_MASK      (0x01u) 
#endif /* (Opamp_Neg_Input_P3_2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Opamp_Neg_Input_P3_2_PS                     (* (reg8 *) Opamp_Neg_Input_P3_2__PS)
/* Data Register */
#define Opamp_Neg_Input_P3_2_DR                     (* (reg8 *) Opamp_Neg_Input_P3_2__DR)
/* Port Number */
#define Opamp_Neg_Input_P3_2_PRT_NUM                (* (reg8 *) Opamp_Neg_Input_P3_2__PRT) 
/* Connect to Analog Globals */                                                  
#define Opamp_Neg_Input_P3_2_AG                     (* (reg8 *) Opamp_Neg_Input_P3_2__AG)                       
/* Analog MUX bux enable */
#define Opamp_Neg_Input_P3_2_AMUX                   (* (reg8 *) Opamp_Neg_Input_P3_2__AMUX) 
/* Bidirectional Enable */                                                        
#define Opamp_Neg_Input_P3_2_BIE                    (* (reg8 *) Opamp_Neg_Input_P3_2__BIE)
/* Bit-mask for Aliased Register Access */
#define Opamp_Neg_Input_P3_2_BIT_MASK               (* (reg8 *) Opamp_Neg_Input_P3_2__BIT_MASK)
/* Bypass Enable */
#define Opamp_Neg_Input_P3_2_BYP                    (* (reg8 *) Opamp_Neg_Input_P3_2__BYP)
/* Port wide control signals */                                                   
#define Opamp_Neg_Input_P3_2_CTL                    (* (reg8 *) Opamp_Neg_Input_P3_2__CTL)
/* Drive Modes */
#define Opamp_Neg_Input_P3_2_DM0                    (* (reg8 *) Opamp_Neg_Input_P3_2__DM0) 
#define Opamp_Neg_Input_P3_2_DM1                    (* (reg8 *) Opamp_Neg_Input_P3_2__DM1)
#define Opamp_Neg_Input_P3_2_DM2                    (* (reg8 *) Opamp_Neg_Input_P3_2__DM2) 
/* Input Buffer Disable Override */
#define Opamp_Neg_Input_P3_2_INP_DIS                (* (reg8 *) Opamp_Neg_Input_P3_2__INP_DIS)
/* LCD Common or Segment Drive */
#define Opamp_Neg_Input_P3_2_LCD_COM_SEG            (* (reg8 *) Opamp_Neg_Input_P3_2__LCD_COM_SEG)
/* Enable Segment LCD */
#define Opamp_Neg_Input_P3_2_LCD_EN                 (* (reg8 *) Opamp_Neg_Input_P3_2__LCD_EN)
/* Slew Rate Control */
#define Opamp_Neg_Input_P3_2_SLW                    (* (reg8 *) Opamp_Neg_Input_P3_2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Opamp_Neg_Input_P3_2_PRTDSI__CAPS_SEL       (* (reg8 *) Opamp_Neg_Input_P3_2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Opamp_Neg_Input_P3_2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Opamp_Neg_Input_P3_2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Opamp_Neg_Input_P3_2_PRTDSI__OE_SEL0        (* (reg8 *) Opamp_Neg_Input_P3_2__PRTDSI__OE_SEL0) 
#define Opamp_Neg_Input_P3_2_PRTDSI__OE_SEL1        (* (reg8 *) Opamp_Neg_Input_P3_2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Opamp_Neg_Input_P3_2_PRTDSI__OUT_SEL0       (* (reg8 *) Opamp_Neg_Input_P3_2__PRTDSI__OUT_SEL0) 
#define Opamp_Neg_Input_P3_2_PRTDSI__OUT_SEL1       (* (reg8 *) Opamp_Neg_Input_P3_2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Opamp_Neg_Input_P3_2_PRTDSI__SYNC_OUT       (* (reg8 *) Opamp_Neg_Input_P3_2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Opamp_Neg_Input_P3_2__SIO_CFG)
    #define Opamp_Neg_Input_P3_2_SIO_HYST_EN        (* (reg8 *) Opamp_Neg_Input_P3_2__SIO_HYST_EN)
    #define Opamp_Neg_Input_P3_2_SIO_REG_HIFREQ     (* (reg8 *) Opamp_Neg_Input_P3_2__SIO_REG_HIFREQ)
    #define Opamp_Neg_Input_P3_2_SIO_CFG            (* (reg8 *) Opamp_Neg_Input_P3_2__SIO_CFG)
    #define Opamp_Neg_Input_P3_2_SIO_DIFF           (* (reg8 *) Opamp_Neg_Input_P3_2__SIO_DIFF)
#endif /* (Opamp_Neg_Input_P3_2__SIO_CFG) */

/* Interrupt Registers */
#if defined(Opamp_Neg_Input_P3_2__INTSTAT)
    #define Opamp_Neg_Input_P3_2_INTSTAT            (* (reg8 *) Opamp_Neg_Input_P3_2__INTSTAT)
    #define Opamp_Neg_Input_P3_2_SNAP               (* (reg8 *) Opamp_Neg_Input_P3_2__SNAP)
    
	#define Opamp_Neg_Input_P3_2_0_INTTYPE_REG 		(* (reg8 *) Opamp_Neg_Input_P3_2__0__INTTYPE)
#endif /* (Opamp_Neg_Input_P3_2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Opamp_Neg_Input_P3_2_H */


/* [] END OF FILE */
