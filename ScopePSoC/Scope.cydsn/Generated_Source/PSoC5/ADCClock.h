/*******************************************************************************
* File Name: ADCClock.h
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

#if !defined(CY_CLOCK_ADCClock_H)
#define CY_CLOCK_ADCClock_H

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

void ADCClock_Start(void) ;
void ADCClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void ADCClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void ADCClock_StandbyPower(uint8 state) ;
void ADCClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 ADCClock_GetDividerRegister(void) ;
void ADCClock_SetModeRegister(uint8 modeBitMask) ;
void ADCClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 ADCClock_GetModeRegister(void) ;
void ADCClock_SetSourceRegister(uint8 clkSource) ;
uint8 ADCClock_GetSourceRegister(void) ;
#if defined(ADCClock__CFG3)
void ADCClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 ADCClock_GetPhaseRegister(void) ;
#endif /* defined(ADCClock__CFG3) */

#define ADCClock_Enable()                       ADCClock_Start()
#define ADCClock_Disable()                      ADCClock_Stop()
#define ADCClock_SetDivider(clkDivider)         ADCClock_SetDividerRegister(clkDivider, 1u)
#define ADCClock_SetDividerValue(clkDivider)    ADCClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define ADCClock_SetMode(clkMode)               ADCClock_SetModeRegister(clkMode)
#define ADCClock_SetSource(clkSource)           ADCClock_SetSourceRegister(clkSource)
#if defined(ADCClock__CFG3)
#define ADCClock_SetPhase(clkPhase)             ADCClock_SetPhaseRegister(clkPhase)
#define ADCClock_SetPhaseValue(clkPhase)        ADCClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(ADCClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define ADCClock_CLKEN              (* (reg8 *) ADCClock__PM_ACT_CFG)
#define ADCClock_CLKEN_PTR          ((reg8 *) ADCClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define ADCClock_CLKSTBY            (* (reg8 *) ADCClock__PM_STBY_CFG)
#define ADCClock_CLKSTBY_PTR        ((reg8 *) ADCClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define ADCClock_DIV_LSB            (* (reg8 *) ADCClock__CFG0)
#define ADCClock_DIV_LSB_PTR        ((reg8 *) ADCClock__CFG0)
#define ADCClock_DIV_PTR            ((reg16 *) ADCClock__CFG0)

/* Clock MSB divider configuration register. */
#define ADCClock_DIV_MSB            (* (reg8 *) ADCClock__CFG1)
#define ADCClock_DIV_MSB_PTR        ((reg8 *) ADCClock__CFG1)

/* Mode and source configuration register */
#define ADCClock_MOD_SRC            (* (reg8 *) ADCClock__CFG2)
#define ADCClock_MOD_SRC_PTR        ((reg8 *) ADCClock__CFG2)

#if defined(ADCClock__CFG3)
/* Analog clock phase configuration register */
#define ADCClock_PHASE              (* (reg8 *) ADCClock__CFG3)
#define ADCClock_PHASE_PTR          ((reg8 *) ADCClock__CFG3)
#endif /* defined(ADCClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define ADCClock_CLKEN_MASK         ADCClock__PM_ACT_MSK
#define ADCClock_CLKSTBY_MASK       ADCClock__PM_STBY_MSK

/* CFG2 field masks */
#define ADCClock_SRC_SEL_MSK        ADCClock__CFG2_SRC_SEL_MASK
#define ADCClock_MODE_MASK          (~(ADCClock_SRC_SEL_MSK))

#if defined(ADCClock__CFG3)
/* CFG3 phase mask */
#define ADCClock_PHASE_MASK         ADCClock__CFG3_PHASE_DLY_MASK
#endif /* defined(ADCClock__CFG3) */

#endif /* CY_CLOCK_ADCClock_H */


/* [] END OF FILE */
