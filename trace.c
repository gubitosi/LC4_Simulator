#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ObjectFiles.h"
#include "binary.h"

int main(int argc, char* argv[])
{
	int i;
	int j;
	i = 0;
	while ((argv[2])[i] != '\0') {
		i++;
	}
	char textFile[i];
	char imageFile[10] = "image.ppm";
	int switchChecker;
	char checkSwitch0;
	char checkSwitch1;
	int errorChecker;
	unsigned short int previousPC;
	unsigned short int writeArrayTrace[2];
	char writeArrayText[100];
	FILE *myFilePtr;
	FILE *myFilePtrTxt;
	FILE *myFilePtrImage;
	ControlSignals *theControls;
	DatapathSignals *theDatapath;
	unsigned short int instruction;
	char instructionRED;
	char instructionGREEN;
	char instructionBLUE;
	
	// Declare a new MachineState and initialize its values.
	MachineState *theMachineState = malloc(sizeof(MachineState));
	Reset(theMachineState);
	
	// Check if "-s" switch is passed
	switchChecker = 2;
	if (1 < argc) {
		i = 0;
		while((argv[1])[i] != '\0') {
			if (i >= 1) {
				checkSwitch0 = (argv[1])[0];
				checkSwitch1 = (argv[1])[1]; 
			}
			i++;
		}

		if (i == 2 && checkSwitch0 == '-' && checkSwitch1 == 's') {
			switchChecker = 3;
		}

		else {
			switchChecker = 2;
		}
	}

	// Run ReadObjectFile on each .obj file.
	for (i = switchChecker; i < argc; i++) {
		errorChecker = ReadObjectFile(argv[i], 
					theMachineState);
		if (errorChecker != 0) {
			return 1;
		}
	}

	// Simulate the LC4
 	theControls = malloc(sizeof(ControlSignals));
	theDatapath = malloc(sizeof(DatapathSignals));
	myFilePtr = fopen(argv[switchChecker - 1], "wb");
	if (switchChecker == 3) {
		i = 0;
		while ((argv[2])[i] != '\0') {
			textFile[i] = (argv[2])[i];
			i++;
		}
		textFile[i] = '.';
		textFile[i + 1] = 't';
		textFile[i + 2] = 'x';
		textFile[i + 3] = 't';
		textFile[i + 4] = '\0';
		myFilePtrTxt = fopen(textFile, "w");
	}
	previousPC = theMachineState->PC;
	i = 0;
	j = 0;
	while (previousPC != 0x80FF && previousPC != 0xFFFF) {
		i++;	
		writeArrayTrace[0] = theMachineState->PC;
		writeArrayTrace[1] = theMachineState->memory[theMachineState->PC];
		previousPC = theMachineState->PC;
		errorChecker = DecodeCurrentInstruction(theMachineState->memory
				[theMachineState->PC],theControls);
		if (errorChecker != 0) {
			break;
		}
		
		errorChecker = SimulateDatapath(theControls, theMachineState, 
				theDatapath);
		if (errorChecker != 0) {
			break;
		}
		
		errorChecker = UpdateMachineState(theControls, theMachineState, 
				theDatapath);
		if (errorChecker != 0) {
			break;
		}
		
		fwrite(writeArrayTrace, 2, 2, myFilePtr);
		if (previousPC == 0x80FF || previousPC == 0xFFFF) {
			break;
		}

		if (switchChecker == 3) {
			fprintf(myFilePtrTxt, "%04x ", previousPC);
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->PCMux_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->rsMux_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->rtMux_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->rdMux_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->regFile_WE));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->regInputMux_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->Arith_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->ArithMux_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->LOGIC_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->LogicMux_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->SHIFT_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->CONST_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->CMP_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->ALUMux_CTL));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->NZP_WE));
			fprintf(myFilePtrTxt, "%04d ", binary(theControls->DATA_WE));
			fprintf(myFilePtrTxt, "%04d", binary(theControls->Privilege_CTL));
			fprintf(myFilePtrTxt, "%c", 0x20);
			fprintf(myFilePtrTxt, "%c", 0x0d);
			fprintf(myFilePtrTxt, "%c", 0x0a);
		}
	}
	
	if (errorChecker != 0) {
		return errorChecker;
	}

	fclose(myFilePtr);
	if (switchChecker == 3) {
		fclose(myFilePtrTxt);
	}
	
	myFilePtrImage = fopen(imageFile, "wb");
	fprintf(myFilePtrImage, "P6\n");
	fprintf(myFilePtrImage, "%d %d\n", 128, 124);
	fprintf(myFilePtrImage, "%d\n", 31);
	i = 0xC000;
	while (i < 0xFE00) {
			instruction = theMachineState->memory[i] << 1;
			instructionRED = (char) (instruction >> 11);
			instruction = instruction << 5;
			instructionGREEN = (char) (instruction >> 11);
			instruction = instruction << 5;
			instructionBLUE = (char) (instruction >> 11);
			fprintf(myFilePtrImage, "%c", instructionRED);
			fprintf(myFilePtrImage, "%c", instructionGREEN);
			fprintf(myFilePtrImage, "%c", instructionBLUE);
			i++;
	}	

	fclose(myFilePtrImage);
	free(theMachineState);
	free(theControls);
	free(theDatapath);
	return 0;
}
