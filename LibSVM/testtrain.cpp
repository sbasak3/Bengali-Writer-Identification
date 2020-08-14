#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
#include <ctype.h>
#include "svm.h"
#define Malloc(type,n)(type *)malloc((n)*sizeof(type))
FILE *input, *output;
int  SVM_Gaussian();
extern int predict_probability;
double firstprobabilty(double  * array);
void exit_with_help()
{
	printf(
	"Usage: svm-train [options] training_set_file [model_file]\n"
	"options:\n"
	"-s svm_type : set type of SVM (default 0)\n"
	"	0 -- C-SVC\n"
	"	1 -- nu-SVC\n"
	"	2 -- one-class SVM\n"
	"	3 -- epsilon-SVR\n"
	"	4 -- nu-SVR\n"
	"-t kernel_type : set type of kernel function (default 2)\n"
	"	0 -- linear: u'*v\n"
	"	1 -- polynomial: (gamma*u'*v + coef0)^degree\n"
	"	2 -- radial basis function: exp(-gamma*|u-v|^2)\n"
	"	3 -- sigmoid: tanh(gamma*u'*v + coef0)\n"
	"	4 -- precomputed kernel (kernel values in training_set_file)\n"
	"-d degree : set degree in kernel function (default 3)\n"
	"-g gamma : set gamma in kernel function (default 1/k)\n"
	"-r coef0 : set coef0 in kernel function (default 0)\n"
	"-c cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)\n"
	"-n nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)\n"
	"-p epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)\n"
	"-m cachesize : set cache memory size in MB (default 100)\n"
	"-e epsilon : set tolerance of termination criterion (default 0.001)\n"
	"-h shrinking: whether to use the shrinking heuristics, 0 or 1 (default 1)\n"
	"-b probability_estimates: whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)\n"
	"-wi weight: set the parameter C of class i to weight*C, for C-SVC (default 1)\n"
	"-v n: n-fold cross validation mode\n"
	);
	exit(1);
}

void set_parameter_svm_poly(char *input_file_name, char *model_file_name,float degree,int probability);
void set_parameter_svm_gaussian( char *input_file_name, char *model_file_name,float gamma,int probability);
void set_parameter_svm_linear( char *input_file_name, char *model_file_name);
void set_parameter_svm( char *input_file_name, char *model_file_name);
void read_problem(const char *filename);
void do_cross_validation();

struct svm_parameter param;		// set by parse_command_line
struct svm_problem prob;		// set by read_problem
struct svm_model *model;
struct svm_node *x_space;
double *prob_estimates;
int cross_validation;
int nr_fold=5;
float gamma=90.00,degree,coef;
    char input_file_name[1024];
    char output_file_name[1024];
	char model_file_name[1024];
	const char *error_msg;
    int numberofclass,i,j;int c;
    float val,v;char l;
	char test_file_name[100];
	int penalty_mult=1;


int main()
{

    //	cout<<" Sukalpa LIBSVM\n";
    printf(" Enter The  Training file name \n");
 	 scanf("%s",input_file_name);

 	 /*printf(" Enter The  Testing file name \n");
 	 	 scanf("%s",input_file_name);*/

	printf(" Enter The model file name \n");
    scanf("%s",model_file_name);
   // fflush(stdin);

	//for(gamma=0.0005;gamma<=80;gamma=gamma+1.50)
		//{
		 set_parameter_svm_gaussian(input_file_name, model_file_name,gamma,0);

		read_problem(input_file_name);
		//do_cross_validation();
		//write_problem();
	/*	time_t t1,t2;*/
	     model = svm_train(&prob,&param);
	     svm_save_model(model_file_name,model);
	    //SVM_Gaussian();
	//	(void) time(&t1);*/
		//

		//}



	//svm_destroy_param(&param);
	///free(prob.y);
	//free(prob.x);
	//free(x_space);

	return 0;
}


void do_cross_validation()
{
	int i;
	int total_correct = 0;
	double total_error = 0;
	double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
	double *target = Malloc(double,prob.l);

	svm_cross_validation(&prob,&param,nr_fold,target);
	if(param.svm_type == EPSILON_SVR ||
	   param.svm_type == NU_SVR)
	{
		for(i=0;i<prob.l;i++)
		{
			double y = prob.y[i];
			double v = target[i];
			total_error += (v-y)*(v-y);
			sumv += v;
			sumy += y;
			sumvv += v*v;
			sumyy += y*y;
			sumvy += v*y;
		}
		printf("Cross Validation Mean squared error = %g\n",total_error/prob.l);
		printf("Cross Validation Squared correlation coefficient = %g\n",
			((prob.l*sumvy-sumv*sumy)*(prob.l*sumvy-sumv*sumy))/
			((prob.l*sumvv-sumv*sumv)*(prob.l*sumyy-sumy*sumy))
			);
	}
	else
	{
		for(i=0;i<prob.l;i++)
			if(target[i] == prob.y[i])
				++total_correct;
		printf("Cross Validation Accuracy = %g%%\n",100.0*total_correct/prob.l);
	}
	free(target);
}




