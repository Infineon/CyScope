/*******************************************************************************
* File Name: TriggerControl.h  
* Version 1.70
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_TriggerControl_H) /* CY_CONTROL_REG_TriggerControl_H */
#define CY_CONTROL_REG_TriggerControl_H

#include "cytypes.h"


/***************************************
*         Function Prototypes 
***************************************/

void    TriggerControl_Write(uint8 control) ;
uint8   TriggerControl_Read(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define TriggerControl_Control        (* (reg8 *) TriggerControl_Sync_ctrl_reg__CONTROL_REG )
#define TriggerControl_Control_PTR    (  (reg8 *) TriggerControl_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_TriggerControl_H */


/* [] END OF FILE */
