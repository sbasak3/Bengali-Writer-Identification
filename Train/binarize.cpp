
#include "binarize.h"
#include<stdlib.h>
#include "Tiff.h"

/********************Binarization of the image***************************/
short** binarize::binarization(short **image, int length, int width)
{

	int i,j,thresh_value;

	short  **binary;



	binary=(short**)malloc(length*sizeof(short*));

	for(i=0;i<length;i++)

		binary[i]=(short*)malloc(width*sizeof(short));

	

	thresh_value=thres_val_finding(length,width, image) ;	



	for(i=0;i<length;i++)

	{

		for(j=0;j<width;j++)

		{

			if(image[i][j]>=0 && image[i][j]< thresh_value)

				binary[i][j]=1;

			else

				binary[i][j]=0;

		}		

	}        	

	Free(image, length);

	return binary;

 

}

/************converting back from binarization to real image***************************/

void binarize::write_back_file(char *out_name,short **b,int length, int width)

{

	int k,r;

	Tiff	Otiff;
	for(k=0;k<length;k++)

	   for(r=0;r<width;r++)

		   if(b[k][r]==0)

				b[k][r]=255;

		   else if (b[k][r]==1)

				b[k][r]=0; 

	

	Otiff.create_file_if_needed(out_name,b,length,width,8);



	for(k=0;k<length;k++)

	   for(r=0;r<width;r++)

	   {		   if(b[k][r]==0)

				b[k][r]=1;

		   	   else if (b[k][r]==255)

				b[k][r]=0; 

	    }


}

/*****************************Threshold value finding*******************************/

int binarize::thres_val_finding(int size_r, int size_c, short **z)

{
                                                                                                         
     int i,j;

     int thres_value, high_grey_val, max_back, max_obj, freq[256],  max_freq_back, max_freq_obj;

	 short k ;

	 //printf("\n Before z = %x\n",z);
	 
	 for (i = 0; i < 256; i++)
      freq[i] = 0;

	for (i = 0; i < size_r; i++) {

		for (j = 0; j < size_c; j++) {

			k =  z[i][j];

			if(k>=0 && k<256)
				freq[k]++;

		}

	}

	for (i = 256 - 1; i >= 0; i--)

		if (freq[i] != 0) {

			high_grey_val = i;

			//printf("high_grey_val is %d\n", high_grey_val);

			break;

		}

	max_freq_back = 0;

	for (i = high_grey_val / 2; i <= high_grey_val; i++)

		if (freq[i] > max_freq_back) {

			max_freq_back = freq[i];

			max_back = i;

		}

	//printf("max_back=%d\n", max_back);

	max_freq_obj = 0;

	for (j = 0; j < high_grey_val / 2; j++) {

		if (freq[j] >= max_freq_obj) {

			max_freq_obj = freq[j];

			max_obj = j;

		}

	}

	//printf("max_obj=%d\n", max_obj);

	thres_value = (max_obj + max_back) / 2;

    //printf("\n After z = %x\n",z);    
	return (thres_value);

}


void binarize::Free(short **head,int length)

{

     int i; 

     for(i=0;i<length;i++)

            free(head[i]); 

     free(head);  

}
