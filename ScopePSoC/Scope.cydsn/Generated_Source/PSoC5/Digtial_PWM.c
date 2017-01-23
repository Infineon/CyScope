/*******************************************************************************
* File Name: Digtial_PWM.c  
* Version 2.40
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks. 
*  The compare value output may be configured to be active when the present 
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "Digtial_PWM.h"

uint8 Digtial_PWM_initVar = 0u;


/*******************************************************************************
* Function Name: Digtial_PWM_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the 
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:  
*  None  
*
* Return: 
*  None
*
* Global variables:
*  Digtial_PWM_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Digtial_PWM_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(Digtial_PWM_initVar == 0u)
    {
        Digtial_PWM_Init();
        Digtial_PWM_initVar = 1u;
    }
    Digtial_PWM_Enable();

}


/*******************************************************************************
* Function Name: Digtial_PWM_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  Digtial_PWM_Start().
*
* Parameters:  
*  None
*
* Return: 
*  None
*
*******************************************************************************/
void Digtial_PWM_Init(void) 
{
    #if (Digtial_PWM_UsingFixedFunction || Digtial_PWM_UseControl)
        uint8 ctrl;
    #endif /* (Digtial_PWM_UsingFixedFunction || Digtial_PWM_UseControl) */
    
    #if(!Digtial_PWM_UsingFixedFunction) 
        #if(Digtial_PWM_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 Digtial_PWM_interruptState;
        #endif /* (Digtial_PWM_UseStatus) */
    #endif /* (!Digtial_PWM_UsingFixedFunction) */
    
    #if (Digtial_PWM_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        Digtial_PWM_CONTROL |= Digtial_PWM_CFG0_MODE;
        #if (Digtial_PWM_DeadBand2_4)
            Digtial_PWM_CONTROL |= Digtial_PWM_CFG0_DB;
        #endif /* (Digtial_PWM_DeadBand2_4) */
                
        /* Set the default Compare Mode */
        #if(CY_PSOC5A)
                ctrl = Digtial_PWM_CONTROL2 & ((uint8)(~Digtial_PWM_CTRL_CMPMODE1_MASK));
                Digtial_PWM_CONTROL2 = ctrl | Digtial_PWM_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC5A) */
        #if(CY_PSOC3 || CY_PSOC5LP)
                ctrl = Digtial_PWM_CONTROL3 & ((uint8 )(~Digtial_PWM_CTRL_CMPMODE1_MASK));
                Digtial_PWM_CONTROL3 = ctrl | Digtial_PWM_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        Digtial_PWM_RT1 &= ((uint8)(~Digtial_PWM_RT1_MASK));
        Digtial_PWM_RT1 |= Digtial_PWM_SYNC;     
                
        /*Enable DSI Sync all all inputs of the PWM*/
        Digtial_PWM_RT1 &= ((uint8)(~Digtial_PWM_SYNCDSI_MASK));
        Digtial_PWM_RT1 |= Digtial_PWM_SYNCDSI_EN;
       
    #elif (Digtial_PWM_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = Digtial_PWM_CONTROL & ((uint8)(~Digtial_PWM_CTRL_CMPMODE2_MASK)) & ((uint8)(~Digtial_PWM_CTRL_CMPMODE1_MASK));
        Digtial_PWM_CONTROL = ctrl | Digtial_PWM_DEFAULT_COMPARE2_MODE | 
                                   Digtial_PWM_DEFAULT_COMPARE1_MODE;
    #endif /* (Digtial_PWM_UsingFixedFunction) */
        
    #if (!Digtial_PWM_UsingFixedFunction)
        #if (Digtial_PWM_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            Digtial_PWM_AUX_CONTROLDP0 |= (Digtial_PWM_AUX_CTRL_FIFO0_CLR);
        #else /* (Digtial_PWM_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            Digtial_PWM_AUX_CONTROLDP0 |= (Digtial_PWM_AUX_CTRL_FIFO0_CLR);
            Digtial_PWM_AUX_CONTROLDP1 |= (Digtial_PWM_AUX_CTRL_FIFO0_CLR);
        #endif /* (Digtial_PWM_Resolution == 8) */

        Digtial_PWM_WriteCounter(Digtial_PWM_INIT_PERIOD_VALUE);
    #endif /* (!Digtial_PWM_UsingFixedFunction) */
        
    Digtial_PWM_WritePeriod(Digtial_PWM_INIT_PERIOD_VALUE);

        #if (Digtial_PWM_UseOneCompareMode)
            Digtial_PWM_WriteCompare(Digtial_PWM_INIT_COMPARE_VALUE1);
        #else
            Digtial_PWM_WriteCompare1(Digtial_PWM_INIT_COMPARE_VALUE1);
            Digtial_PWM_WriteCompare2(Digtial_PWM_INIT_COMPARE_VALUE2);
        #endif /* (Digtial_PWM_UseOneCompareMode) */
        
        #if (Digtial_PWM_KillModeMinTime)
            Digtial_PWM_WriteKillTime(Digtial_PWM_MinimumKillTime);
        #endif /* (Digtial_PWM_KillModeMinTime) */
        
        #if (Digtial_PWM_DeadBandUsed)
            Digtial_PWM_WriteDeadTime(Digtial_PWM_INIT_DEAD_TIME);
        #endif /* (Digtial_PWM_DeadBandUsed) */

    #if (Digtial_PWM_UseStatus || Digtial_PWM_UsingFixedFunction)
        Digtial_PWM_SetInterruptMode(Digtial_PWM_INIT_INTERRUPTS_MODE);
    #endif /* (Digtial_PWM_UseStatus || Digtial_PWM_UsingFixedFunction) */
        
    #if (Digtial_PWM_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        Digtial_PWM_GLOBAL_ENABLE |= Digtial_PWM_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        Digtial_PWM_CONTROL2 |= Digtial_PWM_CTRL2_IRQ_SEL;
    #else
        #if(Digtial_PWM_UseStatus)
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Digtial_PWM_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Digtial_PWM_STATUS_AUX_CTRL |= Digtial_PWM_STATUS_ACTL_INT_EN_MASK;
            
             /* Exit Critical Region*/
            CyExitCriticalSection(Digtial_PWM_interruptState);
            
            /* Clear the FIFO to enable the Digtial_PWM_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            Digtial_PWM_ClearFIFO();
        #endif /* (Digtial_PWM_UseStatus) */
    #endif /* (Digtial_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Digtial_PWM_Enable
