#include <stdio.h>
#include <malloc.h>
extern char str4[50];
int *globalcsegindex;
#include "line_word.h"

//int newstartrow,newendrow,stwidth;
void insert(character **,short **,int,int,int,int,int,int);//pototype declaration

void wordsegment(int *d,int index,int k, short ** bin,short **tempbin,int width,int startrow,int endrow,character **node)
{
	// d array contains line indexes | index is no. of index |  
	
	int i,j,count,max=0,p,sum,e,stwidth,lastj, prevj,k1,length,wordloop,ni,nw,nl,nj,orrj,orri;
	int *stok,newstrow=0;
	int *f;
	short **word;
	short **original,**chararray;
	int strow,edrow;
	int lind=0,wind=0,temp=0,sf,ef;
	
	stok=(int*)malloc(width * sizeof(int));
	for(j=0;j<width;j++)
		stok[j]=0; 
	
	length=d[index+1] - d[index];	// length = lowerline-upperline
	
	globalcsegindex=(int*)malloc(width * sizeof(int));

	for(i=0;i<width;i++){
		globalcsegindex[i]=0;
	}

    f=(int*)malloc(20 * sizeof(int));  //old value length

	word=(short**)malloc(length*sizeof(short*));
    for(i=0;i<length;i++)
		word[i]=(short*)malloc((width)*sizeof(short));

	original=(short**)malloc(length*sizeof(short*));
    for(i=0;i<length;i++)
		original[i]=(short*)malloc((width)*sizeof(short));

	//printf("\nSTART OF WORD SEGMENTATION \n");
			
	/**************************computation of stroke width  *****************/
	//////////////// horizontal scanning ///////////////////

	//printf("\n At the starting of wordsegment startrow=%d,endrow=%d",startrow,endrow);

	for(i=d[index];i<d[index+1];i++){////////// replace all z with startrow
		for(j=0;j<width;j++){
			count=0;
			if(bin[i][j]==1){
				count++; //count=0;
				while(bin[i][j]==1){
					count++;
					j++;		     
				}
				stok[count]=stok[count]+1;
			}

		}
	}

	///////////////////////////  vertical scanning ////////////////////          
    for(j=0;j<width;j++){
		for(i=d[index];i<d[index+1];i++){////////// replace all z with startrow
			count=0;
			if(bin[i][j]==1){
				count++; //count=0;

				while(bin[i][j]==1 && i<d[index+1]-1){
					count++;
					i++;
				}
				stok[count]=stok[count]+1;
			}

		}
	}

	for(j=0;j<width;j++){
		if(stok[j]>max){
			stwidth=j;
			max=stok[j];// max is the stroke width
		}
	} 
    
//*******************the word array is filled by zero  *****************/

	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			word[i][j]=0;
		}
	}

/******* count the run of one vertically and if it is greater than stroke width then *****/
/*******         it is represented by one otherwise  zero     *****************************/
	
	for(j=0;j<width;j++){
		count=0;
		for(i=d[index];i<d[index+1];i++){////////// replace all z with startrow
			if(bin[i][j]==1){
				count=count+1;					
			}		
		}
		if(count>=stwidth*0.2)
							word[4][j]=1;
	}

/***************** smoothing of small black run into white run (word segment) **************/

	for(j=0;j<width;j++){
		prevj=0;
		if(word[4][j]==1){
			prevj=lastj = j;
			count=0;
			while(word[4][j]==1){
				count=count+1;
				j++;
				lastj=j;
			}
			if(count<=stwidth*0.5){
				for(k1=prevj;k1<=lastj;k1++)
					word[4][k1]=0;
			}
		}
			
	}
										
/*****************   smoothing of small white into black (word segment)**************/										
	
	for(j=0;j<width;j++){
		prevj=0;
		if(word[4][j]==0){
			prevj=lastj = j;
			count=0;
		    while(word[4][j]==0){
				count=count+1;
				j++;
				lastj=j;
			}
			if(count<=stwidth*0.5){
				for(k1=prevj;k1<=lastj;k1++)
					word[4][k1]=1;
			}
		}
			
	}
												
