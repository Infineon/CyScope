/*******************************************************************************
* File Name: DigOut_3.h  
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

#if !defined(CY_PINS_DigOut_3_H) /* Pins DigOut_3_H */
#define CY_PINS_DigOut_3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DigOut_3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 DigOut_3__PORT == 15 && ((DigOut_3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    DigOut_3_Write(uint8 value);
void    DigOut_3_SetDriveMode(uint8 mode);
uint8   DigOut_3_ReadDataReg(void);
uint8   DigOut_3_Read(void);
void    DigOut_3_SetInterruptMode(uint16 position, uint16 mode);
uint8   DigOut_3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the DigOut_3_SetDriveMode() function.
     *  @{
     */
        #define DigOut_3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define DigOut_3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define DigOut_3_DM_RES_UP          PIN_DM_RES_UP
        #define DigOut_3_DM_RES_DWN         PIN_DM_RES_DWN
        #define DigOut_3_DM_OD_LO           PIN_DM_OD_LO
        #define DigOut_3_DM_OD_HI           PIN_DM_OD_HI
        #define DigOut_3_DM_STRONG          PIN_DM_STRONG
        #define DigOut_3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define DigOut_3_MASK               DigOut_3__MASK
#define DigOut_3_SHIFT              DigOut_3__SHIFT
#define DigOut_3_WIDTH              1u

/* Interrupt constants */
#if defined(DigOut_3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DigOut_3_SetInterruptMode() function.
     *  @{
     */
        #define DigOut_3_INTR_NONE      (uint16)(0x0000u)
        #define DigOut_3_INTR_RISING    (uint16)(0x0001u)
        #define DigOut_3_INTR_FALLING   (uint16)(0x0002u)
        #define DigOut_3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define DigOut_3_INTR_MASK      (0x01u) 
#endif /* (DigOut_3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DigOut_3_PS                     (* (reg8 *) DigOut_3__PS)
/* Data Register */
#define DigOut_3_DR                     (* (reg8 *) DigOut_3__DR)
/* Port Number */
#define DigOut_3_PRT_NUM                (* (reg8 *) DigOut_3__PRT) 
/* Connect to Analog Globals */                                                  
#define DigOut_3_AG                     (* (reg8 *) DigOut_3__AG)                       
/* Analog MUX bux enable */
#define DigOut_3_AMUX                   (* (reg8 *) DigOut_3__AMUX) 
/* Bidirectional Enable */                                                        
#define DigOut_3_BIE                    (* (reg8 *) DigOut_3__BIE)
/* Bit-mask for Aliased Register Access */
#define DigOut_3_BIT_MASK               (* (reg8 *) DigOut_3__BIT_MASK)
/* Bypass Enable */
#define DigOut_3_BYP                    (* (reg8 *) DigOut_3__BYP)
/* Port wide control signals */                                                   
#define DigOut_3_CTL                    (* (reg8 *) DigOut_3__CTL)
/* Drive Modes */
#define DigOut_3_DM0                    (* (reg8 *) DigOut_3__DM0) 
#define DigOut_3_DM1                    (* (reg8 *) DigOut_3__DM1)
#define DigOut_3_DM2                    (* (reg8 *) DigOut_3__DM2) 
/* Input Buffer Disable Override */
#define DigOut_3_INP_DIS                (* (reg8 *) DigOut_3__INP_DIS)
/* LCD Common or Segment Drive */
#define DigOut_3_LCD_COM_SEG            (* (reg8 *) DigOut_3__LCD_COM_SEG)
/* Enable Segment LCD */
#define DigOut_3_LCD_EN                 (* (reg8 *) DigOut_3__LCD_EN)
/* Slew Rate Control */
#define DigOut_3_SLW                    (* (reg8 *) DigOut_3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DigOut_3_PRTDSI__CAPS_SEL       (* (reg8 *) DigOut_3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DigOut_3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DigOut_3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DigOut_3_PRTDSI__OE_SEL0        (* (reg8 *) DigOut_3__PRTDSI__OE_SEL0) 
#define DigOut_3_PRTDSI__OE_SEL1        (* (reg8 *) DigOut_3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DigOut_3_PRTDSI__OUT_SEL0       (* (reg8 *) DigOut_3__PRTDSI__OUT_SEL0) 
#define DigOut_3_PRTDSI__OUT_SEL1       (* (reg8 *) DigOut_3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DigOut_3_PRTDSI__SYNC_OUT       (* (reg8 *) DigOut_3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(DigOut_3__SIO_CFG)
    #define DigOut_3_SIO_HYST_EN        (* (reg8 *) DigOut_3__SIO_HYST_EN)
    #define DigOut_3_SIO_REG_HIFREQ     (* (reg8 *) DigOut_3__SIO_REG_HIFREQ)
    #define DigOut_3_SIO_CFG            (* (reg8 *) DigOut_3__SIO_CFG)
    #define DigOut_3_SIO_DIFF           (* (reg8 *) DigOut_3__SIO_DIFF)
#endif /* (DigOut_3__SIO_CFG) */

/* Interrupt Registers */
#if defined(DigOut_3__INTSTAT)
    #define DigOut_3_INTSTAT            (* (reg8 *) DigOut_3__INTSTAT)
    #define DigOut_3_SNAP               (* (reg8 *) DigOut_3__SNAP)
    
	#define DigOut_3_0_INTTYPE_REG 		(* (reg8 *) DigOut_3__0__INTTYPE)
#endif /* (DigOut_3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_DigOut_3_H */


/* [] END OF FILE */
