#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>
#include "svm.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
void Writeoutputfile();



FILE *fp, *fs, *ft;
int arr[465], i, count, ascii, index, count2,a;
float class_fet;
char str1[50], str2[50], str4[50], ch;
struct svm_parameter param;		// set by parse_command_line
struct svm_problem prob;		// set by read_problem
struct svm_model *model;
struct svm_node *x_space,*x,*featurevector;
double *prob_estimates;
void Writeoutputfile();

void main()
{
	
    
	printf("\n Enter the filename (with out extension)\n\n\t\t");
    scanf("%s",&str4);
	printf("Enter the feature(weka) file name (without extension)\n\n\t\t");
	scanf("%s",&str2);
	strcpy(str1,str4);
	strcat(str1,str2);
	strcat(str4,".txt");
	strcat(str2,".txt");
	fp=fopen(str4,"r"); //reads libsvm file
	ft=fopen(str2,"r"); //reads weka output list of features
    strcat(str1,"_out.txt");
          
       fs=fopen(str1,"w");
	
	printf("Enter the number the of fetures:");
	scanf("%d", &count);
	for(i=0; i<465; i++)	//initializing all arrays
	{
		arr[i]=0;
	}
	if (fp==NULL)
  {
    printf("cannot open  original  featurefile");
	//getchar();
   // exit(0);
  }
  if(ft==NULL)
  {
    printf("cannot open weka list file");
    fclose(ft);
	//getchar();
    //exit(0);
  }
	// printf("1\n");
	count2=1;
	while(1)
	{
		ch=fgetc(ft);
		if (ch==EOF)
			break;
		if (ch==',')
			count2++;
	}
	printf("%d\n", count2);
	i=0;
	fclose(ft);
	ft=fopen(str2,"r");
	while(!feof(ft))
	{
		fscanf(ft,"%d,", &a);
		printf("%d ",a);
		//getchar();
		arr[a]=1;
		i++;
	}
	for (i=0;i<count;i++)
		printf("%d ",arr[i]);
	//getchar();

    printf("2");

	fclose(fp);
	fclose(fs);
	fclose(ft);
    Writeoutputfile( );
	printf("=======workdone=========");
	getch();
}



void Writeoutputfile()
{
int elements, max_index, i, totalsamples,j;
int *correctindex;

	fp = fopen(str4,"r");
	
	if(fp == NULL)
	{
		printf("can't open input file %s\n",str4); // undefine ??
		//exit(1);
	}

	prob.l = 0;
	elements = 0;
	while(1)
	{
		int c = fgetc(fp);
		switch(c)
		{
			case '\n':
				++prob.l;
				// fall through,
				// count the '-1' element
			case ':':
				++elements;
				break;
			case EOF:
				goto out;
			default:
				;
		}
	}
out:
	rewind(fp);
	printf("%d is total number of samples in file \n",prob.l);
   // prob.l= prob.l-1;
	prob.y = Malloc(double,prob.l);
	totalsamples=prob.l;//
	prob.x = Malloc(struct svm_node *, prob.l);// this seems to be erreneous code syntax
	// not required ...add the svm cpp file to the project I am sending  now... prob.x=(struct svm_node*)malloc(sizeof(svm_problem));// this statement i have added, will it be like this if not please rectify
	x_space = Malloc(struct svm_node,elements);// undefine ?? 
    correctindex=(int*)malloc((prob.l)*sizeof(int));// undefine ??
	max_index = 0;
	j=0;
	for(i=0;i<prob.l;i++)
	{
		double label;
		prob.x[i] = &x_space[j];
		fscanf(fp,"%lf",&label);
		prob.y[i] = label;

		while(1)
		{
			int c;
			do {
				c = getc(fp);
				if(c=='\n') goto out2;
			} while(isspace(c));// undefine ??
			ungetc(c,fp);
			if (fscanf(fp,"%d:%lf",&(x_space[j].index),&(x_space[j].value)) < 2)
			{
				printf("Wrong input format at line %d\n", i+1);
				 //exit(1);///////////////////close this too
			}
			++j;
		}	
out2:
		if(j>=1 && x_space[j-1].index > max_index)
			max_index = x_space[j-1].index;
		x_space[j++].index = -1;
	}

	if(param.gamma == 0)
		param.gamma = 1.0/max_index;

	if(param.kernel_type == PRECOMPUTED)
		for(i=0;i<prob.l;i++)
		{
			if (prob.x[i][0].index != 0)
			{
				printf("Wrong input format: first column must be 0:sample_serial_number\n");
				//exit(1);
			}
			if ((int)prob.x[i][0].value <= 0 || (int)prob.x[i][0].value > max_index)
			{
				printf("Wrong input format: sample_serial_number out of range\n");
				//exit(1);
			}
		}

	fclose(fp);



/////////////

fs=fopen(str1,"a+");

for(i=0;i<prob.l;i++)
	{
        fprintf(fs,"%d ", (int)prob.y[i]);

		featurevector=prob.x[i];

		j=0;

		while(featurevector[j].index!=-1)
		{
                if((featurevector[j].value>0.00) && (arr[featurevector[j].index]==1))

                   fprintf(fs,"%d:%f ",featurevector[j].index,featurevector[j].value);

				j++;

		}
        
		fprintf(fs,"\n");

	}

fclose(fs);

}
