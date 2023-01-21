#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
int main()
{
	// creating file pointer to work with files
    FILE *fptr;

    // opening file in writing mode
    fptr = fopen("D:\\TTU SEM 1\\Logic of Computer Scientists\\Project\\C programs\\program.cnf", "w");
    
    // exiting program 
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    int n,i;
	scanf("%d",&n);
	for(i=1;i<n*n;i++)
	{
		fprintf(fptr,i);
		if(i%n==0)
		{
			fprintf(fptr, i+"\n");
		}
	}
    fprintf(fptr, "-1 -2 0\n");
    fprintf(fptr, "-1 -3 0\n");
    fprintf(fptr, "-1 -4 0\n");
	printf("hello");
}