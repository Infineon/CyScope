/*******************************************************************************
* File Name: Digital_PWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Digital_PWM.h"

static Digital_PWM_backupStruct Digital_PWM_backup;


/*******************************************************************************
* Function Name: Digital_PWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Digital_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Digital_PWM_SaveConfig(void) 
{

    #if(!Digital_PWM_UsingFixedFunction)
        #if(!Digital_PWM_PWMModeIsCenterAligned)
            Digital_PWM_backup.PWMPeriod = Digital_PWM_ReadPeriod();
        #endif /* (!Digital_PWM_PWMModeIsCenterAligned) */
        Digital_PWM_backup.PWMUdb = Digital_PWM_ReadCounter();
        #if (Digital_PWM_UseStatus)
            Digital_PWM_backup.InterruptMaskValue = Digital_PWM_STATUS_MASK;
        #endif /* (Digital_PWM_UseStatus) */

        #if(Digital_PWM_DeadBandMode == Digital_PWM__B_PWM__DBM_256_CLOCKS || \
            Digital_PWM_DeadBandMode == Digital_PWM__B_PWM__DBM_2_4_CLOCKS)
            Digital_PWM_backup.PWMdeadBandValue = Digital_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Digital_PWM_KillModeMinTime)
             Digital_PWM_backup.PWMKillCounterPeriod = Digital_PWM_ReadKillTime();
        #endif /* (Digital_PWM_KillModeMinTime) */

        #if(Digital_PWM_UseControl)
            Digital_PWM_backup.PWMControlRegister = Digital_PWM_ReadControlRegister();
        #endif /* (Digital_PWM_UseControl) */
    #endif  /* (!Digital_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Digital_PWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Digital_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Digital_PWM_RestoreConfig(void) 
{
        #if(!Digital_PWM_UsingFixedFunction)
            #if(!Digital_PWM_PWMModeIsCenterAligned)
                Digital_PWM_WritePeriod(Digital_PWM_backup.PWMPeriod);
            #endif /* (!Digital_PWM_PWMModeIsCenterAligned) */

            Digital_PWM_WriteCounter(Digital_PWM_backup.PWMUdb);

            #if (Digital_PWM_UseStatus)
                Digital_PWM_STATUS_MASK = Digital_PWM_backup.InterruptMaskValue;
            #endif /* (Digital_PWM_UseStatus) */

            #if(Digital_PWM_DeadBandMode == Digital_PWM__B_PWM__DBM_256_CLOCKS || \
                Digital_PWM_DeadBandMode == Digital_PWM__B_PWM__DBM_2_4_CLOCKS)
                Digital_PWM_WriteDeadTime(Digital_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Digital_PWM_KillModeMinTime)
                Digital_PWM_WriteKillTime(Digital_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Digital_PWM_KillModeMinTime) */

            #if(Digital_PWM_UseControl)
                Digital_PWM_WriteControlRegister(Digital_PWM_backup.PWMControlRegister);
            #endif /* (Digital_PWM_UseControl) */
        #endif  /* (!Digital_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Digital_PWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Digital_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Digital_PWM_Sleep(void) 
{
    #if(Digital_PWM_UseControl)
        if(Digital_PWM_CTRL_ENABLE == (Digital_PWM_CONTROL & Digital_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Digital_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Digital_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Digital_PWM_UseControl) */

    /* Stop component */
    Digital_PWM_Stop();

    /* Save registers configuration */
    Digital_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Digital_PWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Digital_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Digital_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Digital_PWM_RestoreConfig();

    if(Digital_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Digital_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
