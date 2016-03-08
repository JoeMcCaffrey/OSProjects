/* File system code 

Jim Calabro 27970265
Joe McCaffrey 27434201

used 2 late days

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NDUMMYSPACE 1023


typedef struct iNode {
	char name[8];
	unsigned int size;
	unsigned int block_pointers[8];
	unsigned int used; // 0 = free, 1 = in use
} iNode;

// 128 bit ints not supported on 32 bit (edlab)

typedef struct superblock {
	char block_list[128]; // 128 bytes
	iNode iNode_array[16]; // 768 bytes
	unsigned int unused_space[32]; // 128 bytes used to fill the superblock to 1024 bytes

	
} superblock; // total 1024 bytes

// so theres 128 bytes left over in the file

char * diskpt;

char dummy_write[1024] = "lolololololololololololo";

char * dummy_write_ptr = dummy_write;

char dummy_read[1024] = "";
char * dummy_read_ptr = dummy_read;

char *parseFileName(char *line) {
	char *temp = (char *) malloc(sizeof(line));
	strcpy(temp, line);
	char *token;
	const char demiliter[2] = " ";
	token = strtok(temp, demiliter);
	token = strtok(NULL, demiliter);
	return token;
}
// for the delete that has no space after the end of the name
char *parseFileName2(char *line) {
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

char *removeTrailingNewLine(char *line) {
	int i;
	for (i = (sizeof(line) / sizeof(char)) + 2; i >= 0; --i) {
		if (line[i] == '\n') {
			line[i] = '\0';
		}
	}

	return line;
}



int create(char *name, int size) {

	printf("creating file %s\n",name);

	// open file and read in the superblock

	FILE * ff = fopen(diskpt, "rb+");
	
 	fseek(ff, sizeof(struct superblock), SEEK_SET );

	struct superblock * sblock = calloc(sizeof(struct superblock),sizeof(struct superblock));

	fread( sblock, sizeof(struct superblock), 1, ff);

	int found_node = 0;
	int location;

	// check to see if there is an empty inode


	int i;
	for(i=0 ; i< 16; i++){
		if(sblock->iNode_array[i].used == 0){
			found_node =1;
			location = i;
			break;
		}
	} 

	if(found_node == 0 ){
		printf("no empty inodes found\n");
		return -1;
	}
	
	// check to see if a duplicate name is in the filesystem

	for(i = 0; i < 15; i++){
		if(strcmp(sblock->iNode_array[i].name, name)== 0){
			printf("same name found\n");
			return -1;
		}
	}		

	// check to see if we have enough storage blocks
	int block_check =0;
	unsigned int found_blocks[8]= {0,0,0,0,0,0,0,0};

	for(i =1 ; i<= 127 ; i++){
		if(block_check == size){
			break;
		}
		if(sblock->block_list[i] == '0'){
			found_blocks[block_check] = i;
			block_check ++;
		}
	}
	
	if(block_check != size){
		printf("not enough inodes found\n");
		return -1;
	}
	
	// if weve made it this far then we can write the buf to file



	strcpy(sblock->iNode_array[location].name, name)  ;

	sblock->iNode_array[location].size = size;
	
	for(i = 0; i<8 ; i++){
	sblock->iNode_array[location].block_pointers[i] = found_blocks[i];
	}

	sblock->iNode_array[location].used = 1;
	
	//update free block list

	for(i =0; i < 8; i++){
		if(found_blocks[i] != 0){
			printf("CREATED BLOCKS: \n");
			printf("%d\n", found_blocks[i]);
			sblock->block_list[found_blocks[i]] = '1';
		}
	}

	
	fseek(ff,sizeof(struct superblock),SEEK_SET);
	fwrite(sblock, sizeof(struct superblock),sizeof(sblock), ff);
	printf("create sucessful \n");
        fclose(ff);
	free(sblock);



	// first block in the block list will always be 1, dont check it


	return 0;
}


int delete(char *name) {
	printf("Deleting....%s\n",name);
	int i;
	int fileFound = 0;
	int found =0;

    FILE * ff = fopen(diskpt, "rb+");

    fseek(ff, sizeof(struct superblock), SEEK_SET );

    struct superblock * sblock = calloc(sizeof(struct superblock),sizeof(struct superblock));

    fread( sblock, sizeof(struct superblock), 1, ff);

	for(i = 0; i < 16; i++){
		
		//intf("found %d\n",strcmp(sblock->iNode_array[i].name, name));
		if(strcmp(sblock->iNode_array[i].name , name) == 0){
			fileFound = i;
			found = 1;
			break;
		}
		
	}

	if(found == 0){
		printf("file %s not found to delete\n", name);
		return -1;
	}
	
	for( i = 0; i< 8; i++){
		sblock->block_list[sblock->iNode_array[fileFound].block_pointers[i]] = '0';
			
	}

	sblock->iNode_array[fileFound].used = 0;
	sblock->iNode_array[fileFound].size = 0;
	
	
	for(i =0; i<8; i++){
		sblock->iNode_array[fileFound].block_pointers[i] = 0;
		sblock->iNode_array[fileFound].name[i] = '0';
	}


	fseek(ff,sizeof(struct superblock),SEEK_SET);
        fwrite(sblock, sizeof(struct superblock),sizeof(sblock), ff);
	printf("Delete success\n");
        fclose(ff);
        free(sblock);
	

	return 0;

}


int ls() {
printf("ls -------------------\n");	
	FILE * ff = fopen(diskpt, "r+");
	if(ff == NULL){
		printf("File not found, make sure its saved\n");
	}
 	fseek(ff, sizeof(struct superblock), SEEK_SET );
	struct superblock * sblock = calloc(sizeof(struct superblock),sizeof(struct superblock));

	fread( sblock, sizeof(struct superblock), 1, ff);
	int i;
	for (i = 0; i < 15; ++i) {
		if (sblock->iNode_array[i].used == 1) {
			printf("filename: %s, size: %u\n", sblock->iNode_array[i].name, sblock->iNode_array[i].size);
		}
		else{
		printf("file is empty\n");
		}
	}
	printf("ls sucessful\n");
	fclose(ff);
	free(sblock);
	return 0;
}


int read_file(char *name, int block_num, char buffer[1024]) {

	printf("Reading file......%s\n",name);
	int i;
	FILE * ff = fopen(diskpt, "rb+");

	fseek(ff, sizeof(struct superblock), SEEK_SET );

	struct superblock * sblock = calloc(sizeof(struct superblock),sizeof(struct superblock));

	fread( sblock, sizeof(struct superblock), 1, ff);


	int found_node = 0;
	int location = 0;
	
	for(i = 0 ; i < 16; i++){
		 if(strcmp(sblock->iNode_array[i].name, name) == 0){
            found_node = 1;
            location = i;
            break;
        }
	}

    if(found_node == 0 ){
            printf("no nodes found with the name\n");
            return -1;
    }

	int spot = sblock->iNode_array[location].block_pointers[block_num];

	if(sblock->block_list[spot] == '1'){

		fseek(ff, sizeof(char)*1024, (1024* spot)-1024);
		fread(buffer, sizeof(char)*1024,1,ff);
		
		printf("read success, buffer:%s\n ", buffer);
		fclose(ff);
		return 0;
	}
	else{
		printf("does not exist, read failed\n");
		return -1;
	}
	


}


int write_file(char *name, int block_num, char buffer[1024]) {
	printf("writing.....%s\n",name);
	int i;
	FILE * ff = fopen(diskpt, "rb+");

	fseek(ff, sizeof(struct superblock), SEEK_SET );

	struct superblock * sblock = calloc(sizeof(struct superblock),sizeof(struct superblock));

	fread( sblock, sizeof(struct superblock), 1, ff);


	int found_node = 0 ;
	int location =0;
	
	for(i=0 ; i< 16; i++){
		if(strcmp(sblock->iNode_array[i].name, name)== 0){
		    found_node =1;
		    location = i;
		    break;
		}
	}

	if(found_node == 0 ){
		printf("no nodes found with that name\n");
		return -1;
	}

	int file_size = sblock->iNode_array[location].size ;
	
	if(sblock->iNode_array[location].block_pointers[block_num] == '0'){
		printf("not enough room in file\n");
		return -1;
	}
	int empty_block =0 ;
	for(i =1; i<=127 ; i++){
		if(sblock->block_list[i] == '0'){
			empty_block =i;
			sblock->block_list[i] = '1';
			break;
		}
	} 

	sblock->iNode_array[location].block_pointers[block_num] = empty_block;

	fseek(ff, sizeof(char)*1024, 1024* empty_block);
	fwrite(buffer, sizeof(char)*1024, sizeof(char)*1024,ff);
	
 
	fseek(ff,sizeof(struct superblock),SEEK_SET);
        fwrite(sblock, sizeof(struct superblock),sizeof(sblock), ff);
	printf("write sucessful\n");
        fclose(ff);
        free(sblock);

	return 0;
}

// save this into a buffer so we can write it in


int initDisk(char * disk_name){
	int i;

	FILE * ff = fopen(disk_name, "r+");

	if(ff == NULL){
		printf("Disk not found, make sure disk and input file are saved\n");
	}

    fseek(ff, sizeof(struct superblock), SEEK_SET );
    struct superblock * sblock = calloc(sizeof(struct superblock),sizeof(struct superblock));

    fread( sblock, sizeof(struct superblock), sizeof(sblock), ff);

	if(sblock-> block_list[0] != '1'){
		printf("new disk so init\n");
		sblock->block_list[0] = '1';
		for(i =1 ; i<= 127; i++){
			sblock->block_list[i] = '0';
		}
	
		for(i =0; i <16 ; i++){
			sblock->iNode_array[i].used = 0;
		}
		fseek(ff,sizeof(struct superblock),SEEK_SET);
		
		fwrite(sblock, sizeof(struct superblock),sizeof(sblock), ff);

	}
	else{
		printf("disk is not new\n");
	}


    fclose(ff);
	free(sblock);
	return 0;
}


int main(int argc, char **argv) {
	// validate program arguments
	if (argc != 2) {
		printf("Usage: \t$ ./FileSystem <name_of_disk>\n");
		return 1;
	}

	// Open the file and read it into the lines array
	FILE *filePointer;
	char *lines[255];
	char buffy[8] = "";
	char * line = NULL;
	size_t len =0;
	size_t read;
	diskpt = buffy;

	
	filePointer = fopen(argv[1], "r");

	int nLines = 0;
	while ((read = getline(&line, &len, filePointer)) != -1) {
		lines[nLines] = malloc(len);
		strcpy(lines[nLines], line);
		nLines++ ;
	}
	fclose(filePointer);


	diskpt = lines[0];

	int i;

	// if the disk name ends with a newline, remove it
	if (diskpt[(sizeof(diskpt) / sizeof(char))] == '\n') {
		printf("inside n \n");
		char *tempString = (char *) calloc(1,(sizeof(diskpt) / sizeof(char)) - 1);
		for (i = 0; i < (sizeof(tempString) / sizeof(char)); ++i) {
			tempString[i] = diskpt[i];
		}

		diskpt = tempString;
	}

 	initDisk(diskpt);

for (i = 1; i < nLines; ++i) {
		char *currentLine = removeTrailingNewLine(lines[i]);
		printf("%s\n", currentLine);
		if (currentLine[0] != 'L' && currentLine[0] != 'F' && currentLine[0] != 'D') {
			char *fileName = parseFileName(currentLine);
			int fileSize = parseFileSize(currentLine);
			switch (currentLine[0]) {
				case 'C' : // create 
				{
					create(fileName, fileSize);
					break;
				}
				case 'R' : // read
				{
					int a = 3;
					char buffer[NDUMMYSPACE + 1]; // 1024 chars
					read_file(fileName, fileSize, buffer);
					break;
				}
				case 'W' : // write
				{
					char buffer[NDUMMYSPACE]; // 1023 chars
					int bufIndex;
					for (bufIndex = 0; bufIndex < sizeof(buffer) / sizeof(char); ++bufIndex) {
						buffer[bufIndex] = 'b';
					}
					write_file(fileName, fileSize, buffer);			
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
			char *filefile = parseFileName2(currentLine);
			delete(filefile);
		}

		else if (currentLine[0] == 'L') { // list
			ls();
		}
		else if (currentLine[0] == 'F') { // defragment
		//	defragment(disk);
		}
		else {
			printf("Invalid command character on line %d: %c\n", i, lines[i][0]);
			return 1;
		}
	}






	free(diskpt);
 
}