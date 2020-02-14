#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "mymalloc.h"


static char fakeHeap[4096];
int mallocCallCount = 0;
//Pointer to beginning of Memory.
struct metadataNode *head = (void*) fakeHeap;

size_t UpdateSize (metadataNode *node, metadataNode *nodetobeInherited, size_t metaSize){
	// printf("%d\n",node->sizeContained);
	// printf("%d\n",nodetobeInherited->sizeContained );
	// printf("%d\n",metaSize);

	if(!nodetobeInherited){
		size_t newSize = (node->sizeContained);
		// printf("merge took place. new size is: %d \n", newSize);
		return newSize;
	}
	else{
		size_t newSize = (node->sizeContained)+ (nodetobeInherited->sizeContained) + metaSize;
		// printf("merge took place. new size is: %d \n", newSize);
		return newSize;
	}

	return 1;
}


// void printMetaDetails(metadataNode *ptr){
// 	if(ptr!=NULL){
// 		printf("size is: %lu, freeFlag is : %hd \n", ptr->sizeContained, ptr->freeFlag);
// 	}
// 	else{
// 		printf("curr Node is NULL\n" );
// 	}
// }


void UpdateMetaData(metadataNode *NodetobeUpdated, metadataNode *next, size_t size, short flag){
	NodetobeUpdated->freeFlag = flag;
	NodetobeUpdated->sizeContained = size;
	NodetobeUpdated->next = next;
}



metadataNode *findValidNode(size_t sizeNeeded){
	metadataNode *temp = head;
	if(temp->next == NULL){
		if((temp->sizeContained < sizeNeeded) || (temp->freeFlag == 0)){
				return temp = NULL;
		}
	}
	while((temp->next != NULL) && (((temp->sizeContained) < sizeNeeded ) || ((temp->freeFlag) == 0))){
		temp = temp->next;
	}
	return temp;
}

void makeFirstMetaData(){
	head -> sizeContained = 4096 - sizeof(struct metadataNode);
	head -> freeFlag = 1;
	head -> next = NULL;
	printf("This is the first time mymalloc was called.\n" );
}

