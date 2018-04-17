/*******************************************************************************
* File Name: Scope_A_Gnd.h  
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

#if !defined(CY_PINS_Scope_A_Gnd_H) /* Pins Scope_A_Gnd_H */
#define CY_PINS_Scope_A_Gnd_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Scope_A_Gnd_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Scope_A_Gnd__PORT == 15 && ((Scope_A_Gnd__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Scope_A_Gnd_Write(uint8 value);
void    Scope_A_Gnd_SetDriveMode(uint8 mode);
uint8   Scope_A_Gnd_ReadDataReg(void);
uint8   Scope_A_Gnd_Read(void);
void    Scope_A_Gnd_SetInterruptMode(uint16 position, uint16 mode);
uint8   Scope_A_Gnd_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Scope_A_Gnd_SetDriveMode() function.
     *  @{
     */
        #define Scope_A_Gnd_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Scope_A_Gnd_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Scope_A_Gnd_DM_RES_UP          PIN_DM_RES_UP
        #define Scope_A_Gnd_DM_RES_DWN         PIN_DM_RES_DWN
        #define Scope_A_Gnd_DM_OD_LO           PIN_DM_OD_LO
        #define Scope_A_Gnd_DM_OD_HI           PIN_DM_OD_HI
        #define Scope_A_Gnd_DM_STRONG          PIN_DM_STRONG
        #define Scope_A_Gnd_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Scope_A_Gnd_MASK               Scope_A_Gnd__MASK
#define Scope_A_Gnd_SHIFT              Scope_A_Gnd__SHIFT
#define Scope_A_Gnd_WIDTH              1u

/* Interrupt constants */
#if defined(Scope_A_Gnd__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Scope_A_Gnd_SetInterruptMode() function.
     *  @{
     */
        #define Scope_A_Gnd_INTR_NONE      (uint16)(0x0000u)
        #define Scope_A_Gnd_INTR_RISING    (uint16)(0x0001u)
        #define Scope_A_Gnd_INTR_FALLING   (uint16)(0x0002u)
        #define Scope_A_Gnd_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Scope_A_Gnd_INTR_MASK      (0x01u) 
#endif /* (Scope_A_Gnd__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Scope_A_Gnd_PS                     (* (reg8 *) Scope_A_Gnd__PS)
/* Data Register */
#define Scope_A_Gnd_DR                     (* (reg8 *) Scope_A_Gnd__DR)
/* Port Number */
#define Scope_A_Gnd_PRT_NUM                (* (reg8 *) Scope_A_Gnd__PRT) 
/* Connect to Analog Globals */                                                  
#define Scope_A_Gnd_AG                     (* (reg8 *) Scope_A_Gnd__AG)                       
/* Analog MUX bux enable */
#define Scope_A_Gnd_AMUX                   (* (reg8 *) Scope_A_Gnd__AMUX) 
/* Bidirectional Enable */                                                        
#define Scope_A_Gnd_BIE                    (* (reg8 *) Scope_A_Gnd__BIE)
/* Bit-mask for Aliased Register Access */
#define Scope_A_Gnd_BIT_MASK               (* (reg8 *) Scope_A_Gnd__BIT_MASK)
/* Bypass Enable */
#define Scope_A_Gnd_BYP                    (* (reg8 *) Scope_A_Gnd__BYP)
/* Port wide control signals */                                                   
#define Scope_A_Gnd_CTL                    (* (reg8 *) Scope_A_Gnd__CTL)
/* Drive Modes */
#define Scope_A_Gnd_DM0                    (* (reg8 *) Scope_A_Gnd__DM0) 
#define Scope_A_Gnd_DM1                    (* (reg8 *) Scope_A_Gnd__DM1)
#define Scope_A_Gnd_DM2                    (* (reg8 *) Scope_A_Gnd__DM2) 
/* Input Buffer Disable Override */
#define Scope_A_Gnd_INP_DIS                (* (reg8 *) Scope_A_Gnd__INP_DIS)
/* LCD Common or Segment Drive */
#define Scope_A_Gnd_LCD_COM_SEG            (* (reg8 *) Scope_A_Gnd__LCD_COM_SEG)
/* Enable Segment LCD */
#define Scope_A_Gnd_LCD_EN                 (* (reg8 *) Scope_A_Gnd__LCD_EN)
/* Slew Rate Control */
#define Scope_A_Gnd_SLW                    (* (reg8 *) Scope_A_Gnd__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Scope_A_Gnd_PRTDSI__CAPS_SEL       (* (reg8 *) Scope_A_Gnd__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Scope_A_Gnd_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Scope_A_Gnd__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Scope_A_Gnd_PRTDSI__OE_SEL0        (* (reg8 *) Scope_A_Gnd__PRTDSI__OE_SEL0) 
#define Scope_A_Gnd_PRTDSI__OE_SEL1        (* (reg8 *) Scope_A_Gnd__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Scope_A_Gnd_PRTDSI__OUT_SEL0       (* (reg8 *) Scope_A_Gnd__PRTDSI__OUT_SEL0) 
#define Scope_A_Gnd_PRTDSI__OUT_SEL1       (* (reg8 *) Scope_A_Gnd__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Scope_A_Gnd_PRTDSI__SYNC_OUT       (* (reg8 *) Scope_A_Gnd__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Scope_A_Gnd__SIO_CFG)
    #define Scope_A_Gnd_SIO_HYST_EN        (* (reg8 *) Scope_A_Gnd__SIO_HYST_EN)
    #define Scope_A_Gnd_SIO_REG_HIFREQ     (* (reg8 *) Scope_A_Gnd__SIO_REG_HIFREQ)
    #define Scope_A_Gnd_SIO_CFG            (* (reg8 *) Scope_A_Gnd__SIO_CFG)
    #define Scope_A_Gnd_SIO_DIFF           (* (reg8 *) Scope_A_Gnd__SIO_DIFF)
#endif /* (Scope_A_Gnd__SIO_CFG) */

/* Interrupt Registers */
#if defined(Scope_A_Gnd__INTSTAT)
    #define Scope_A_Gnd_INTSTAT            (* (reg8 *) Scope_A_Gnd__INTSTAT)
    #define Scope_A_Gnd_SNAP               (* (reg8 *) Scope_A_Gnd__SNAP)
    
	#define Scope_A_Gnd_0_INTTYPE_REG 		(* (reg8 *) Scope_A_Gnd__0__INTTYPE)
#endif /* (Scope_A_Gnd__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Scope_A_Gnd_H */


/* [] END OF FILE */
