////////////////////////////////////////////////////////////////////////
//
// hello-world.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file, inverts it, and displays the result. 
//
////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <malloc.h>
#include <sys/types.h>
#include "dirent.h"
int classlable;
char str1[60];
char str4[250];
int original_imageheight,original_imagewidth;
void oldmain(short**,int ,int);
int word_number=0;

int main()
{
  IplImage* img = 0; 
  int height,width,step,channels;
  uchar *data;
  int i,j,k;
  short** gray;
  short**bin;
   DIR *dirp;
   struct dirent *dp;
   char *temp;


  char singlechar[1];
  char path[250];
  char *dirname="E:\\ImageInput\\";    ///// Set directory as per your PC
  dirp = opendir("E:\\ImageInput\\");  ///// Set directory as per your PC
  

    while ((dp=readdir(dirp)) != NULL)
    {    word_number=0;
			//printf(" Enter the image  name\n");
            //scanf("%s",&path[250]);
		  if(strcmp(dp->d_name,"Thumbs.db")!=0)
		  if(strlen(dp->d_name)>2)
		  {
		    /*strcpy(path,dirname);
			strcat(path,dp->d_name);
			printf("\n%s\n",path);
            img=cvLoadImage(path);
		    strcpy(str1,dp->d_name);
			strcpy(str4,str1);*/
			strcpy(str4,"\0");
		    strcpy(path,dirname);
			strcat(path,dp->d_name);
			printf("\n%s\n",path);
			printf("Enter the class lable:");
			scanf("%d",&classlable);
            img=cvLoadImage(path);
		    strcpy(str1,dp->d_name);
             temp=str1;
			 	printf("before processing %d \n",word_number);
			// printf(" Enter The Class Lable of the revese_training image \n");
			// scanf("%d",&classlable);
			 while(temp)
			 {
                 if(*temp=='.')
					 break;
				 else
				 {  
					  sprintf(singlechar,"%c",*temp);

			          strcat(str4,singlechar);
				 }
				 temp++;
			 }
		
			//printf(" Enter The Class Lable \n");
			//scanf("%d",&classlable);

			///////////////////////////////////////
			if(!img){
                printf("Could not load image file: \n");
                exit(0);
			}

  // get the image data
             original_imageheight = height  = img->height;
             original_imagewidth = width  = img->width;
             step      = img->widthStep;
             channels  = img->nChannels;
             data      = (uchar *)img->imageData;
             gray=(short**)malloc(height*sizeof(short*));
             bin= (short**)malloc(height*sizeof(short*));

             for(i=0;i<height;i++)
			 {
	             gray[i]=(short*)malloc(width*sizeof(short));
	             bin[i]=(short*)malloc(width*sizeof(short));
			 }

             printf("Processing a %dx%d image with %d channels\n",height,width,channels); 
			// getchar();
			 
              for(i=0;i<height;i++) 
	              for(j=0;j<width;j++) 
			             for(k=0;k<channels;k++)
                              gray[i][j]=data[i*step+j*channels+k];
		        oldmain(gray,height,width);
				printf ("word present  %d \n", word_number);
				getchar();
				//cvReleaseImage(&img );
				remove(path);
		  }
	   }
	
  

  // show the image

 // cvNamedWindow("mainWinOut", CV_WINDOW_AUTOSIZE); 
 // cvMoveWindow("mainWinOut", 100, 100);
 // cvShowImage("mainWinOut", img );

  // wait for a key
 // cvWaitKey(0);

  // release the image
  
    exit(0);
  return 0;
}