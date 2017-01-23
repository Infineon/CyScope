/*******************************************************************************
* File Name: Amux_Trigger.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_Amux_Trigger_H)
#define CY_AMUX_Amux_Trigger_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void Amux_Trigger_Start(void) ;
#define Amux_Trigger_Init() Amux_Trigger_Start()
void Amux_Trigger_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void Amux_Trigger_Stop(void); */
/* void Amux_Trigger_Select(uint8 channel); */
/* void Amux_Trigger_Connect(uint8 channel); */
/* void Amux_Trigger_Disconnect(uint8 channel); */
/* void Amux_Trigger_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define Amux_Trigger_CHANNELS  2u
#define Amux_Trigger_MUXTYPE   1
#define Amux_Trigger_ATMOSTONE 1

/***************************************
*             API Constants
***************************************/

#define Amux_Trigger_NULL_CHANNEL 0xFFu
#define Amux_Trigger_MUX_SINGLE   1
#define Amux_Trigger_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if Amux_Trigger_MUXTYPE == Amux_Trigger_MUX_SINGLE
# if !Amux_Trigger_ATMOSTONE
#  define Amux_Trigger_Connect(channel) Amux_Trigger_Set(channel)
# endif
# define Amux_Trigger_Disconnect(channel) Amux_Trigger_Unset(channel)
#else
# if !Amux_Trigger_ATMOSTONE
void Amux_Trigger_Connect(uint8 channel) ;
# endif
void Amux_Trigger_Disconnect(uint8 channel) ;
#endif

#if Amux_Trigger_ATMOSTONE
# define Amux_Trigger_Stop() Amux_Trigger_DisconnectAll()
# define Amux_Trigger_Select(channel) Amux_Trigger_FastSelect(channel)
void Amux_Trigger_DisconnectAll(void) ;
#else
# define Amux_Trigger_Stop() Amux_Trigger_Start()
void Amux_Trigger_Select(uint8 channel) ;
# define Amux_Trigger_DisconnectAll() Amux_Trigger_Start()
#endif

#endif /* CY_AMUX_Amux_Trigger_H */


/* [] END OF FILE */
