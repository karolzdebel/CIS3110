#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _GBS 1000000000
#define _MBS 1000000
#define _KBS 1000
#define _BS 1

#define LONG 2814585000

static FILE *createFile(int size, char *name){
	int i;
	FILE *file = fopen(name,"w+");

	for (i=0;i<size;i++){
		fputc('a',file);
	}

	return file;
}

static double testTime(long int size){
	char *buffer = malloc(sizeof(char)*size);
	double total,dubStart,dubEnd;
	FILE *fp;
	struct timeval start, end;

	fp = createFile(size,"test.tmp");
	fseek(fp, SEEK_SET, 0);


	gettimeofday(&start, NULL);
	fread(buffer, sizeof(char), size, fp);
	gettimeofday(&end, NULL);
    
    dubStart = start.tv_sec + (double)start.tv_usec/1000000;
    dubEnd = end.tv_sec + (double)end.tv_usec/1000000;
    total = dubEnd - dubStart;

	free(buffer);
	remove("test.tmp");

	return total;
}

int main(){
	FILE *fp;
	long int i;
	double test;
	
	printf("Processing small scale results\n");
	
	/*Small scale testing*/	
	fp = fopen("smallresults.txt","w+");
	for (i=0;i<=_KBS*10;i+=100){
		test = testTime(i);
		
		fprintf(fp,"%ld, %lf\n",i,test);
	}
	fclose(fp);

	printf("\nProcessing medium scale results\n");

	/*Medium scale testing*/
	fp = fopen("mediumresults.txt","w+");
	for (i=0;i<=_KBS*100;i+=_KBS){
		test = testTime(i);

		fprintf(fp,"%ld, %lf\n",i/_KBS,test);
	}	
	fclose(fp);

	printf("\nProcessing large scale results\n");
	
	/*Large scale testing*/
	fp = fopen("largeresults.txt","w+");
	for (i=0;i<=_GBS;i+=_MBS*100){
		test = testTime(i);
		
		fprintf(fp,"%ld, %lf\n",i/_MBS,test);
	}
	fclose(fp);

	printf("\nResults successfully written\n");
	return EXIT_SUCCESS;
}