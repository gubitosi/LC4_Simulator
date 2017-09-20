#include <stdio.h>
#include "ObjectFiles.h"

int ReadObjectFile(char *filename, MachineState *theMachineState) 
{
	char char1;
	char char2;
	char char3;
	char char4;
	int i;
	i = 0;

	// Check each character until the end of the string.
	// Assign char1, char2, char3, and char4 to the last 4
	//	characters in the string.
	while (filename[i] != '\0') {
		if (i >= 3) {
			char4 = filename[i];
			char3 = filename[i - 1];
			char2 = filename[i - 2];
			char1 = filename[i - 3];
		}
		i++;
	}

	// Print an error message if there are less than or equal to
	// 	4 characters in the string, since this means there
	//	can't be text before the .obj (if the file even is
	//	a .obj file. Then return 1.
	if (i <= 4) {
		printf("Error: The filename is illegal.");
		return 1;
	}

	// Print an error message if the file is not a .obj file
	//	and return 1.
	if (char4 != 'j' || char3 != 'b' || char2 != 'o' || char1 != '.') 
	{
		printf("Error: The file is not a .obj file.");
		return 1;
	}

	// Declare variables that are to be used in the rest of the program.
	int first_portion;
	int second_portion;
	int full_portion;
	FILE *myFilePtr;
	int first_part_n;
	int second_part_n;
	int full_n;
	int first_part_address;
	int second_part_address;
	int full_address;
	int first_part_memory;
	int second_part_memory;
	int full_memory;

	// Open the file.
	myFilePtr = fopen(filename, "rb");
	
	// While we aren't at the end of the file, form a word out of
	//	the next two bytes.
	while (feof(myFilePtr) == 0) {
		first_portion = fgetc(myFilePtr);
		second_portion = fgetc(myFilePtr);
		full_portion = second_portion | (first_portion << 8);

		// If we have reached a header for code or data, read the next
		//	word as the address, the following word as the number of
		//	instructions/data values, and then write the instructions/
		// 	data values  to	memory, starting at the given address.
		if (full_portion == 0xCADE || full_portion == 0xDADA) {
			first_part_address = fgetc(myFilePtr);
			second_part_address = fgetc(myFilePtr);
			full_address = second_part_address | 
					(first_part_address << 8);
			first_part_n = fgetc(myFilePtr);
			second_part_n = fgetc(myFilePtr);
			full_n = second_part_n | (first_part_n << 8);
			for (i = 0; i < full_n; i++) {
				first_part_memory = fgetc(myFilePtr);
				second_part_memory = fgetc(myFilePtr);
				full_memory = second_part_memory | 
					(first_part_memory << 8);
				theMachineState->memory[full_address] = 
							full_memory;
				full_address++;
			}				
		}

		// If we have reached a header for symbol, read the next word as the
		//	address, the following word as the number of characters in
		//	the symbol string, and then read the characters and ignore them.
		if (full_portion == 0xC3B7 && feof(myFilePtr) == 0) {
			first_part_address = fgetc(myFilePtr);
			second_part_address = fgetc(myFilePtr);
			full_address = second_part_address |
					(first_part_address << 8);
			first_part_n = fgetc(myFilePtr);
			second_part_n = fgetc(myFilePtr);
			full_n = second_part_n | (first_part_n << 8);
			for (i = 0; i < full_n; i++) {
				fgetc(myFilePtr);
			}
		}

		// If we have reached a header for filename, read the next word as 
		//	the number of characters in the filename string.
		//	Then read the characters and ignore them.
		if (full_portion == 0xF17E && feof(myFilePtr) == 0) {
			first_part_n = fgetc(myFilePtr);
			second_part_n = fgetc(myFilePtr);
			full_n = second_part_n | (first_part_n << 8);
			for (i = 0; i < full_n; i++) {
				fgetc(myFilePtr);
			} 
		}

		// If we have reached a header for line number, read the next 6
		//	words and ignore them.
		if (full_portion == 0x715E && feof(myFilePtr) == 0) {
			for (i = 0; i < 6; i++) {
				fgetc(myFilePtr);
			}
		}

		// If we are supposed to have reached a header file, but instead
		//	actually have not reached a header file, print an
		//	error message and return 1.
		if  (full_portion != 0xCADE && full_portion != 0xDADA && full_portion 
			!= 0xC3B7 && full_portion != 0xF17E && full_portion != 0x715E
			&& feof(myFilePtr) == 0) {
			printf("There is a formatting error within the file.");
			return 1;
		}
	}
	
	// Close the file.
	fclose(myFilePtr);
	return 0;
}
