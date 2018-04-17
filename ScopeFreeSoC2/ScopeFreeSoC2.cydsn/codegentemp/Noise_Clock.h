/*******************************************************************************
* File Name: Noise_Clock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Noise_Clock_H)
#define CY_CLOCK_Noise_Clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void Noise_Clock_Start(void) ;
void Noise_Clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Noise_Clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Noise_Clock_StandbyPower(uint8 state) ;
void Noise_Clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Noise_Clock_GetDividerRegister(void) ;
void Noise_Clock_SetModeRegister(uint8 modeBitMask) ;
void Noise_Clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 Noise_Clock_GetModeRegister(void) ;
void Noise_Clock_SetSourceRegister(uint8 clkSource) ;
uint8 Noise_Clock_GetSourceRegister(void) ;
#if defined(Noise_Clock__CFG3)
void Noise_Clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 Noise_Clock_GetPhaseRegister(void) ;
#endif /* defined(Noise_Clock__CFG3) */

#define Noise_Clock_Enable()                       Noise_Clock_Start()
#define Noise_Clock_Disable()                      Noise_Clock_Stop()
#define Noise_Clock_SetDivider(clkDivider)         Noise_Clock_SetDividerRegister(clkDivider, 1u)
#define Noise_Clock_SetDividerValue(clkDivider)    Noise_Clock_SetDividerRegister((clkDivider) - 1u, 1u)
#define Noise_Clock_SetMode(clkMode)               Noise_Clock_SetModeRegister(clkMode)
#define Noise_Clock_SetSource(clkSource)           Noise_Clock_SetSourceRegister(clkSource)
#if defined(Noise_Clock__CFG3)
#define Noise_Clock_SetPhase(clkPhase)             Noise_Clock_SetPhaseRegister(clkPhase)
#define Noise_Clock_SetPhaseValue(clkPhase)        Noise_Clock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Noise_Clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Noise_Clock_CLKEN              (* (reg8 *) Noise_Clock__PM_ACT_CFG)
#define Noise_Clock_CLKEN_PTR          ((reg8 *) Noise_Clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Noise_Clock_CLKSTBY            (* (reg8 *) Noise_Clock__PM_STBY_CFG)
#define Noise_Clock_CLKSTBY_PTR        ((reg8 *) Noise_Clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Noise_Clock_DIV_LSB            (* (reg8 *) Noise_Clock__CFG0)
#define Noise_Clock_DIV_LSB_PTR        ((reg8 *) Noise_Clock__CFG0)
#define Noise_Clock_DIV_PTR            ((reg16 *) Noise_Clock__CFG0)

/* Clock MSB divider configuration register. */
#define Noise_Clock_DIV_MSB            (* (reg8 *) Noise_Clock__CFG1)
#define Noise_Clock_DIV_MSB_PTR        ((reg8 *) Noise_Clock__CFG1)

/* Mode and source configuration register */
#define Noise_Clock_MOD_SRC            (* (reg8 *) Noise_Clock__CFG2)
#define Noise_Clock_MOD_SRC_PTR        ((reg8 *) Noise_Clock__CFG2)

#if defined(Noise_Clock__CFG3)
/* Analog clock phase configuration register */
#define Noise_Clock_PHASE              (* (reg8 *) Noise_Clock__CFG3)
#define Noise_Clock_PHASE_PTR          ((reg8 *) Noise_Clock__CFG3)
#endif /* defined(Noise_Clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Noise_Clock_CLKEN_MASK         Noise_Clock__PM_ACT_MSK
#define Noise_Clock_CLKSTBY_MASK       Noise_Clock__PM_STBY_MSK

/* CFG2 field masks */
#define Noise_Clock_SRC_SEL_MSK        Noise_Clock__CFG2_SRC_SEL_MASK
#define Noise_Clock_MODE_MASK          (~(Noise_Clock_SRC_SEL_MSK))

#if defined(Noise_Clock__CFG3)
/* CFG3 phase mask */
#define Noise_Clock_PHASE_MASK         Noise_Clock__CFG3_PHASE_DLY_MASK
#endif /* defined(Noise_Clock__CFG3) */

#endif /* CY_CLOCK_Noise_Clock_H */


/* [] END OF FILE */