// read in a problem (in svmlight format)

void read_problem(const char *filename)
{
	int elements, max_index, i, j;
	FILE *fp = fopen(filename,"r");
	
	if(fp == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",filename);
		exit(1);
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

	prob.y = Malloc(double,prob.l);
	prob.x = Malloc(struct svm_node *,prob.l);
	x_space = Malloc(struct svm_node,elements);

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

	if(param.gamma == 0)
		param.gamma = 1.0/max_index;

	if(param.kernel_type == PRECOMPUTED)
		for(i=0;i<prob.l;i++)
		{
			if (prob.x[i][0].index != 0)
			{
				fprintf(stderr,"Wrong input format: first column must be 0:sample_serial_number\n");
				exit(1);
			}
			if ((int)prob.x[i][0].value <= 0 || (int)prob.x[i][0].value > max_index)
			{
				fprintf(stderr,"Wrong input format: sample_serial_number out of range\n");
				exit(1);
			}
		}

	fclose(fp);
	cout<<"End of Reading\n";
}

void set_parameter_svm_gaussian( char *input_file_name, char *model_file_name,float gamma,int probability)
{
	int i;

	// default values
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 0;
	param.gamma = gamma;	// 1/k
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = penalty_mult;//////////////   this is the value of penanlty multiplier////////////////
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = probability;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	cross_validation = 0;


}

int SVM_Gaussian()
{  
	float gamma,endgamma,increment,correct=0;int *indexerror;
	int print=0,hand=0,line=0,flag=0,text=0,picture=0,correctnonskin=0,correctskin=0,nonskin=0,skin=0;
	 double error=0,target;
	 char testfile[100];int total;int max_index,elements;
	 int firstcolumn=0;
	 int colon;FILE *chk,*fp,*read64,*write64,*lp;
	 double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0,highestprobability;
   
	int probability;double label;int nr_class;
	
	//write64=fopen("R-64-50.400.txt","a+");
  
	model=svm_load_model("model");
	nr_class=svm_get_nr_class(model);
	printf(" Number of class %d \n",nr_class);
//	getchar();
	prob_estimates=(double*)malloc(nr_class*sizeof(double));

	fp=fopen(output_file_name,"r");

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
	printf(" READING STARTED \n");
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
			if (fscanf(fp,"%d:%lf",&(x_space[j].index),&(x_space[j].value))< 2)
			{
				fprintf(stderr,"Wrong input format at line %d\n", i+1);
				getch();
				exit(1);///////////////// close  this one and do necessary test
			}
			++j;
		}	
out2:
		if(j>=1 && x_space[j-1].index > max_index)
			max_index = x_space[j-1].index;
		x_space[j++].index = -1;
	}

  printf(" READING ENDED \n");
  chk=fopen("Result-for-Original and Forged Signature.txt","a+");
////////////// Now testing each sample from test file/////////////////////////
  printf(" TESTING STARTED \n");
    for(i=0;i<prob.l;i++)
	{             indexerror[i+1]=0;
	              highestprobability=0.00;
                 v = svm_predict(model,prob.x[i]);
                 
                 if(prob.y[i]==(float)v)
					 correct++;
				 if(prob.y[i]==1)
					 skin++;
				  if(prob.y[i]==0)
					 nonskin++;
				  if((prob.y[i]==1)&&(prob.y[i]==(float)v))
					  correctskin++;
			   if((prob.y[i]==0)&&(prob.y[i]==(float)v))
			          correctnonskin++;
	}
	fclose(fp);/////////////////////////////////////////////////

	fprintf(chk,"  Total Correct is %f amongst %d samples -> Accuracy is %d  with gamma %f ",correct,prob.l,(correct)/prob.l*100,gamma);
	//fprintf(chk,"  Total positive is %d Correct Skin is %d and total nonskin is %d correct nonSkin is %d \n ",skin,correctskin,nonskin, correctnonskin);

	fclose(chk);


    
return 1;

}



