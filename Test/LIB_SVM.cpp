#include <stdio.h>
#include <ctype.h>
#include "svm.h"
#include <string>
using namespace std;
#include <iostream>


extern struct svm_parameter param;		// set by parse_command_line
extern struct svm_problem prob;		// set by read_problem
extern struct svm_model *model64,*model400;
extern struct svm_node *x_space64,*x_space400,*x_space;

#define Malloc(type,n)(type *)malloc((n)*sizeof(type));
double *prob_estimates;

int SVM_Gaussian_test(float *vector,int charnumber);
struct svm_model *svm_load_model(const char *model_file_name);

double firstprobabilty(double  * );

extern int resultclass[400];


int SVM_Gaussian_test(float *vector,int charnumber)
{  
	float gamma,endgamma,increment,correct=0;int *indexerror;
	int roman=0,japanese=0,chinese=0,korean=0,line=0,flag=0,i,j,newflag=0;
	 double error=0,target;
	 char testfile[100];int charcount[5];int total;int max_index,index,elements;
	 int firstcolumn=0;
	 
	 int colon;FILE *chk,*fp,*read64,*write64,*lp;
	 double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0,highestprobability;
   
	int probability;double label,v;int nr_class;
	//model400=svm_load_model("300writer-gradient-model");
	nr_class=svm_get_nr_class(model400);
	//printf(" Number of class %d \n",nr_class);
	//nr_class=nr_class+1;
	prob_estimates=(double*)malloc(nr_class*sizeof(double));
    //printf(" sukalpa  %d  ",charnumber);
	fp=fopen("featurevector.txt","w");

	                                  fprintf (fp,"500 ");         
								      for(i=0;i<464;i++)
									  {
	        									if(vector[i]==0)
												{
	                                             	newflag++;
		
                                                	continue;
												}
	                                        else
											{
                                                
												 fprintf (fp,"%d:%f ",i+1, *(vector+i));
												/*x_space64[i+1].index=i+1;
												x_space64[i+1].value=*(vector+i);
												printf ("%d :%f ",x_space64[i+1].index,x_space64[i+1].value);*/
                                                newflag++;
											}
									  } fprintf (fp,"\n" );
									   fclose(fp);

/////pls specify the name of the test file here//////*/
    


	//printf(" sukalpa \n %d 2 ",charnumber);

	fp=fopen("featurevector.txt","r");
	

   if(fp)
   {
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
	
    
	prob.y = Malloc(double,prob.l);
	prob.x = Malloc(struct svm_node *,prob.l);
	x_space = Malloc(struct svm_node,elements);
    indexerror=(int*)malloc((prob.l+1)*sizeof(int));
     max_index = 0;
	j=0;
	//printf(" Number of characters in the word %d \n",prob.l);
	//printf(" READING STARTED \n");
	//getchar();
	for(i=0;i<prob.l;i++)
	{
		double label;
		prob.x[i] = &x_space[j];
		//printf(" J is %d \n",j);
		fscanf(fp,"%lf",&label);
		prob.y[i] = label;

		while(1)
		{
			int c;
			do {
				c = getc(fp);
				if(c=='\n') goto out2;
			} while(isspace(c));
			ungetc(c,fp);
			if (fscanf(fp,"%d:%lf",&(x_space[j].index),&(x_space[j].value)) < 2)
			{
				fprintf(stderr,"Wrong input format at line %d\n", i+1);
				exit(1);
			}
			++j;
		}	
out2:
		if(j>=1 && x_space[j-1].index > max_index)
			max_index = x_space[j-1].index;
		x_space[j++].index = -1;
	}
   //printf(" READING ENDED \n");
   
   
  //getchar();
   
   fclose(fp);
////////////// Now testing each sample from test file, here each sample is a character or parts of character/////////////////////////
//  printf(" TESTING STARTED \n");
    for(i=0;i<prob.l;i++)
	{             indexerror[i+1]=0;
	              highestprobability=0.00;
                 
				  v = svm_predict_probability(model400,prob.x[i],prob_estimates);
				//  v = svm_predict(model400,prob.x[i]);
                  resultclass[(int)v]++;

				// printf("\n Class  %f \n ",v); 
			//	 getchar();
                 
			

	}
	
//	read64=fopen("20-400test-rdb.txt","r");
//	printf(" TESTING ENDS......\n");
//	lp=read64;
	//max_index=charcount[0];
    index=0;
	max_index=0.00;
	
///////////////////////////////////// end of testing //////////////////////////////
   



}
return 0;
}


double firstprobabilty(double  *array)
{
	int i;
	double index=0;
	for(i=0;i<5;i++)
	{
		if(index<array[i])
			index=array[i];
	}
    //printf(" Hiighest prob is %f \n",index);
	return(index);
}

