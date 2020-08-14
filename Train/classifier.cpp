
/***  READ ME for this program ***
   This is a triplet based script identification

   Local  script class no 0, it marks as vertical and horizantal
   Devnagari 1,  it marks  as   horizontal
   English 2  it marks as  vertical  

    Need to be changed: for different triplet 
     (a) Feature file  should be changed for each triplet  (line no 148).
	 

      -------SVM parameter may be modified for better accuracy---
 ***/


#include<cstdio>
//#include <iostream.h>
#include <malloc.h>
#include "line-word.h"
#include "line_word.h"
#include "script.h"
#include <cstdio>
#include <ctype.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <ml.h>
#include<conio.h>

 int Matra_Less(Comp_list *Header,int Boundary_start,int Boundary_end,int mincol,int maxcol,short **binary,int length,int width,int Print_flag,int *matra_maxrow, int stroke_width);
 int midhorizon(int,int,int);
 ln_width * Line_detection( short ** , int  , int) ;
 int Character_Width(int,int,short**,int,int,int)   ;
 word_seg* Word_segmentation( int , int , int , int , short ** , short ** , int  , int  ,int * );
 int findtopconvex(short **,int,int,int);
 int findoverlap(short **,int,int);
 int validbot(short**,int,int,int);
 int findthai_topreservoir(short **,int,int,int);
 int finditalic(short **,int ,int ,int );
 Comp_list* comAnalysis(int max, int col,short **binary,int min,int length,int width);
 C_flow Reservoir(short **bin,int length,int width,int res_height_thres,int Print_flag);
 char Invoke_italic( Comp_list * , int );
 int findvertical(short**,int,int,int);
 int Vertical_line( Comp_list *  ,int , int );
 int numofchar,totalchar,widthchar,english,numofword;
 float vvector,vvector3,vvector4,vvector5,vvector2,vvector7,vvector6;
 //charactersegment1(int *,int,int*,int,int,short **,short **,int,int,int);
	
extern character *node;
extern int mode;
int index_vector;
int clasiffier_call=0;
int result;
Comp_list* comAnalysis1(int max, int mincol, int maxcol, short **binary,int min,int length,int width,int *t,char group,int decision,int Print_flag);
void Line(short **,int,int,char );
float *dimension400feature(short **image,int row,int col);
void print_2Darray(short **,int ,int );


