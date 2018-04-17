/*******************************************************************************
* File Name: CyTrigClock.h
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

#if !defined(CY_CLOCK_CyTrigClock_H)
#define CY_CLOCK_CyTrigClock_H

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

void CyTrigClock_Start(void) ;
void CyTrigClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CyTrigClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CyTrigClock_StandbyPower(uint8 state) ;
void CyTrigClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CyTrigClock_GetDividerRegister(void) ;
void CyTrigClock_SetModeRegister(uint8 modeBitMask) ;
void CyTrigClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 CyTrigClock_GetModeRegister(void) ;
void CyTrigClock_SetSourceRegister(uint8 clkSource) ;
uint8 CyTrigClock_GetSourceRegister(void) ;
#if defined(CyTrigClock__CFG3)
void CyTrigClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 CyTrigClock_GetPhaseRegister(void) ;
#endif /* defined(CyTrigClock__CFG3) */

#define CyTrigClock_Enable()                       CyTrigClock_Start()
#define CyTrigClock_Disable()                      CyTrigClock_Stop()
#define CyTrigClock_SetDivider(clkDivider)         CyTrigClock_SetDividerRegister(clkDivider, 1u)
#define CyTrigClock_SetDividerValue(clkDivider)    CyTrigClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define CyTrigClock_SetMode(clkMode)               CyTrigClock_SetModeRegister(clkMode)
#define CyTrigClock_SetSource(clkSource)           CyTrigClock_SetSourceRegister(clkSource)
#if defined(CyTrigClock__CFG3)
#define CyTrigClock_SetPhase(clkPhase)             CyTrigClock_SetPhaseRegister(clkPhase)
#define CyTrigClock_SetPhaseValue(clkPhase)        CyTrigClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CyTrigClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CyTrigClock_CLKEN              (* (reg8 *) CyTrigClock__PM_ACT_CFG)
#define CyTrigClock_CLKEN_PTR          ((reg8 *) CyTrigClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CyTrigClock_CLKSTBY            (* (reg8 *) CyTrigClock__PM_STBY_CFG)
#define CyTrigClock_CLKSTBY_PTR        ((reg8 *) CyTrigClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CyTrigClock_DIV_LSB            (* (reg8 *) CyTrigClock__CFG0)
#define CyTrigClock_DIV_LSB_PTR        ((reg8 *) CyTrigClock__CFG0)
#define CyTrigClock_DIV_PTR            ((reg16 *) CyTrigClock__CFG0)

/* Clock MSB divider configuration register. */
#define CyTrigClock_DIV_MSB            (* (reg8 *) CyTrigClock__CFG1)
#define CyTrigClock_DIV_MSB_PTR        ((reg8 *) CyTrigClock__CFG1)

/* Mode and source configuration register */
#define CyTrigClock_MOD_SRC            (* (reg8 *) CyTrigClock__CFG2)
#define CyTrigClock_MOD_SRC_PTR        ((reg8 *) CyTrigClock__CFG2)

#if defined(CyTrigClock__CFG3)
/* Analog clock phase configuration register */
#define CyTrigClock_PHASE              (* (reg8 *) CyTrigClock__CFG3)
#define CyTrigClock_PHASE_PTR          ((reg8 *) CyTrigClock__CFG3)
#endif /* defined(CyTrigClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CyTrigClock_CLKEN_MASK         CyTrigClock__PM_ACT_MSK
#define CyTrigClock_CLKSTBY_MASK       CyTrigClock__PM_STBY_MSK

/* CFG2 field masks */
#define CyTrigClock_SRC_SEL_MSK        CyTrigClock__CFG2_SRC_SEL_MASK
#define CyTrigClock_MODE_MASK          (~(CyTrigClock_SRC_SEL_MSK))

#if defined(CyTrigClock__CFG3)
/* CFG3 phase mask */
#define CyTrigClock_PHASE_MASK         CyTrigClock__CFG3_PHASE_DLY_MASK
#endif /* defined(CyTrigClock__CFG3) */

#endif /* CY_CLOCK_CyTrigClock_H */


/* [] END OF FILE */
