/* ========================================
 *
 * Copyright Cypress Semiconductor, 2016
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Cypress Semiconductor.
 *
 * ========================================
*/
#include <device.h>
#include <string.h>
#include <stdio.h>

void DMA_Config(void);
void Hardware_Config(void);
uint16 GetLastSampleIndex(uint8 chHandle);
void CommandCheck(void);
void PutString(char * string);  // actually checks to see if the CDC device is ready before sending the string
void PutChar(char Data);

// 2047 is the max buffer size in samples
#define BUFFER_SIZE 1024

// trigger setup control register definitions
#define FALLING_EDGE_TRIGGERED 0x01
#define RISING_EDGE_TRIGGERED 0x02
#define EITHER_EDGE_TRIGGERED 0x04
#define EXTERNAL_TRIGGERED 0x08

#define TRIGGER_MASK 0x0F

#define MANUAL_TRIGGER 0x10
#define TRIGGER_ARM 0x40
#define RESET_PWM 0x80

// analog mux channel selection defines
#define SCOPE_CHANNEL_1 0
#define SCOPE_CHANNEL_2 1

// status register defines
#define COMP_LEVEL 0x01
#define TRIGGERED 0x02
#define CAPTURE_COMPLETE 0x04

// Wave_Control control register definitions
#define WAVE_SELECT_MASK 0x01
#define WAVE_SELECT_WAVE 0x00
#define WAVE_SELECT_NOISE 0x01

/* DMA Configuration for DMA_Channel_1 */
#define DMA_Channel_1_BYTES_PER_BURST 2
#define DMA_Channel_1_REQUEST_PER_BURST 1
#define DMA_Channel_1_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_Channel_1_DST_BASE (CYDEV_SRAM_BASE)

/* DMA Configuration for DMA_Channel_2 */
#define DMA_Channel_2_BYTES_PER_BURST 2
#define DMA_Channel_2_REQUEST_PER_BURST 1
#define DMA_Channel_2_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_Channel_2_DST_BASE (CYDEV_SRAM_BASE)

/* Variable declarations for DMA_Channel_1 */
/* Move these variable declarations to the top of the function */
uint8 DMA_Channel_1_Chan;
uint8 DMA_Channel_1_TD[1];

/* Variable declarations for DMA_Channel_2 */
/* Move these variable declarations to the top of the function */
uint8 DMA_Channel_2_Chan;
uint8 DMA_Channel_2_TD[1];

// RAM data arrays for storing the sampled data
uint16 Channel_1_Data[BUFFER_SIZE];
uint16 Channel_2_Data[BUFFER_SIZE];

// USB UART variables
#define COMMAND_BUFFER_SIZE 64
char szTempBuffer[COMMAND_BUFFER_SIZE];
uint8 cCharsReceived;

#define CIRCULAR_BUFFER_SIZE 4096
char szCommandsBuffer[CIRCULAR_BUFFER_SIZE]; // need to make this many times bigger to deal with the case when I cant keep up with the commands coming in
uint16 iRead, iWrite;
uint16 cCommandsReady;

char szCommand[COMMAND_BUFFER_SIZE];
uint8 bCommandReady;

// scope status variable
uint8 preamp_a, preamp_b, trigger_source, preamp_trig;

// for dumping data out (its 64 bytes, but if you want anything less, it has to be a multiple of 4)
// also a good idea to make it a multiple of the ADC buffer size, defined above
#define USBUART_BUFFER_SIZE 64
uint8 USB_Out_Buffer[USBUART_BUFFER_SIZE];

// Waveform generator variables
#define WAVEFORM_BUFFER_SIZE 256
uint8 Waveform_Buffer[WAVEFORM_BUFFER_SIZE];
uint8 Waveform_Temp_Buffer[WAVEFORM_BUFFER_SIZE];
uint8 wave_gain = 0;
uint8 wave_offset = 0;

//GJL
//// digital input last state variable
//uint8 Digital_Input_Status_Last;
//uint8 Digital_AutoUpdate;

// For the SAR ADC to run at 18MHz, it *must* have a 50% duty cycle. 
// There are 3 frequencies from the PLL that satisfy
// generating an 18 Mhz clock from integer divides off the PLL: 36 Mhz
// 54 Mhz and 72 Mhz.  Of these three choices, 54 Mhz requires a divide of
// 3 which does not produce a 50% duty cycle.

