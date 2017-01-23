/*******************************************************************************
* File Name: Vtrigger.c  
* Version 1.90
*
* Description:
*  This file provides the source code to the API for the 8-bit Voltage DAC 
*  (VDAC8) User Module.
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Vtrigger.h"

#if (CY_PSOC5A)
#include <CyLib.h>
#endif /* CY_PSOC5A */

uint8 Vtrigger_initVar = 0u;

#if (CY_PSOC5A)
    static uint8 Vtrigger_restoreVal = 0u;
#endif /* CY_PSOC5A */

#if (CY_PSOC5A)
    static Vtrigger_backupStruct Vtrigger_backup;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: Vtrigger_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
* 
* Parameters:
*  void:
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Vtrigger_Init(void) 
{
    Vtrigger_CR0 = (Vtrigger_MODE_V );

    /* Set default data source */
    #if(Vtrigger_DEFAULT_DATA_SRC != 0 )
        Vtrigger_CR1 = (Vtrigger_DEFAULT_CNTL | Vtrigger_DACBUS_ENABLE) ;
    #else
        Vtrigger_CR1 = (Vtrigger_DEFAULT_CNTL | Vtrigger_DACBUS_DISABLE) ;
    #endif /* (Vtrigger_DEFAULT_DATA_SRC != 0 ) */

    /* Set default strobe mode */
    #if(Vtrigger_DEFAULT_STRB != 0)
        Vtrigger_Strobe |= Vtrigger_STRB_EN ;
    #endif/* (Vtrigger_DEFAULT_STRB != 0) */

    /* Set default range */
    Vtrigger_SetRange(Vtrigger_DEFAULT_RANGE); 

    /* Set default speed */
    Vtrigger_SetSpeed(Vtrigger_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: Vtrigger_Enable
********************************************************************************
* Summary:
*  Enable the VDAC8
* 
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Vtrigger_Enable(void) 
{
    Vtrigger_PWRMGR |= Vtrigger_ACT_PWR_EN;
    Vtrigger_STBY_PWRMGR |= Vtrigger_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */
    #if (CY_PSOC5A)
        if(Vtrigger_restoreVal == 1u) 
        {
             Vtrigger_CR0 = Vtrigger_backup.data_value;
             Vtrigger_restoreVal = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: Vtrigger_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as
*  executing the stop function.
*
* Parameters:
*  Power: Sets power level between off (0) and (3) high power
*
* Return:
*  void 
*
* Global variables:
*  Vtrigger_initVar: Is modified when this function is called for the 
*  first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Vtrigger_Start(void)  
{
    /* Hardware initiazation only needs to occure the first time */
    if(Vtrigger_initVar == 0u)
    { 
        Vtrigger_Init();
        Vtrigger_initVar = 1u;
    }

    /* Enable power to DAC */
    Vtrigger_Enable();

    /* Set default value */
    Vtrigger_SetValue(Vtrigger_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: Vtrigger_Stop
********************************************************************************
*
* Summary:
*  Powers down DAC to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Vtrigger_Stop(void) 
{
    /* Disble power to DAC */
    Vtrigger_PWRMGR &= (uint8)(~Vtrigger_ACT_PWR_EN);
    Vtrigger_STBY_PWRMGR &= (uint8)(~Vtrigger_STBY_PWR_EN);

    /* This is a work around for PSoC5A  ,
    this sets VDAC to current mode with output off */
    #if (CY_PSOC5A)
        Vtrigger_backup.data_value = Vtrigger_CR0;
        Vtrigger_CR0 = Vtrigger_CUR_MODE_OUT_OFF;
        Vtrigger_restoreVal = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: Vtrigger_SetSpeed
********************************************************************************
*
* Summary:
*  Set DAC speed
*
* Parameters:
*  power: Sets speed value
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Vtrigger_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    Vtrigger_CR0 &= (uint8)(~Vtrigger_HS_MASK);
    Vtrigger_CR0 |=  (speed & Vtrigger_HS_MASK);
}


/*******************************************************************************
* Function Name: Vtrigger_SetRange
********************************************************************************
*
* Summary:
*  Set one of three current ranges.
*
* Parameters:
*  Range: Sets one of Three valid ranges.
*
* Return:
*  void 
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Vtrigger_SetRange(uint8 range) 
{
    Vtrigger_CR0 &= (uint8)(~Vtrigger_RANGE_MASK);      /* Clear existing mode */
    Vtrigger_CR0 |= (range & Vtrigger_RANGE_MASK);      /*  Set Range  */
    Vtrigger_DacTrim();
}


/*******************************************************************************
* Function Name: Vtrigger_SetValue
********************************************************************************
*
* Summary:
*  Set 8-bit DAC value
*
* Parameters:  
*  value:  Sets DAC value between 0 and 255.
*
* Return: 
*  void 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void Vtrigger_SetValue(uint8 value) 
{
    #if (CY_PSOC5A)
        uint8 Vtrigger_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    Vtrigger_Data = value;                /*  Set Value  */

    /* PSOC5A requires a double write */
    /* Exit Critical Section */
    #if (CY_PSOC5A)
        Vtrigger_Data = value;
        CyExitCriticalSection(Vtrigger_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: Vtrigger_DacTrim
********************************************************************************
*
* Summary:
*  Set the trim value for the given range.
*
* Parameters:
*  range:  1V or 4V range.  See constants.
*
* Return:
*  void
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void Vtrigger_DacTrim(void) 
{
    uint8 mode;

    mode = (uint8)((Vtrigger_CR0 & Vtrigger_RANGE_MASK) >> 2) + Vtrigger_TRIM_M7_1V_RNG_OFFSET;
    Vtrigger_TR = CY_GET_XTND_REG8((uint8 *)(Vtrigger_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
