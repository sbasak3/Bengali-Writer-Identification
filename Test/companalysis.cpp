



/*******************************************************************************************************/
/*                                                                                                     */ 
/*                          M O D U L E   N A M E  ::  connect .c                                      */
/*                                                                                                     */  
/*                    P U R P O S E   O F   T H I S   M O D U L E   I S   ::                           */
/*                                                                                                     */
/*                                                                                                     */ 
/*               1 :  TO  FIND  THE  NO.  OF  COMPONENTS  IN  A  PARTICULAR  LINE                      */
/*                                                                                                     */
/*               2 :  TO  FIND  THE  INFORMATION  ABOUT  RESERVOIRS  OF  A  PARTICULAR  COMPONENT      */
/*                                                                                                     */
/*               3 :  TO  DISPLAY  EACH  COMPONENT  OF  A  LINE                                        */
/*                                                                                                     */
/*                                                                                                     */ 
/*                                 E X T E R N A L  C A L L S                                          */
/*                                                                                                     */
/*               1 :  Reservoir(short**,int,int);  =>   Module Name : Comp_reservoir.c                 */
/*                                                                                                     */ 
/*******************************************************************************************************/                                                      
#include <cstdio>
#include <malloc.h>
#include"Script.h"


void Free_list(short **head,int length);

   typedef struct MyStack
    {
	int top;
	int *i, *j;
    }stk;	

       void Init(stk *vertex)
	 { 
	   vertex->top = 0;
	   vertex->i = (int*)malloc(1024*1024*sizeof(int));///////chainging 
	   vertex->j = (int*)malloc(1024*1024*sizeof(int));////////changing
	 }
	 
       void  Stack_del(stk *vertex) 
          { 
            free(vertex->i);
            free(vertex->j); 
          }  
          
       int IsEmpty(stk *vertex) 
        	{ 
        	 return (vertex->top == 0);
          }
          
       void push(int p, int q,stk *vertex)
	 { vertex->i[vertex->top] = p;
	   vertex->j[vertex->top] = q;
	   vertex->top++; 
	 }
	 
       void pop(stk *vertex) 
          { 
             vertex->top--;
          }
          
       int peepi(stk *vertex) 
          { 
             return vertex->i[vertex->top-1];
          }
          
       int peepj(stk *vertex) 
          { 
            return vertex->j[vertex->top-1];
          }
          
       int Size(stk *vertex) 
          { 
            return vertex->top;
          }


Comp_measure  Label(int i1, int j1, int l,short **array,int min,int max,int width)
{
	stk myStack;
	int i,j,p,q,w;
	
	
	int Comp_length,Comp_width;
	
          Comp_measure result;	
          result.minRow=i1;
          result.maxRow=i1;
          result.minCol=j1;
          result.maxCol=j1;
          Init(&myStack); 
	push(i1,j1,&myStack);
	do{
		i1 = peepi(&myStack);
		j1 = peepj(&myStack);
		
        pop(&myStack);
		
		array[i1][j1] = l;
		for (i = i1+1; i >= i1-1; i--)
			for (j = j1+1; j >= j1-1; j--)
				if ( i>=min && i<max && j>=0 && j<width )
				 {
				   //printf("\n The Unit component described");
				   if(array[i][j]==1)
				   {
				     if(result.minRow>i)
		                          result.minRow=i;
		                        if(result.maxRow<i)
		                          result.maxRow=i;
		                        if(result.minCol>j)
		                          result.minCol=j;
		                        if(result.maxCol<j)
		                          result.maxCol=j;
				     push(i,j,&myStack);
				     }
				  } 
	} while(!IsEmpty(&myStack));
         Stack_del(&myStack);	
         
         Comp_length=result.maxRow-result.minRow+3;
         Comp_width =result.maxCol-result.minCol+3;
                       
         result.Comp_array=(short**)malloc(Comp_length*sizeof(short*));
         
         for(i=0;i<Comp_length;i++)
           result.Comp_array[i]=(short*)malloc(Comp_width*sizeof(short));
         
         for(i=0;i<Comp_length;i++)
          for(j=0;j<Comp_width;j++)
            result.Comp_array[i][j]=0;
            
        
                    
         for(p=result.minRow,i=1;p<=result.maxRow;p++,i++)
             {
              for(q=result.minCol,j=1;q<=result.maxCol;q++,j++)
               {
                 if(array[p][q]==l)
                  result.Comp_array[i][j]=1;
               } 
             } 

/*		 for(i=0;i<Comp_length;i++)
		 {
			 printf("\n");
			 for(j=0;j<Comp_width;j++)
				 printf("%d",result.Comp_array[i][j]);
		 }*/
		 
		 return result;
}

Comp_list* comAnalysis(int max, int col,short **binary,int min,int length,int width)
{
          int label = 2,i,j,m,n;
          short **array;
          int flag=0;
          Comp_list *header,*part;          
          int mid;                
          mid=min + (max - min)*0.4;
          array=(short**)malloc(length*sizeof(short*));
          for(i=0;i<length;i++)
           array[i]=(short*)malloc(width*sizeof(short));
                   
         for(i=0;i<length;i++)
          for(j=0;j<width;j++)
           array[i][j]=binary[i][j];
         
         header=(Comp_list*)malloc(sizeof(Comp_list));
            
//         printf("\n\n44444CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n\n");   
          for(i=min; i<max; ++i)
	    {
		for(j = 0; j<col; ++j)
		  {
			if(array[i][j]==1)
			 {
//			   printf("\neeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
			   header->Comp_data=Label(i,j,label++,array,min,max,width);
			   flag=1;
			   break;
	                        }
		  }
		if(flag==1)
		 break;  
             } 
	  
//	 printf("\n Part Analysis started *************");
		  
	 part=header;
	 min=i;
	 for(i=min; i<max; ++i)
	   {
		for(j = 0; j<col; ++j)
		  {
			if(array[i][j]==1)
			 {
			   part->next=(Comp_list*)malloc(sizeof(Comp_list));
			   part=part->next;
			   part->Comp_data=Label(i,j,label++,array,min,max,width);
			 }
		  }
	  } 
	  
	 part->next=NULL  ;
       	
//     printf("\n Part Analysis end !!!!!!!!!!!!!!*************");	
         //Free_list(array,length);          	  
         return header; 
         
	
}


void Free_list(short **head,int length)
  {
     int i;
     for(i=0;i<length;i++)
       free(head[i]);
     free(head);  
   }         
 
