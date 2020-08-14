#include <malloc.h>
#include <cstdio>
extern int mode;

int modevalue(short **binary,int length,int width)
{

int non,i,j,flag=0,max=0,mod=0;

 int *strokemode;
 strokemode=(int*)malloc(width * sizeof(int));
	for(j=0;j<width;j++)
	{
		strokemode[j]=0;
	    
	}


                     for(i=0;i<length;i++)
					 {   non=0;
						 for(j=0;j<width;j++)
						 {
                            if(binary[i][j]==1) 
							{
                               non++;    
							   flag=1;
							}
                            else
							{
                                 if(binary[i][j]==0 && flag==1)
								 {
                                    strokemode[non]=strokemode[non]+1;
								    flag=0;
								    non=0;
								 }
							}
						 }
						 if(flag>1)
						 {
                            strokemode[non]=strokemode[non]+1;
							
						 }
					 }
                       max=strokemode[0];
	                   mod=0;
					   for(j=1;j<width;j++)
					   {
						   if(strokemode[j]>max)
						   {    max=strokemode[j];
						        mod=j;

						   }
					   }		
					   mode=mod;
					   free(strokemode);
					//	printf("the value of mode is %d",mod);
						return(mod);
}
