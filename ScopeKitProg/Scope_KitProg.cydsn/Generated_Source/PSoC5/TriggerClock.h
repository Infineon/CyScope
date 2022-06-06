/*******************************************************************************
* File Name: TriggerClock.h
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

#if !defined(CY_CLOCK_TriggerClock_H)
#define CY_CLOCK_TriggerClock_H

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

void TriggerClock_Start(void) ;
void TriggerClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void TriggerClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void TriggerClock_StandbyPower(uint8 state) ;
void TriggerClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 TriggerClock_GetDividerRegister(void) ;
void TriggerClock_SetModeRegister(uint8 modeBitMask) ;
void TriggerClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 TriggerClock_GetModeRegister(void) ;
void TriggerClock_SetSourceRegister(uint8 clkSource) ;
uint8 TriggerClock_GetSourceRegister(void) ;
#if defined(TriggerClock__CFG3)
void TriggerClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 TriggerClock_GetPhaseRegister(void) ;
#endif /* defined(TriggerClock__CFG3) */

#define TriggerClock_Enable()                       TriggerClock_Start()
#define TriggerClock_Disable()                      TriggerClock_Stop()
#define TriggerClock_SetDivider(clkDivider)         TriggerClock_SetDividerRegister(clkDivider, 1u)
#define TriggerClock_SetDividerValue(clkDivider)    TriggerClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define TriggerClock_SetMode(clkMode)               TriggerClock_SetModeRegister(clkMode)
#define TriggerClock_SetSource(clkSource)           TriggerClock_SetSourceRegister(clkSource)
#if defined(TriggerClock__CFG3)
#define TriggerClock_SetPhase(clkPhase)             TriggerClock_SetPhaseRegister(clkPhase)
#define TriggerClock_SetPhaseValue(clkPhase)        TriggerClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(TriggerClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define TriggerClock_CLKEN              (* (reg8 *) TriggerClock__PM_ACT_CFG)
#define TriggerClock_CLKEN_PTR          ((reg8 *) TriggerClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define TriggerClock_CLKSTBY            (* (reg8 *) TriggerClock__PM_STBY_CFG)
#define TriggerClock_CLKSTBY_PTR        ((reg8 *) TriggerClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define TriggerClock_DIV_LSB            (* (reg8 *) TriggerClock__CFG0)
#define TriggerClock_DIV_LSB_PTR        ((reg8 *) TriggerClock__CFG0)
#define TriggerClock_DIV_PTR            ((reg16 *) TriggerClock__CFG0)

/* Clock MSB divider configuration register. */
#define TriggerClock_DIV_MSB            (* (reg8 *) TriggerClock__CFG1)
#define TriggerClock_DIV_MSB_PTR        ((reg8 *) TriggerClock__CFG1)

/* Mode and source configuration register */
#define TriggerClock_MOD_SRC            (* (reg8 *) TriggerClock__CFG2)
#define TriggerClock_MOD_SRC_PTR        ((reg8 *) TriggerClock__CFG2)

#if defined(TriggerClock__CFG3)
/* Analog clock phase configuration register */
#define TriggerClock_PHASE              (* (reg8 *) TriggerClock__CFG3)
#define TriggerClock_PHASE_PTR          ((reg8 *) TriggerClock__CFG3)
#endif /* defined(TriggerClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define TriggerClock_CLKEN_MASK         TriggerClock__PM_ACT_MSK
#define TriggerClock_CLKSTBY_MASK       TriggerClock__PM_STBY_MSK

/* CFG2 field masks */
#define TriggerClock_SRC_SEL_MSK        TriggerClock__CFG2_SRC_SEL_MASK
#define TriggerClock_MODE_MASK          (~(TriggerClock_SRC_SEL_MSK))

#if defined(TriggerClock__CFG3)
/* CFG3 phase mask */
#define TriggerClock_PHASE_MASK         TriggerClock__CFG3_PHASE_DLY_MASK
#endif /* defined(TriggerClock__CFG3) */

#endif /* CY_CLOCK_TriggerClock_H */


/* [] END OF FILE */
