#ifndef MYMALLOC_H
#define MYMALLOC_H
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)


int mallocCallCount;

typedef struct metadataNode{
   size_t sizeContained;
   short freeFlag;
   struct metadataNode *next;
}metadataNode;
int checkValidPtr(metadataNode * node);
size_t UpdateSize (metadataNode *node, metadataNode *nodetobeInherited, size_t metaSize);
void printMetaDetails(metadataNode *ptr);
metadataNode *UpdateNext(metadataNode * node);
void UpdateMetaData(metadataNode *NodetobeUpdated, metadataNode *next, size_t size, short flag);
metadataNode *findValidNode(size_t sizeNeeded);
void* mymalloc(size_t size, char * FILE, int LINE);
void myfree(void *freeptr, char *FILE, int LINE);
void merge();
void makeFirstMetaData();
//finished implementing:
 //done
#endif
