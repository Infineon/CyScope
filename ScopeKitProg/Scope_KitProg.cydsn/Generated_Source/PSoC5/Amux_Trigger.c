/*******************************************************************************
* File Name: Amux_Trigger.c
* Version 1.80
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Amux_Trigger.h"

static uint8 Amux_Trigger_lastChannel = Amux_Trigger_NULL_CHANNEL;


/*******************************************************************************
* Function Name: Amux_Trigger_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Amux_Trigger_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < Amux_Trigger_CHANNELS ; chan++)
    {
#if (Amux_Trigger_MUXTYPE == Amux_Trigger_MUX_SINGLE)
        Amux_Trigger_Unset(chan);
#else
        Amux_Trigger_CYAMUXSIDE_A_Unset(chan);
        Amux_Trigger_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    Amux_Trigger_lastChannel = Amux_Trigger_NULL_CHANNEL;
}


#if (!Amux_Trigger_ATMOSTONE)
/*******************************************************************************
* Function Name: Amux_Trigger_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Amux_Trigger_Select(uint8 channel) 
{
    Amux_Trigger_DisconnectAll();        /* Disconnect all previous connections */
    Amux_Trigger_Connect(channel);       /* Make the given selection */
    Amux_Trigger_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: Amux_Trigger_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Amux_Trigger_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( Amux_Trigger_lastChannel != Amux_Trigger_NULL_CHANNEL)
    {
        Amux_Trigger_Disconnect(Amux_Trigger_lastChannel);
    }

    /* Make the new channel connection */
#if (Amux_Trigger_MUXTYPE == Amux_Trigger_MUX_SINGLE)
    Amux_Trigger_Set(channel);
#else
    Amux_Trigger_CYAMUXSIDE_A_Set(channel);
    Amux_Trigger_CYAMUXSIDE_B_Set(channel);
#endif


    Amux_Trigger_lastChannel = channel;   /* Update last channel */
}


#if (Amux_Trigger_MUXTYPE == Amux_Trigger_MUX_DIFF)
#if (!Amux_Trigger_ATMOSTONE)
/*******************************************************************************
* Function Name: Amux_Trigger_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Amux_Trigger_Connect(uint8 channel) 
{
    Amux_Trigger_CYAMUXSIDE_A_Set(channel);
    Amux_Trigger_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: Amux_Trigger_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Amux_Trigger_Disconnect(uint8 channel) 
{
    Amux_Trigger_CYAMUXSIDE_A_Unset(channel);
    Amux_Trigger_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (Amux_Trigger_ATMOSTONE)
/*******************************************************************************
* Function Name: Amux_Trigger_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Amux_Trigger_DisconnectAll(void) 
{
    if(Amux_Trigger_lastChannel != Amux_Trigger_NULL_CHANNEL) 
    {
        Amux_Trigger_Disconnect(Amux_Trigger_lastChannel);
        Amux_Trigger_lastChannel = Amux_Trigger_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */
