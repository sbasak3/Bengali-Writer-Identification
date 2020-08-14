#include <malloc.h>
#include <stdio.h>
#include "script.h"
#include <math.h>

#include<string.H>
#include<stdlib.H>
#include "Tiff.h"


#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <time.h>
#include "svm.h"
#include "binarize.h"
#include "compo.h"
#include "script.h"
#include "image_info.h"
#include "line-word.h"

#define Malloc(type,n)(type *)malloc((n)*sizeof(type));

extern int resultclass[90];

int flag=0;
extern char str1[60];
extern char str4[250];
int component;
extern int classlable;
char *handdir;
	static int total_print,total_hand,total_wordcomp;
char *printdir;
Comp_list *global;
extern int word_number;
extern int mode;
extern int  charallograph;
float **covar;
float eigenvalue;
Comp_list* comAnalysis(int max, int col,short **binary,int min,int length,int width);

float *dimension64feature(short **arr,int row,int col);
float *dimension400feature(short **image,int row,int col);

int SVM_Gaussian_test(float *vector, int );


double  process_word(short ** duplicatebinary,int length, int width,int mode)
{
     
	
	Comp_list *head,*storehead,*storecomp;
	int numofcomponent=0,complength=0,compwidth=0,row ,result, index,numofpixel,i,j,step,rlength,rwidth,k,l,avgcomplength,avgcompwidth,crlength,crwidth,classifier_result=5;

	float **pixelvector;float *vector,*vector1,*featurevector,*princivector,*vectoropencv; double  univector;double vectormag,theta,angle,normunivector,normunivectorx=1,sumunivector;
    short **smoothcomponent,**rotimage,**rotcomp;
     
 
    FILE *fp,*ft,*fq,*fs,*ft1,*fq1,*fs1,*hmm;
	binarize    Obina;
	static int countofchar;
	
	 char temp[60];
	char *path;
    int *compindex=(int*)malloc(3*sizeof(int));
    
	        head=comAnalysis(length,width,duplicatebinary,0,length,width);
		
	//global=comAnalysis(length,width,binary,0,length,width);;
	storehead=head;

    complength=0;
    compwidth=0;

    ft=fopen("training_out400.txt","a+");
	fq=fopen("training_out64.txt","a+");
	fs=fopen("training_out464.txt","a+");
	ft1=fopen("training_out400(with 0).txt","a+");
	fq1=fopen("training_out64(with 0).txt","a+");
	fs1=fopen("training_out464(with 0).txt","a+");
	
   /* printf("  After  \n");
		for(i=0;i<21;i++)
			printf("%d ",resultclass[i]);*/
	if(component==0)
       	countofchar=0;

    	while(head)
		{   
			complength=complength+head->Comp_data.maxRow-head->Comp_data.minRow;

            compwidth= compwidth+head->Comp_data.maxCol - head->Comp_data.minCol;
		     
			
			//if((complength<length/10)&&(head->Comp_data.maxRow>=length/2))
			numofcomponent++;
		    head=head->next;
		}

		head=storehead;
		avgcomplength=complength/numofcomponent;
        avgcompwidth= compwidth/numofcomponent;
        //printf(" NO of component in the document is %d \n",numofcomponent);

        //fp=fopen("train-20writer.txt","a+");
        
		
		while(head)
		{
			complength=head->Comp_data.maxRow - head->Comp_data.minRow+3;
			compwidth=head->Comp_data.maxCol  - head->Comp_data.minCol+3;
           
			if((complength>=avgcomplength)&&(compwidth>=avgcompwidth)&&(complength>10)&&(compwidth>10))
			{
                
				component++;
				word_number++;
				countofchar++;
				charallograph=countofchar;
				vector=dimension400feature(head->Comp_data.Comp_array,complength,compwidth);

			//if(word_number<=150)
			//{
			    fprintf(ft,"%d ",classlable);
				fprintf(fs,"%d ",classlable);
				fprintf(ft1,"%d ",classlable);
				fprintf(fs1,"%d ",classlable);
				for (i=0;i<400;i++)
				{
					if (vector[i]!=0) //correct libsvm
					{
					   fprintf(ft,"%d:%f ", i+1,vector[i]); //400 feature
					   fprintf(fs,"%d:%f ", i+1,vector[i]); //464 feature
					}
					fprintf(ft1,"%d:%f ", i+1,vector[i]); //400 feature
					fprintf(fs1,"%d:%f ", i+1,vector[i]); //libsvm with 0
				}
				fprintf(ft,"\n");
				fprintf(ft1,"\n");
				//fprintf(fq,"\n");
                //path=(char*)malloc(100*sizeof(char));
				//strcpy(path,"C:\\UGTP-WRITER-CHAR\\");
				 
				vector1=dimension64feature(head->Comp_data.Comp_array,complength,compwidth);

				//Obina.write_back_file("segmentedchar.tiff", head->Comp_data.Comp_array, complength, compwidth);

		        	//printf(" Parikshit -- Check if the  segmented char is a char or not \n");

			    //getchar();

				fprintf(fq,"%d ",classlable);
				fprintf(fq1,"%d ",classlable);
				for (i=0;i<64;i++)
				{
					if(vector1[i]!=0)
					{
						fprintf(fq,"%d:%f ", i+1,vector1[i]); //64 feature
						fprintf(fs,"%d:%f ", i+401,vector1[i]); //464 feature
					}
						fprintf(fq1,"%d:%f ", i+1,vector1[i]); //64 feature
						fprintf(fs1,"%d:%f ", i+401,vector1[i]);//464 feature	
				}
				fprintf(fq,"\n");
				fprintf(fs,"\n");
				fprintf(fq1,"\n");
				fprintf(fs1,"\n");
			//}                          
          
			}
		   //getchar();
			head=head->next;
		   
		}
		// fclose(hmm);
       fclose(ft);
	   fclose(fq);
	   fclose(fs);
       fclose(ft1);
	   fclose(fq1);
	   fclose(fs1);
        

//		printf(" Total Words/Components processed %d  Class %d \n", total_wordcomp,index);
		return(0);
		
	

}

