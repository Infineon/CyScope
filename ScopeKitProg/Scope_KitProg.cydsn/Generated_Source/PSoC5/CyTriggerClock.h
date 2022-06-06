/*******************************************************************************
* File Name: CyTriggerClock.h
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

#if !defined(CY_CLOCK_CyTriggerClock_H)
#define CY_CLOCK_CyTriggerClock_H

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

void CyTriggerClock_Start(void) ;
void CyTriggerClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CyTriggerClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CyTriggerClock_StandbyPower(uint8 state) ;
void CyTriggerClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CyTriggerClock_GetDividerRegister(void) ;
void CyTriggerClock_SetModeRegister(uint8 modeBitMask) ;
void CyTriggerClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 CyTriggerClock_GetModeRegister(void) ;
void CyTriggerClock_SetSourceRegister(uint8 clkSource) ;
uint8 CyTriggerClock_GetSourceRegister(void) ;
#if defined(CyTriggerClock__CFG3)
void CyTriggerClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 CyTriggerClock_GetPhaseRegister(void) ;
#endif /* defined(CyTriggerClock__CFG3) */

#define CyTriggerClock_Enable()                       CyTriggerClock_Start()
#define CyTriggerClock_Disable()                      CyTriggerClock_Stop()
#define CyTriggerClock_SetDivider(clkDivider)         CyTriggerClock_SetDividerRegister(clkDivider, 1u)
#define CyTriggerClock_SetDividerValue(clkDivider)    CyTriggerClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define CyTriggerClock_SetMode(clkMode)               CyTriggerClock_SetModeRegister(clkMode)
#define CyTriggerClock_SetSource(clkSource)           CyTriggerClock_SetSourceRegister(clkSource)
#if defined(CyTriggerClock__CFG3)
#define CyTriggerClock_SetPhase(clkPhase)             CyTriggerClock_SetPhaseRegister(clkPhase)
#define CyTriggerClock_SetPhaseValue(clkPhase)        CyTriggerClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CyTriggerClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CyTriggerClock_CLKEN              (* (reg8 *) CyTriggerClock__PM_ACT_CFG)
#define CyTriggerClock_CLKEN_PTR          ((reg8 *) CyTriggerClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CyTriggerClock_CLKSTBY            (* (reg8 *) CyTriggerClock__PM_STBY_CFG)
#define CyTriggerClock_CLKSTBY_PTR        ((reg8 *) CyTriggerClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CyTriggerClock_DIV_LSB            (* (reg8 *) CyTriggerClock__CFG0)
#define CyTriggerClock_DIV_LSB_PTR        ((reg8 *) CyTriggerClock__CFG0)
#define CyTriggerClock_DIV_PTR            ((reg16 *) CyTriggerClock__CFG0)

/* Clock MSB divider configuration register. */
#define CyTriggerClock_DIV_MSB            (* (reg8 *) CyTriggerClock__CFG1)
#define CyTriggerClock_DIV_MSB_PTR        ((reg8 *) CyTriggerClock__CFG1)

/* Mode and source configuration register */
#define CyTriggerClock_MOD_SRC            (* (reg8 *) CyTriggerClock__CFG2)
#define CyTriggerClock_MOD_SRC_PTR        ((reg8 *) CyTriggerClock__CFG2)

#if defined(CyTriggerClock__CFG3)
/* Analog clock phase configuration register */
#define CyTriggerClock_PHASE              (* (reg8 *) CyTriggerClock__CFG3)
#define CyTriggerClock_PHASE_PTR          ((reg8 *) CyTriggerClock__CFG3)
#endif /* defined(CyTriggerClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CyTriggerClock_CLKEN_MASK         CyTriggerClock__PM_ACT_MSK
#define CyTriggerClock_CLKSTBY_MASK       CyTriggerClock__PM_STBY_MSK

/* CFG2 field masks */
#define CyTriggerClock_SRC_SEL_MSK        CyTriggerClock__CFG2_SRC_SEL_MASK
#define CyTriggerClock_MODE_MASK          (~(CyTriggerClock_SRC_SEL_MSK))

#if defined(CyTriggerClock__CFG3)
/* CFG3 phase mask */
#define CyTriggerClock_PHASE_MASK         CyTriggerClock__CFG3_PHASE_DLY_MASK
#endif /* defined(CyTriggerClock__CFG3) */

#endif /* CY_CLOCK_CyTriggerClock_H */


/* [] END OF FILE */
