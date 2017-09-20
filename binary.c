#include <stdlib.h>
#include <math.h>
#include "binary.h"

int binary(unsigned short int number) {
    	double i = 0;
    	int remainder = 0;
    	double binaryNumber = 0;
    	int numberTemp = number;
    	while (numberTemp != 0) {
		remainder = numberTemp % 2;
		numberTemp = numberTemp / 2;
		binaryNumber = binaryNumber + remainder * pow(10.0, i);
		i++;
    	}
    	return (int) binaryNumber;
}
