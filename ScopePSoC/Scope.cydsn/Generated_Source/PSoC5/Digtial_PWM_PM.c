/*******************************************************************************
* File Name: Digtial_PWM_PM.c
* Version 2.40
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cytypes.h"
#include "Digtial_PWM.h"

static Digtial_PWM_backupStruct Digtial_PWM_backup;


/*******************************************************************************
* Function Name: Digtial_PWM_SaveConfig
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
*  Digtial_PWM_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Digtial_PWM_SaveConfig(void) 
{
    
    #if(!Digtial_PWM_UsingFixedFunction)
        #if (CY_UDB_V0)
            Digtial_PWM_backup.PWMUdb = Digtial_PWM_ReadCounter();
            Digtial_PWM_backup.PWMPeriod = Digtial_PWM_ReadPeriod();
            #if (Digtial_PWM_UseStatus)
                Digtial_PWM_backup.InterruptMaskValue = Digtial_PWM_STATUS_MASK;
            #endif /* (Digtial_PWM_UseStatus) */
            
            #if(Digtial_PWM_UseOneCompareMode)
                Digtial_PWM_backup.PWMCompareValue = Digtial_PWM_ReadCompare();
            #else
                Digtial_PWM_backup.PWMCompareValue1 = Digtial_PWM_ReadCompare1();
                Digtial_PWM_backup.PWMCompareValue2 = Digtial_PWM_ReadCompare2();
            #endif /* (Digtial_PWM_UseOneCompareMode) */
            
           #if(Digtial_PWM_DeadBandUsed)
                Digtial_PWM_backup.PWMdeadBandValue = Digtial_PWM_ReadDeadTime();
            #endif /* (Digtial_PWM_DeadBandUsed) */
          
            #if ( Digtial_PWM_KillModeMinTime)
                Digtial_PWM_backup.PWMKillCounterPeriod = Digtial_PWM_ReadKillTime();
            #endif /* ( Digtial_PWM_KillModeMinTime) */
        #endif /* (CY_UDB_V0) */
        
        #if (CY_UDB_V1)
            #if(!Digtial_PWM_PWMModeIsCenterAligned)
                Digtial_PWM_backup.PWMPeriod = Digtial_PWM_ReadPeriod();
            #endif /* (!Digtial_PWM_PWMModeIsCenterAligned) */
            Digtial_PWM_backup.PWMUdb = Digtial_PWM_ReadCounter();
            #if (Digtial_PWM_UseStatus)
                Digtial_PWM_backup.InterruptMaskValue = Digtial_PWM_STATUS_MASK;
            #endif /* (Digtial_PWM_UseStatus) */
            
            #if(Digtial_PWM_DeadBandMode == Digtial_PWM__B_PWM__DBM_256_CLOCKS || \
                Digtial_PWM_DeadBandMode == Digtial_PWM__B_PWM__DBM_2_4_CLOCKS)
                Digtial_PWM_backup.PWMdeadBandValue = Digtial_PWM_ReadDeadTime();
            #endif /*  deadband count is either 2-4 clocks or 256 clocks */
            
            #if(Digtial_PWM_KillModeMinTime)
                 Digtial_PWM_backup.PWMKillCounterPeriod = Digtial_PWM_ReadKillTime();
            #endif /* (Digtial_PWM_KillModeMinTime) */
        #endif /* (CY_UDB_V1) */
        
        #if(Digtial_PWM_UseControl)
            Digtial_PWM_backup.PWMControlRegister = Digtial_PWM_ReadControlRegister();
        #endif /* (Digtial_PWM_UseControl) */
    #endif  /* (!Digtial_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Digtial_PWM_RestoreConfig
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
*  Digtial_PWM_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Digtial_PWM_RestoreConfig(void) 
{
        #if(!Digtial_PWM_UsingFixedFunction)
            #if (CY_UDB_V0)
                /* Interrupt State Backup for Critical Region*/
                uint8 Digtial_PWM_interruptState;
                /* Enter Critical Region*/
                Digtial_PWM_interruptState = CyEnterCriticalSection();
                #if (Digtial_PWM_UseStatus)
                    /* Use the interrupt output of the status register for IRQ output */
                    Digtial_PWM_STATUS_AUX_CTRL |= Digtial_PWM_STATUS_ACTL_INT_EN_MASK;
                    
                    Digtial_PWM_STATUS_MASK = Digtial_PWM_backup.InterruptMaskValue;
                #endif /* (Digtial_PWM_UseStatus) */
                
                #if (Digtial_PWM_Resolution == 8)
                    /* Set FIFO 0 to 1 byte register for period*/
                    Digtial_PWM_AUX_CONTROLDP0 |= (Digtial_PWM_AUX_CTRL_FIFO0_CLR);
                #else /* (Digtial_PWM_Resolution == 16)*/
                    /* Set FIFO 0 to 1 byte register for period */
                    Digtial_PWM_AUX_CONTROLDP0 |= (Digtial_PWM_AUX_CTRL_FIFO0_CLR);
                    Digtial_PWM_AUX_CONTROLDP1 |= (Digtial_PWM_AUX_CTRL_FIFO0_CLR);
                #endif /* (Digtial_PWM_Resolution == 8) */
                /* Exit Critical Region*/
                CyExitCriticalSection(Digtial_PWM_interruptState);
                
                Digtial_PWM_WriteCounter(Digtial_PWM_backup.PWMUdb);
                Digtial_PWM_WritePeriod(Digtial_PWM_backup.PWMPeriod);
                
                #if(Digtial_PWM_UseOneCompareMode)
                    Digtial_PWM_WriteCompare(Digtial_PWM_backup.PWMCompareValue);
                #else
                    Digtial_PWM_WriteCompare1(Digtial_PWM_backup.PWMCompareValue1);
                    Digtial_PWM_WriteCompare2(Digtial_PWM_backup.PWMCompareValue2);
                #endif /* (Digtial_PWM_UseOneCompareMode) */
                
               #if(Digtial_PWM_DeadBandMode == Digtial_PWM__B_PWM__DBM_256_CLOCKS || \
                   Digtial_PWM_DeadBandMode == Digtial_PWM__B_PWM__DBM_2_4_CLOCKS)
                    Digtial_PWM_WriteDeadTime(Digtial_PWM_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
                #if ( Digtial_PWM_KillModeMinTime)
                    Digtial_PWM_WriteKillTime(Digtial_PWM_backup.PWMKillCounterPeriod);
                #endif /* ( Digtial_PWM_KillModeMinTime) */
            #endif /* (CY_UDB_V0) */
            
            #if (CY_UDB_V1)
                #if(!Digtial_PWM_PWMModeIsCenterAligned)
                    Digtial_PWM_WritePeriod(Digtial_PWM_backup.PWMPeriod);
                #endif /* (!Digtial_PWM_PWMModeIsCenterAligned) */
                Digtial_PWM_WriteCounter(Digtial_PWM_backup.PWMUdb);
                #if (Digtial_PWM_UseStatus)
                    Digtial_PWM_STATUS_MASK = Digtial_PWM_backup.InterruptMaskValue;
                #endif /* (Digtial_PWM_UseStatus) */
                
                #if(Digtial_PWM_DeadBandMode == Digtial_PWM__B_PWM__DBM_256_CLOCKS || \
                    Digtial_PWM_DeadBandMode == Digtial_PWM__B_PWM__DBM_2_4_CLOCKS)
                    Digtial_PWM_WriteDeadTime(Digtial_PWM_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
                
                #if(Digtial_PWM_KillModeMinTime)
                    Digtial_PWM_WriteKillTime(Digtial_PWM_backup.PWMKillCounterPeriod);
                #endif /* (Digtial_PWM_KillModeMinTime) */
            #endif /* (CY_UDB_V1) */
            
            #if(Digtial_PWM_UseControl)
                Digtial_PWM_WriteControlRegister(Digtial_PWM_backup.PWMControlRegister); 
            #endif /* (Digtial_PWM_UseControl) */
        #endif  /* (!Digtial_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Digtial_PWM_Sleep
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
*  Digtial_PWM_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Digtial_PWM_Sleep(void) 
{
    #if(Digtial_PWM_UseControl)
        if(Digtial_PWM_CTRL_ENABLE == (Digtial_PWM_CONTROL & Digtial_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Digtial_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Digtial_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Digtial_PWM_UseControl) */

    /* Stop component */
    Digtial_PWM_Stop();
    
    /* Save registers configuration */
    Digtial_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Digtial_PWM_Wakeup
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
*  Digtial_PWM_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Digtial_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Digtial_PWM_RestoreConfig();
    
    if(Digtial_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Digtial_PWM_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
