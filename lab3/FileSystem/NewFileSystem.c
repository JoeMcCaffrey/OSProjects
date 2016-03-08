#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define NINODES 16
#define NDATABLOCKS 127
#define NDUMMYSPACE 1023


// 48 bytes
typedef struct {
	char name[8];
	unsigned int size;
	unsigned int blockArray[8];
	unsigned int used; // 0 = free, 1 = in use
} iNode;

// 1KB
typedef struct {
	unsigned char blockList[NDATABLOCKS]; // 0 or 1
	iNode iNodeArray[NINODES];
	unsigned int unusedSpace[32];
} superBlock;

// 1 KB
typedef struct {
	unsigned char used;
	unsigned char dummySpace[NDUMMYSPACE];
} dataBlock;

// 128 KB
typedef struct {
	superBlock superblock;
	dataBlock dataBlocks[NDATABLOCKS];
} diskStruct;



void writeEntireDisk(diskStruct *disk, FILE *diskFilePointer) {
	fwrite(disk, sizeof(diskStruct), 1, diskFilePointer);
}


char *removeNewLine(char *line) {
	int i;
	for (i = 8; i >= 0; --i) {
		if (line[i] == '\n') {
			line[i] = '\0';
		}
	}

	return line;
}


char *parseFileName(char *line) {
	char *temp = (char *) malloc(sizeof(line));
	strcpy(temp, line);
	char *token;
	const char demiliter[2] = " ";
	token = strtok(temp, demiliter);
	token = strtok(NULL, demiliter);
	return token;
}

int parseFileSize(char *line) {
	char *temp = (char *) malloc(sizeof(line));
	strcpy(temp, line);
	char *token;
	const char demiliter[2] = " ";
	token = strtok(temp, demiliter);
	token = strtok(NULL, demiliter);
	token = strtok(NULL, demiliter);
	return atoi(token);
}


diskStruct *initDisk(char *diskName, FILE *diskFilePointer) {
	printf("Initializing disk: %s\n", diskName);

	diskStruct *disk = (diskStruct *) malloc(sizeof(diskStruct));

	rewind(diskFilePointer);
	superBlock *sb = (superBlock *) diskFilePointer;
	disk->superblock = *sb;

	// set the diskFilePointer to the first data block
	fseek(diskFilePointer, sizeof(superBlock), SEEK_CUR);

	int i;
	for (i = 0; i < NDATABLOCKS; ++i) {
		dataBlock *currentDataBlock = (dataBlock *) diskFilePointer;
		disk->dataBlocks[i] = *currentDataBlock;
		if (i != NDATABLOCKS - 1) {
			fseek(diskFilePointer, sizeof(dataBlock), SEEK_CUR);
		}
	}

	rewind(diskFilePointer);

	printf("Finished initializing disk: %s\n", diskName);
	return disk;
}


void ls(diskStruct *disk) {
	int i;
	for (i = 0; i < NINODES; ++i) {
		printf("%u\n", disk->superblock.iNodeArray[i].used);
		if (disk->superblock.iNodeArray[i].used == 1) {
			printf("filename: %s, size: %u\n", disk->superblock.iNodeArray[i].name, disk->superblock.iNodeArray[i].size);
		}
	}
}


void create(char name[8], unsigned int nBlocks, diskStruct *disk, FILE *diskFilePointer) {
	if (nBlocks > 7) {
		printf("Invalid block count: %d\n", nBlocks);
		exit(-1);
	}

	int iNodeIndex;
	char freeINodeFound = 0;
	for (iNodeIndex = NINODES - 1; iNodeIndex >= 0; --iNodeIndex) {
		if (disk->superblock.iNodeArray[iNodeIndex].used == 0) {
			freeINodeFound = 1;
			break;
		}
	}

	if (freeINodeFound == 0) {
		printf("Error: no available iNodes during \"create\" call for file %s\n", name);
		return;
	}

	int foundBlockNums[nBlocks];
	int i;
	for (i = 0; i < nBlocks; ++i) {
		foundBlockNums[i] = -1;
	}

	// look for the free blocks
	int foundBlockCount = 0;
	int blockIndex;
	for (blockIndex = NDATABLOCKS - 1; blockIndex >= 0; --blockIndex) {
		if (disk->dataBlocks[blockIndex].used == 0) {
			foundBlockNums[foundBlockCount++] = blockIndex;
		}

		if (foundBlockCount > 7) {
			break;
		}
	}

	for (i = 0; i < nBlocks; ++i) {
		if (foundBlockNums[i] == -1) {
			printf("Error: not enough available data blocks during \"create\" call for file %s\n", name);
			return;
		}
	}

	strcpy(disk->superblock.iNodeArray[iNodeIndex].name, name);
	disk->superblock.iNodeArray[iNodeIndex].size = nBlocks;
	for (i = 0; i < nBlocks; ++i) {
		disk->superblock.iNodeArray[iNodeIndex].blockArray[i] = foundBlockNums[i];
		disk->dataBlocks[i].used = 1;
	}

	// write the entire disk back because I store data about whether or not
	// the diskBlock is used in the diskBlock itself, so it's not enough
	// to just write the superBlock
	rewind(diskFilePointer);
	fwrite(disk, sizeof(diskStruct), 1, diskFilePointer);

	printf("File %s with size %d created\n", name, nBlocks);
}