/************ segmented  words are in the middle of the white space ***********/
/************ and computation of co-ordinates of segmented lines **************/
       
	//i=d[index];
	j=0;
	while(word[4][j]!=1)
		j++;
	//printf("the first column the lines is %d\n",j);
	//for(i=d[index]+startrow;i<d[index+1];i++)
	//-----------------------------------

	for(i=d[index];i<d[index+1];i++)//new
		tempbin[i][j-1]=1;				//**first line printing for word separation**

	//----------------------------------
	if(j-1>0){
		if(j-stwidth>0 )
			f[0]=j-stwidth;
		else
			f[0]=j-1;
	}
	else
	{ f[0]=0;}
	//---------------------------------
	
    p=1;// p is a counter counting the words

	
	for(j=0;j<width;j++){   
		prevj=0;
		count=0;
		
	    if(word[4][j]==1 && word[4][j+1]==0){
			count=0;
			prevj=j+1;
			lastj=0;
			while(word[4][j+1]!=1 && j<width){//**
			//while(word[4][j+1]!=1){
				lastj=j+1;
			    j++;
				count++;
			}
			strow=d[index];
			edrow=d[index+1];
			//printf("\n strow=%d,edrow=%d,prevj=%d,lastj=%d ",strow,edrow,prevj,lastj);

			//insert(node,bin,strow,edrow,lastj,prevj,lind,wind);

			/***********Thershold value for word segmentation **************/

			if(count>=1.5*stwidth||j==width){//*0.75)
				prevj=prevj+(lastj-prevj)*0.5; 
				//printf("the value of middlej is %d \n",prevj);
				f[p]=prevj;
				p++;


				//-------------------------------------------------------
				// it draws the lines betw.words //tempbin instead of bin
				//-------------------------------------------------------
				for(i=d[index];i<d[index+1];i+=2){
					if (prevj>=width) prevj=width-2;
					tempbin[i][prevj]=1;   
				}//end for
				//-------------------------------------------------------
			
			
			
			}//end count if
		}//end word if
		else{
		
			if(j==(width-1))
				f[p++]=width;		
		}
	}//end j for
	//*****************************************************************************
	

	sf=f[0];
	int SR=0,ER=0,SC=0,EC=0,s=0,t=0;
	int flag=0;

	for(i=1;i<p;i++)
	{
			ef=f[i];
			
			//---bounding box finding------------------
			
			//---------- start row---------------------
			for(s=d[index];s<d[index+1]&& flag==0;s++){
				for(t=sf;t<ef;t++){
					if(bin[s][t]==1)
					{
						SR=s;
						flag=1;
					}
				}
			}
			flag=0;
			//---------- end row---------------------
			for(s=d[index+1];s>SR && flag==0;s--){
				for(t=sf;t<ef;t++){
					if(bin[s][t]==1)
					{
						ER=s;
						flag=1;
					}
				}
			}
			
			flag=0;
			//---------- start column---------------------
			for(t=sf;t<ef&& flag==0;t++){
				for(s=d[index];s<d[index+1];s++){
					if(bin[s][t]==1)
					{
						SC=t;
						flag=1;
					}
				}
			}

			flag=0;
			//---------- end column---------------------
			for(t=ef;t>SC && flag==0;t--){
				for(s=d[index];s<d[index+1];s++){
					if(bin[s][t]==1)
					{
						EC=t;
						flag=1;
					}
				}
			}


			
			
 //-----------***************** INSERT FUNCTION CALLS*****************-------------
			if(ER-SR>=stwidth && EC-SC>=stwidth)
				insert(node,bin,d[index],d[index+1],SC-2,EC+2,0,0);
				//insert(node,bin,SR,ER,SC,EC,0,0);
			sf=ef;
//-----------*****************END OF  INSERT FUNCTION CALLS************-------------
		
	}//end of for i

	//*************************character segmentation calling***********************
	
	//charactersegment1(d,index,f,p,width,bin,tempbin,stwidth,length,k);
	
	//recog_ekar(d,index,f,p,width,bin,tempbin,stwidth,length,startrow,endrow,k);
	
	//recover_Gha(d,index,width,bin,tempbin,stwidth,length,startrow,endrow);
	//-----------inserting isolated character into linklist-----------------
	//newstrow=startrow-0.5*stwidth;
	//insert_globalchar(d,index,f,p,width,bin,stwidth,newstrow,endrow);
	//----------------------------------------------------------------------
	//display_charsegment(d,index,width,tempbin);


}//end of word segment function