int main()
{
	uint8 status;
	uint16 ending_offset;
	char * psz;
	uint16 avar, bvar, i;
	uint32 phase_int;
	float freq;
	float cvar;
    float wave_temp;
 
    Hardware_Config();
    DMA_Config();

    for(;;)
    {
	
		CommandCheck();
		
		if(bCommandReady != 0)
		{
			psz = strtok(szCommand, " ");
			if(*psz == 'i')
			{
				// it all started here.  NOTE!  \n\r THE ORDER MATTERS!
				PutString("*Syscomp CircuitGear V1.3\n\r");
			}
//GJL
            //			else if(*psz == 'D')
//			{
//				// Digital I/O commands
//				
//				psz = strtok(0, " ");
//				if(*psz == 'O')
//				{
//					// Digital output
//					// D O N, where N is the output byte value as an ascii 8 bit number
//					psz = strtok(0, " ");
//					sscanf(psz,"%hu", &avar);
//					
//					Digital_Out_Control_Write((uint8) avar);
//				}
//				else if(*psz == 'I')
//				{
//					// reads digital inputs
//					// returns Ix where x is an 8 bit (binary) value representing the status
//					// of the digital input pins
//					
//					PutChar('I');
//					PutChar((uint8) Digital_Input_Status_Read());
//				}
//				else if(*psz == 'A')
//				{
//					Digital_AutoUpdate = 1;
//                    PutChar('I');
//					PutChar((uint8) Digital_Input_Status_Read());
//				}
//				else if(*psz == 'a')
//				{
//					Digital_AutoUpdate = 0;
//				}
//				else if(*psz == 'F')
//				{
//					// set PWM output frequency
//			    	// D F N where N is an 8 bit ascii value that corresponds to these frequencies
//					// 0 72 Khz
//					// 1 36 Khz
//					// 2 9 Khz
//					// 3 4.5 Khz
//					// 4 1.125 Khz
//					// 5 564 Hz
//					// 6 281 Hz
//					// 7 141 Hz
//					// 8 70 Hz
//					// 9 35 Hz
//					// 10 - 255 disabled
//					psz = strtok(0, " ");
//					sscanf(psz,"%hu", &avar);
//					
//					switch(avar)
//					{
//						// GJL - change PWM period to 250 to get exact frequency
//                        case 0: // (36 Mhz / 250 (period of the PWM)) / 72 Khz = 2
//							PWM_Clock_SetDividerValue(2);
//							break;
//							
//						case 1:
//							PWM_Clock_SetDividerValue(4);
//							break;
//							
//						case 2:
//							PWM_Clock_SetDividerValue(16);
//							break;
//						
//						case 3:
//							PWM_Clock_SetDividerValue(32);
//							break;
//							
//						case 4:
//							PWM_Clock_SetDividerValue(128);
//							break;
//							
//						case 5:
//							PWM_Clock_SetDividerValue(255);
//							break;
//							
//						case 6:
//							PWM_Clock_SetDividerValue(512);
//							break;
//							
//						case 7:
//							PWM_Clock_SetDividerValue(1021);
//							break;
//							
//						case 8:
//							PWM_Clock_SetDividerValue(2057);
//							break;
//						
//						case 9:
//							PWM_Clock_SetDividerValue(4114);
//							break;
//							
//						default:
//							//PWM_Stop();
//							break;
//					}
//				}
//				else if(*psz == 'D')
//				{
//					// Set PWM Duty cycle
//					// D D N where N is an 8 bit ascii value representing the duty cycle
//					// from 0 to 255
//					psz = strtok(0, " ");
//					sscanf(psz,"%hu", &avar);
//					// GJL: Limit upper compare value to 249 since period is 249
//                    if(avar > 249)
//                    {
//                        avar = 249;
//                    }
//					Digital_PWM_WriteCompare1((uint8) avar);
//					Digital_PWM_WriteCompare2((uint8) avar);
//				}
//				else if(*psz == '!')
//				{
//					// interrupt line configuration.  not implemented yet
//					// sends back a '!' character when the interrupt condition is met
//					// D ! M where M is the mode:
//					// M = D : Disable
//					// M = R : Rising Edge
//					// M = F : Falling Edge
//					// M = H : High Level
//					// M = L : Low Level
//					psz = strtok(0, " ");
//					
//					if(*psz == 'D')
//					{
//					}
//					else if(*psz == 'R')
//					{
//					}
//					else if(*psz == 'F')
//					{
//					}
//					else if(*psz == 'H')
//					{
//					}
//					else if(*psz == 'L')
//					{
//					}
//					
//					// if trigger condition is met, send back the '!' character
//					//PutChar('!');
//				}
//			}
			else if(*psz == 'W')
			{
				// waveform generator commands
				
				psz = strtok(0, " ");
				if(*psz == 'S')
				{
					// sample data into the buffer
					// command is W S ADDR DATA
					// ADDR is the 8 bit address 0 - 255
					// DATA is the waveform amplitude 0 - 255
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					bvar = avar;  // something is wacky with sscanf.  if I dont move the data into bvar, then on the next call to sscanf, even
					// if I use another variable, avar gets reset to zero!
					// got the address
					
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					// got the data
					
					// store it in the buffer
					Waveform_Temp_Buffer[bvar] = avar;
				}
				else if(*psz == 'P')
				{
					// program the waveform by copying it into the array that the waveform generator uses
					for(i = 0; i < WAVEFORM_BUFFER_SIZE; i++)
					{
						// GJL - add offset and max checking
                        wave_temp = (((float) Waveform_Temp_Buffer[i]) * ((float)wave_gain)/255.0) + ((float)wave_offset);
                        if(wave_temp > 255.0)
                        {
                            wave_temp = 255.0;
                        }
                        Waveform_Buffer[i] = (uint8) wave_temp;
					}   
				}
				else if(*psz == 'A')
				{
					// set the waveform amplitude
					// W A A0, where A0 is the a bit amplitude, 0 = 0%, 255 = 100%
					// program the waveform amplitude by copying it into the array that the waveform generator uses
					// but scale it during the copy
					
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					
					wave_gain = avar;
					
					for(i = 0; i < WAVEFORM_BUFFER_SIZE; i++)
					{
						// GJL - add offset and max checking
                        wave_temp = (((float) Waveform_Temp_Buffer[i]) * ((float)wave_gain)/255.0) + ((float)wave_offset);
                        if(wave_temp > 255.0)
                        {
                            wave_temp = 255.0;
                        }
                        Waveform_Buffer[i] = (uint8) wave_temp;
					}              
                    
				}
				// GJL - add offset functionality
                else if(*psz == 'O')
                {
                    // set waveform offset
                    // W O O0 where O0 is the offset - 0 = 0%, 255 = 100%
                    // program teh offset by copying it into the array that the waveform generator uses
                    // but adjust the values upwards during the copy
                    
                    psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					
					wave_offset = avar;
					
					for(i = 0; i < WAVEFORM_BUFFER_SIZE; i++)
					{
						// GJL temp - add offset
                        wave_temp = (((float) Waveform_Temp_Buffer[i]) * ((float)wave_gain)/255.0) + ((float)wave_offset);
                        if(wave_temp > 255.0)
                        {
                            wave_temp = 255.0;
                        }
                        Waveform_Buffer[i] = (uint8) wave_temp;
					}              
                }
                else if(*psz == 'W')
				{
					// select the waveform output
					Wave_Control_Write(Wave_Control_Read() & ~WAVE_SELECT_MASK);
					Wave_Control_Write(Wave_Control_Read() | WAVE_SELECT_WAVE);
				}
				else if(*psz == 'N')
				{
					// select the noise output
					Wave_Control_Write(Wave_Control_Read() & ~WAVE_SELECT_MASK);
					Wave_Control_Write(Wave_Control_Read() | WAVE_SELECT_NOISE);
				}
				else if(*psz == 'F')
				{
					// set the frequency output of the waveform generator
					// best thing to have here is an actual DDS in the hardware
					// but to get this working, I'm just going to fake it by setting
					// the clock divider.  It wont work for everything, but it should be
					// passable.  The command is W F F3 F2 F1 F0 where Fn are 8 bit ascii
					// numbers that are combined into a 32 bit phase accumulator value
					// phase value = desired output frequency / 0.09313225746 Hz
					
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					phase_int = ((uint32) avar)*16777216;
					
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					phase_int += ((uint32) avar)*65536;
					
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					phase_int += ((uint32) avar)*256;
					
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					phase_int += ((uint32) avar);
					
					freq = ((float) phase_int)*.09313225746;
					
					// calculate the clock divider
					phase_int = (int32) (36000000.0/((float) WAVEFORM_BUFFER_SIZE))/freq;
					
					if(phase_int >= 65535)
					{
						phase_int = 65535;
					}
					else if(phase_int <= 15) // anything less than this and the DMA doesn't have time to execute
					{
						phase_int = 15;
					}
					
					Wave_Clock_SetDividerValue((uint16) phase_int);
                    Noise_Clock_SetDividerValue((uint16) phase_int);
					
				}
			}
			else if(*psz == 'S')
			{
				// scope commands
				
				psz = strtok(0, " ");
				if(*psz == 'O')
				{
					// read offset from EEPROM command
					PutChar('O');
					PutChar(128); // A
					PutChar(128); // a
					PutChar(128); // B
					PutChar(128); // b
					
				}
				else if(*psz == 'T')
				{
					// set the trigger level
					// data comes back as two ascii strings, each representing an 8 bit value
					// S T T_HIGH T_LOW where T_HIGH and T_LOW represent a 10 bit number
					// the trigger is set by the following equation:
					// 511 - Gain*Trigger_Voltage/0.052421484375 (383 without the offsets set to 128)
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					bvar = avar*256;
					
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					bvar += avar;
					
					// voltage = (511 - Value)/Gain * 0.05242 -> (511-Value)/1*0.05242/0.016(mv per bit, 4 volt Vdac range) -> (511-value)*3.27635
					// there are 2 gain options, 1x and 10x.
					if(trigger_source == SCOPE_CHANNEL_1)
					{
						preamp_trig = preamp_a;
					}
					else
					{
						preamp_trig = preamp_b;
					}
					
					if(preamp_trig == 1)
					{
						cvar = (511-bvar)*3.3093; // 3.27635 (before emperical correction value)
					}
					else
					{
                        //cvar = (511-bvar)*0.37685; // .327635 (before empirical correction value)
                        cvar = (511-bvar)*0.75668;
					}
					
					if(cvar <= 0.0)
					{
						cvar = 0.0;
					}
					else if(cvar >= 255.0)
					{
						cvar = 255.0;
					}
					Vtrigger_SetValue((uint8) cvar);
				}
				else if(*psz == 'C')
				{
					// Post trigger buffer size
					// data comes back as two ascii strings, each representing an 8 bit value
					// S C C_HIGH C_LOW where C_HIGH and C_LOW represent a 10 bit number
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					bvar = avar*256;
					
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					bvar += avar;
					
					if(bvar >= (BUFFER_SIZE-1))
					{
						bvar = (BUFFER_SIZE-1);  // maximum post trigger buffer size
					}
					else if(bvar <= 2)
					{
						bvar = 2; // minimum post trigger buffer size
					}
					
					// update the period to match the trigger buffer size post trigger capture
					PWM_Sample_Buffer_WritePeriod(bvar); // post trigger buffer
					// after setting the period, need to set the count to the proper value as well
					PWM_Sample_Buffer_WriteCounter(bvar); // post trigger buffer					
				}
				else if(*psz == 'R')
				{
					// Scope control register update
					// data comes back as an ascii number, representing an 8 bit value
					// bit 7: unused
					// bit 6: Internal (0, a or b as defined later) or External Trigger (1)
					// bit 5: trigger polarity, rising (0) or falling (1)
					// bit 4: trigger source, A (0) or B (1)
					// bits 3:0 -> sample rate 20 Msps / (2^thisval) this val can be 0 - 15
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					
					// internal versus external trigger
					// bit 6
					if((avar & 0x40) == 0)
					{
						// internal trigger
						// trigger polarity
						// bit 5
						if((avar & 0x20) == 0)
						{
							// rising edge
							Trigger_Control_Write(Trigger_Control_Read() & ~TRIGGER_MASK);
    						Trigger_Control_Write(Trigger_Control_Read() | RISING_EDGE_TRIGGERED);
						}
						else
						{
							// falling edge
							Trigger_Control_Write(Trigger_Control_Read() & ~TRIGGER_MASK);
    						Trigger_Control_Write(Trigger_Control_Read() | FALLING_EDGE_TRIGGERED);
						}
					}
					else
					{
						// external trigger
						Trigger_Control_Write(Trigger_Control_Read() & ~TRIGGER_MASK);
    					Trigger_Control_Write(Trigger_Control_Read() | EXTERNAL_TRIGGERED);
					}
					
					// bit 4
					// trigger source select
					if((avar & 0x10) == 0)
					{
						// channel 1 / A
						trigger_source = SCOPE_CHANNEL_1;
						Amux_Trigger_Select(SCOPE_CHANNEL_1);
					}
					else
					{
						// channel 2 / B
						trigger_source = SCOPE_CHANNEL_2;
						Amux_Trigger_Select(SCOPE_CHANNEL_2);
					}
					
					// sample rate
					// bit 3:0
					if((avar & 0x0f) <= 0) // 818.2 Ksps "maximum" sample rate
					{
                        // 36 Mhz master clock divided by this number and then divided by 
                        // 18 due to the freq divider set the ADC sample rate.
						SampleClock_SetDividerValue(1); 
					}
					else if((avar & 0x0f) == 1) // 409.1 ksps
					{
						SampleClock_SetDividerValue(2);
					}
					else if((avar & 0x0f) == 2) // 204.5 ksps
					{
						SampleClock_SetDividerValue(4);
					}
					else if((avar & 0x0f) == 3) // 102.3 ksps
					{
						SampleClock_SetDividerValue(8);
					}
					else if((avar & 0x0f) == 4) // 51.1 ksps
					{ 
						SampleClock_SetDividerValue(16);
					}
					else if((avar & 0x0f) == 5) // 25.6 ksps
					{
						SampleClock_SetDividerValue(32);
					}
					else if((avar & 0x0f) == 6) // 12.8 ksps
					{
						SampleClock_SetDividerValue(64);
					}
					else if((avar & 0x0f) == 7) // 6.4 ksps
					{ 
						SampleClock_SetDividerValue(128);
					}
					else if((avar & 0x0f) == 8) // 3.2 ksps
					{
						SampleClock_SetDividerValue(256);
					}
					else if((avar & 0x0f) == 9) // 1.6 ksps
					{
						SampleClock_SetDividerValue(512);
					}
					else if((avar & 0x0f) == 10) // 799 sps 
					{ 
						SampleClock_SetDividerValue(1024);
					}
					else if((avar & 0x0f) == 11) // 400 sps
					{ 
						SampleClock_SetDividerValue(2048);
					}                    
					else if((avar & 0x0f) == 12) // 200 sps
					{ 
						SampleClock_SetDividerValue(4096);
					}
					else if((avar & 0x0f) >= 13) // 100 sps
					{ 
						SampleClock_SetDividerValue(8192);
					}
                }
				else if(*psz == 'D')
				{
					// Scope debug hardware command
					// data comes back as an ascii number, representing a single value
					// 0 set reset to 0
					// 1 set reset to 1
					// 2 set REQ = 0
					// 3 set REQ = 1
					// 4 set MAN_TRIG = 1
					// 5 set MAN_TRIG = 0
					psz = strtok(0, " ");
					sscanf(psz,"%hu", &avar);
					
					if(avar == 0) // reset PWM = 0
					{
						Trigger_Control_Write(Trigger_Control_Read() & ~RESET_PWM);
					}
					else if(avar == 1) // reset PWM = 1
					{
						Trigger_Control_Write(Trigger_Control_Read() | RESET_PWM);
					}
					else if(avar == 2) // set REQ = 0
					{
						Trigger_Control_Write(Trigger_Control_Read() & ~TRIGGER_ARM);
					}
					else if(avar == 3) // set REQ = 1
					{
						Trigger_Control_Write(Trigger_Control_Read() | TRIGGER_ARM);
					}
					else if(avar == 4) // manual trigger = 0
					{
						Trigger_Control_Write(Trigger_Control_Read() & ~MANUAL_TRIGGER);
					}
					else if(avar == 5) // manual trigger = 1
					{
						//Trigger_Control_Write(Trigger_Control_Read() | TRIGGER_ARM);
						//Trigger_Control_Write(Trigger_Control_Read() & ~TRIGGER_ARM);
						Trigger_Control_Write(Trigger_Control_Read() | MANUAL_TRIGGER);
					}
					
				}
				else if(*psz == 'G')
				{
					// Scope GO, arms the scope and waits for a trigger event
					// if a trigger occurs, sends back the character 'A' followed
					// by a 2nd and 3rd byte in binary format indicating the address
					// where the capture ended in the buffer (10 bits).  I'm assuming Aa format
					
					// arm the scope
					Trigger_Control_Write(Trigger_Control_Read() | TRIGGER_ARM);
					Trigger_Control_Write(Trigger_Control_Read() & ~TRIGGER_ARM);
					
				}
				else if(*psz == 'B')
				{
					// dump the scope buffer
					PutChar('D');
					
					for(i = 0; i < BUFFER_SIZE; i++)
					{
						// truncate to 10 bits and "offset" to work with scope hardware
						if(preamp_a == 1)
						{ // preamp gain of 1
							avar = (uint16) (511 - (Channel_1_Data[i]/42.68));
						}
						else
						{ // preamp gain of 10
							// GJL - fix truncation at 3V by changing 511 to 1023 and fixing in scope.tcl also
                            avar = (uint16) (1023 - (Channel_1_Data[i]/4.8497));
						}
						
						if(preamp_b == 1)
						{ // preamp gain of 1
							bvar = (uint16) (511 - (Channel_2_Data[i]/42.68));
						}
						else
						{ // preamp gain of 10
							bvar = (uint16) (1023 - (Channel_2_Data[i]/4.8497));
						}
						
						// send the data out to the terminal
						USB_Out_Buffer[(i*4+0) % USBUART_BUFFER_SIZE] = (uint8) ((avar >> 8) & 0x0003); // address of the end, MSbits
						USB_Out_Buffer[(i*4+1) % USBUART_BUFFER_SIZE] = (uint8) (avar & 0x00FF); // address of the end, LSbits
						USB_Out_Buffer[(i*4+2) % USBUART_BUFFER_SIZE] = (uint8) ((bvar >> 8) & 0x0003); // address of the end, MSbits
						USB_Out_Buffer[(i*4+3) % USBUART_BUFFER_SIZE] = (uint8) (bvar & 0x00FF); // address of the end, LSbits
						
						// our buffer has been filled up, dump the data out the serial port
						if(((i*4+4) % USBUART_BUFFER_SIZE) == 0)
						{
							while(USBUART_CDCIsReady() == 0){};
							USBUART_PutData(USB_Out_Buffer, USBUART_BUFFER_SIZE);
						}
					}
				}
				else if(*psz == 'P')
				{
					// Scope Preamp settings
					// a = channel 1 preamp 10x
					// A = channel 1 preamp 1x
					// b = channel 2 preamp 10x
					// B = channel 2 preamp 1x
					psz = strtok(0, " ");
					
					if(*psz == 'a')
					{
						preamp_a = 10;
					}
					else if(*psz == 'A')
					{
						preamp_a = 1;
					}
					else if(*psz == 'b')
					{
						preamp_b = 10;
					}
					else if(*psz == 'B')
					{
						preamp_b = 1;
					}
					
				}
				else
				{
					PutString("Error: Unrecognized Scope Command\n\r");
				}
			// END OF SCOPE 'S' COMMANDS
			}
			else
			{
				// command not recognized, toss it
				PutString("Error: Unknown Command\n\r");
			}
			bCommandReady = 0;
		}
		
		status = Trigger_Status_Read();
						
		// if we have completed a capture, get the index of the last sample
		if((status & CAPTURE_COMPLETE ) != 0)
		{
			ending_offset = GetLastSampleIndex(DMA_Channel_1_Chan);
			
			// return the capture info
			PutChar('A');
			PutChar((uint8) ((ending_offset >> 8) & 0x0003)); // address of the end, MSbits
			PutChar((uint8) (ending_offset & 0x00FF)); // address of the end, LSbits
			//PutChar('\n'); // terminate the return data, through testing, there is no need to send this character
		}

        //GJL
//		// digital input update
//		if((Digital_Input_Status_Last != Digital_Input_Status_Read()) && (Digital_AutoUpdate != 0))
//		{
//			PutChar('I');
//			PutChar((uint8) Digital_Input_Status_Read());
//			Digital_Input_Status_Last = Digital_Input_Status_Read();
//		}
	}
}