int SuTrainSVM(short **binary,int length,int widthh,char group)
{
	
	FILE *file,*tempfile; 
	
	CV_FUNCNAME( "SuTrainSVM" );

    __BEGIN__;
	CvMat *TrainingData,*labels;
    
 // CvMat *sample_idx; 
//	CvMat *feature_idx;
	CvMat *vector_test;
    
	
	char *csize_database[] = {"_1500","_3000","_4500","vector"};
	int  isize[] = {1500, 3000, 4500, 6140};
	int width=400; //n_samples=200;
	
	int i,j,step,n;
	int n_features = 400; //5;
	//int n_sv;
	int fold = 5;
	//const float *sv;	
	float aux;
	float   res;int m,p,iLand=0,nl,nw,engchk,engchk1;
    int total_width=0;  
    int flag=0;
    int headline,zero=0,one=1;
    Comp_list *Result  ;  
    int d_row = 0  ;  
    int vertical_flag ;
    short **arr ;
	short **narray;
    word_seg *Word_part ;
    int total_word_line=0 , group_word=0  ;
    int Print_flag = 0 ;    
    int Devnagari_word = 0 ;
    int Telugu_word = 0 ;
    int English_word = 0 ;
    int Gujrati_word = 0 ;
    int Kannada_word = 0 ;
    int Bengali_word = 0 ;
    int Confusion = 0 ;
    int max_matra_row ;
    int stroke_width ;
    int word_number = 0 ;
    int word_gt_6 = 0 ;
    int word_3_6 = 0  ;
    int word_1_3 = 0  ;
    int t,decision=0 ;
    int bot_res=0,vertical=0,top_oriya=0;
    int italic=0,newflag=0;
	char group1='p';
	float *vectorarray;
	int nl_word;
	int nw_word;
    
    ln_width *Line_head,*Line_pt;
    int Total_count1=0,Total_count2=0,Comp_line = 1,Total_top=0,Total_mean=0,Total_Comp_Res=0;    
    FILE *fp , *fp1;
	//Training SVM
 	
	CvTermCriteria stopSVM;
 	stopSVM.type = CV_TERMCRIT_EPS | CV_TERMCRIT_ITER;
	stopSVM.max_iter = 1000;
	stopSVM.epsilon = 0.01;
    float gamma=42.00;
  /*CvSVMParams params(  CvSVM::C_SVC, 
       	                 CvSVM::LINEAR, 
           	             0.0 ,0.0 ,0.0 , // poly
               	         1.0, // C
                   	     0.0, 0.0,
                       	 NULL, stopSVM   );
	*/				 

	CvSVMParams params(  CvSVM::C_SVC, 
                         CvSVM::RBF, 
                         0.0 , 42.00 ,0.0 , // poly
                         100.0, // C
                         0.0, 0.0,
                         NULL, stopSVM);
                     

    
 	CvSVM SVMClassifier; //( TrainingData, labels, 0, 0, params );   
	
	
	for(n = 3 ; n < 4 ; n++)
	{
		
		//---------------line counting logic for feature file-----------------
		//FILE * pFile;
		int linec;
		int n = 0;
		//********************************************************************************
		/**************** change feature file name   *****************/

		file=fopen ("feature400-e-h-jap.txt","r");

		//********************************************************************************
		printf("\n after declaration fopen");
		if (file==NULL)
			perror("\nError opening file");
		 else
		{
			do { //DO
				linec = fgetc (file);// reads a charactr one by one from a file
				//printf(" %d",linec);
				if (linec =='\n')
					n++;
			}    //DO
			while (linec != EOF);
			//fclose (file);
		 }
		//---------------------------------------------------------------------
         rewind(file);


		//int height = isize[n];
		printf("\n n=%d",n);
		//getch();
		int height=n-1; 	 /*** here n is the Number of record in trainning file**/
		int size_training = (int)((fold-1.0)*height/(double)fold);
		int size_test = height - size_training;
		vector_test = cvCreateMat(1,width , CV_32FC1 );
	

		//create OpenCV matrix to store data

		TrainingData = cvCreateMat( height, width, CV_32FC1 );
		labels = cvCreateMat( height, 1 , CV_32FC1 );
	
		// step is the length of columns  or it indicates the number of feature in the matrix
		step = TrainingData->step;  
	
		for( i = 0 ; i < height ; i++)
		{
			fscanf(file,"%g",&aux);
			labels->data.fl[i] = aux;
			for(j = 0; j < width ; j++ )
			{
				fscanf(file,"%g",&aux);
				((float*)(TrainingData->data.ptr + i*step))[j] = aux;
			}
		}

		// -- end of loading data from text file
	
                       
		//Perform 5-fold correlation
	//	double mitja[fold];
	    double *mitja;
		mitja=(double *)malloc(fold * sizeof(double));
		double mean_fold = 0;
		double std_fold = 0;

	    printf("\n Training .............\n");

		SVMClassifier.train( TrainingData, labels,NULL,NULL, params );

		printf(" Recognition..................\n");
        
        
		arr = ( short**)malloc(length * sizeof(short*));
		for( i = 0 ; i < length ; i++ )
			arr[i] = (short*)malloc(widthh * sizeof(short));
     
		for(i=0;i<length;i++)
			for(j=0;j<widthh;j++)
				arr[i][j] = binary[i][j] ;    
    
///	tempfile=fopen ("linklistout.txt","a+");
	///while(node!=NULL){
	///	fprintf(tempfile,"\n sr=%d er=%d sc=%d ec=%d",node->startrow,node->endrow,node->startcolumn,node->stopcolumn);
	///node=node->next;
	///}
	 //fclose(tempfile);
		///Line_pt = Line_detection( binary , length , widthh ) ;
                  
		///Line_head = Line_pt->next ;
	 
		///Line_pt = Line_pt->next ;
     
		///while(Line_pt!=NULL)

		 //{ // printf("hello \n");
        
			///if( Line_pt->valid_line != 0 )
			///{   
				///	printf("\n Executing....");
				///	Word_part = Word_segmentation( Line_pt->Boundary_start , Line_pt->Boundary_end , Line_pt->Line_start, Line_pt->Line_end,  binary , arr , length , widthh , &stroke_width ); 
					
					while(node!=NULL)///while(Word_part!=NULL)
					{     
					///if(Word_part->word_valid==1)
					///{   
							headline=0;
			  				bot_res=0;
							vertical=0;
							italic=0;
							vertical_flag=0;

							///	nw= Word_part->word_end - Word_part->word_start;
							
							nl_word=node->endrow - node->startrow;		 //nl-word is word length
							nw_word=node->stopcolumn - node->startcolumn;//nw-word is word width
							///nw=node
							/// nw=nw+1;
					///	nl=Line_pt->Line_end - Line_pt->Line_start ;
						narray=(short**)malloc((nl_word)*sizeof(short*));
						for(i=0;i<nl_word;i++){
							narray[i]=(short*)malloc((nw_word) * sizeof(short));
							for(j=0;j<(nw_word);j++){    
								narray[i][j]=0;
							}
						}
                          
							   
						for(i=0,m=node->startrow;i<nl_word;m++,i++)
						{    
							for(j=0,n=node->startcolumn;j<nw_word;j++,n++)
							{
								narray[i][j]=binary[m][n];
					             
							}
						}
						numofword++;		
						vertical_flag=0;
						bot_res= 0;
						top_oriya=0;
						newflag=0;
						//print_2Darray(narray,nl_word ,nw_word);
						vectorarray=dimension400feature(narray,nl_word,nw_word);
					
		     			for(j = 0; j <400 ; j++ )
						{
			    			aux=vectorarray[j];
							//printf(" %f ",vectorarray[j]);
							//vectorarray[j]=0;
							vector_test->data.fl[j]= aux;
						}///////////////////////
						//printf("\n");
		
						res = SVMClassifier.predict( vector_test );

						//-----------------------------------------------
						//******Character Segmentation Calling **********
						//-----------------------------------------------

					//	switch(res){
						//	case 1.00://Local-Bengali character segment calling****

					
							//	break;
							//case 2.00://hindi character segment calling****
							//	break;
							//case 3.00://English character segment calling****
							//	break;
						//	default:
						//		printf("\n default");
						//}//end of switch

						//-----------------------------------------------
						//******End Character Segmentation Calling ******
						//-----------------------------------------------
						
						
                        if(res==2.00)   /** Local  script class no 0, Devnagari 1, English 2 **/
						{  	//printf(" %f bengali \n",res);
							///for(m=Line_pt->Line_start;m<Line_pt->Line_end;m++)//old line
	
                          	//void charactersegment( *d, index, k, ** bin, **tempbin, width, startrow, endrow, **node)
							for(m=node->startrow;m<node->endrow;m++)
							{  
								///for(n=Word_part->word_start;n<Word_part->word_end;n=n+8)///old line
								for(n=node->startcolumn;n<node->stopcolumn;n=n+8)
									if(binary[m][n]==0)   
										arr[m][n]=1;
								    else
								        arr[m][n]=0;
								}
							}
						
			  				else
							{  	     
								if(res==1.00)
								{ //printf(" \n is hindi  \n");	getchar();
									///for(m=Line_pt->Line_start;m<Line_pt->Line_end;m=m+8)
									for(m=node->startrow;m<node->endrow;m=m+8)
									{  
										///for(n=Word_part->word_start;n<Word_part->word_end;n++)
										for(n=node->startcolumn;n<node->stopcolumn;n++)
											if(binary[m][n]==0)   
												arr[m][n]=1;
								     		else
												arr[m][n]=0;
										}
									}
									else
									{
										///for(m=Line_pt->Line_start;m<Line_pt->Line_end;m=m+8)
										for(m=node->startrow;m<node->endrow;m=m+8)
										{  
											///for(n=Word_part->word_start;n<Word_part->word_end;n++)
											for(n=node->startcolumn;n<node->stopcolumn;n++)
												if(binary[m][n]==0)   
													arr[m][n]=1;
								     	        else
													arr[m][n]=0;
											}
										
											///for(m=Line_pt->Line_start;m<Line_pt->Line_end;m++)
											for(m=node->startrow;m<node->endrow;m++)
											{  
												///for(n=Word_part->word_start;n<Word_part->word_end;n=n+8)
												for(n=node->startcolumn;n<node->stopcolumn;n=n+8)
													if(binary[m][n]==0)   
														arr[m][n]=1;
								     				else
								         				arr[m][n]=0;
											}
										}
									}
												
             node=node->next;              
		}/*ending of the word while */ 
				

                   /*  for(i=0;i<nl;i++)
					 {
		               	 free(narray[i]);
					 } 
	            	 free(narray);*/
		///Word_part= Word_part->next;

	
   //}  /* ending of the Line if*/
               
         ///Line_pt=Line_pt->next; 
		 
 /// }  /*ending of the Line While*/
        
     for(i=0;i<length;i++)
     for(j=0;j<widthh;j++)
      binary[i][j] = arr[i][j] ;    
      
     /*return ;*/
     	cvReleaseMat( &TrainingData );
		cvReleaseMat( &labels );
//		cvReleaseMat( &sample_idx );
//		cvReleaseMat( &feature_idx );
		cvReleaseMat( &vector_test );
		
	}   /// end of for loop with variable n

	
	// Release memory
/***	Line_pt = Line_head ;
      
      while(Line_pt!=NULL)
       {
         if( Line_pt->valid_line != 0 )
          {
              for(p=0;p<widthh;p+=2)
               {
                 arr[Line_pt->Boundary_start][p] = 1 ;
                 arr[Line_pt->Boundary_start + 1][p] = 1 ;
                 arr[Line_pt->Boundary_end][p] = 1 ;*/
       /**        }
          }
               
         Line_pt=Line_pt->next;       
       }     
 */
	 // fprintf(fp,"%d\n",English_word);
	  	 
	  //fclose(fp);
    /**for(i=0;i<length;i++)
     for(j=0;j<widthh;j++)
       binary[i][j] = arr[i][j] ; 
	 //fprintf(fp1,"%d\n",english); **/
	 //
	 //fclose(fp1);

	
	__END__;
	
	//cvReleaseMat( &sample_idx );
	

//	fclose(file);

return result;
//	return 0;
    //return result;
}  // End of function
 