********************************************************************************
*
* Summary: 
*  Enables the PWM block operation
*
* Parameters:  
*  None
*
* Return: 
*  None
*
* Side Effects: 
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void Digtial_PWM_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Digtial_PWM_UsingFixedFunction)
        Digtial_PWM_GLOBAL_ENABLE |= Digtial_PWM_BLOCK_EN_MASK;
        Digtial_PWM_GLOBAL_STBY_ENABLE |= Digtial_PWM_BLOCK_STBY_EN_MASK;
    #endif /* (Digtial_PWM_UsingFixedFunction) */
    
    /* Enable the PWM from the control register  */
    #if (Digtial_PWM_UseControl || Digtial_PWM_UsingFixedFunction)
        Digtial_PWM_CONTROL |= Digtial_PWM_CTRL_ENABLE;
    #endif /* (Digtial_PWM_UseControl || Digtial_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Digtial_PWM_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:  
*  None  
*
* Return: 
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void Digtial_PWM_Stop(void) 
{
    #if (Digtial_PWM_UseControl || Digtial_PWM_UsingFixedFunction)
        Digtial_PWM_CONTROL &= ((uint8)(~Digtial_PWM_CTRL_ENABLE));
    #endif /* (Digtial_PWM_UseControl || Digtial_PWM_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Digtial_PWM_UsingFixedFunction)
        Digtial_PWM_GLOBAL_ENABLE &= ((uint8)(~Digtial_PWM_BLOCK_EN_MASK));
        Digtial_PWM_GLOBAL_STBY_ENABLE &= ((uint8)(~Digtial_PWM_BLOCK_STBY_EN_MASK));
    #endif /* (Digtial_PWM_UsingFixedFunction) */
}


