#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "mymalloc.h"



int genRandNum(int ceiling);

// int i = 0;
// int j =0;
int testcount = 0;
char *ptr;
char *wkloadB[150];

double wkloadAtime;
double wkloadBtime;
double wkloadCtime;
double wkloadDtime;
double wkloadEtime;
double wkloadFtime;




void workloadA(){
	int i = 0;
	for(i=0; i<150;i++){
		ptr = (char*)malloc(1);
		free(ptr);
	}
	// testcount++;
}
 void workloadB(){
	 int i = 0;
	 int j = 0;
	 for(i = 0; i < 150; i++){
		 //writes ptrs to indicies 0-149
		 wkloadB[i]= (char*)malloc(1);

		 if(i == 49){
			  //frees ptrs in indicies 0-49
			 for(j = 0; j<50;j++){
				 free(wkloadB[j]);
			 }
		 }
		 if (i == 99){
			  //frees ptrs in indicies 50-99
			 for(j = 50; j<100;j++){
				 free(wkloadB[j]);
			 }
		 }
		 if(i == 149){
			 //frees ptrs in indicies 100-149
			 for(j = 100; j<150;j++){
				 free(wkloadB[j]);
			 }
		 }
	 }


 }
 void workloadC(){

	 int allocCount = 0;
	 int runCount = 0;
	 srand(time(0));
	 while(allocCount <= 50){
		 printf("C has run %d times so far\n", runCount);
		 runCount++;
		 int num = genRandNum(10);
		 printf("%d\n", num);
		 //if even malloc
			 if(num % 2 == 0){
				ptr = (char*) malloc(1);
				 allocCount++;
				 printf("alloc count is: %d\n",allocCount );
			 }
		 // if odd free
			 else{
				 free(ptr);
			 }
	 }
 }

 void workloadD(){
	 int i = 0;
	 int allocCount = 0;
	 char *ptrarr[50];
	 // int runCount = 0;
	 srand(time(0));
	 while(allocCount <= 50){
		 // printf("C has run %d times so far\n", runCount);
		 // runCount++;
		 int num = genRandNum(10);
		 // printf("%d\n", num);
		 //if even malloc
			 if(num % 2 == 0){
				 int mallocSize = genRandNum(64);
				 printf("malloc size is: %d\n", mallocSize);
				ptrarr[allocCount] = (char*) malloc(mallocSize);
				 allocCount++;
				 // printf("alloc count is: %d\n",allocCount );
			 }
		 // if odd free
			 else{
				 free(ptrarr[allocCount]);
			 }
	 }
	 //free all the remaining pointers. If it has already been freed there will be an error instead and then continute till i = 50.
	 for(i = 0; i < 50; i ++){
		 free(ptrarr[i]);
	 }

 }

 void workloadE(){
	 int i = 0;
	 int j = 0;
	//mallocs 1 byte, then 2 bytes , then 3 bytes.... and then 50 bytes.
	int mallocAmount = 1;
	char * p [50];
	for(i = 0; i < 50; i++){
		p[i] = (char*)malloc(mallocAmount);
		mallocAmount++;
	}
	for(i = 0; i < 50; i++){
		free(p[i]);
	}
	//malloc 50bytes 50 times then free every 25
	for(i = 0; i < 50; i++){
		p[i] = (char*)malloc(mallocAmount);
		if(i == 24){
			for(j = 0; j < 25; j++){
				free(p[j]);
			}
		}
		if(i == 49){
			for(j = 25; j < 50; j++){
				free(p[j]);
			}
		}
	}

 }
 void workloadF(){
	 //malloc 1500 bytes twice then free them. Do this 2x;
	 //then malloc a single byte twice and then free that.
	 //then repeat the first process one last time.
	 int i = 0;
	 char * p[2];
	 for(i = 0; i < 2; i++){
		 p[i] = (char*)malloc(1500);
	 }
	 for(i = 0; i < 2; i++){
		 free(p[i]);
	 }
	 for(i = 0; i < 2; i++){
		 p[i] = (char*)malloc(1500);
	 }
	 for(i = 0; i < 2; i++){
		 free(p[i]);
	 }
	 for(i = 0; i < 2; i++){
		 p[i] = (char*)malloc(1);
	 }
	 for(i = 0; i < 2; i++){
		 free(p[i]);
	 }
	 for(i = 0; i < 2; i++){
		 p[i] = (char*)malloc(1500);
	 }
	 for(i = 0; i < 2; i++){
		 free(p[i]);
	 }


}


//helper methods
int genRandNum(int ceiling){
	int randNum =  1 + (rand() % ceiling);
	return randNum;
}



//main
int main(int argc, char const *argv[]) {
	struct timeval before;
	struct timeval after;
	int k = 0;
	//-------------------------------------WORKD LOAD A----------------------------------//
	gettimeofday (&before, NULL);
	for(k = 0; k <100; k++){
		workloadA();
	}
	gettimeofday (&after, NULL);
	wkloadAtime = ((after.tv_sec - before.tv_sec)*1000000L +after.tv_usec) - before.tv_usec;
    //
	// //-------------------------------------WORKD LOAD B----------------------------------//

	gettimeofday (&before, NULL);
	for(k = 0; k <100; k++){
		workloadB();
	}
	gettimeofday (&after, NULL);
	wkloadBtime = ((after.tv_sec - before.tv_sec)*1000000L +after.tv_usec) - before.tv_usec;
    //
	// // //-------------------------------------WORKD LOAD C----------------------------------//
    //
	gettimeofday (&before, NULL);
	for(k = 0; k <100; k++){
		workloadC();
	}
	gettimeofday (&after, NULL);
	wkloadCtime = ((after.tv_sec - before.tv_sec)*1000000L +after.tv_usec) - before.tv_usec;

	// -------------------------------------WORKD LOAD D----------------------------------//

	gettimeofday (&before, NULL);
	for(k = 0; k <100; k++){
		workloadD();
	}
	gettimeofday (&after, NULL);
	wkloadDtime = ((after.tv_sec - before.tv_sec)*1000000L +after.tv_usec) - before.tv_usec;

	//-------------------------------------WORKD LOAD E----------------------------------//

	gettimeofday (&before, NULL);
	for(k = 0; k <100; k++){
		workloadE();
	}
	gettimeofday (&after, NULL);
	wkloadEtime = ((after.tv_sec - before.tv_sec)*1000000L +after.tv_usec) - before.tv_usec;

// //-------------------------------------WORKD LOAD F----------------------------------//
	gettimeofday (&before, NULL);
	for(k = 0; k <100; k++){
		workloadF();
	}
	gettimeofday (&after, NULL);
	wkloadFtime = ((after.tv_sec - before.tv_sec)*1000000L +after.tv_usec) - before.tv_usec;

//----------------------------------------------------------------------------------------------
	//time is given in micro seconds!
	printf("average time of workload A was: %f micro seconds.\n",wkloadAtime/100);
	printf("average time of workload B was: %f micro seconds.\n",wkloadBtime/100);
	printf("average time of workload C was: %f micro seconds.\n",wkloadCtime/100);
	printf("average time of workload D was: %f micro seconds.\n",wkloadDtime/100);
	printf("average time of workload E was: %f micro seconds.\n",wkloadEtime/100);
	printf("average time of workload F was: %f micro seconds.\n",wkloadFtime/100);


	// workloadB();
	// workloadC();
	// workloadD();
	// workloadE();
	// workloadF();
}
