/*******************************************************************************
* File Name: DigIn_6.h  
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

#if !defined(CY_PINS_DigIn_6_H) /* Pins DigIn_6_H */
#define CY_PINS_DigIn_6_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DigIn_6_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 DigIn_6__PORT == 15 && ((DigIn_6__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    DigIn_6_Write(uint8 value);
void    DigIn_6_SetDriveMode(uint8 mode);
uint8   DigIn_6_ReadDataReg(void);
uint8   DigIn_6_Read(void);
void    DigIn_6_SetInterruptMode(uint16 position, uint16 mode);
uint8   DigIn_6_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the DigIn_6_SetDriveMode() function.
     *  @{
     */
        #define DigIn_6_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define DigIn_6_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define DigIn_6_DM_RES_UP          PIN_DM_RES_UP
        #define DigIn_6_DM_RES_DWN         PIN_DM_RES_DWN
        #define DigIn_6_DM_OD_LO           PIN_DM_OD_LO
        #define DigIn_6_DM_OD_HI           PIN_DM_OD_HI
        #define DigIn_6_DM_STRONG          PIN_DM_STRONG
        #define DigIn_6_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define DigIn_6_MASK               DigIn_6__MASK
#define DigIn_6_SHIFT              DigIn_6__SHIFT
#define DigIn_6_WIDTH              1u

/* Interrupt constants */
#if defined(DigIn_6__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DigIn_6_SetInterruptMode() function.
     *  @{
     */
        #define DigIn_6_INTR_NONE      (uint16)(0x0000u)
        #define DigIn_6_INTR_RISING    (uint16)(0x0001u)
        #define DigIn_6_INTR_FALLING   (uint16)(0x0002u)
        #define DigIn_6_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define DigIn_6_INTR_MASK      (0x01u) 
#endif /* (DigIn_6__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DigIn_6_PS                     (* (reg8 *) DigIn_6__PS)
/* Data Register */
#define DigIn_6_DR                     (* (reg8 *) DigIn_6__DR)
/* Port Number */
#define DigIn_6_PRT_NUM                (* (reg8 *) DigIn_6__PRT) 
/* Connect to Analog Globals */                                                  
#define DigIn_6_AG                     (* (reg8 *) DigIn_6__AG)                       
/* Analog MUX bux enable */
#define DigIn_6_AMUX                   (* (reg8 *) DigIn_6__AMUX) 
/* Bidirectional Enable */                                                        
#define DigIn_6_BIE                    (* (reg8 *) DigIn_6__BIE)
/* Bit-mask for Aliased Register Access */
#define DigIn_6_BIT_MASK               (* (reg8 *) DigIn_6__BIT_MASK)
/* Bypass Enable */
#define DigIn_6_BYP                    (* (reg8 *) DigIn_6__BYP)
/* Port wide control signals */                                                   
#define DigIn_6_CTL                    (* (reg8 *) DigIn_6__CTL)
/* Drive Modes */
#define DigIn_6_DM0                    (* (reg8 *) DigIn_6__DM0) 
#define DigIn_6_DM1                    (* (reg8 *) DigIn_6__DM1)
#define DigIn_6_DM2                    (* (reg8 *) DigIn_6__DM2) 
/* Input Buffer Disable Override */
#define DigIn_6_INP_DIS                (* (reg8 *) DigIn_6__INP_DIS)
/* LCD Common or Segment Drive */
#define DigIn_6_LCD_COM_SEG            (* (reg8 *) DigIn_6__LCD_COM_SEG)
/* Enable Segment LCD */
#define DigIn_6_LCD_EN                 (* (reg8 *) DigIn_6__LCD_EN)
/* Slew Rate Control */
#define DigIn_6_SLW                    (* (reg8 *) DigIn_6__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DigIn_6_PRTDSI__CAPS_SEL       (* (reg8 *) DigIn_6__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DigIn_6_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DigIn_6__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DigIn_6_PRTDSI__OE_SEL0        (* (reg8 *) DigIn_6__PRTDSI__OE_SEL0) 
#define DigIn_6_PRTDSI__OE_SEL1        (* (reg8 *) DigIn_6__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DigIn_6_PRTDSI__OUT_SEL0       (* (reg8 *) DigIn_6__PRTDSI__OUT_SEL0) 
#define DigIn_6_PRTDSI__OUT_SEL1       (* (reg8 *) DigIn_6__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DigIn_6_PRTDSI__SYNC_OUT       (* (reg8 *) DigIn_6__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(DigIn_6__SIO_CFG)
    #define DigIn_6_SIO_HYST_EN        (* (reg8 *) DigIn_6__SIO_HYST_EN)
    #define DigIn_6_SIO_REG_HIFREQ     (* (reg8 *) DigIn_6__SIO_REG_HIFREQ)
    #define DigIn_6_SIO_CFG            (* (reg8 *) DigIn_6__SIO_CFG)
    #define DigIn_6_SIO_DIFF           (* (reg8 *) DigIn_6__SIO_DIFF)
#endif /* (DigIn_6__SIO_CFG) */

/* Interrupt Registers */
#if defined(DigIn_6__INTSTAT)
    #define DigIn_6_INTSTAT            (* (reg8 *) DigIn_6__INTSTAT)
    #define DigIn_6_SNAP               (* (reg8 *) DigIn_6__SNAP)
    
	#define DigIn_6_0_INTTYPE_REG 		(* (reg8 *) DigIn_6__0__INTTYPE)
#endif /* (DigIn_6__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_DigIn_6_H */


/* [] END OF FILE */
