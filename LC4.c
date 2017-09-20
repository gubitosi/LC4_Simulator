#include <stdio.h>
#include <stdlib.h>
#include "LC4.h"

int DecodeCurrentInstruction (unsigned short int INSN, ControlSignals *theControls) {
	// Create a variable to help determine what the instruction is
	unsigned short int checker;
	int i;
	i = 1;

	// Set default values for theControls
	theControls->PCMux_CTL = 0;
	theControls->rsMux_CTL = 0;
	theControls->rtMux_CTL = 0;
	theControls->rdMux_CTL = 0;
	theControls->regFile_WE = 0;
	theControls->regInputMux_CTL = 0;
	theControls->Arith_CTL = 0;
	theControls->ArithMux_CTL = 0;
	theControls->LOGIC_CTL = 0;
	theControls->LogicMux_CTL = 0;
	theControls->SHIFT_CTL = 0;
	theControls->CONST_CTL = 0;
	theControls->CMP_CTL = 0;
	theControls->ALUMux_CTL = 0;
	theControls->NZP_WE = 0;
	theControls->DATA_WE = 0;
	theControls->Privilege_CTL = 2;
	
	// Decode NOP Instructions
	checker = INSN >> 9;
	if (checker == 0) {
		return 0;
	}

	// Decode Branch Instructions
	checker = INSN >> 9;
	if (checker == 4 || checker == 6 || checker == 5 || checker == 2 || checker == 3 || checker == 1 || 
		checker == 7) {
		return 0;
	}
 
	// Decode Arithmetic Instructions
	checker = INSN >> 12;
	if (checker == 1) {
		theControls->PCMux_CTL = 1;
		theControls->regFile_WE = 1;
		theControls->NZP_WE = 1;
		checker = INSN << 10;
		checker = checker >> 13;

		if (checker == 0) {
			return 0;
		}		

		if (checker == 1) {
			theControls->Arith_CTL = 1;
			return 0;
		}

		if (checker == 2) {
			theControls->Arith_CTL = 2;
			return 0;
		}

		if (checker == 3) {
			theControls->Arith_CTL = 3;
			return 0;
		}

		checker = checker >> 2;
		if (checker == 1) {
			theControls->Arith_CTL = 0;
			theControls->ArithMux_CTL = 1;
			return 0;
		} 
	}

	// Decode Comparison Instructions
	checker = INSN >> 12;
	if (checker == 2) {
		theControls->PCMux_CTL = 1;
		theControls->rsMux_CTL = 2;
		theControls->ALUMux_CTL = 4;
		theControls->NZP_WE = 1;
		checker = INSN << 7;
		checker = checker >> 14;
		
		if (checker == 0) {
			return 0;
		}

		if (checker == 1) {
			theControls->CMP_CTL = 1;
			return 0;
		}
		
		if (checker == 2) {
			theControls->CMP_CTL = 2;
			return 0;
		}

		if (checker == 3) {
			theControls->CMP_CTL = 3;
			return 0;
		}
	}
	
	// Decode JSR Instructions
	checker = INSN >> 11;
	if (checker == 9) {
		theControls->PCMux_CTL = 5;
		theControls->rdMux_CTL = 1;
		theControls->regFile_WE = 1;
		theControls->regInputMux_CTL = 2;
		theControls->NZP_WE = 1;
		return 0;
	}

	// Decode JSRR Instructions
	if (checker == 8) {
		theControls->PCMux_CTL = 3;
		theControls->rdMux_CTL = 1; 
		theControls->regFile_WE = 1;
		theControls->regInputMux_CTL = 2;
		theControls->NZP_WE = 1;
		return 0;
	}
	
	// Decode Logical Instructions
	checker = INSN >> 12;
	if (checker == 5) {
		theControls->PCMux_CTL = 1;
		theControls->regFile_WE = 1;
		theControls->ALUMux_CTL = 1;
		theControls->NZP_WE = 1;
		checker = INSN << 10;
		checker = checker >> 13;

		if (checker == 0) {
			return 0;
		}

		if (checker == 1) {
			theControls->LOGIC_CTL = 1;
			return 0;
		}

		if (checker == 2) {
			theControls->LOGIC_CTL = 2;
			return 0;
		}

		if (checker == 3) {
			theControls->LOGIC_CTL = 3;
			return 0;
		}

		checker = checker >> 2;
		if (checker == 1) {
			theControls->LOGIC_CTL = 0;
			theControls->LogicMux_CTL = 1;
			return 0;
		}
	}	

	// Decode Load and Store Instructions
	checker = INSN >> 12;
	// Load
	if (checker == 6) {
		theControls->PCMux_CTL = 1;
		theControls->regFile_WE = 1;
		theControls->regInputMux_CTL = 1;
		theControls->ArithMux_CTL = 2;
		theControls->NZP_WE = 1;
		return 0;
	}

	// Store
	if (checker == 7) {
		theControls->PCMux_CTL = 1;
		theControls->rtMux_CTL = 1;
		theControls->ArithMux_CTL = 2;
		theControls->DATA_WE = 1;
		return 0;
	}

	// Decode RTI
	checker = INSN >> 12;
	if (checker == 8) {
		theControls->PCMux_CTL = 3;
		theControls->rsMux_CTL = 1;
		theControls->Privilege_CTL = 0;
		return 0;
	}

	// Decode CONST
	checker = INSN >> 12;
	if (checker == 9) {
		theControls->PCMux_CTL = 1;
		theControls->regFile_WE = 1;
		theControls->ALUMux_CTL = 3;
		theControls->NZP_WE = 1;
		return 0;
	}

	// Decode Shift and Mod Instructions
	checker = INSN >> 12;
	if (checker == 10) {
		theControls->PCMux_CTL = 1;
		theControls->regFile_WE = 1;
		theControls->ALUMux_CTL = 2;
		theControls->NZP_WE = 1;
		checker = INSN << 10;
		checker = checker >> 14;

		if (checker == 0) {
			return 0;
		}

		if (checker == 1) {
			theControls->SHIFT_CTL = 1;
			return 0;
		}

		if (checker == 2) {
			theControls->SHIFT_CTL = 2;
			return 0;
		}

		if (checker == 3) {
			theControls->Arith_CTL = 4;
			theControls->ALUMux_CTL = 0;
			return 0;
		}
	}

	// Decode JMPR Instruction
	checker = INSN >> 11;
	if (checker == 24) {
		theControls->PCMux_CTL = 3;
		return 0;
	}

	// Decode JMP Instruction
	checker = INSN >> 11;
	if (checker == 25) {
		theControls->PCMux_CTL = 2;
		return 0;
	}

	// Decode HICONST
	checker = INSN >> 12;
	if (checker == 13) {
		checker = INSN << 7;
		checker = checker >> 15;
		if (checker == 1) {
			theControls->PCMux_CTL = 1;
			theControls->rsMux_CTL = 2;
			theControls->regFile_WE = 1;
			theControls->CONST_CTL = 1;
			theControls->ALUMux_CTL = 3;
			theControls->NZP_WE = 1;
			return 0;
		}
	}
	// Decode TRAP Instruction
	checker = INSN >> 12;
	if (checker == 15) {
		theControls->PCMux_CTL = 4;
		theControls->rdMux_CTL = 1;
		theControls->regFile_WE = 1;
		theControls->regInputMux_CTL = 2;
		theControls->NZP_WE = 1;
		theControls->Privilege_CTL = 1;
		return 0;
	}
	
	printf("Error: The instruction is illegal."); 	
	return 1;
}

