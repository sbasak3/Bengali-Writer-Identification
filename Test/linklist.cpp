#include"line_word.h"
#include<stdio.h>
#include<conio.h>
#include<malloc.h>
#include "binarize.h"
#include"string.h"
#include "svm.h"
extern char str4[250];
extern char strout[60];
//char str1[50];
char strtemp[100];
int charallograph;
extern character *node;
struct svm_parameter param;		// set by parse_command_line
struct svm_problem prob;		// set by read_problem
struct svm_model *model64,*model400;
struct svm_node *x_space64,*x_space400,*x_space;
binarize    Obina;
int startrowwrd,endrowwrd;
extern int mode;
int resultclass[400];
int findbuzyzone_l_startrow_endrow(short ** chararray,int length,int width);
double  process_word(short ** duplicatebinary,int length, int width,int mode);

ichar *store_pix_position(short **array,int strow,int edrow,int stcol,int edcol){
	int i,j;
	ichar *points=NULL,*head=NULL,*tempoint=NULL;
	for(i=strow;i<edrow;i++){
		for(j=stcol;j<edcol;j++){
			if(array[i][j]==1) {
				if(head==NULL){
					tempoint=(ichar*)malloc(sizeof(ichar));
					tempoint->pos_i=i;
					tempoint->pos_j=j;
					tempoint->next=NULL;
					head=points=tempoint;
					continue;
				}
				tempoint=(ichar*)malloc(sizeof(ichar));
				tempoint->pos_i=i;
				tempoint->pos_j=j;
				tempoint->next=NULL;
				points->next=tempoint;
				points=points->next;
				//printf("\n inside store pix");
			}
		}
	}
	//printf("\n inside store pix");
	return head;
}//store pix position

void insert(character **node,short **bin,int startrow,int stoprow,int startcolumn,int endcolumn,int lindex,int windex)
{
	character *record=(character*)malloc(sizeof(character));

	//	ichar *pix;//inner node

	//node->next = record;
	record->startrow = startrow;
	record->endrow = stoprow;
	record->startcolumn = startcolumn;
	record->stopcolumn = endcolumn;
	record->lineindex=lindex;
	record->wordindex=windex;
	record->pixels=store_pix_position(bin,startrow,stoprow,startcolumn,endcolumn);
	record->next = NULL;

	if(*node==NULL)
	{
		*node=record;
		printf("\n from insert function node checking*******************************************");
		printf("\n Insert funtion node->strow=%d",(*node)->startrow);
		//node=node->next;
	}
	else
	{
		character *p = *node;
		while(p->next)
		{
			p=p->next;
		}
		p->next=record;
	}

}//END OF INSERT FUNCTION.


void display(character *node)
{
	while(node!=NULL){
		int i,j;
		short **smallbin;
		ichar *temppix;
		int ln= node->endrow-node->startrow;
		int wd=node->stopcolumn-node->startcolumn;
		smallbin=(short**)malloc(ln*sizeof(short*));
		for(i=0;i<ln;i++)
			smallbin[i]=(short*)malloc((wd)*sizeof(short));

		for(i=0;i<ln;i++){
			for(j=0;j<wd;j++){
				smallbin[i][j]=0;

			}
		}


		//*****
		temppix=node->pixels;

		while(temppix!=NULL){
			i=temppix->pos_i-node->startrow;
			j=temppix->pos_j-node->startcolumn;
			smallbin[i][j]=1;
			temppix =temppix->next;

		}
		for(i=0;i<ln;i++){
			for(j=0;j<wd;j++){
				printf("%d",smallbin[i][j]);
			}
			printf("\n");
		}

		getch();
		node=node->next;
	}
}//end display