void delete(char *name, diskStruct *disk, FILE *diskFilePointer) {
	char found = 0;
	int iNodeIndex;
	for (iNodeIndex = NINODES - 1; iNodeIndex >= 0; --iNodeIndex) {
		if (strcmp(disk->superblock.iNodeArray[iNodeIndex].name, name) == 0) {
			found = 1;
			break;
		}
	}

	if (found == 0) {
		printf("Error: could not delete file %s because file does not exist on disk\n", name);
		return;
	}

	int blockIndex;
	for (blockIndex = 7; blockIndex >= 0; --blockIndex) {
		disk->superblock.iNodeArray[iNodeIndex].blockArray[blockIndex] = 0;
	}

	disk->superblock.iNodeArray[iNodeIndex].used = 0;

	rewind(diskFilePointer);
	fwrite(disk, sizeof(diskStruct), 1, diskFilePointer);

	printf("File %s deleted\n", name);
}


void readFile(char *name, unsigned int blockNum, char buffer[1024], diskStruct *disk) {
	name = (char *) malloc(8);

}


void writeFile(char *name, unsigned int blockNum, char *buffer, diskStruct *disk) {

}



void defragment(diskStruct *disk) {

}


int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: \t$ ./FileSystem <command_list_file>\n");
		return 1;
	}

	// Open the file and read it into the lines array
	FILE *filePointer;
	char *lines[255];

	char * line = NULL;
	size_t len =0;
	size_t read;
	
	filePointer = fopen(argv[1], "r");

	int nLines = 0;
	while ((read = getline(&line, &len, filePointer)) != -1) {
		lines[nLines] = malloc(len);
		strcpy(lines[nLines], line);
		nLines++;
	}

	char *diskName = lines[0];

	diskName = removeNewLine(diskName);

	// check if the disk already exists, if file doesn't exist, error out
	if(access(diskName, F_OK) != -1) {
	    printf("Disk with name \"%s\" found, loading disk...\n", diskName);
	}
	else {
	    printf("ERROR: Disk with name \"%s\" does not exist.\n", diskName);
	    return 1;
	}

	// set the pointer to start of the file
	FILE *diskFilePointer;
	diskFilePointer = fopen(diskName, "r+");

	// initialize the disk
	diskStruct *disk = initDisk(diskName, diskFilePointer);

	int i;
	for (i = 1; i < nLines; ++i) {
		char *currentLine = removeNewLine(lines[i]);
		// printf("%s", currentLine);
		if (currentLine[0] != 'L' && currentLine[0] != 'F' && currentLine[0] != 'D') {
			char *fileName = parseFileName(currentLine);
			int fileSize = parseFileSize(currentLine);
			switch (currentLine[0]) {
				case 'C' : // create 
				{
					create(fileName, fileSize, disk, diskFilePointer);
					break;
				}
				case 'R' : // read
				{
					int a = 3;
					char buffer[NDUMMYSPACE + 1]; // 1024 chars
					readFile(fileName, fileSize, buffer, disk);
					break;
				}
				case 'W' : // write
				{
					char buffer[NDUMMYSPACE]; // 1023 chars
					int bufIndex;
					for (bufIndex = 0; bufIndex < sizeof(buffer) / sizeof(char); ++bufIndex) {
						buffer[bufIndex] = 'b';
					}



					break;
				}
				default :
				{
					printf("Invalid command character on line %d: %c\n", i, lines[i][0]);
					return 1;
				}
			}
		}
		else if (currentLine[0] == 'D') { // delete
			char *fileName = parseFileName(currentLine);
			delete(fileName, disk, diskFilePointer);
		}
		else if (currentLine[0] == 'L') { // list
			ls(disk);
		}
		else if (currentLine[0] == 'F') { // defragment
			defragment(disk);
		}
		else {
			printf("Invalid command character on line %d: %c\n", i, lines[i][0]);
			return 1;
		}
	}

	fclose(diskFilePointer);
	return 0;
}
