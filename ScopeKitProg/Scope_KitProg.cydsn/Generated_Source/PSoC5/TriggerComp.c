/*******************************************************************************
* File Name: TriggerComp.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the Comparator component
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "TriggerComp.h"

uint8 TriggerComp_initVar = 0u;

/* Internal functions definitoin */
static void TriggerComp_trimAdjust(uint8 nibble) ;

/* static TriggerComp_backupStruct  TriggerComp_backup; */
#if (CY_PSOC5A)
    static TriggerComp_LOWPOWER_BACKUP_STRUCT  TriggerComp_lowPowerBackup;
#endif /* CY_PSOC5A */

/* variable to decide whether or not to restore the control register in 
   Enable() API for PSoC5A only */
#if (CY_PSOC5A)
    static uint8 TriggerComp_restoreReg = 0u;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: TriggerComp_Init
********************************************************************************
*
* Summary:
*  Initialize to the schematic state
* 
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void TriggerComp_Init(void) 
{
    /* Set default speed/power */
    TriggerComp_SetSpeed(TriggerComp_DEFAULT_SPEED);

    /* Set default Hysteresis */
    #if ( TriggerComp_DEFAULT_HYSTERESIS == 0u )
        TriggerComp_CR |= TriggerComp_HYST_OFF;
    #else
        TriggerComp_CR &= (uint8)(~TriggerComp_HYST_OFF);
    #endif /* TriggerComp_DEFAULT_HYSTERESIS == 0u */
    /* Power down override feature is not supported for PSoC5A. */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Set default Power Down Override */
        #if ( TriggerComp_DEFAULT_PWRDWN_OVRD == 0u )
            TriggerComp_CR &= (uint8)(~TriggerComp_PWRDWN_OVRD);
        #else 
            TriggerComp_CR |= TriggerComp_PWRDWN_OVRD;
        #endif /* TriggerComp_DEFAULT_PWRDWN_OVRD == 0u */
    #endif /* CY_PSOC3 || CY_PSOC5LP */
    
    /* Set mux always on logic */
    TriggerComp_CR |= TriggerComp_MX_AO;

    /* Set default sync */
    TriggerComp_CLK &= (uint8)(~TriggerComp_SYNCCLK_MASK);
    #if ( TriggerComp_DEFAULT_BYPASS_SYNC == 0u )
        TriggerComp_CLK |= TriggerComp_SYNC_CLK_EN;
    #else
        TriggerComp_CLK |= TriggerComp_BYPASS_SYNC;
    #endif /* TriggerComp_DEFAULT_BYPASS_SYNC == 0u */
}


