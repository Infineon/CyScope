/*******************************************************************************
* File Name: Opamp_Pos_Input_P3_3.h  
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

#if !defined(CY_PINS_Opamp_Pos_Input_P3_3_H) /* Pins Opamp_Pos_Input_P3_3_H */
#define CY_PINS_Opamp_Pos_Input_P3_3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Opamp_Pos_Input_P3_3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Opamp_Pos_Input_P3_3__PORT == 15 && ((Opamp_Pos_Input_P3_3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Opamp_Pos_Input_P3_3_Write(uint8 value);
void    Opamp_Pos_Input_P3_3_SetDriveMode(uint8 mode);
uint8   Opamp_Pos_Input_P3_3_ReadDataReg(void);
uint8   Opamp_Pos_Input_P3_3_Read(void);
void    Opamp_Pos_Input_P3_3_SetInterruptMode(uint16 position, uint16 mode);
uint8   Opamp_Pos_Input_P3_3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Opamp_Pos_Input_P3_3_SetDriveMode() function.
     *  @{
     */
        #define Opamp_Pos_Input_P3_3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Opamp_Pos_Input_P3_3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Opamp_Pos_Input_P3_3_DM_RES_UP          PIN_DM_RES_UP
        #define Opamp_Pos_Input_P3_3_DM_RES_DWN         PIN_DM_RES_DWN
        #define Opamp_Pos_Input_P3_3_DM_OD_LO           PIN_DM_OD_LO
        #define Opamp_Pos_Input_P3_3_DM_OD_HI           PIN_DM_OD_HI
        #define Opamp_Pos_Input_P3_3_DM_STRONG          PIN_DM_STRONG
        #define Opamp_Pos_Input_P3_3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Opamp_Pos_Input_P3_3_MASK               Opamp_Pos_Input_P3_3__MASK
#define Opamp_Pos_Input_P3_3_SHIFT              Opamp_Pos_Input_P3_3__SHIFT
#define Opamp_Pos_Input_P3_3_WIDTH              1u

/* Interrupt constants */
#if defined(Opamp_Pos_Input_P3_3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Opamp_Pos_Input_P3_3_SetInterruptMode() function.
     *  @{
     */
        #define Opamp_Pos_Input_P3_3_INTR_NONE      (uint16)(0x0000u)
        #define Opamp_Pos_Input_P3_3_INTR_RISING    (uint16)(0x0001u)
        #define Opamp_Pos_Input_P3_3_INTR_FALLING   (uint16)(0x0002u)
        #define Opamp_Pos_Input_P3_3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Opamp_Pos_Input_P3_3_INTR_MASK      (0x01u) 
#endif /* (Opamp_Pos_Input_P3_3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Opamp_Pos_Input_P3_3_PS                     (* (reg8 *) Opamp_Pos_Input_P3_3__PS)
/* Data Register */
#define Opamp_Pos_Input_P3_3_DR                     (* (reg8 *) Opamp_Pos_Input_P3_3__DR)
/* Port Number */
#define Opamp_Pos_Input_P3_3_PRT_NUM                (* (reg8 *) Opamp_Pos_Input_P3_3__PRT) 
/* Connect to Analog Globals */                                                  
#define Opamp_Pos_Input_P3_3_AG                     (* (reg8 *) Opamp_Pos_Input_P3_3__AG)                       
/* Analog MUX bux enable */
#define Opamp_Pos_Input_P3_3_AMUX                   (* (reg8 *) Opamp_Pos_Input_P3_3__AMUX) 
/* Bidirectional Enable */                                                        
#define Opamp_Pos_Input_P3_3_BIE                    (* (reg8 *) Opamp_Pos_Input_P3_3__BIE)
/* Bit-mask for Aliased Register Access */
#define Opamp_Pos_Input_P3_3_BIT_MASK               (* (reg8 *) Opamp_Pos_Input_P3_3__BIT_MASK)
/* Bypass Enable */
#define Opamp_Pos_Input_P3_3_BYP                    (* (reg8 *) Opamp_Pos_Input_P3_3__BYP)
/* Port wide control signals */                                                   
#define Opamp_Pos_Input_P3_3_CTL                    (* (reg8 *) Opamp_Pos_Input_P3_3__CTL)
/* Drive Modes */
#define Opamp_Pos_Input_P3_3_DM0                    (* (reg8 *) Opamp_Pos_Input_P3_3__DM0) 
#define Opamp_Pos_Input_P3_3_DM1                    (* (reg8 *) Opamp_Pos_Input_P3_3__DM1)
#define Opamp_Pos_Input_P3_3_DM2                    (* (reg8 *) Opamp_Pos_Input_P3_3__DM2) 
/* Input Buffer Disable Override */
#define Opamp_Pos_Input_P3_3_INP_DIS                (* (reg8 *) Opamp_Pos_Input_P3_3__INP_DIS)
/* LCD Common or Segment Drive */
#define Opamp_Pos_Input_P3_3_LCD_COM_SEG            (* (reg8 *) Opamp_Pos_Input_P3_3__LCD_COM_SEG)
/* Enable Segment LCD */
#define Opamp_Pos_Input_P3_3_LCD_EN                 (* (reg8 *) Opamp_Pos_Input_P3_3__LCD_EN)
/* Slew Rate Control */
#define Opamp_Pos_Input_P3_3_SLW                    (* (reg8 *) Opamp_Pos_Input_P3_3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Opamp_Pos_Input_P3_3_PRTDSI__CAPS_SEL       (* (reg8 *) Opamp_Pos_Input_P3_3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Opamp_Pos_Input_P3_3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Opamp_Pos_Input_P3_3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Opamp_Pos_Input_P3_3_PRTDSI__OE_SEL0        (* (reg8 *) Opamp_Pos_Input_P3_3__PRTDSI__OE_SEL0) 
#define Opamp_Pos_Input_P3_3_PRTDSI__OE_SEL1        (* (reg8 *) Opamp_Pos_Input_P3_3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Opamp_Pos_Input_P3_3_PRTDSI__OUT_SEL0       (* (reg8 *) Opamp_Pos_Input_P3_3__PRTDSI__OUT_SEL0) 
#define Opamp_Pos_Input_P3_3_PRTDSI__OUT_SEL1       (* (reg8 *) Opamp_Pos_Input_P3_3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Opamp_Pos_Input_P3_3_PRTDSI__SYNC_OUT       (* (reg8 *) Opamp_Pos_Input_P3_3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Opamp_Pos_Input_P3_3__SIO_CFG)
    #define Opamp_Pos_Input_P3_3_SIO_HYST_EN        (* (reg8 *) Opamp_Pos_Input_P3_3__SIO_HYST_EN)
    #define Opamp_Pos_Input_P3_3_SIO_REG_HIFREQ     (* (reg8 *) Opamp_Pos_Input_P3_3__SIO_REG_HIFREQ)
    #define Opamp_Pos_Input_P3_3_SIO_CFG            (* (reg8 *) Opamp_Pos_Input_P3_3__SIO_CFG)
    #define Opamp_Pos_Input_P3_3_SIO_DIFF           (* (reg8 *) Opamp_Pos_Input_P3_3__SIO_DIFF)
#endif /* (Opamp_Pos_Input_P3_3__SIO_CFG) */

/* Interrupt Registers */
#if defined(Opamp_Pos_Input_P3_3__INTSTAT)
    #define Opamp_Pos_Input_P3_3_INTSTAT            (* (reg8 *) Opamp_Pos_Input_P3_3__INTSTAT)
    #define Opamp_Pos_Input_P3_3_SNAP               (* (reg8 *) Opamp_Pos_Input_P3_3__SNAP)
    
	#define Opamp_Pos_Input_P3_3_0_INTTYPE_REG 		(* (reg8 *) Opamp_Pos_Input_P3_3__0__INTTYPE)
#endif /* (Opamp_Pos_Input_P3_3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Opamp_Pos_Input_P3_3_H */


/* [] END OF FILE */
