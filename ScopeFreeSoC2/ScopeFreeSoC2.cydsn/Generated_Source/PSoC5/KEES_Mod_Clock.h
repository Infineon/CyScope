/*******************************************************************************
* File Name: KEES_Mod_Clock.h
* Version 2.0
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

#if !defined(CY_CLOCK_KEES_Mod_Clock_H)
#define CY_CLOCK_KEES_Mod_Clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void KEES_Mod_Clock_Start(void) ;
void KEES_Mod_Clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void KEES_Mod_Clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void KEES_Mod_Clock_StandbyPower(uint8 state) ;
void KEES_Mod_Clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 KEES_Mod_Clock_GetDividerRegister(void) ;
void KEES_Mod_Clock_SetModeRegister(uint8 modeBitMask) ;
void KEES_Mod_Clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 KEES_Mod_Clock_GetModeRegister(void) ;
void KEES_Mod_Clock_SetSourceRegister(uint8 clkSource) ;
uint8 KEES_Mod_Clock_GetSourceRegister(void) ;
#if defined(KEES_Mod_Clock__CFG3)
void KEES_Mod_Clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 KEES_Mod_Clock_GetPhaseRegister(void) ;
#endif /* defined(KEES_Mod_Clock__CFG3) */

#define KEES_Mod_Clock_Enable()                       KEES_Mod_Clock_Start()
#define KEES_Mod_Clock_Disable()                      KEES_Mod_Clock_Stop()
#define KEES_Mod_Clock_SetDivider(clkDivider)         KEES_Mod_Clock_SetDividerRegister(clkDivider, 1)
#define KEES_Mod_Clock_SetDividerValue(clkDivider)    KEES_Mod_Clock_SetDividerRegister((clkDivider) - 1, 1)
#define KEES_Mod_Clock_SetMode(clkMode)               KEES_Mod_Clock_SetModeRegister(clkMode)
#define KEES_Mod_Clock_SetSource(clkSource)           KEES_Mod_Clock_SetSourceRegister(clkSource)
#if defined(KEES_Mod_Clock__CFG3)
#define KEES_Mod_Clock_SetPhase(clkPhase)             KEES_Mod_Clock_SetPhaseRegister(clkPhase)
#define KEES_Mod_Clock_SetPhaseValue(clkPhase)        KEES_Mod_Clock_SetPhaseRegister((clkPhase) + 1)
#endif /* defined(KEES_Mod_Clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define KEES_Mod_Clock_CLKEN              (* (reg8 *) KEES_Mod_Clock__PM_ACT_CFG)
#define KEES_Mod_Clock_CLKEN_PTR          ((reg8 *) KEES_Mod_Clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define KEES_Mod_Clock_CLKSTBY            (* (reg8 *) KEES_Mod_Clock__PM_STBY_CFG)
#define KEES_Mod_Clock_CLKSTBY_PTR        ((reg8 *) KEES_Mod_Clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define KEES_Mod_Clock_DIV_LSB            (* (reg8 *) KEES_Mod_Clock__CFG0)
#define KEES_Mod_Clock_DIV_LSB_PTR        ((reg8 *) KEES_Mod_Clock__CFG0)
#define KEES_Mod_Clock_DIV_PTR            ((reg16 *) KEES_Mod_Clock__CFG0)

/* Clock MSB divider configuration register. */
#define KEES_Mod_Clock_DIV_MSB            (* (reg8 *) KEES_Mod_Clock__CFG1)
#define KEES_Mod_Clock_DIV_MSB_PTR        ((reg8 *) KEES_Mod_Clock__CFG1)

/* Mode and source configuration register */
#define KEES_Mod_Clock_MOD_SRC            (* (reg8 *) KEES_Mod_Clock__CFG2)
#define KEES_Mod_Clock_MOD_SRC_PTR        ((reg8 *) KEES_Mod_Clock__CFG2)

#if defined(KEES_Mod_Clock__CFG3)
/* Analog clock phase configuration register */
#define KEES_Mod_Clock_PHASE              (* (reg8 *) KEES_Mod_Clock__CFG3)
#define KEES_Mod_Clock_PHASE_PTR          ((reg8 *) KEES_Mod_Clock__CFG3)
#endif /* defined(KEES_Mod_Clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define KEES_Mod_Clock_CLKEN_MASK         KEES_Mod_Clock__PM_ACT_MSK
#define KEES_Mod_Clock_CLKSTBY_MASK       KEES_Mod_Clock__PM_STBY_MSK

/* CFG2 field masks */
#define KEES_Mod_Clock_SRC_SEL_MSK        KEES_Mod_Clock__CFG2_SRC_SEL_MASK
#define KEES_Mod_Clock_MODE_MASK          (~(KEES_Mod_Clock_SRC_SEL_MSK))

#if defined(KEES_Mod_Clock__CFG3)
/* CFG3 phase mask */
#define KEES_Mod_Clock_PHASE_MASK         KEES_Mod_Clock__CFG3_PHASE_DLY_MASK
#endif /* defined(KEES_Mod_Clock__CFG3) */

#endif /* CY_CLOCK_KEES_Mod_Clock_H */


/* [] END OF FILE */
