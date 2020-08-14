
#include<time.h> 
#include<stdio.h>
#include<math.h>
#include<malloc.H>
#include<string.H>
#include<stdlib.H>
#include "line_word.h"
#include "Tiff.h"
//#include "thin.h"
#include "binarize.h"
//#include "compo.h"
#include "script.h"
#include "image_info.h"
//#include "water_reservoir.h"

//short Min(int i,int j,short **image,int filter,int length,int width);
//double Find_Skew_Angle(short **image,int length,int width);
//short **Skew_Correct(short **bin_image, int rows, int cols, float theta,char filename[50] );

int modevalue(short ** ,int,int);
int SuTrainSVM(short**,int,int ,char );
short** reverse_image(short **a,int length,int width);
void linesegment(short **,short **,int,int,character **);
void display(character *);
void print_single_char(character *);
short **ndup;
int mode;
float vvector1;

char strout[60]="E:\\Test\\ImageInput\\";
//---------------
character *node;
//---------------
void oldmain(short **gray,int length ,int width)
	//void main( int argc, char *argv[] )
{

	char		str1[250];
	short		**bin,**binary,**tempbin;
	//	short   	**skel_full,**skel_up,**skel_down,**back_up,**back_up_new,**back_down,**back_down_new;
	int			i,j,cnt=0;
	int			note=0;
	char group = 'g';
	//static int  count,a[100],b[100],x,y,z,r,s; fscanffopen
	//int         clock[8][2]={{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1}};

	FILE	*fp;
	Tiff	Mtiff;
	binarize    Obina;

	/*	printf("\n Enter the  filename (with out extension)\n\n\t\t");
	scanf("%s",&str4);
	strcpy(str1,str4);
	strcat(str1,".tif");
	//	printf(" The Input File is  %s \n",str1); 
	if((fp=fopen(str1,"r")) == NULL)
	{

	printf("\n**********Can't open file--%s:*********",str1);
	exit(0);
	}
	fclose(fp);*/

	//gray = Mtiff.read_tiff_image(str1,&length,&width);
	bin = Obina.binarization( gray, length, width);
	//bin=smoothing(tbin,length,width);

	//	C_flow	R;
	//	water_reservoir	WR;


	binary=(short**)calloc((length+2),sizeof(short*));

	for(i=0;i<(length+2);i++)
		binary[i]=(short*)calloc((width+2),sizeof(short));		  				  

	for(i=0;i<length;i++) 
	{
		for(j=0;j<width;j++) 
		{
			if(bin[i][j]==1)
				binary[i+1][j+1]=1;

		}

	}
	//-------------Creating tempbin array----------
	tempbin=(short**)malloc(length*sizeof(short*));
	for(i=0;i<length;i++){
		tempbin[i]=(short*)malloc(width*sizeof(short));
	}

	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			tempbin[i][j]=bin[i][j];
		}
	}


	//**********------------ LINE SEGMENT CALL --------*****************

	node=NULL;
	linesegment(bin,tempbin,length,width,&node);
	//printf("\naaa  ");

	print_single_char(node);


	//**********-----END OF LINE SEGMENT CALL -----*********************

	//display(node->next);
	//--------------------------------------------
	// modevalue(binary,length,width);
	//printf("  Now  mode value %d\n",mode);
	// SuTrainSVM(binary,length,width,group);
	// Line(binary,length,width,group);
	//strcpy(str1,str4);
	//strcat(str4,"out.tif");
	//printf(" \n The Output File is  %s \n",str4);
	//getchar();
	//Obina.write_back_file(str4,tempbin, length,  width);


}/*End of main()*///////////////////////////////////////////////////////////////////////////



