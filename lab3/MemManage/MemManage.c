#include <stdio.h>
#include <limits.h>



int main(int argc, char **argv) {

	// check that the user has supplied an input argument to the text file
	// from which we will parse the addresses and other info
	if (argc != 2) {
		printf("Useage: \n\t$ ./MemManage <inputFile.txt>\n\n");
		return 1;
	}

	// get the file name
	const char *fileName = argv[1];

	// get the size of a uint on this platform
	unsigned int maxUInt = UINT_MAX;
	unsigned int sizeOfUInt = 0;
	while(maxUInt) {
		sizeOfUInt += maxUInt & 1;
		maxUInt >>= 1;
	}

	// declare some variables that will be used to read from the file
	unsigned int lineValue;
	char line[15];

	// the array that will store the values read from the text file
	// the 0th value is the page number, the 1st is the offset,
	// and the rest are addresses
	unsigned int values[255];

	// the file
	FILE *filePointer = fopen(fileName, "rt");

	// read each line of the file into the values array
	int index = 0;
	while(fgets(line, 80, filePointer) != NULL) {
		sscanf(line, "%u", &lineValue);
		values[index++] = lineValue;
	}

	fclose(filePointer);

	// record the number of addresses, the number of bits for the page number,
	// and the number of bits of the offset
	int nAddresses = index - 2;
	unsigned int pageNumberLength = values[0];
	unsigned int offsetLength = values[1];

	// check to make sure that the length of the page number and the length
	// of the offset add to 16, otherwise there is a problem with the input file
	if (pageNumberLength + offsetLength != 16) {
		printf("There is a problem with the input file %s: age # Length + Offset does not equal 16\n\n", fileName);
		return 3;
	}

	// iterate through each address
	for (int addressIndex = 2; addressIndex < nAddresses + 2; ++addressIndex) {
		unsigned int currentAddress = values[addressIndex];

		// get the page number by right shifting away the offset
		unsigned int pageNum = currentAddress >> pageNumberLength;

		// get the offset by left shifting away the page number, then right shifting back
		unsigned int offset = (currentAddress << (sizeOfUInt - offsetLength)) >> (sizeOfUInt - offsetLength);
		
		printf("virtual address %d is in page number %d and offset %d\n", currentAddress, pageNum, offset);
	}


	return 0;
}
