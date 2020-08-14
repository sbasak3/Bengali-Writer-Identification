//extern char str4[50];

#include<malloc.h>
#include"line_word.h"
void wordsegment(int *,int,int, short**,short**,int,int,int,character **);
int startrow,endrow,stwidth;

void linesegment(short **bin,short **tempbin,int length,int width,character **node)
{ 
	int i,j,jj,k,k1=0,m,n,t,p,index,s,max,maxlen,avg,prevj=0,previ=0,lasti=0,iif=0,iil=0,nl,nw,ni,nj;
    int count,count1,count3,stwidth=0,sum,lastj,firstline,orri,orrj,headlineindex=0;
	int cstart=0,cend=0;
	char str1[50]="",str2[50]="";
	int loweri=0,upperi=0,ttt;
	int *b,*c,*d,*e,*stok;
    short **duphbin,**smoothbin;
	short **chararray,**histobin,**dup_bin;
	
	b=(int*)malloc(length * sizeof(int));
    c=(int*)malloc(length * sizeof(int));
    d=(int*)malloc(length * sizeof(int));
    e=(int*)malloc(length * sizeof(int));
	stok=(int*)malloc(width * sizeof(int));

    duphbin=(short**)malloc(length*sizeof(short*));
    for(i=0;i<length;i++)
		duphbin[i]=(short*)malloc((2*width)*sizeof(short));
	
	smoothbin=(short**)malloc(length*sizeof(short*));
    for(i=0;i<length;i++)
		smoothbin[i]=(short*)malloc((2*width)*sizeof(short));

	/*creation of duphbin whose width is double of bin and containing elements of bin***/
	
	for(i=0;i<length;i++)
		for(j=0;j<(2*width);j++)
			duphbin[i][j]=0;
	
	for(i=0;i<length;i++)
		for(j=0;j<width;j++)
			duphbin[i][j]=bin[i][j];
	
	for(i=0;i<length;i++){ 
		k=width;
		for(j=0;j<width;j++){
			if(bin[i][j]==1){ 
				duphbin[i][k]=1;
				k++;
			}
			
			
		}
    }

	/******computation of stroke width taking the whole text *********/
	for(j=0;j<width;j++)
		stok[j]=0;

	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			if(bin[i][j]==1){
				count=0;
				while(bin[i][j+1]!=0){
					count++;
					j++;
				}
				stok[count]=stok[count]+1;
			}

		}
	}

	max=stok[0];
	for(i=0;i<width;i++)
		if(stok[i+1]>max)
			stwidth=i;
	max=stok[i+1];

	//********Computation of stroke width is complete*********************/  
	
	
	//creation of smoothbin whose width is double of bin  and containing elements of bin

	for(i=0;i<length;i++)
		for(j=0;j<width*2;j++)
			if(j>=width)
				smoothbin[i][j]=0;
			else 
				smoothbin[i][j]=bin[i][j];
		
	//counting the total no.of one in every line horizantaly and keep in b array
	for(i=0;i<length;i++){
		count=0;
		for(j=0;j<width;j++){
			if(bin[i][j]==1)
			 count=count+1;
		}
		b[i]=count;
	}
	
	//******************** find the max. no. one among every lines ************************
	max=b[0];
	for(i=1;i<length;i++)
		if(b[i]>max){
			max=b[i];
			headlineindex=i;
		}
	//********************* drawing the histogram on duphbin array  *************************
	/*
	for(i=0;i<length;i++){ 
		k=width;
		for(j=0;j<width;j++){
			if(bin[i][j]==1){ 
				duphbin[i][k]=1;
				k++;
			}
		}
    }
	*/
	j=width+max/4;