#if (Digtial_PWM_UseOneCompareMode)
	#if (Digtial_PWM_CompareMode1SW)


		/*******************************************************************************
		* Function Name: Digtial_PWM_SetCompareMode
		********************************************************************************
		* 
		* Summary:
		*  This function writes the Compare Mode for the pwm output when in Dither mode,
		*  Center Align Mode or One Output Mode.
		*
		* Parameters:
		*  comparemode:  The new compare mode for the PWM output. Use the compare types
		*                defined in the H file as input arguments.
		*
		* Return:
		*  None
		*
		*******************************************************************************/
		void Digtial_PWM_SetCompareMode(uint8 comparemode) 
		{
		    #if(Digtial_PWM_UsingFixedFunction)
            
                #if(0 != Digtial_PWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << Digtial_PWM_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != Digtial_PWM_CTRL_CMPMODE1_SHIFT) */
            
		        #if(CY_PSOC5A)
		            Digtial_PWM_CONTROL2 &= ((uint8)(~Digtial_PWM_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
		            Digtial_PWM_CONTROL2 |= comparemodemasked;  
		        #endif /* (CY_PSOC5A) */
		                
		        #if(CY_PSOC3 || CY_PSOC5LP)
		            Digtial_PWM_CONTROL3 &= ((uint8)(~Digtial_PWM_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
		            Digtial_PWM_CONTROL3 |= comparemodemasked;     
		        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
		                
		    #elif (Digtial_PWM_UseControl)
		        
                #if(0 != Digtial_PWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << Digtial_PWM_CTRL_CMPMODE1_SHIFT)) & 
    		                                    Digtial_PWM_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & Digtial_PWM_CTRL_CMPMODE1_MASK;                
                #endif /* (0 != Digtial_PWM_CTRL_CMPMODE1_SHIFT) */
                
                #if(0 != Digtial_PWM_CTRL_CMPMODE2_SHIFT)                            
    		        uint8 comparemode2masked = ((uint8)((uint8)comparemode << Digtial_PWM_CTRL_CMPMODE2_SHIFT)) & 
    		                                   Digtial_PWM_CTRL_CMPMODE2_MASK;
                #else
    		        uint8 comparemode2masked = comparemode & Digtial_PWM_CTRL_CMPMODE2_MASK;                
                #endif /* (0 != Digtial_PWM_CTRL_CMPMODE2_SHIFT) */
                
		        /*Clear existing mode */
		        Digtial_PWM_CONTROL &= ((uint8)(~(Digtial_PWM_CTRL_CMPMODE1_MASK | Digtial_PWM_CTRL_CMPMODE2_MASK))); 
		        Digtial_PWM_CONTROL |= (comparemode1masked | comparemode2masked);
		        
		    #else
		        uint8 temp = comparemode;
		    #endif /* (Digtial_PWM_UsingFixedFunction) */
		}
	#endif /* Digtial_PWM_CompareMode1SW */

#else /* UseOneCompareMode */

	#if (Digtial_PWM_CompareMode1SW)


		/*******************************************************************************
		* Function Name: Digtial_PWM_SetCompareMode1
		********************************************************************************
		* 
		* Summary:
		*  This function writes the Compare Mode for the pwm or pwm1 output
		*
		* Parameters:  
		*  comparemode:  The new compare mode for the PWM output. Use the compare types
		*                defined in the H file as input arguments.
		*
		* Return: 
		*  None
		*
		*******************************************************************************/
		void Digtial_PWM_SetCompareMode1(uint8 comparemode) 
		{
		    #if(0 != Digtial_PWM_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << Digtial_PWM_CTRL_CMPMODE1_SHIFT)) & 
    		                               Digtial_PWM_CTRL_CMPMODE1_MASK;
		    #else
                uint8 comparemodemasked = comparemode & Digtial_PWM_CTRL_CMPMODE1_MASK;                
            #endif /* (0 != Digtial_PWM_CTRL_CMPMODE1_SHIFT) */
                   
		    #if (Digtial_PWM_UseControl)
		        Digtial_PWM_CONTROL &= ((uint8)(~Digtial_PWM_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
		        Digtial_PWM_CONTROL |= comparemodemasked;
		    #endif /* (Digtial_PWM_UseControl) */
		}
	#endif /* Digtial_PWM_CompareMode1SW */

#if (Digtial_PWM_CompareMode2SW)


    /*******************************************************************************
    * Function Name: Digtial_PWM_SetCompareMode2
    ********************************************************************************
    * 
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:  
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void Digtial_PWM_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != Digtial_PWM_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << Digtial_PWM_CTRL_CMPMODE2_SHIFT)) & 
                                                 Digtial_PWM_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & Digtial_PWM_CTRL_CMPMODE2_MASK;            
        #endif /* (0 != Digtial_PWM_CTRL_CMPMODE2_SHIFT) */
        
        #if (Digtial_PWM_UseControl)
            Digtial_PWM_CONTROL &= ((uint8)(~Digtial_PWM_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            Digtial_PWM_CONTROL |= comparemodemasked;
        #endif /* (Digtial_PWM_UseControl) */
    }
    #endif /*Digtial_PWM_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!Digtial_PWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Digtial_PWM_WriteCounter
    ********************************************************************************
    * 
    * Summary:
    *  This function is used to change the counter value.
    *
    * Parameters:  
    *  counter:  This value may be between 1 and (2^Resolution)-1.   
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void Digtial_PWM_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(Digtial_PWM_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: Digtial_PWM_ReadCounter
    ********************************************************************************
    * 
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:  
    *  None  
    *
    * Return: 
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 Digtial_PWM_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
        (void)Digtial_PWM_COUNTERCAP_LSB;
        
        /* Read the data from the FIFO (or capture register for Fixed Function)*/
        return (CY_GET_REG8(Digtial_PWM_CAPTURE_LSB_PTR));
    }

        #if (Digtial_PWM_UseStatus)


            /*******************************************************************************
            * Function Name: Digtial_PWM_ClearFIFO
            ********************************************************************************
            * 
            * Summary:
            *  This function clears all capture data from the capture FIFO
            *
            * Parameters:  
            *  None
            *
            * Return: 
            *  None
            *
            *******************************************************************************/
            void Digtial_PWM_ClearFIFO(void) 
            {
                while(0u != (Digtial_PWM_ReadStatusRegister() & Digtial_PWM_STATUS_FIFONEMPTY))
                {
                    (void)Digtial_PWM_ReadCapture();
                }
            }

        #endif /* Digtial_PWM_UseStatus */

#endif /* !Digtial_PWM_UsingFixedFunction */


/*******************************************************************************
* Function Name: Digtial_PWM_WritePeriod
********************************************************************************
* 
* Summary:
*  This function is used to change the period of the counter.  The new period 
*  will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0 
*           will result in the counter remaining at zero.
*
* Return: 
*  None
*
*******************************************************************************/
void Digtial_PWM_WritePeriod(uint8 period) 
{
    #if(Digtial_PWM_UsingFixedFunction)
        CY_SET_REG16(Digtial_PWM_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(Digtial_PWM_PERIOD_LSB_PTR, period);
    #endif /* (Digtial_PWM_UsingFixedFunction) */
}

#if (Digtial_PWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Digtial_PWM_WriteCompare
    ********************************************************************************
    * 
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock. 
    *  The compare output will be driven high when the present counter value is 
    *  compared to the compare value based on the compare mode defined in 
    *  Dither Mode.
    *
    * Parameters:  
    *  compare:  New compare value.  
    *
    * Return: 
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void Digtial_PWM_WriteCompare(uint8 compare) \
                                       
    {	
		#if(Digtial_PWM_UsingFixedFunction)
			CY_SET_REG16(Digtial_PWM_COMPARE1_LSB_PTR, (uint16)compare);
		#else
	        CY_SET_REG8(Digtial_PWM_COMPARE1_LSB_PTR, compare);	
		#endif /* (Digtial_PWM_UsingFixedFunction) */
        
        #if (Digtial_PWM_PWMMode == Digtial_PWM__B_PWM__DITHER)
            #if(Digtial_PWM_UsingFixedFunction)
    			CY_SET_REG16(Digtial_PWM_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
    		#else
    	        CY_SET_REG8(Digtial_PWM_COMPARE2_LSB_PTR, (compare + 1u));	
    		#endif /* (Digtial_PWM_UsingFixedFunction) */
        #endif /* (Digtial_PWM_PWMMode == Digtial_PWM__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: Digtial_PWM_WriteCompare1
    ********************************************************************************
    * 
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will 
    *  reflect the new value on the next UDB clock.  The compare output will be 
    *  driven high when the present counter value is less than or less than or 
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:  
    *  compare:  New compare value.  
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void Digtial_PWM_WriteCompare1(uint8 compare) \
                                        
    {
        #if(Digtial_PWM_UsingFixedFunction)
            CY_SET_REG16(Digtial_PWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(Digtial_PWM_COMPARE1_LSB_PTR, compare);
        #endif /* (Digtial_PWM_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: Digtial_PWM_WriteCompare2
    ********************************************************************************
    * 
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.  
    *  The compare output will reflect the new value on the next UDB clock.  
    *  The compare output will be driven high when the present counter value is 
    *  less than or less than or equal to the compare register, depending on the 
    *  mode.
    *
    * Parameters:  
    *  compare:  New compare value.  
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void Digtial_PWM_WriteCompare2(uint8 compare) \
                                        
    {
        #if(Digtial_PWM_UsingFixedFunction)
            CY_SET_REG16(Digtial_PWM_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(Digtial_PWM_COMPARE2_LSB_PTR, compare);
        #endif /* (Digtial_PWM_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (Digtial_PWM_DeadBandUsed)


    /*******************************************************************************
    * Function Name: Digtial_PWM_WriteDeadTime
    ********************************************************************************
    * 
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:  
    *  deadtime:  Number of counts for dead time 
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void Digtial_PWM_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!Digtial_PWM_DeadBand2_4)
            CY_SET_REG8(Digtial_PWM_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */        
            /* Clear existing data */
            Digtial_PWM_DEADBAND_COUNT &= ((uint8)(~Digtial_PWM_DEADBAND_COUNT_MASK));
            
            /* Set new dead time */
            #if(Digtial_PWM_DEADBAND_COUNT_SHIFT)        
                Digtial_PWM_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << Digtial_PWM_DEADBAND_COUNT_SHIFT)) & 
                                                    Digtial_PWM_DEADBAND_COUNT_MASK;
            #else
                Digtial_PWM_DEADBAND_COUNT |= deadtime & Digtial_PWM_DEADBAND_COUNT_MASK;        
            #endif /* (Digtial_PWM_DEADBAND_COUNT_SHIFT) */
        
        #endif /* (!Digtial_PWM_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: Digtial_PWM_ReadDeadTime
    ********************************************************************************
    * 
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 Digtial_PWM_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!Digtial_PWM_DeadBand2_4)
            return (CY_GET_REG8(Digtial_PWM_DEADBAND_COUNT_PTR));
        #else
        
            /* Otherwise the data has to be masked and offset */
            #if(Digtial_PWM_DEADBAND_COUNT_SHIFT)      
                return ((uint8)(((uint8)(Digtial_PWM_DEADBAND_COUNT & Digtial_PWM_DEADBAND_COUNT_MASK)) >> 
                                                                           Digtial_PWM_DEADBAND_COUNT_SHIFT));
            #else
                return (Digtial_PWM_DEADBAND_COUNT & Digtial_PWM_DEADBAND_COUNT_MASK);
            #endif /* (Digtial_PWM_DEADBAND_COUNT_SHIFT) */
        #endif /* (!Digtial_PWM_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (Digtial_PWM_UseStatus || Digtial_PWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Digtial_PWM_SetInterruptMode
    ********************************************************************************
    * 
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt 
    *  source status register.
    *
    * Parameters:  
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled 
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void Digtial_PWM_SetInterruptMode(uint8 interruptMode)  
    {
    	CY_SET_REG8(Digtial_PWM_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: Digtial_PWM_ReadStatusRegister
    ********************************************************************************
    * 
    * Summary:
    *  This function returns the current state of the status register. 
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 Digtial_PWM_ReadStatusRegister(void)   
    {
    	uint8 result;
    	
    	result = CY_GET_REG8(Digtial_PWM_STATUS_PTR);
    	return (result);
    }

#endif /* (Digtial_PWM_UseStatus || Digtial_PWM_UsingFixedFunction) */


#if (Digtial_PWM_UseControl)


    /*******************************************************************************
    * Function Name: Digtial_PWM_ReadControlRegister
    ********************************************************************************
    * 
    * Summary:
    *  Returns the current state of the control register. This API is available 
    *  only if the control register is not removed.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  uint8 : Current control register value
    *
    *******************************************************************************/    
    uint8 Digtial_PWM_ReadControlRegister(void) 
    {
    	uint8 result;
    	
    	result = CY_GET_REG8(Digtial_PWM_CONTROL_PTR);
    	return (result);
    }


    /*******************************************************************************
    * Function Name: Digtial_PWM_WriteControlRegister
    ********************************************************************************
    * 
    * Summary:
    *  Sets the bit field of the control register. This API is available only if 
    *  the control register is not removed.
    *
    * Parameters:  
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return: 
    *  None
    *
    *******************************************************************************/  
    void Digtial_PWM_WriteControlRegister(uint8 control) 
    {
    	CY_SET_REG8(Digtial_PWM_CONTROL_PTR, control);
    }
	
#endif /* (Digtial_PWM_UseControl) */


#if (!Digtial_PWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Digtial_PWM_ReadCapture
    ********************************************************************************
    * 
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/  
    uint8 Digtial_PWM_ReadCapture(void)  
    {
    	return (CY_GET_REG8(Digtial_PWM_CAPTURE_LSB_PTR));
    }
	
#endif /* (!Digtial_PWM_UsingFixedFunction) */


#if (Digtial_PWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Digtial_PWM_ReadCompare
    ********************************************************************************
    * 
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is 
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/  
    uint8 Digtial_PWM_ReadCompare(void)  
    {
		#if(Digtial_PWM_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(Digtial_PWM_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(Digtial_PWM_COMPARE1_LSB_PTR));
        #endif /* (Digtial_PWM_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: Digtial_PWM_ReadCompare1
    ********************************************************************************
    * 
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/  
    uint8 Digtial_PWM_ReadCompare1(void) 
    {
		return (CY_GET_REG8(Digtial_PWM_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: Digtial_PWM_ReadCompare2
    ********************************************************************************
    * 
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/  
    uint8 Digtial_PWM_ReadCompare2(void)  
    {
		return (CY_GET_REG8(Digtial_PWM_COMPARE2_LSB_PTR));
    }

#endif /* (Digtial_PWM_UseOneCompareMode) */


/*******************************************************************************
* Function Name: Digtial_PWM_ReadPeriod
********************************************************************************
* 
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:  
*  None
*
* Return: 
*  uint8/16: Period value
*
*******************************************************************************/ 
uint8 Digtial_PWM_ReadPeriod(void) 
{
	#if(Digtial_PWM_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Digtial_PWM_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(Digtial_PWM_PERIOD_LSB_PTR));
    #endif /* (Digtial_PWM_UsingFixedFunction) */
}

#if ( Digtial_PWM_KillModeMinTime)


    /*******************************************************************************
    * Function Name: Digtial_PWM_WriteKillTime
    ********************************************************************************
    * 
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode 
    *  is set to Minimum Time.
    *
    * Parameters:  
    *  uint8: Minimum Time kill counts
    *
    * Return: 
    *  None
    *
    *******************************************************************************/ 
    void Digtial_PWM_WriteKillTime(uint8 killtime) 
    {
    	CY_SET_REG8(Digtial_PWM_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: Digtial_PWM_ReadKillTime
    ********************************************************************************
    * 
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set 
    *  to Minimum Time.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/ 
    uint8 Digtial_PWM_ReadKillTime(void) 
    {
    	return (CY_GET_REG8(Digtial_PWM_KILLMODEMINTIME_PTR));
    }

#endif /* ( Digtial_PWM_KillModeMinTime) */

/* [] END OF FILE */
