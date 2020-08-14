//extern short **ndup;
//extern int hgt_of_busyzone;
extern int startrowwrd,endrowwrd;
extern int mode;
extern int nll,nlu;
int modevalue(short **binary,int length,int width);
//extern int newstartrow,newendrow;
/*********** this module detects the buzyzone row no. ************/
#include <stdio.h>
#include <malloc.h>
int findbuzyzone_l_startrow_endrow(short ** chararray,int length,int width)
{
	short **chardup,**upper;
	char name[50];
	int i,j,r,flag,dupmode,maxcount=0,k,maxcountindex=0,firstindex=0;
	int *row,*col,count,*check_array;
/****************** Allocating Memory *************************/
    static int word=0;
	row=(int *)calloc(length,sizeof(int));
	col=(int *)calloc(width,sizeof(int));

	chardup = (short **)malloc(length * sizeof(short *));
	for(i=0;i<length;i++)
		chardup[i]=(short *)malloc(2 * width * sizeof(short));
	
    word++;
/****************** Copy The Orginal Array Into a Duplicate Array **************/
	
	for(i=0;i<length;i++)
	{
		for(j=0;j<(width*2);j++)
		{
			if(j<width)
			chardup[i][j]=chararray[i][j];
			else
				chardup[i][j]=0;
		}
	}

 mode=modevalue(chararray,length,width);
/***************** Threshold Value is 70 % of mode For Finding The Crossing *************/
	
//	printf("stroke_width=%d\n",mode);
//	dupmode=(int)mode;
	//printf("\nPREVIOUS MODE=%d\n",dupmode);
 	//mode=(int)(70*mode)/100;

/********************* Finding The Bondary Of The Character Row Wise ****************/

	for(i=0;i<length;i++)
	{
		count=0;
		for(j=0;j<width;j++)
		{  
			if((chararray[i][j]==0)&&(chararray[i][j-1]==1))
			{    count=0;
			    firstindex=j;
				for(k=j;k<width;k++)
				{
					if(chararray[i][k]==0)
						count++;
					else 
						break;
				}
			
		      if(count<(mode*6))
				{  
				//printf( " First index = %d", firstindex);
				//printf("  Count = %d \n",count);
			  	for(j=firstindex;j<(k);j++)
					chardup[i][j]=1;
				}
			   else
				   count=0;
			}


		
		}
	}

/************** horizontal projection profile ******************/
  firstindex=0;

	for(i=0;i<length;i++)
	{
		
		k=width+1;row[i]=0;
		for(j=0;j<width;j++)
		{
			if(chardup[i][j]==1)
			{
				chardup[i][k]=1;
				k++;
				row[i]=row[i]+1;
			}
			
		}
	}
    



/******************* Finding The max number of 1 its occurance  row ****************/

  maxcount=0;
for(i=0;i<length;i++)
{
    if(maxcount<row[i])
	{
		maxcount=row[i];
		firstindex=i;
	}
	if(maxcount<=row[i])
	{
	   //	printf(" The number of crossing %d \n",row[i]);
		maxcount=row[i];
        maxcountindex=i;
	}
}
  //  printf(" The first crossing is %d \n",firstindex);
//	printf(" the max run is  in row no %d \n",firstindex);

/*********************** Free The Memory ***************************************/
    //firstindex=(firstindex) /2;
	firstindex=width+maxcount/2 ;
    startrowwrd=0; endrowwrd=length-1; 
	for(i=maxcountindex;i<length;i++) 
	{
		if(chardup[i][firstindex]==1)
			endrowwrd=i;
		else
			break;
	}


	for(i=maxcountindex;i>0;i--)
	{
		if(chardup[i][firstindex]==1)
			startrowwrd=i;
		else
			break;
	}
    //printf(" the end row is %d \n",endrow);
    //printf(" The startrow =%d \n", startrow);

  /*     for(k=0;k<length;k++)
          { 
           for(r=0;r<(width *2);r++)
             {  
               if(chardup[k][r]==0)
                  chardup[k][r] =255;
               else
                {  
                 if(chardup[k][r]==1)
                   chardup[k][r] = 0 ;
                 /*else
                   out_image[k][r] = 128 ;      
                }   
		   }
          }
         // printf("%f \n",vvector2);        
        //  fprintf(fp2,"\n");
	     //   sprintf(name,"%d",word);
		//	strcat(name,"projection");
		 //   strcat(name,".tif");
         // create_file_if_needed(name,chardup,length,width*2);
   //printf(" The endrow =%d \n", endrow);
	free(row);
   /*	for(i=0;i<length;i++)
		free(chardup[i]);
	free(chardup);*/
  //hgt_of_busyzone = endrow-startrow;
	
 /* upper=(short**)malloc(length*(sizeof(short*)));
  for(i=0;i<length;i++)
	  upper[i]=(short*)malloc(width*sizeof(short));

  for(i=0;i<length;i++)
  for(j=0;j<width;j++)
	  upper[i][j]=chararray[i][j];*/
    maxcountindex= (endrowwrd - startrowwrd)/2 + startrowwrd;
  /// startrow= startrow+maxcountindex;
   //maxcountindex=startrow+maxcountindex;////// added this line lasttime


  /*for(i=startrow;i<=endrow;i++)
  for(j=0;j<width;j++)
	  upper[i][j]=0;*/
  
	  /*for(i=0;i<length;i++)
		  free(chardup[i]);
	  free(chardup);*/

	  free(row);
	  free(col);

      

	return(maxcountindex);


}