//	k= duphbin[0][935];
	/*******find the maximum height of histogram on 1/4th distance from starting point********/
	
	count=0;
	k=0;
	for(i=0;i<length;i++){
		j=width+max/4;
		if((duphbin[i][j])!=0){
			count=0;
			while(duphbin[i][j]!=0 && i<length-1){
				count=count+1;
				i++;
			}
			c[k]=count;
			k++;
		}
	}

	maxlen=c[0];
	for(i=0;i<length;i++){
		if(c[i]>maxlen)
			maxlen=c[i];
	}
	
	/**********drawing the white line horizantaly through histogram  ****************/
	for(i=0;i<length;i++){
		j=width+max/4;
		duphbin[i][j]=0;
	}
	
	//--------------------------------------------------------------------------
	//***Smoothing horizontal of small white gaps(<2*maxlen) between characters into black***
	//--------------------------------------------------------------------------

	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			if(smoothbin[i][j]==0){ 
				count3=0;
				prevj=j;
				while(smoothbin[i][j]==0 && j<width-1){
					count3++;
					j++;
				}
				if(count3<=2*maxlen){
					for (jj=prevj ; jj<= j; jj++)
						smoothbin[i][jj]=1;
				}
			}
		}
	}

	/*** smoothing of small black gaps between chracters into white ************/
	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			if(smoothbin[i][j]==1){ 
				count3=0;
				prevj=j;
				while(smoothbin[i][j]!=0){
					count3++;
					if( j == width -1) break;
					j++;
				}
				if(count3<=2*maxlen){
					for (jj=prevj ; jj<= j; jj++)
						smoothbin[i][jj]=0;
				}
			}
		}
	}

	count=0;
	for(i=0;i<length;i++){ 
		k=0;
		for(j=0;j<width;j++){
			if(smoothbin[i][j]==1){
				count++;
				if(count>maxlen*2){
					smoothbin[i][k]=1;
				}
			}
		}
    }
	//*** Smoothig of  small  black run into white run  in smoothbin array****************/

	for(i=0;i<length-1; i++){
		j=0;
		previ=0;
		if((smoothbin[i][j]==0) && (smoothbin[i+1][j]==1)){
			previ=i+1;
			count=0;
			while(smoothbin[i+1][j]!=0 && i<length-2){
				count=count+1;
				i++;
				lasti=i;
			}
			if(count<maxlen*0.5){
				//printf("count %d is less maxlen & corrs val of previ is %d and lasti=%d\n",count,previ,lasti);
				for(k=previ;k<=lasti;k++){
					smoothbin[k][j]=0;
				}
			}

		
		}

	}

	/********smoothig of  small  white  run into black run.in smoothbin array***************/

	k=0;
	for(i=0;i<length-1; i++){
		j=0;
		previ=0;
		if((smoothbin[i][j]==1) && (smoothbin[i+1][j]==0)){
			previ=i+1;
			count=0;
			while(smoothbin[i+1][j]!=1){
				count=count+1;
				i++;
				lasti=i;
				if(i==length-2)
				break;
			}
	
			if(count<(maxlen* 0.10)){
				//printf("count %d is less maxlen & corrs val of previ is %d and lasti=%d\n",count,previ,lasti);
				for(k=previ;k<=lasti;k++){
					smoothbin[k][j]=1;
				
				}
			}
		}
	}


	/**************** drawing the histogram on smoothbin array  **************************/
	for(i=0;i<length;i++){ 
		k=width;
		for(j=0;j<width;j++){
			if(smoothbin[i][j]==1){ 
				smoothbin[i][k]=1;
				k++;
			}
		}
    }
	/******drawing the white line on smoothbin horizantaly through histogram***************/
	for(i=0;i<length;i++){
		j=width+max/4;
		smoothbin[i][j]=0;
	}
	/**************** segmented lines are in the middle of the white space ************/
	/************ and computation of co-ordinates of segmented lines ******************/
	/*
	i=0;
	j=width+1;
	while(duphbin[i][j]!=1)
		i++;
	
	d[0]=i-2;
	k=1;
	for(i=0;i<length-2;i++){ 
		previ=0;
		j=0;
		if((smoothbin[i][j]==1) && (smoothbin[i+1][j]==0)){
	   		previ=i+1;
			while(smoothbin[i+1][j]!=1){
				lasti=i;
				i++;
				if(i>length-2)	break;
			}
			previ=previ+(lasti-previ)*0.5;
			d[k]=previ;////////////change k into k+1
			k=k+1;
		}
	}
	*/
	//*************************************************************************
	//**********Smothing of small black run into white in duphbin array********
	//*************************************************************************
	for(i=0;i<length-1; i++){
		j=width;
		previ=0;
		if((duphbin[i][j]==0) && (duphbin[i+1][j]==1)){
			previ=i+1;
			count=0;
			while(duphbin[i+1][j]!=0 && i<length-2){
				count=count+1;
				i++;
				lasti=i;
			}
			if(count<maxlen*0.5){
				for(k=previ;k<=lasti;k++){
					duphbin[k][j]=0;
				}
			}
		}// end if smoothbin
	}// end for i

	//*
	m=0,n=0;
	i=0;
	j=width;
	while(duphbin[i][j]!=1)
		i++;
	if(i-2>0)
		d[0]=i-2;
	else d[0]=i;
	
	k=1;
	//*********//
	for(i=0;i<length-2;i++){ 
		previ=0;
		j=0;
		if((smoothbin[i][j]==1) && (smoothbin[i+1][j]==0)){
	   		previ=i+1;
			while(smoothbin[i+1][j]!=1){
				lasti=i;
				i++;
				if(i>length-2)	break;
			}
			//*************************************
			upperi=0;
			for(m=previ;m<lasti;m++){
				if(duphbin[m][width]==0){//width+1 changed to width
					upperi=m; 
					loweri=m;
					break;
				}
			}
			while(duphbin[m][width]==0 && m<length-1){
				loweri++;
				m++;
			}
			//*************************************
			if(upperi!=0)
				previ=upperi+(loweri-upperi)*0.5;

			else
				previ=previ+(lasti-previ)*0.5;
			d[k]=previ;
			k=k+1;

		}
	}

	//	*/

	//***************************************************************
	// end of segmented lines are in the middle of the white space//
	
	for(i=0;i<k-1;i++){
		startrow=0;endrow=0;
	
		// make a new array copy elements of d[index]+1 to d[index+1]
		//in to it and pass to function 
		nl=d[i+1] - d[i];
		nw=width;
		chararray=(short**)malloc(nl * sizeof(short*));
		  for(ni=0;ni<nl;ni++)
		chararray[ni]=(short*)malloc(nw*sizeof(short));
		
		dup_bin=(short**)malloc(nl * sizeof(short*));
		  for(ni=0;ni<nl;ni++)
		dup_bin[ni]=(short*)malloc(nw*sizeof(short));

		for(ni=0, orri=d[i];ni<nl;ni++,orri++){
			for(nj=0,orrj=0;nj<width;nj++,orrj++){
				chararray[ni][nj]=smoothbin[orri][orrj];

			}
		}
		//*
		for(ni=0, orri=d[i];ni<nl;ni++,orri++){
			for(nj=0,orrj=0;nj<width;nj++,orrj++){
				//chararray[ni][nj]=bin[orri][orrj];
				dup_bin[ni][nj]=bin[orri][orrj];

			}
		}
		//*/
		index=d[i];

		//------------------------------------------------------------------
		//************** fun. call of findbusyzone *************************
		//------------------------------------------------------------------
		
		//findbuzyzone(chararray,dup_bin,nl,nw,stwidth);
		
		//------------------------------------------------------------------
		//printf("\n After findbusyzone startrow=%d,endrow=%d",startrow,endrow);
		cstart=startrow+index;
		cend=endrow+index;

		//startbzone[k]=cstart; endbzone[k]=cend; 
		//printf("\n***end=%d",end);
		//*

		//for(t=0;t<width;t=t+2){
		//tempbin[cstart][t]=1;	//startrow
		//}										//draw horizontal row(start and end)
		//for(t=0;t<width;t=t+2){					//for showing busyzone
		//tempbin[cend][t]=1; //endrow
		//}
		//*/
		//getch();
		//*******************UPZONE SEGMENTATION CALLING*************************
		//upzonesegment(d,i,k,bin,tempbin,upbin,length,width,startrow,endrow);
		//getch();
		//*******************LOWZONE SEGMENTATION CALLING*************************
		//lowzonesegment(d,i,k,bin,tempbin,lowbin,length,width,startrow,endrow);
		//getch();
		//******************* WORD SEGMENTATION CALL ****************************
		wordsegment(d,i,k,bin,tempbin,width,startrow,endrow,node);//WORD SEGMENT. CALLING

		//***************** END OF WORD SEGMENTATION CALL*************************
		
	}//endof for i

	//*******this 2 statement is for drawing the lines between lines  *************



	for(i=0;i<k;i++){          
		for(j=0;j<width;j++){
			tempbin[d[i]][j]=1; // bin replaced by tempbin
		}
	}



	//**********************  end of drawing lines  ******************************
	//strcpy(str1,str4);
	//strcat(str1,"R.tif");//****	//results redirected to fullpageBZ.tif	
	//write_file(str1,tempbin,length,width);
	//write_file(str1,duphbin,length,2*width);
	//printf("\nEND OF LINE SEGMENTATION \n");
	//free(duphbin);
	
} //end of line segment
