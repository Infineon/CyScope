#CyScope

This is a 2-channel oscilloscope and waveform generator implemented on
a Cypress' PSoC5LP device. 
The implementation has been done for three evaluation kits:

  CY8CKIT-059 kit -> ScopePSoC

  See the file PSoC_5LP_Scope.docx for details on how to program and use
  the oscilloscope and waveform generator.

  CY8CKIT-043 kit -> Scope_KitProg

  The file KitProg_Scope.docx describes how to use the PSoC 5LP programmer
  on the kit as the oscilloscope and waveform generator. Other
  functionality (PWM and digitial inputs/outputs) are removed due to
  pin limitations.

  FreeSoC2 kit -> ScopeFreeSoC2

  The file FreeSoC2_Scope.docx describes changes to use the original project
  on the Sparkfun kit including a hardware adapter for demonstration purpose.


The GUI is a TCL program baesed on the Open Instrumentation Project
created by Syscomp Electronic Design.