void Hardware_Config(void)
{
    CyGlobalIntEnable;
	
	cCommandsReady = 0;
	bCommandReady = 0;
    
    USBUART_Start(0, USBUART_5V_OPERATION);
	while(!USBUART_GetConfiguration())
    {
        /* Wait for USBUART to complete configuration */
    }
    USBUART_CDC_Init();
	
	Amux_Trigger_Select(SCOPE_CHANNEL_1);
    
    ADC_1_Start();
    ADC_2_Start();
    
    Vtrigger_Start();
    Trigger_Comp_Start();
    
    // The extras
    Trig_Buf_Start();
    
    PWM_Sample_Buffer_Start();
	// update the period to match the trigger buffer size for maximum post trigger capture
	PWM_Sample_Buffer_WritePeriod(BUFFER_SIZE-1); // maximum post trigger buffer
	//PWM_Sample_Buffer_WritePeriod(2); // minimum post trigger buffer, 3 samples after trigger (used to be 2, but a glitch on the trigger would cause it to run again.  I used the PWM output to shut down the trigger gating one clock early to prevent the spurious retrigger of the PWM)
	// after setting the period, need to set the count to the proper value as well
	PWM_Sample_Buffer_WriteCounter(BUFFER_SIZE-1); // maximum post trigger buffer
	//PWM_Sample_Buffer_WriteCounter(2); // minimum post trigger buffer, 3 samples after trigger
	
	preamp_a = 1;
	preamp_b = 1;
	trigger_source = SCOPE_CHANNEL_1;
    
	// basic configuration is trigger on the rising edge of channel 0
	Trigger_Control_Write(Trigger_Control_Read() & ~TRIGGER_MASK);
    Trigger_Control_Write(Trigger_Control_Read() | RISING_EDGE_TRIGGERED);
    //Trigger_Control_Write(Trigger_Control_Read() | FALLING_EDGE_TRIGGERED);
    //Trigger_Control_Write(Trigger_Control_Read() | EITHER_EDGE_TRIGGERED);
    //Trigger_Control_Write(Trigger_Control_Read() | EXTERNAL_TRIGGERED);
	
	// waveform generator control
	Wave_Control_Write(Wave_Control_Read() & ~WAVE_SELECT_MASK);
	Wave_Control_Write(Wave_Control_Read() | WAVE_SELECT_WAVE);
	// Wave_Control_Write(Wave_Control_Read() | WAVE_SELECT_NOISE);
	Wave_DAC_Start();
	Wave_Buf_Start();
	Wave_DAC_Wave1Setup(&Waveform_Buffer[0], WAVEFORM_BUFFER_SIZE);
	wave_gain = 0;
	
	PRS_Start();
	Wave_DAC_Wave2Setup((uint8 *) PRS_SEED_PTR, 1);

    //GJL
//	// digital IO initializations
//	Digital_Input_Status_Last = 0;
//	Digital_Out_Control_Write(0);
//	Digital_AutoUpdate = 0;
//	
//	Digital_PWM_Start();
}