int SimulateDatapath (ControlSignals *theControls, MachineState *theMachineState, DatapathSignals *theDatapath) {
	unsigned short int instruction = theMachineState->memory[theMachineState->PC];
	unsigned short int instructionTemp;
	unsigned short int PSRtemp;
	unsigned short int NZP;
	unsigned short int RS;
	unsigned short int RT;	

	// Default values for the Datapath
	theDatapath->RS = 0;
	theDatapath->RT = 0;
	theDatapath->ArithmeticOps = 0;
	theDatapath->LogicalOps = 0;
	theDatapath->Shifter = 0;
	theDatapath->Constants = 0;
	theDatapath->Comparator = 0;
	theDatapath->ALUMux = 0;
	theDatapath->regInputMux = 0;
	theDatapath->PCMux = 0;
	
	// Determine RS
	if (theControls->rsMux_CTL == 0) {
		instructionTemp = instruction << 7;
		instructionTemp = instructionTemp >> 13;
		theDatapath->RS = theMachineState->R[instructionTemp];
	}

	if (theControls->rsMux_CTL == 1) {
		theDatapath->RS = (theMachineState->R[7]);
	}

	if (theControls->rsMux_CTL == 2) {
		instructionTemp = instruction << 4;
		instructionTemp = instructionTemp >> 13;
		theDatapath->RS = theMachineState->R[instructionTemp];
	}

	RS = theDatapath->RS;
	
	// Determine RT
	if (theControls->rtMux_CTL == 0) {
		instructionTemp = instruction << 13;
		instructionTemp = instructionTemp >> 13;
		theDatapath->RT = theMachineState->R[instructionTemp];
	}
	
	if (theControls->rtMux_CTL == 1) {
		instructionTemp = instruction << 4;
		instructionTemp = instructionTemp >> 13;
		theDatapath->RT = theMachineState->R[instructionTemp];
	}

	RT = theDatapath->RT;
	
	// Determine ArithmeticOps
	if (theControls->Arith_CTL == 0) {
		if (theControls->ArithMux_CTL == 0) {
			theDatapath->ArithmeticOps = (signed short int)RS + (signed short int)RT;
		}

		if (theControls->ArithMux_CTL == 1) {
			instructionTemp = instruction << 11;
			instructionTemp = instructionTemp >> 11;
			if ((instructionTemp >> 4) == 0) {
				theDatapath->ArithmeticOps = (signed short int)RS + (signed 
								short int)instructionTemp;
			}

			if ((instructionTemp >> 4) == 1) {
				instructionTemp = instructionTemp + 65504;
				theDatapath->ArithmeticOps = (signed short int)RS + (signed 
								short int)instructionTemp;
			}
		}
		
		if (theControls->ArithMux_CTL == 2) {
			instructionTemp = instruction << 10;
			instructionTemp = instructionTemp >> 10;
			if ((instructionTemp >> 5) == 0) {
				theDatapath->ArithmeticOps = (signed short int)RS + (signed 
								short int)instructionTemp;	
			}

			if  ((instructionTemp >> 5) == 1) {
				instructionTemp = instructionTemp + 65472;
				theDatapath->ArithmeticOps = (signed short int)RS + (signed 
								short int)instructionTemp;
			}
		}
	}

	if (theControls->Arith_CTL == 1) {
		theDatapath->ArithmeticOps = RS * RT;
	}

	if (theControls->Arith_CTL == 2) {
		theDatapath->ArithmeticOps = (signed short int)RS - (signed short int)RT;
	}

	if (theControls->Arith_CTL == 3) {
		theDatapath->ArithmeticOps = RS / RT;
	}

	if (theControls->Arith_CTL == 4) {
		theDatapath->ArithmeticOps = RS % RT;
	}

	// Determine LogicalOps
	if (theControls->LOGIC_CTL == 0) {
		if (theControls->LogicMux_CTL == 0) {
			theDatapath->LogicalOps = RS & RT;
		}

		if (theControls->LogicMux_CTL == 1) {
			instructionTemp = instruction << 11;			
			instructionTemp = instructionTemp >> 11;
			if ((signed short int) instructionTemp >= 0) {
				theDatapath->LogicalOps = RS & (signed short int) instructionTemp;
			}
			if ((signed short int) instructionTemp < 0) {
				instructionTemp = instructionTemp + 65504;
				theDatapath->LogicalOps = RS & (signed short int)instructionTemp;
			}
		}
	}

	if (theControls->LOGIC_CTL == 1) {
		theDatapath->LogicalOps = ~RS;
	}
	
	if (theControls->LOGIC_CTL == 2) {
		theDatapath->LogicalOps = RS | RT;
	}

	if (theControls->LOGIC_CTL == 3) {
		theDatapath->LogicalOps = RS ^ RT;
	}

	if (theControls->LOGIC_CTL == 4) {
		instructionTemp = instruction << 11;
		instructionTemp = instructionTemp >> 11;
		theDatapath->LogicalOps = RS & instructionTemp;
	}

	// Shifter
	if (theControls->SHIFT_CTL == 0) {
		instructionTemp = instruction << 12;
		instructionTemp = instructionTemp >> 12;
		theDatapath->Shifter = RS << instructionTemp;
	}

	if (theControls->SHIFT_CTL == 1) {
		instructionTemp = instruction << 12;
		instructionTemp = instructionTemp >> 12;
		theDatapath->Shifter = (signed short int)RS >> instructionTemp;
	}		

	if (theControls->SHIFT_CTL == 2) {
		instructionTemp = instruction << 12;
		instructionTemp = instructionTemp >> 12;
		theDatapath->Shifter = (unsigned short int)RS >> instructionTemp;
	}

	// Constants
	if (theControls->CONST_CTL == 0) {
		instructionTemp = instruction & 511;
		if ((instructionTemp >> 8) == 0) {
			theDatapath->Constants = (signed short int)instructionTemp;
		}
		
		if ((instructionTemp >> 8) == 1) {
			instructionTemp = instructionTemp + 65024;
			theDatapath->Constants = (signed short int)instructionTemp;
		}
	}
	
	if (theControls->CONST_CTL == 1) {
		instructionTemp = instruction << 8;
		theDatapath->Constants = (RS & 0xFF) | instructionTemp;
	}

	// Comparator
	if (theControls->CMP_CTL == 0) {
		if ((signed short int) RS > (signed short int) RT) {
			theDatapath->Comparator = 1;
		}

		if ((signed short int) RS == (signed short int) RT) {
			theDatapath->Comparator = 0;
		}

		if ((signed short int) RS < (signed short int) RT) {
			theDatapath->Comparator = -1;
		}
	}
	
	if (theControls->CMP_CTL == 1) {
		if (abs(RS) > abs(RT)) {
			theDatapath->Comparator = 1;
		}

		if (abs(RS) == abs(RT)) {
			theDatapath->Comparator = 0;
		}

		if (abs(RS) < abs(RT)) {
			theDatapath->Comparator = -1;
		}
	}

	if (theControls->CMP_CTL == 2) {
		instructionTemp = instruction & 127;
		if ((instructionTemp >> 6) == 1) {
			instructionTemp = instructionTemp + 65408;
		}

		if ((signed short int) RS > (signed short int) instructionTemp) {
			theDatapath->Comparator = 1;
		}	

		if ((signed short int) RS == (signed short int) instructionTemp) {
			theDatapath->Comparator = 0;
		}

		if ((signed short int) RS < (signed short int) instructionTemp) {
			theDatapath->Comparator = -1;
		}
	}

	if (theControls->CMP_CTL == 3) {
		instructionTemp = instruction << 9;
		instructionTemp = instructionTemp >> 9;
		if (abs(RS) > abs(instructionTemp)) {
			theDatapath->Comparator = 1;
		}

		if (abs(RS) == abs(instructionTemp)) {
			theDatapath->Comparator = 0;
		}

		if (abs(RS) < abs(instructionTemp)) {
			theDatapath->Comparator = -1;
		}
	}

	// ALUMux
	if (theControls->ALUMux_CTL == 0) {
		theDatapath->ALUMux = theDatapath->ArithmeticOps;
	}

	if (theControls->ALUMux_CTL == 1) {
		theDatapath->ALUMux = theDatapath->LogicalOps;
	}

	if (theControls->ALUMux_CTL == 2) {
		theDatapath->ALUMux = theDatapath->Shifter;
	}

	if (theControls->ALUMux_CTL == 3) {
		theDatapath->ALUMux = theDatapath->Constants;
	}

	if (theControls->ALUMux_CTL == 4) {
		theDatapath->ALUMux = theDatapath->Comparator;
	}

	// Update DATA memory
	if (theControls->DATA_WE == 1) {
		theMachineState->memory[theDatapath->ALUMux] = RT;
	}

	// regInputMux
	if (theControls->regInputMux_CTL == 0) {
		theDatapath->regInputMux = theDatapath->ALUMux;
		if (theDatapath->regInputMux - 0x8000 < 0) {
			NZP = 1;
		}

		if (theDatapath->regInputMux == 0) {
			NZP = 2;
		}

		if (theDatapath->regInputMux - 0x8000 >= 0) {
			NZP = 4;
		}

		if (theControls->NZP_WE == 1 && theControls->Privilege_CTL == 0) {
			theMachineState->PSR = NZP;
		}

		if (theControls->NZP_WE == 1 && theControls->Privilege_CTL == 1) {
			theMachineState->PSR = NZP + 32768; // Set NZP and the privilege 
							    //     bit in PSR
		}
		
		if (theControls->NZP_WE == 1 && theControls->Privilege_CTL == 2) {
			PSRtemp = theMachineState->PSR >> 3;
			PSRtemp = PSRtemp << 3;
			theMachineState->PSR = PSRtemp + NZP;
		}
	}

	if (theControls->regInputMux_CTL == 1) {
		theDatapath->regInputMux = theMachineState->memory[theDatapath->ALUMux];
		if (theDatapath->regInputMux - 0x8000 < 0) {
			NZP = 1;
		}

		if (theDatapath->regInputMux == 0) {
			NZP = 2;
		}

		if (theDatapath->regInputMux - 0x8000 >= 0) {
			NZP = 4;
		}

		if (theControls->NZP_WE == 1 && theControls->Privilege_CTL == 0) {
			theMachineState->PSR = NZP;
		}

		if (theControls->NZP_WE == 1 && theControls->Privilege_CTL == 0) {
			theMachineState->PSR = NZP + 32768; // Set NZP and the privilege
							    //     bit in PSR
		}
		
		if (theControls->NZP_WE == 1 && theControls->Privilege_CTL == 0) {
			PSRtemp = theMachineState->PSR >> 3;
			PSRtemp = PSRtemp << 3;
			theMachineState->PSR = PSRtemp + NZP;
		}
	}

	if (theControls->regInputMux_CTL == 2) {
		theDatapath->regInputMux = theMachineState->PC + 1;
		if (theDatapath->regInputMux - 0x8000 < 0) {
			NZP = 1;
		}

		if (theDatapath->regInputMux == 0) {
			NZP = 2;
		}

		if (theDatapath->regInputMux - 0x8000 >= 0) {
			NZP = 4;
		}

		if (theControls->NZP_WE == 1 && theControls->Privilege_CTL == 0) {
			theMachineState->PSR = NZP;
		}

		if (theControls->NZP_WE == 1 && theControls->Privilege_CTL == 1) {
			theMachineState->PSR = NZP + 32768; // Set NZP and the privilege
							    //     bit in PSR
		}

		if (theControls->NZP_WE == 1 && theControls->Privilege_CTL == 2) {
			PSRtemp = theMachineState->PSR >> 3;
			PSRtemp = PSRtemp << 3;
			theMachineState->PSR = PSRtemp + NZP;
		}
	}

	// PCMux
	if (theControls->PCMux_CTL == 0) {
		PSRtemp = theMachineState->PSR << 1;
		PSRtemp = PSRtemp >> 1;
		instructionTemp = instruction << 4;
		instructionTemp = instructionTemp >> 13;
		if ((PSRtemp & instructionTemp) == PSRtemp) {
			instructionTemp = instruction << 7;
			instructionTemp = instructionTemp >> 7;
			if ((instructionTemp >> 8) == 0) {
				theDatapath->PCMux = theMachineState->PC + 1 + (signed short 
									int)instructionTemp;
			}
			if ((instructionTemp >> 8) == 1) {
				instructionTemp = instructionTemp + 65024;
				theDatapath->PCMux = theMachineState->PC + 1 + (signed short 
									int)instructionTemp;
			}
		}
		
		else {
			theDatapath->PCMux = theMachineState->PC + 1;
		}
	}

	if (theControls->PCMux_CTL == 1) {
		theDatapath->PCMux = theMachineState->PC + 1;
	}

	if (theControls->PCMux_CTL == 2) {
		instructionTemp = instruction & 2047;
		if ((instructionTemp >> 10) == 0) {
			theDatapath->PCMux = theMachineState->PC + 1 + (signed short int)instructionTemp;
		}
		if ((instructionTemp >> 10) == 1) {
			instructionTemp = instructionTemp + 63488;
			theDatapath->PCMux = theMachineState->PC + 1 + (signed short int)instructionTemp;
		}
	}

	if (theControls->PCMux_CTL == 3) {
		theDatapath->PCMux = theDatapath->RS;
	}

	if (theControls->PCMux_CTL == 4) {
		instructionTemp = instruction << 8;
		instructionTemp = instructionTemp >> 8;
		theDatapath->PCMux = 0x8000 | instructionTemp;
	}

	if (theControls->PCMux_CTL == 5) {
		instructionTemp = instruction & 2047;
		instructionTemp = instructionTemp << 4;
		theDatapath->PCMux = (theMachineState->PC & 0x8000) | (signed short int)instructionTemp; 
	}

	return 0;
}

