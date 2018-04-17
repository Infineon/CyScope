/*******************************************************************************
* File Name: TriggerStatus.h  
* Version 1.80
*
* Description:
*  This file containts Status Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_STATUS_REG_TriggerStatus_H) /* CY_STATUS_REG_TriggerStatus_H */
#define CY_STATUS_REG_TriggerStatus_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 TriggerStatus_Read(void) ;
void TriggerStatus_InterruptEnable(void) ;
void TriggerStatus_InterruptDisable(void) ;
void TriggerStatus_WriteMask(uint8 mask) ;
uint8 TriggerStatus_ReadMask(void) ;


/***************************************
*           API Constants
***************************************/

#define TriggerStatus_STATUS_INTR_ENBL    0x10u


/***************************************
*         Parameter Constants
***************************************/

/* Status Register Inputs */
#define TriggerStatus_INPUTS              8


/***************************************
*             Registers
***************************************/

/* Status Register */
#define TriggerStatus_Status             (* (reg8 *) TriggerStatus_sts_sts_reg__STATUS_REG )
#define TriggerStatus_Status_PTR         (  (reg8 *) TriggerStatus_sts_sts_reg__STATUS_REG )
#define TriggerStatus_Status_Mask        (* (reg8 *) TriggerStatus_sts_sts_reg__MASK_REG )
#define TriggerStatus_Status_Aux_Ctrl    (* (reg8 *) TriggerStatus_sts_sts_reg__STATUS_AUX_CTL_REG )

#endif /* End CY_STATUS_REG_TriggerStatus_H */


/* [] END OF FILE */
