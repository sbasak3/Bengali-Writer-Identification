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
//#include "compo.h"
#include "script.h"
#include "image_info.h"
//#include "line-word.h"

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

	float **pixelvector;float *vector1,*vector2,*featurevector,*princivector,*vectoropencv; double  univector;double vectormag,theta,angle,normunivector,normunivectorx=1,sumunivector;
    short **smoothcomponent,**rotimage,**rotcomp;
	float vector[464];
     
 
    FILE *fp;
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
				countofchar++;
				charallograph=countofchar;
				vector1=dimension400feature(head->Comp_data.Comp_array,complength,compwidth);
              //  path=(char*)malloc(100*sizeof(char));
				//strcpy(path,"C:\\UGTP-WRITER-CHAR\\Test\\");
				 
			         vector2=dimension64feature(head->Comp_data.Comp_array,complength,compwidth);
					//sprintf(temp,"%d\\%s_%d.tif",classlable,str4,countofchar);
					//strcat(path,temp);
				//Obina.write_back_file(path, head->Comp_data.Comp_array,complength,compwidth);
				//free(path);
			for(i=0;i<400;i++)
			vector[i]=vector1[i];

				for(i=0;i<64;i++)
					vector[i+400]=vector2[i];
					
		    	  result= SVM_Gaussian_test(vector,component);
			                                    
          
			}
		   //getchar();
			head=head->next;
		   
		}

      // fclose(fp);
       
        

		//printf(" Total Words/Components processed %d  Class %d \n", total_wordcomp,index);
		return(0);
		
	
   
}