void print_character(character *node,short **bin,int length,int width){
	int i,j;
	short **testbin;
	ichar *temppix;//=(ichar*)malloc(sizeof(ichar));

	testbin=(short**)malloc(length*sizeof(short*));
	for(i=0;i<length;i++){
		testbin[i]=(short*)malloc(width*sizeof(short));
	}

	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			testbin[i][j]=0;
		}
	}

	if(node==NULL){

		//printf("\n node is null");
	}

	//for(i=0;i<length;i++){
	//testbin[i][25]=1;
	//}

	while(node!=NULL){
		temppix=node->pixels;
		//printf("\n ******************inside node***");
		while(temppix!=NULL){
			///if(temppix->next->next!= NULL){
			//printf("\n ******************inside temppix=%d",node->pixels->pos_i);
			//printf("\n ******************inside temppix=%d",temppix->pos_i);
			i=temppix->pos_i;
			j=temppix->pos_j;
			testbin[i][j]=1;
			temppix =temppix->next;

			//}
		}
		node=node->next;
	}
	//strcpy(str1,str4);
	//strcat(str1,"link.tif");
	//	write_file(str1,testbin,length,width);*************

}//end print_character_whole_page

//-----------------------------------------------
// creating files for a single character
//-----------------------------------------------

void print_single_char(character *node)
{
	int count=1; int i,j,midofbuzyzone,k,index,result=0,startj;
	//printf("\na");

	//	model64=svm_load_model("104-gradient-48");
	model400=svm_load_model("303Writer-GradientChain-464-150allograph-Model-Reverse");///////// \Change \The Name of \Model File here ok dada

	int *whitecount;
	for(i=0;i<400;i++)
		resultclass[i]=0;
	FILE *fp, *fp2;
	fp=fopen("Writer-Results with 464-dimension-150Allograph.txt","a+");  ////   \\C\hange result log file name accordingly

	fp2=fopen("Writer-Class Distribution with 464-dimension-150Allograph.txt","a+"); //  Change classification vote log file name accordingly
	while(node!=NULL){
		int i,j;
		short **smallbin;
		ichar *temppix;
		int ln= node->endrow - node->startrow;
		int wd=node->stopcolumn - node->startcolumn;
		//printf("\nln=%d wd=%d",ln,wd);
		smallbin=(short**)malloc(ln*sizeof(short*));
		for(i=0;i<ln;i++)
			smallbin[i]=(short*)malloc((wd)*sizeof(short));
		for(i=0;i<ln;i++){
			for(j=0;j<wd;j++){
				smallbin[i][j]=0;
			}
		}

		temppix=node->pixels;

		while(temppix!=NULL){
			i=temppix->pos_i - node->startrow;
			j=temppix->pos_j - node->startcolumn;
			smallbin[i][j]=1;
			temppix =temppix->next;

		}
		/*
		for(i=0;i<ln;i++){
		for(j=0;j<wd;j++){
		printf("%d",smallbin[i][j]);
		}
		printf("\n");
		}
		getch();
		*/
		//strcat(strout,"\\");

		//sprintf(strfinal,"%s%d_%04d.tif",str5,m1,n1);
		sprintf(strtemp,"%s%s_%04d.tif",strout,str4,count);
		//printf("\n str4=%s, srtout=%s,",str4,strout);
		//printf("\n strtemp=%s",strtemp);
		//write_file(strtemp,smallbin,ln,wd);
		whitecount=(int*)malloc(wd*sizeof(int));
		for(j=0;j<wd;j++)
			whitecount[j]=0;
		midofbuzyzone=findbuzyzone_l_startrow_endrow(smallbin,ln,wd);

		//printf(" Start  %d  End  %d  mode is %d \n",startrowwrd,endrowwrd,mode);
		if(midofbuzyzone>mode)
		{
			for(j=0;j<wd;j++)
				for(i=midofbuzyzone-mode;i<ln;i++)
				{   

					if(smallbin[i][j]==1)
					{  whitecount[j]++;
					}
				}

				for(j=0;j<wd;j++)
				{
					if(whitecount[j]==0)
					{    startj=j;index=0;
					for(k=j;k<wd;k++)
					{
						if(whitecount[k]==0)
							index++;
						else
							break;
					}
					index=index/2;
					index=startj+index;
					j=k+1;

					for(i=0;i<ln;i++)
						smallbin[i][index]=0;    //open this to perform  character segmentation 
					}
				}

				/*for(j=0;j<wd;j++)
				{     
				smallbin[endrowwrd][j]=1;
				smallbin[midofbuzyzone][j]=0;
				smallbin[startrowwrd][j]=1;
				}*/

				process_word(smallbin,ln, wd, mode);
		}

		//Obina.write_back_file(strtemp, smallbin, ln,  wd);

		free(whitecount);

		for(i=0;i<ln;i++)
		{
			free(smallbin[i]);
		}
		free(smallbin);
		count++;
		node=node->next;
	}//end of while

	result=0;
	fprintf(fp2,"\nImage FILE %s \n", str4);
	for(i=1;i<400;i++)
	{
		fprintf(fp2,"Class  %d: Vote %d  ,", i, resultclass[i]);
		if(result<resultclass[i])
		{
			result=resultclass[i];
			index=i;
		}
	}
	//getchar();
	fprintf(fp," \n Original Writer %s -> Identified writer   %d \n", str4,index);
	fprintf(fp2," \n");
	fclose(fp);
	fclose(fp2);
	//printf("\n end of single character printer");
}//end of print sigle character