int checkValidPtr(metadataNode * node){
	metadataNode * temp = head;
	while(temp != NULL){
		if(temp == node){
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

//first-fit-algorithm
//malloc function
void* mymalloc(size_t sizeofBytes, char * FILE, int LINE){
	struct metadataNode *curr;
	void *returnPointer;
	//if users asks for more than is allowed or less than 0
	if(sizeofBytes > 4096 || sizeofBytes < 0){
		fprintf(stderr, "Error in file:%s, on line: %d :You cannot ask for that number of bytes. Please choose a number between 0 and  %lu\n", FILE, LINE, (4096-sizeof(metadataNode)));
		returnPointer = NULL;
		return returnPointer;
	}
	//if user asks for zero just return NULL pointer
	if(sizeofBytes == 0){
		printf("User asked for 0 bytes, but that is okay. NULL was returned\n");
		returnPointer = NULL;
		return returnPointer;
	}
	//check if this is the first time malloc has been called
	//if it is make the first Meta Data and initialize the memory.
	if(mallocCallCount == 0){
		makeFirstMetaData();
		mallocCallCount = 1;
	}
	//this is where the algorithm actually starts...
		//start pointer at first metadata.
		// curr = head;
		//traverse through all metadata until find one with freeflag AND enough sizeContained. Else loop will terminate at the last metadata.
		curr = findValidNode(sizeofBytes);
		//curr is now pointing to a block which has the free flag and has at least enough space to allocate the requested number of bytes
		//OR NULL if could not find any valid node.
		size_t currentBlockSize = 0;
		if(curr != NULL){
			 currentBlockSize = (curr->sizeContained);
		}
		size_t metadata = sizeof(metadataNode);
		//now that loop has terminated curr is either at a valid metadata that has a big enough Blocksize to handle amount of memory asked by the user
		//OR
		// printf("block size is: %lu \n", currentBlockSize);
		//there was more than enough space in the current block so you need to allocate the space and
		//then create a new metaData after describing the remaining amount of space leftover
		if(currentBlockSize > (sizeofBytes + sizeof(metadataNode))){
			if(curr->freeFlag == 0){
				fprintf(stderr, " Error in FILE: %s in LINE: %d: Sorry, at this moment, the heap does not have available that amount of memory. \n", FILE, LINE );
				return NULL;
			}
			else{
				// printf("current block size is: %lu, requested bytes was : %lu...size is greater so need to split up the block and make a new metadata. \n",currentBlockSize, sizeofBytes );
				metadataNode *excessBlock=(void*)((void*)curr+sizeofBytes+sizeof(metadataNode));
				 size_t excessFreeMem = (currentBlockSize - sizeofBytes - metadata);
				 UpdateMetaData(excessBlock, curr->next, excessFreeMem, 1);
				 UpdateMetaData(curr, excessBlock, sizeofBytes, 0);
				 returnPointer = (void*)(++curr);
				 return returnPointer;
			}

		}
		//there was exactly enough space -- pretty rare case
		else if(sizeofBytes == currentBlockSize){
			// printf("current block size is: %lu, requested bytes was : %lu...this was exactly enough space \n",currentBlockSize, sizeofBytes );

			//set freeflag to 0
			//return void pointer to the block of memory after the metaData
			//curr ++ means curr + sizeof(curr) = pointer to beginning address location of array of bytes
			(curr->freeFlag) = 0;
			returnPointer = (void*)(++curr);
			return returnPointer;
		}
		else{
			fprintf(stderr, " Error in FILE: %s in LINE: %d: Sorry, at this moment, the heap does not have available that amount of memory. \n", FILE, LINE );
			return NULL;
		}

	return returnPointer;
}






void myfree(void *freeptr, char* FILE, int LINE){
	//if you try to free a null pointer throw error.=
	/*
		else: There a few things you need to check for.
		1)Check to see if the pointer passed in is actually an address within the space allocated by malloc.
					This can be done by a simple if check by just checking the bounds.
					All you need to do is make sure address of curr is >= lowerbound && <= upperbound.. where lowerbound = beginning address of the fakeHeap and upperbound is the last address of the fakeHeap
					if this check fails return an error because you cannot free pointers that were not allocated by malloc.
		2)If the pointer is in fact a valid pointer then you can free the block by setting free flag to 1.
			However you can only do this if there are no consecutive free blocks.
		 	If there are two or free blocks in a row you need to delete the extra metadata
		*/
	if(freeptr == NULL){
		fprintf(stderr, "Error in file:%s on line: %d: Invalid pointer. You cannot free a null pointer. \n", FILE, LINE );
		return;
	}
	void *lowerbound = (void*)(fakeHeap);
	void *upperbound = (void*)(fakeHeap+4096);
	// printf("free was called\n");
	if(freeptr <= upperbound && freeptr >= lowerbound){
		metadataNode *UpdatedFreeNode = freeptr; // currently points to beginning of allocated memory.
		--UpdatedFreeNode; // ptr now points to the MetaDataNode that contained info about the freed block. == UpdatedFreeNode = UpdatedFreeNode -sizeof(metadataNode);
			if(UpdatedFreeNode->freeFlag == 1 ){
				fprintf(stderr, "ERROR in file  %s, on line %d: you cannot free an already freed pointer!\n", FILE,LINE );
				return;
			}
			if(checkValidPtr(UpdatedFreeNode) == 0){
				fprintf(stderr, "ERROR in file  %s, on line %d: The pointer trying to be freed was not allocated by malloc!\n", FILE,LINE);
				return;
			}
			else{
				UpdatedFreeNode->freeFlag=1; //free()ing so set free flag to 1;
				size_t metadata = sizeof(metadataNode);
				metadataNode *tempptr = head;
					while(tempptr && (tempptr->next!=NULL)){
						 if(((tempptr->freeFlag) == 1 && (tempptr->next->freeFlag) == 1) && tempptr){
							// printf("size currently is %d \n", tempptr->sizeContained );
						  tempptr->sizeContained = UpdateSize(tempptr, tempptr->next, metadata);
						  tempptr->next = tempptr->next->next;
						 }
					tempptr=tempptr->next;
					}
			}
	}
	//Did not give a valid ptr so return error.
	else{
		fprintf(stderr, "ERROR in file  %s, on line %d: The pointer trying to be freed was not allocated by malloc!\n", FILE,LINE);
	}

}
