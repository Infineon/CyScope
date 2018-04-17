/*******************************************************************************
* File Name: External_Trigger.h  
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

#if !defined(CY_PINS_External_Trigger_H) /* Pins External_Trigger_H */
#define CY_PINS_External_Trigger_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "External_Trigger_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 External_Trigger__PORT == 15 && ((External_Trigger__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    External_Trigger_Write(uint8 value);
void    External_Trigger_SetDriveMode(uint8 mode);
uint8   External_Trigger_ReadDataReg(void);
uint8   External_Trigger_Read(void);
void    External_Trigger_SetInterruptMode(uint16 position, uint16 mode);
uint8   External_Trigger_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the External_Trigger_SetDriveMode() function.
     *  @{
     */
        #define External_Trigger_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define External_Trigger_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define External_Trigger_DM_RES_UP          PIN_DM_RES_UP
        #define External_Trigger_DM_RES_DWN         PIN_DM_RES_DWN
        #define External_Trigger_DM_OD_LO           PIN_DM_OD_LO
        #define External_Trigger_DM_OD_HI           PIN_DM_OD_HI
        #define External_Trigger_DM_STRONG          PIN_DM_STRONG
        #define External_Trigger_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define External_Trigger_MASK               External_Trigger__MASK
#define External_Trigger_SHIFT              External_Trigger__SHIFT
#define External_Trigger_WIDTH              1u

/* Interrupt constants */
#if defined(External_Trigger__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in External_Trigger_SetInterruptMode() function.
     *  @{
     */
        #define External_Trigger_INTR_NONE      (uint16)(0x0000u)
        #define External_Trigger_INTR_RISING    (uint16)(0x0001u)
        #define External_Trigger_INTR_FALLING   (uint16)(0x0002u)
        #define External_Trigger_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define External_Trigger_INTR_MASK      (0x01u) 
#endif /* (External_Trigger__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define External_Trigger_PS                     (* (reg8 *) External_Trigger__PS)
/* Data Register */
#define External_Trigger_DR                     (* (reg8 *) External_Trigger__DR)
/* Port Number */
#define External_Trigger_PRT_NUM                (* (reg8 *) External_Trigger__PRT) 
/* Connect to Analog Globals */                                                  
#define External_Trigger_AG                     (* (reg8 *) External_Trigger__AG)                       
/* Analog MUX bux enable */
#define External_Trigger_AMUX                   (* (reg8 *) External_Trigger__AMUX) 
/* Bidirectional Enable */                                                        
#define External_Trigger_BIE                    (* (reg8 *) External_Trigger__BIE)
/* Bit-mask for Aliased Register Access */
#define External_Trigger_BIT_MASK               (* (reg8 *) External_Trigger__BIT_MASK)
/* Bypass Enable */
#define External_Trigger_BYP                    (* (reg8 *) External_Trigger__BYP)
/* Port wide control signals */                                                   
#define External_Trigger_CTL                    (* (reg8 *) External_Trigger__CTL)
/* Drive Modes */
#define External_Trigger_DM0                    (* (reg8 *) External_Trigger__DM0) 
#define External_Trigger_DM1                    (* (reg8 *) External_Trigger__DM1)
#define External_Trigger_DM2                    (* (reg8 *) External_Trigger__DM2) 
/* Input Buffer Disable Override */
#define External_Trigger_INP_DIS                (* (reg8 *) External_Trigger__INP_DIS)
/* LCD Common or Segment Drive */
#define External_Trigger_LCD_COM_SEG            (* (reg8 *) External_Trigger__LCD_COM_SEG)
/* Enable Segment LCD */
#define External_Trigger_LCD_EN                 (* (reg8 *) External_Trigger__LCD_EN)
/* Slew Rate Control */
#define External_Trigger_SLW                    (* (reg8 *) External_Trigger__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define External_Trigger_PRTDSI__CAPS_SEL       (* (reg8 *) External_Trigger__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define External_Trigger_PRTDSI__DBL_SYNC_IN    (* (reg8 *) External_Trigger__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define External_Trigger_PRTDSI__OE_SEL0        (* (reg8 *) External_Trigger__PRTDSI__OE_SEL0) 
#define External_Trigger_PRTDSI__OE_SEL1        (* (reg8 *) External_Trigger__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define External_Trigger_PRTDSI__OUT_SEL0       (* (reg8 *) External_Trigger__PRTDSI__OUT_SEL0) 
#define External_Trigger_PRTDSI__OUT_SEL1       (* (reg8 *) External_Trigger__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define External_Trigger_PRTDSI__SYNC_OUT       (* (reg8 *) External_Trigger__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(External_Trigger__SIO_CFG)
    #define External_Trigger_SIO_HYST_EN        (* (reg8 *) External_Trigger__SIO_HYST_EN)
    #define External_Trigger_SIO_REG_HIFREQ     (* (reg8 *) External_Trigger__SIO_REG_HIFREQ)
    #define External_Trigger_SIO_CFG            (* (reg8 *) External_Trigger__SIO_CFG)
    #define External_Trigger_SIO_DIFF           (* (reg8 *) External_Trigger__SIO_DIFF)
#endif /* (External_Trigger__SIO_CFG) */

/* Interrupt Registers */
#if defined(External_Trigger__INTSTAT)
    #define External_Trigger_INTSTAT            (* (reg8 *) External_Trigger__INTSTAT)
    #define External_Trigger_SNAP               (* (reg8 *) External_Trigger__SNAP)
    
	#define External_Trigger_0_INTTYPE_REG 		(* (reg8 *) External_Trigger__0__INTTYPE)
#endif /* (External_Trigger__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_External_Trigger_H */


/* [] END OF FILE */