//-----------THIS METHOD WILL CREATE AN ARRAY -----------
//BY USING THE LINK LIST FOR EACH ELEMENTS OF THE LINKLIST
short** LList2Array(character *node)
{
	int count=1;
	int i,j;
	short **smallbin;
	ichar *temppix;
	int ln= node->endrow - node->startrow;
	int wd=node->stopcolumn - node->startcolumn;
	smallbin=(short**)malloc(ln*sizeof(short*));
	for(i=0;i<ln;i++)
		smallbin[i]=(short*)malloc((wd)*sizeof(short));
	for(i=0;i<ln;i++){
		for(j=0;j<wd;j++){
			smallbin[i][j]=0;
		}
	}

	temppix=node->pixels;
	while(temppix!=NULL){
		i=temppix->pos_i - node->startrow;
		j=temppix->pos_j - node->startcolumn;
		smallbin[i][j]=1;
		temppix =temppix->next;
	}
	return(smallbin);
}//end of print sigle character


///------------------------------------------------------
/**********************************************
void word_insert(word *node,int start,int stop)
{
word *record;
record=(word*)malloc(sizeof(word));

while(node->next )
{
node=node->next ;
}
node->next = record;
record->start = start;
record->stop = stop;
record->next = NULL;
}


/*

void word_insert(word *node,int start,int stop)
{
word *record;
record=(word*)malloc(sizeof(word));

while(node->next )
{
node=node->next ;
}
node->next = record;
record->start = start;
record->stop = stop;
record->next = NULL;
}



void word_display(word *node)
{
while(node!=NULL)
{
printf("word start= %d\tword stop=%d\n",node->start ,node->stop );
node=node->next ;
}
}

void free_char(character *node)
{
character *temp;
while(node!=NULL)
{
temp=node->next ;
free(node);
node=temp;
}
}

void free_word(word *node)
{
word *temp;
while(node!=NULL)
{
temp=node->next ;
free(node);
node=temp;
}
}


void pitch_insert(pitch *node,int block_start,int block_stop)
{
pitch *record;
record=(pitch*)malloc(sizeof(pitch));

while(node->next)
{
node=node->next ;
}
node->next = record;
record->block_start=block_start;
record->block_stop=block_stop;
record->next=NULL;

}

void pitch_display(pitch *node)
{
printf("\nIN Pitch display\n");

node=node->next;
while(node!=NULL)
{
printf("block_start=%d\n",node->block_start );
printf("block_stop=%d\n",node->block_stop );

node=node->next ;
}
}

void pitch_free(pitch *node)
{
pitch *temp;
while(node!=NULL)
{
temp=node->next ;
free(node);
node=temp;
}
}*/

void print_2Darray(short **narray,int length,int width)
{
	static int count=0;	
	//strcat("n",strout);
	sprintf(strtemp,"%s%s_%04d.tif",strout,str4,count);
	//printf("\n str4=%s, srtout=%s,",str4,strout);
	//printf("\n strtemp=%s",strtemp);
	//write_file(strtemp,smallbin,ln,wd);
	Obina.write_back_file(strtemp, narray, length,width);

	count++;

}//end print_2D array