void DMA_Config(void)
{
    DMA_Channel_1_Chan = DMA_Channel_1_DmaInitialize(DMA_Channel_1_BYTES_PER_BURST, DMA_Channel_1_REQUEST_PER_BURST, 
        HI16(DMA_Channel_1_SRC_BASE), HI16(DMA_Channel_1_DST_BASE));
    DMA_Channel_1_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_Channel_1_TD[0], BUFFER_SIZE*sizeof(int16), DMA_Channel_1_TD[0], DMA_Channel_1__TD_TERMOUT_EN | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_Channel_1_TD[0], LO16((uint32)ADC_1_SAR_WRK0_PTR), LO16((uint32)&Channel_1_Data[0]));
    CyDmaChSetInitialTd(DMA_Channel_1_Chan, DMA_Channel_1_TD[0]);
    CyDmaChRoundRobin(DMA_Channel_1_Chan, 1);
    CyDmaChSetRequest(DMA_Channel_1_Chan, CPU_TERM_CHAIN);
    CyDmaChEnable(DMA_Channel_1_Chan, 1);
    CyDmaChEnable(DMA_Channel_1_Chan, 1);

    DMA_Channel_2_Chan = DMA_Channel_2_DmaInitialize(DMA_Channel_2_BYTES_PER_BURST, DMA_Channel_2_REQUEST_PER_BURST, 
        HI16(DMA_Channel_2_SRC_BASE), HI16(DMA_Channel_2_DST_BASE));
    DMA_Channel_2_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_Channel_2_TD[0], BUFFER_SIZE*sizeof(int16), DMA_Channel_2_TD[0], DMA_Channel_2__TD_TERMOUT_EN | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_Channel_2_TD[0], LO16((uint32)ADC_2_SAR_WRK0_PTR), LO16((uint32)&Channel_2_Data[0]));
    CyDmaChSetInitialTd(DMA_Channel_2_Chan, DMA_Channel_2_TD[0]);
    CyDmaChRoundRobin(DMA_Channel_2_Chan, 1);
    CyDmaChSetRequest(DMA_Channel_2_Chan, CPU_TERM_CHAIN);
    CyDmaChEnable(DMA_Channel_2_Chan, 1);
    CyDmaChEnable(DMA_Channel_2_Chan, 1);

    return;
}

