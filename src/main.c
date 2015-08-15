#include "header.h"

int numFrames = 0;
char *fileContents;

int main() {
	promptFrames();	//prompt frames
	checkFrames();	//make sure user didnt enter 0 frames/make sure it doesnt explode
	importPage();	//pages.txt into fileContents
	printInfo();	//prints framecount and pages.txt contents
	FIFO();
	LRU();			//4 algorithms
	OPT();
	Clock();
	read();			//use this so cmd doesn't close on a windows machine
	return 0;
}
//fifo implementation
void FIFO() {
	int count = 0, ptr = 0, full = 0;
	int frames[numFrames];	//initialize array with desired frame count
	int *stack = createArray(fileContents);
	int stackSize = getArraySize(fileContents);
	//////////////////FUNCTIONS USED BY FIFO/////////////////////////////////////
	//replaces frame with element
	void replace(int element) {
		frames[ptr] = element;
		ptr++;
		if(ptr == numFrames)	//if ptr = numFrames we reset
			ptr = 0;
	}
	void fault(int element) {
		if(full != numFrames)	//this will fill the frames until full
			frames[full++] = element;
		else
			replace(element);
	}
	//returns 1 if element is found in frame array
	int search(int element) {
		int flag = 0;
		int i;
		if (full != 0) {		//run if not empty
			for (i = 0; i < full; i++)
				if (element == frames[i]) {	//if found set flag to 1
					flag = 1;
					break;
				}
		}
		return flag;
	}
///////////END FUNCTIONS////////////////////////////////////////////
	int x;
	for(x = 0; x < stackSize; x++) {
		if(search(stack[x])!= 1) {	//if element not found
			fault(stack[x]);	//we have a fault
			count++;
			}
		}
		printf("FIFO has %d page faults.\n", count);
}
//lru with a counter
void LRU() {
	int count = 0, ptr, full = 0;
	int time = 0;
	int frames[numFrames];	//initialize array with desired frame count
	int *stack = createArray(fileContents);
	int stackSize = getArraySize(fileContents);
	int counter[numFrames];
	//FUNCTIONS//////////////////////////////////////////
	int findLRUpage() {
		int minimum = 0, i;
		for (i = 0; i < numFrames; i++)
			if(counter[minimum] > counter[i])	//find smallest counter and return it
				minimum = i;
				ptr = minimum;
			return ptr;
	}

	void replace(int element) {
		ptr = findLRUpage();
		frames[ptr] = element;
		counter[ptr] = time;
	}

	void fault(int element) {
		if(full != numFrames) {	//this will fill the frames until full
			counter[full] = time;
			frames[full++] = element;
		}
		else
			replace(element);
	}

	int search(int element) {
		int x, flag = 0;
		if (full != 0) {	//if we have something do work
			for (x = 0; x < full; x++)
				if (element == frames[x]) {		//if we find the page
					flag = 1;
					counter[x] = time;
					break;
				}
			}
			return flag;
	}
	//ENDFUNCTIONS//////////////////////////////////////
	int i;
	for (i = 0; i < stackSize; i++)
	{
		time++;
		if(search(stack[i]) != 1) {
			fault(stack[i]);
			count++;
		}
	}
	printf("LRU has %d page faults.\n", count);
}
//optimal implementation
void OPT() {
	int count = 0, ptr, full = 0;
	int time = 0;
	int frames[numFrames];	//initialize array with desired frame count
	int *stack = createArray(fileContents);
	int stackSize = getArraySize(fileContents);

	int findFurthest() {
	int temp[numFrames];
		int c = 0, i, j;
		int iden = 0;
		for (i = time + 1; i < stackSize; i++) {
			for (j = 0; j < numFrames; j++) {	//we see if a frame is in the future
				if(stack[i] == frames[j]) {
					if(temp[j] != 1) {
						temp[j] = 1;
						c++;
					}
					break;
				}
			}
			if (c == 4)	//get out once we have 4 future frames/chances are we'll have 4
				break;
		}
		iden = 4;
		while (iden != 0) {		//iterate backwards
			if(temp[iden] == 0)	//if there is a match get out; iden is ptr and return ptr
				break;
				iden--;			//otherwise we increment down
		}
		ptr = iden;
		return ptr;				//we return its location
	}
	void replace(int element) {
		ptr = findFurthest();		//we find furthest and replace it
		frames[ptr] = element;
	}

	void fault(int element) {
		if(full != numFrames)		//fill until frames is full
			frames[full++] = element;
			else
				replace(element);	//otherwise we replace
	}
	int search(int element) {	//simple search, if we find a match return 1
		int flag = 0, y;
		if (full != 0) {
		for (y = 0; y < full; y++)
			if(element == frames[y]) {
				flag = 1;
				break;
			}
		}
		return flag;
	}
	//ENDFUNCTIONS////////////////////////////
	int i;
	for (i = 0; i < stackSize; i++) {
		time = i;		//we pass back time for the farthest function
		if(search(stack[i]) != 1) {
			fault(stack[i]);
			count++;
		}
	}
		printf("Optimal has %d page faults.\n", count);
}
//clock implementation
void Clock() {
	int count = 0, ptr = 0, full = 0;
	int frames[numFrames];	//initialize array with desired frame count
	int *stack = createArray(fileContents);
	int stackSize = getArraySize(fileContents);
	int bits[numFrames];
	//FUNCTIONS//////////////////////////////////////
	void replace(int element) {
		while(bits[ptr] != 0) {	//while bit is 1 we set it to 0
			bits[ptr++] = 0;
			if(ptr == numFrames)	//if we hit numFrames we reset
				ptr = 0;
		}	//if we hit a 0 bit we put element into frames and set bit to 1
		frames[ptr] = element;
		bits[ptr] = 1;
	}
	void fault(int element) {
		if(full != numFrames) {	//we fill up the frames array
			bits[full] = 1;		//we set the bit reference bit to 1
			frames[full++] = element;	//fill the next frame with element until we hit numFrames-1 so all are filled
 		}
		else
			replace(element);
	}
	//search returns a 1 if an element is found
	//and will set the bit to 1
	int search(int element) {
		int flag = 0, i;
		if (full != 0) {
			for (i = 0; i < full; i++)
				if (element == frames[i]) {
					flag = 1;
					bits[i] = 1;
					break;
				}
			}
			return flag;
		}
		///ENDFUNCTIONS////////////////////////////
	int i;
	for (i = 0; i < stackSize; i ++) {
		if (search(stack[i]) != 1) {
			fault(stack[i]);
			count++;
		}
	}
	printf("Clock has %d page faults.\n", count);
}
//prints your frame count and contents of pages.txt
void printInfo() {
	printf("You chose to allocate %d frames.\n",numFrames);
	printf("Contents of pages.txt: %s\n",fileContents);
}
//will create an array with the pages for easy use
int *createArray(char *contents) {
	int *tempArray = malloc(sizeof(int) * getArraySize(fileContents));
	int count = 0, number, length;
	while (sscanf(contents, "%d%n", &number, &length) == 1) {
		tempArray[count] = number;
		contents = contents + length;
		count++;
	}
		return tempArray;
}
//not the nicest way to get the number of elements but it works
int getArraySize(char *contents) {
	int count = 0, number, length;
	while (sscanf(contents, "%d%n", &number, &length) == 1) {
		contents = contents + length;
		count++;
	}
	return count;
}
//sets numFrames to how many frames user specified **FIX THIS LATER NOT COMPLETE
void promptFrames() {
	printf("Please enter the amount of frames you wish to use: \n");
		while (scanf("%d", &numFrames) != 1) {	//scanf only returns 1 if it is an integer
			while (getchar() != '\n') {			//we clear the stdin buffer otherwise prog locks up
			}
			printf("Please enter a valid integer: \n");
		}
}
//check to make sure user didnt enter 0 frames
void checkFrames() {
	if (numFrames == 0) {
		printf("You entered 0 frames, INVALID\n");
		exit(EXIT_FAILURE);
	}
}
//Will load page.txt into fileContents
void importPage() {
	long fileSize;
	FILE *fp = fopen("pages.txt", "rt");
	if (fp == NULL) {
		printf("ERROR FILE NOT FOUND\n");
		exit(EXIT_FAILURE); //file not found call macro
	}
	else {
		fseek(fp,0,SEEK_END);		//lets get the size of the file
		fileSize = ftell(fp);
		rewind(fp);
		fileContents = malloc(sizeof(char) * fileSize+1);	//allocate memory for contents
		fread(fileContents, sizeof(char), fileSize, fp);	//fill contents with file_contents
		fclose(fp);
		fileContents[fileSize] = 0;		//set the end to 0, otherwise we have jibberish at the end
	}
}