int UpdateMachineState (ControlSignals *theControls, MachineState *theMachineState, DatapathSignals 
*theDatapath) {
	unsigned short int instruction;
	unsigned short int instructionTemp;
	unsigned short int PSRtemp;
	int Rd_register_number;
	instruction = theMachineState->memory[theMachineState->PC];
	
	// Find register number for Rd
	if (theControls->rdMux_CTL == 0) {
		instructionTemp = instruction << 4;
		instructionTemp = instructionTemp >> 13;
		Rd_register_number = instructionTemp;
	}

	if (theControls->rdMux_CTL == 1) {
		Rd_register_number = 7;
	}

	// Update Rd in memory
	if (theControls->regFile_WE == 1) {
		theMachineState->R[Rd_register_number] = theDatapath->regInputMux;
	}

	// Update PC
	theMachineState->PC = theDatapath->PCMux;

	// Update PSR
	if (theControls->Privilege_CTL == 0) {
		PSRtemp = theMachineState->PSR << 1;
		PSRtemp = PSRtemp >> 1;
		theMachineState->PSR = PSRtemp;
	}
	if (theControls->Privilege_CTL == 1) {
		PSRtemp = theMachineState->PSR << 1;
		PSRtemp = PSRtemp >> 1;
		theMachineState->PSR = PSRtemp + 32768; // Update privilege bit to 1
	}

	// Type 1 Exception
	if ((theMachineState->PC > 0x1FFF && theMachineState->PC < 0x8000) || theMachineState->PC >= 0xA000) {
		printf("Error: Type 1 Exception");
		return 1;
	} 
	
	// Type 2 Exception
	if ((theDatapath->ALUMux < 0x2000 || (theDatapath->ALUMux >= 0x8000 && theDatapath->ALUMux < 
		0xA000)) && theControls->ArithMux_CTL == 2) { 
		printf("Error: Type 2 Exception");
		return 2;
	}

	// Type 3 Exceptions
	PSRtemp = theMachineState->PSR >> 15;
	if (theMachineState->PC >= 0x8000 && PSRtemp == 0) {
		printf("Error: Type 3 Exception");
		return 3;
	}

	if (theDatapath->ALUMux >= 0x8000 && PSRtemp == 0 && 
		theControls->ArithMux_CTL == 2) {
		printf("Error: Type 3 Exception");
		return 3;
	}	

	return 0;
}

void Reset (MachineState *theMachineState) {
	int i;
	theMachineState->PC = 0x8200;
	theMachineState->PSR = 0x8002;
	for (i = 0; i < 65536; i++) {
		theMachineState->memory[i] =0x0000;
	}
	for (i = 0; i < 8; i++) {
		theMachineState->R[i] = 0x0000;
	}
}