/*******************************************************************************
* Function Name: TriggerComp_Enable
********************************************************************************
*
* Summary:
*  Enable the Comparator
* 
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void TriggerComp_Enable(void) 
{
    TriggerComp_PWRMGR |= TriggerComp_ACT_PWR_EN;
    TriggerComp_STBY_PWRMGR |= TriggerComp_STBY_PWR_EN;
     
     /* This is to restore the value of register CR which is saved 
    in prior to the modification in stop() API */
    #if (CY_PSOC5A)
        if(TriggerComp_restoreReg == 1u)
        {
            TriggerComp_CR = TriggerComp_lowPowerBackup.compCRReg;

            /* Clear the flag */
            TriggerComp_restoreReg = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: TriggerComp_Start
********************************************************************************
*
* Summary:
*  The start function initializes the Analog Comparator with the default values.
*
* Parameters:
*  void
*
* Return:
*  void 
*
* Global variables:
*  TriggerComp_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*  
*******************************************************************************/
void TriggerComp_Start(void) 
{

    if ( TriggerComp_initVar == 0u )
    {
        TriggerComp_Init();
        
        TriggerComp_initVar = 1u;
    }   

    /* Enable power to comparator */
    TriggerComp_Enable();    
}


/*******************************************************************************
* Function Name: TriggerComp_Stop
********************************************************************************
*
* Summary:
*  Powers down amplifier to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void TriggerComp_Stop(void) 
{
    /* Disable power to comparator */
    TriggerComp_PWRMGR &= (uint8)(~TriggerComp_ACT_PWR_EN);
    TriggerComp_STBY_PWRMGR &= (uint8)(~TriggerComp_STBY_PWR_EN);
    
    #if (CY_PSOC5A)
        /* Enable the variable */
        TriggerComp_restoreReg = 1u;

        /* Save the control register before clearing it */
        TriggerComp_lowPowerBackup.compCRReg = TriggerComp_CR;
        TriggerComp_CR = TriggerComp_COMP_REG_CLR;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: TriggerComp_SetSpeed
********************************************************************************
*
* Summary:
*  This function sets the speed of the Analog Comparator. The faster the speed
*  the more power that is used.
*
* Parameters:
*  speed: (uint8) Sets operation mode of Comparator
*
* Return:
*  void
*
*******************************************************************************/
void TriggerComp_SetSpeed(uint8 speed) 
{
    /* Clear and Set power level */    
    TriggerComp_CR = (TriggerComp_CR & (uint8)(~TriggerComp_PWR_MODE_MASK)) |
                           (speed & TriggerComp_PWR_MODE_MASK);

    /* Set trim value for high speed comparator */
    if(speed == TriggerComp_HIGHSPEED)
    {
        /* PSoC5A */
        #if (CY_PSOC5A)
            TriggerComp_TR = TriggerComp_HS_TRIM_TR0;
        #endif /* CY_PSOC5A */
        
        /* PSoC3, PSoC5LP or later */
        #if (CY_PSOC3 || CY_PSOC5LP) 
            TriggerComp_TR0 = TriggerComp_HS_TRIM_TR0;
            TriggerComp_TR1 = TriggerComp_HS_TRIM_TR1;
        #endif /* CY_PSOC3 || CY_PSOC5LP */
    }
    else
    {
    /* PSoC5A */
        #if (CY_PSOC5A)
            TriggerComp_TR = TriggerComp_LS_TRIM_TR0;
        #endif /* CY_PSOC5A */
        
        /* PSoC3, PSoC5LP or later */
        #if (CY_PSOC3 || CY_PSOC5LP) 
            TriggerComp_TR0 = TriggerComp_LS_TRIM_TR0;
            TriggerComp_TR1 = TriggerComp_LS_TRIM_TR1;
        #endif /* CY_PSOC3 || CY_PSOC5LP */
    }

}


/*******************************************************************************
* Function Name: TriggerComp_GetCompare
********************************************************************************
*
* Summary:
*  This function returns the comparator output value.
*  This value is not affected by the Polarity parameter.
*  This valuea lways reflects a noninverted state.
*
* Parameters:
*   None
*
* Return:
*  (uint8)  0     - if Pos_Input less than Neg_input
*           non 0 - if Pos_Input greater than Neg_input.
*
*******************************************************************************/
uint8 TriggerComp_GetCompare(void) 
{
    return( TriggerComp_WRK & TriggerComp_CMP_OUT_MASK);
}


/*******************************************************************************
* Function Name: TriggerComp_trimAdjust
********************************************************************************
*
* Summary:
*  This function adjusts the value in the low nibble/high nibble of the Analog 
*  Comparator trim register
*
* Parameters:  
*  nibble:
*      0 -- adjusts the value in the low nibble
*      1 -- adjusts the value in the high nibble
*
* Return:
*  None
*
* Theory: 
*  Function assumes comparator block is setup for trim adjust.
*  Intended to be called from Comp_ZeroCal()
* 
* Side Effects:
*  Routine uses a course 1ms delay following each trim adjustment to allow 
*  the comparator output to respond.
*
*******************************************************************************/
static void TriggerComp_trimAdjust(uint8 nibble) 
{
    uint8 trimCnt, trimCntMax;
    uint8 cmpState;   

    /* get current state of comparator output */
    cmpState = TriggerComp_WRK & TriggerComp_CMP_OUT_MASK;
    
    if (nibble == 0u)
    {    
        /* if comparator output is high, negative offset adjust is required */
        if ( cmpState != 0u )
        {
            /* PSoC5A */
            #if (CY_PSOC5A)
                TriggerComp_TR |= TriggerComp_CMP_TRIM1_DIR;
            #endif /* CY_PSOC5A */
            
            /* PSoC3, PSoC5LP or later */
            #if (CY_PSOC3 || CY_PSOC5LP)
                TriggerComp_TR0 |= TriggerComp_CMP_TR0_DIR;
            #endif /* CY_PSOC3 || CY_PSOC5LP */
        }
    }
    else
    {
        /* if comparator output is low, positive offset adjust is required */
        if ( cmpState == 0u )
        {
            /* PSoC5A */
            #if (CY_PSOC5A)
                TriggerComp_TR |= TriggerComp_CMP_TRIM2_DIR; 
            #endif /* CY_PSOC5A */
            
            /* PSoC3, PSoC5LP or later */
            #if (CY_PSOC3 || CY_PSOC5LP)
                TriggerComp_TR1 |= TriggerComp_CMP_TR1_DIR;
            #endif /* CY_PSOC3 || CY_PSOC5LP */
        }
    }

    /* Increment trim value until compare output changes state */
	
    /* PSoC5A */
	#if (CY_PSOC5A)
	    trimCntMax = 7u;
    #endif
	
	/* PSoC3, PSoC5LP or later */
	#if (CY_PSOC3 || CY_PSOC5LP)
    	if(nibble == 0u)
    	{
    		trimCntMax = 15u;
    	}
    	else
    	{
    		trimCntMax = 7u;
    	}
	#endif
	
    for ( trimCnt = 0u; trimCnt < trimCntMax; trimCnt++ )
	{
        if (nibble == 0u)
        {
            /* PSoC5A */
            #if (CY_PSOC5A)
                TriggerComp_TR += 1u;
            #endif /* CY_PSOC5A */
            
            /* PSoC3, PSoC5LP or later */
            #if (CY_PSOC3 || CY_PSOC5LP)
                TriggerComp_TR0 += 1u;
            #endif /* CY_PSOC3 || CY_PSOC5LP */
        }
        else
        {
            /* PSoC5A */
            #if (CY_PSOC5A)
                TriggerComp_TR += 0x10u;
            #endif /* CY_PSOC5A */
            
            /* PSoC3, PSoC5LP or later */
            #if (CY_PSOC3 || CY_PSOC5LP)
                TriggerComp_TR1 += 1u;
            #endif /* CY_PSOC3 || CY_PSOC5LP */
        }
        
        CyDelayUs(10u);
        
        /* Check for change in comparator output */
        if ((TriggerComp_WRK & TriggerComp_CMP_OUT_MASK) != cmpState)
        {
            break;      /* output changed state, trim phase is complete */
        }        
    }    
}


/*******************************************************************************
* Function Name: TriggerComp_ZeroCal
********************************************************************************
*
* Summary:
*  This function calibrates the offset of the Analog Comparator.
*
* Parameters:
*  None
*
* Return:
*  (uint16)  value written in trim register when calibration complete.
*
* Theory: 
*  This function is used to optimize the calibration for user specific voltage
*  range.  The comparator trim is adjusted to counter transistor offsets
*   - offset is defined as positive if the output transitions to high before inP
*     is greater than inN
*   - offset is defined as negative if the output transitions to high after inP
*     is greater than inP
*
*  PSoC5A
*  The Analog Comparator provides 1 byte for offset trim.  The byte contains two
*  4 bit trim fields - one is a course trim and the other allows smaller
*  offset adjustments only for slow modes.
*  - low nibble - fine trim
*  - high nibble - course trim
*  PSoC3, PSoC5LP or later
*  The Analog Comparator provides 2 bytes for offset trim.  The bytes contain two
*  5 bit trim fields - one is a course trim and the other allows smaller
*  offset adjustments only for slow modes.
*  - TR0 - fine trim
*  - TR1 - course trim
*
*  Trim algorithm is a two phase process
*  The first phase performs course offset adjustment
*  The second phase serves one of two purposes depending on the outcome of the
*  first phase
*  - if the first trim value was maxed out without a comparator output 
*    transition, more offset will be added by adjusting the second trim value.
*  - if the first trim phase resulted in a comparator output transition, the
*    second trim value will serve as fine trim (in the opposite direction)to
*    ensure the offset is < 1 mV.
*
* Trim Process:   
*  1) User applies a voltage to the negative input.  Voltage should be in the
*     comparator operating range or an average of the operating voltage range.
*  2) Clear registers associated with analog routing to the positive input.
*  3) Disable Hysteresis
*  4) Set the calibration bit to short the negative and positive inputs to
*     the users calibration voltage.
*  5) Clear the TR register  ( TR = 0x00 )
*  ** LOW MODES
*  6) Check if compare output is high, if so, set the MSb of course trim field 
*     to a 1.
*  7) Increment the course trim field until the compare output changes
*  8) Check if compare output is low, if so, set the MSb of fine trim field
*     to a 1.
*  9) Increment the fine trim field until the compare output changes
*  ** FAST MODE - skip the steps 8,9
*
* Side Effects:
*  Routine clears analog routing associated with the comparator positive input.  
*  This may affect routing of signals from other components that are connected
*  to the positive input of the comparator.
*
*******************************************************************************/
uint16 TriggerComp_ZeroCal(void) 
{
    uint8 tmpSW0;
    uint8 tmpSW2;
    uint8 tmpSW3;
    uint8 tmpCR;

    /* Save a copy of routing registers associated with inP */
    tmpSW0 = TriggerComp_SW0;
    tmpSW2 = TriggerComp_SW2;
    tmpSW3 = TriggerComp_SW3;

     /* Clear routing for inP, retain routing for inN */
    TriggerComp_SW0 = 0x00u;
    TriggerComp_SW2 = 0x00u;
    TriggerComp_SW3 = tmpSW3 & (uint8)(~TriggerComp_CMP_SW3_INPCTL_MASK);

    /* Preserve original configuration
     * - turn off Hysteresis
     * - set calibration bit - shorts inN to inP
    */
    tmpCR = TriggerComp_CR;
    TriggerComp_CR |= (TriggerComp_CAL_ON | TriggerComp_HYST_OFF);
    
    /* Write default low values to trim register - no offset adjust */
    /* PSoC5A */
    #if (CY_PSOC5A)
        TriggerComp_TR = TriggerComp_DEFAULT_CMP_TRIM;
    #endif /* CY_PSOC5A */
    
    /* PSoC3, PSoC5LP or later */
    #if (CY_PSOC3 || CY_PSOC5LP)
        TriggerComp_TR0 = TriggerComp_DEFAULT_CMP_TRIM;
        TriggerComp_TR1 = TriggerComp_DEFAULT_CMP_TRIM;
    #endif /* CY_PSOC3 || CY_PSOC5LP */
	
	/* Two phase trim - slow modes, one phase trim - for fast */ 
    if ( (TriggerComp_CR & TriggerComp_PWR_MODE_MASK) == TriggerComp_PWR_MODE_FAST)
    {
        TriggerComp_trimAdjust(0u);
    }
    else /* default to trim for fast modes */
    {
        TriggerComp_trimAdjust(1u);
		TriggerComp_trimAdjust(0u);
    }
   
    /* Restore Config Register */
    TriggerComp_CR = tmpCR;
    
    /* Restore routing registers for inP */
    TriggerComp_SW0 = tmpSW0;
    TriggerComp_SW2 = tmpSW2;
    TriggerComp_SW3 = tmpSW3;
    
    /* PSoC5A */
    #if (CY_PSOC5A)
        return (uint16) TriggerComp_TR;
    #endif /* CY_PSOC5A */
    
    /* PSoC3, PSoC5LP or later */
    #if (CY_PSOC3 || CY_PSOC5LP)
        return ((uint16)((uint16)TriggerComp_TR1 << 8u) | (TriggerComp_TR0));        
    #endif /* CY_PSOC3 || CY_PSOC5LP */
}


/*******************************************************************************
* Function Name: TriggerComp_LoadTrim
********************************************************************************
*
* Summary:
*  This function stores a value in the Analog Comparator trim register.
*
* Parameters:  
*  uint8 trimVal - trim value.  This value is the same format as the value 
*  returned by the _ZeroCal routine.
*
* Return:
*  None
*
*******************************************************************************/
void TriggerComp_LoadTrim(uint16 trimVal) 
{
    /* Stores value in the Analog Comparator trim register */
    /* PSoC5A */
    #if (CY_PSOC5A)
        TriggerComp_TR = (uint8) trimVal;
    #endif /* CY_PSOC5A */
    
    /* PSoC3, PSoC5LP or later */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Stores value in the Analog Comparator trim register for P-type load */
        TriggerComp_TR0 = (uint8) trimVal;
        
        /* Stores value in the Analog Comparator trim register for N-type load */
        TriggerComp_TR1 = (uint8) (trimVal >> 8); 
    #endif /* CY_PSOC3 || CY_PSOC5LP */
}


#if (CY_PSOC3 || CY_PSOC5LP)

    /*******************************************************************************
    * Function Name: TriggerComp_PwrDwnOverrideEnable
    ********************************************************************************
    *
    * Summary:
    *  This is the power down over-ride feature. This function ignores sleep 
    *  parameter and allows the component to stay active during sleep mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void TriggerComp_PwrDwnOverrideEnable(void) 
    {
        /* Set the pd_override bit in CMP_CR register */
        TriggerComp_CR |= TriggerComp_PWRDWN_OVRD;
    }


    /*******************************************************************************
    * Function Name: TriggerComp_PwrDwnOverrideDisable
    ********************************************************************************
    *
    * Summary:
    *  This is the power down over-ride feature. This allows the component to stay
    *  inactive during sleep.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void TriggerComp_PwrDwnOverrideDisable(void) 
    {
        /* Reset the pd_override bit in CMP_CR register */
        TriggerComp_CR &= (uint8)(~TriggerComp_PWRDWN_OVRD);
    }
#endif /* (CY_PSOC3 || CY_PSOC5LP) */


/* [] END OF FILE */