// returns the address of the first sample in the circular DMA buffer

uint16 GetLastSampleIndex(uint8 chHandle)
{
	uint16 index;

	// get the upper 16 bits of the destination (not strictly necessary) (actually it doesn't work since the CPU and the DMA use different
	// upper 16 bit addresses for the same RAM information
	//dest_address = (((uint32) CY_DMA_CFGMEM_STRUCT_PTR[chHandle].CFG1[3]) << 24) | (((uint32) CY_DMA_CFGMEM_STRUCT_PTR[chHandle].CFG1[2]) << 16);
	
	// grab the lower 16 bits of the destination address
	// the assumption here is that "work seperate" has been enabled, so a copy of the TD is stored in the TD associated with the channel number being used
	index = ((uint32) CY_DMA_TDMEM_STRUCT_PTR[chHandle].TD1[3] << 8) | ((uint32) CY_DMA_TDMEM_STRUCT_PTR[chHandle].TD1[2]);
	
	// calculate the index by taking the difference between the starting and ending destination address mod the buffer size
	index = (index - sizeof(int16) - LO16((uint32) &Channel_1_Data[0]))/sizeof(int16) % BUFFER_SIZE;
	
	return index;
}

void CommandCheck(void)
{
	uint16 i, ReceivedBytes;
	// check to see if line state has changed? thought this might be necessary, but it probably isn't
	USBUART_IsLineChanged();
	
	// if data is waiting
	if(USBUART_DataIsReady() != 0)
	{
		// lets find out how much the PC sent to us
		ReceivedBytes = USBUART_GetCount();
		
		// if the amount of data received is non-zero
		if(ReceivedBytes != 0)
		{
			cCharsReceived = USBUART_GetAll((uint8 *) &szTempBuffer[0]);
		}
		else
		{
			// its a zero length data packet, it *should* be safe to do this and not worry about corrupting the command buffer
			USBUART_GetAll((uint8 *) &szTempBuffer[0]);
			//PutString("Zero Length Packet\n");
		}
		
		// move the data from the temporary buffer into the circular command buffer
		i = 0;
		do
		{
			szCommandsBuffer[iWrite] = szTempBuffer[i];
			i++;
			iWrite++;
			iWrite = iWrite % CIRCULAR_BUFFER_SIZE;
		}while((iWrite != iRead) && (i != cCharsReceived));
		
		// scan the command buffer for commands
		i = iRead;
		do
		{
			// if we see a carriage return or a line feed
			if((szCommandsBuffer[i] == '\n') || (szCommandsBuffer[i] == '\r'))
			{
				cCommandsReady++; // indicate the buffer contains commands to be dealt with
				szCommandsBuffer[i] = 0; // null terminate the string
			}
			i++;
			i = i % CIRCULAR_BUFFER_SIZE;
		}while(i != iWrite);
	}
	
	i = 0;
	if(cCommandsReady > 0 && bCommandReady == 0)
	{
		while(szCommandsBuffer[iRead] != 0)
		{
			szCommand[i] = szCommandsBuffer[iRead];
			i++;
			iRead++;
			iRead = iRead % CIRCULAR_BUFFER_SIZE;
		}
		
		szCommand[i] = 0;
		iRead++;
		iRead = iRead % CIRCULAR_BUFFER_SIZE;
		cCommandsReady--;
		if(i == 0)
		{
			// this is a zero length command, nothing to do
		}
		else
		{
			// the command was at least 1 character, report it
			bCommandReady = 1;
		}
	}
}

void PutString(char * string)
{
	while(USBUART_CDCIsReady() == 0){};
	USBUART_PutString(string);
}

void PutChar(char Data)
{
	while(USBUART_CDCIsReady() == 0){};
	USBUART_PutChar(Data);
}
/* [] END OF FILE */
