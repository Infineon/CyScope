/*******************************************************************************
* File Name: SampleClock.h
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

#if !defined(CY_CLOCK_SampleClock_H)
#define CY_CLOCK_SampleClock_H

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

void SampleClock_Start(void) ;
void SampleClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void SampleClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void SampleClock_StandbyPower(uint8 state) ;
void SampleClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 SampleClock_GetDividerRegister(void) ;
void SampleClock_SetModeRegister(uint8 modeBitMask) ;
void SampleClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 SampleClock_GetModeRegister(void) ;
void SampleClock_SetSourceRegister(uint8 clkSource) ;
uint8 SampleClock_GetSourceRegister(void) ;
#if defined(SampleClock__CFG3)
void SampleClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 SampleClock_GetPhaseRegister(void) ;
#endif /* defined(SampleClock__CFG3) */

#define SampleClock_Enable()                       SampleClock_Start()
#define SampleClock_Disable()                      SampleClock_Stop()
#define SampleClock_SetDivider(clkDivider)         SampleClock_SetDividerRegister(clkDivider, 1u)
#define SampleClock_SetDividerValue(clkDivider)    SampleClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define SampleClock_SetMode(clkMode)               SampleClock_SetModeRegister(clkMode)
#define SampleClock_SetSource(clkSource)           SampleClock_SetSourceRegister(clkSource)
#if defined(SampleClock__CFG3)
#define SampleClock_SetPhase(clkPhase)             SampleClock_SetPhaseRegister(clkPhase)
#define SampleClock_SetPhaseValue(clkPhase)        SampleClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(SampleClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define SampleClock_CLKEN              (* (reg8 *) SampleClock__PM_ACT_CFG)
#define SampleClock_CLKEN_PTR          ((reg8 *) SampleClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define SampleClock_CLKSTBY            (* (reg8 *) SampleClock__PM_STBY_CFG)
#define SampleClock_CLKSTBY_PTR        ((reg8 *) SampleClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define SampleClock_DIV_LSB            (* (reg8 *) SampleClock__CFG0)
#define SampleClock_DIV_LSB_PTR        ((reg8 *) SampleClock__CFG0)
#define SampleClock_DIV_PTR            ((reg16 *) SampleClock__CFG0)

/* Clock MSB divider configuration register. */
#define SampleClock_DIV_MSB            (* (reg8 *) SampleClock__CFG1)
#define SampleClock_DIV_MSB_PTR        ((reg8 *) SampleClock__CFG1)

/* Mode and source configuration register */
#define SampleClock_MOD_SRC            (* (reg8 *) SampleClock__CFG2)
#define SampleClock_MOD_SRC_PTR        ((reg8 *) SampleClock__CFG2)

#if defined(SampleClock__CFG3)
/* Analog clock phase configuration register */
#define SampleClock_PHASE              (* (reg8 *) SampleClock__CFG3)
#define SampleClock_PHASE_PTR          ((reg8 *) SampleClock__CFG3)
#endif /* defined(SampleClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define SampleClock_CLKEN_MASK         SampleClock__PM_ACT_MSK
#define SampleClock_CLKSTBY_MASK       SampleClock__PM_STBY_MSK

/* CFG2 field masks */
#define SampleClock_SRC_SEL_MSK        SampleClock__CFG2_SRC_SEL_MASK
#define SampleClock_MODE_MASK          (~(SampleClock_SRC_SEL_MSK))

#if defined(SampleClock__CFG3)
/* CFG3 phase mask */
#define SampleClock_PHASE_MASK         SampleClock__CFG3_PHASE_DLY_MASK
#endif /* defined(SampleClock__CFG3) */

#endif /* CY_CLOCK_SampleClock_H */


/* [] END OF FILE */
