# include <malloc.h>
#include  <stdio.h>
extern int wrod;
extern char str1[60];extern int flag;
float *reductionfrm49to16(int *b,int ii,int jj,int kk,float count);
int least64(int c[300]);
int greatest64(int d[300]);
void division64(int k,int a[10],float dim);
float *dimension64feature(short **arr,int row,int col)
{
	int i,u,y1,j,t,s,r[10],c[10],y,z,m,ro,co,n,l=0,h8,h9,c1[2300],c2[2300],c3[2300],c4[2300],h1,h2,h3,h4,l1=0,l2=0,l3=0,l4=0 ;
	short **a,**b1;
	float count=0,*app;
	int b[7][7][4]={0};
	a =(short **) malloc(row * sizeof(short *));

 for (i = 0; i < row; i++) {
    a[i] = (short *)malloc(col * sizeof(short));
	}


 for(i=0;i<row;i++)
			   {
				   for(j=0;j<col;j++)
						{
					   
						a[i][j]=*(*(arr + i) + j);
				   }
				   
 }
 b1 =(short **) malloc(row * sizeof(short *));
 for (i = 0; i < row; i++) {
    b1[i] = (short *)malloc(col * sizeof(short));
	}

 for(i=0;i<row;i++)
			   {
				   for(j=0;j<col;j++)
						{
					   
						b1[i][j]=a[i][j];
				   }
				   
 }
 
for(i=0;i<2300;i++)
{
c1[i]=999;
c2[i]=0;
c3[i]=999;
c4[i]=0;
}
u=col-1;
y1=row-1;
for(i=0;i<row;i++)
			   {
				   for(j=0;j<col;j++)
						
					   
						if(a[i][j]==1)
						{	c1[l1]=j;
						l1++;
						break;
				   }
}
h1=least64(c1);

for(i=0;i<row;i++)
			   {
				   for(j=u;j>=0;j--)
					   if(a[i][j]==1)
					   {	c2[l2]=j;
						l2++;
						break;
				   }
}
h2=greatest64(c2);
for(i=0;i<col;i++)
			   {
				   for(j=0;j<row;j++)
						
					   
						if(a[j][i]==1)
						{   c3[l3]=j;
						l3++;
						break;
				   }
}
h3=least64(c3);
for(i=0;i<col;i++)
			   {
				   for(j=y1;j>=0;j--)
						
					   
						if(a[j][i]==1)
						{	c4[l4]=j;
						l4++;
						break;
				   }
}
h4=greatest64(c4);
ro=h4-h3+1;
co=h2-h1+1;
//printf("%d-%d %d-%d\n",h3,h4,h1,h2);
/*for(i=h3;i<=h4;i++)
		{
			for(j=h1;j<=h2;j++)
				printf("%d",a[i][j]);
		printf("\n");
		}*/
division64(ro,r,7);//for row division
/*for(i=0;i<=6;i++)
printf("%d ",r[i]);
printf("\n");*/

division64(co,c,7);//for column division
/*for(i=0;i<=6;i++)
printf("%d ",c[i]);
printf("\n");*/
h8=h3+1;
h9=h1+1;
 for(i=h8;i<h4;i++)
			   {
				   for(j=h9;j<h2;j++)
						{

					   if(a[i][j]==0)
						   b1[i][j]=0;
						   
					   else 
							   if(a[i][j]==1){
						  if(a[i][j+1]==1&&a[i-1][j+1]==1&&a[i-1][j]==1&&a[i-1][j-1]==1&&a[i][j-1]==1&&a[i+1][j-1]==1&&a[i+1][j]==1&&a[i+1][j+1]==1)
							  b1[i][j]=1;
							   else b1[i][j]=3;}
						
				   }
			   }
 for(i=h3;i<=h4;i++)
 {
	 if(a[i][h1]==0)
		 b1[i][h1]=0;
	 else b1[i][h1]=3;
	 if(a[i][h2]==0)
		 b1[i][h2]=0;
	 else b1[i][h2]=3;
 }
 for(j=h1;j<=h2;j++)
 {
	 if(a[h3][j]==0)
		 b1[h3][j]=0;
	 else b1[h3][j]=3;
	 if(a[h4][j]==0)
		 b1[h4][j]=0;
	 else b1[h4][j]=3;
 }

			   
			   
			   
for(m=0;m<=6;m++)

   for(n=0;n<=6;n++)
   {
s=h1;
t=0;
y=h3;z=0;
for(i=0;i<m;i++)//for row
y=y+r[i];
z=y+r[m];
for(j=0;j<n;j++)//for colm
s=s+c[j];
t=s+c[n];
//printf("%d-%d %d-%d\n",z,y,t,s);
			   
			   
			   for(i=y;i<z;i++)
			   {
				   for(j=s;j<t;j++)
						{
			   if(b1[i][j]==0)
								continue;
			   else if(b1[i][j]==1) continue;

			   		else //if(b1[i][j]==3)
					{
							if((j+1)<t)
							{
									if(b1[i][j+1]==3)
									b[m][n][0]++;
							}

							if((i-1)>=y&&(j+1)<t)
							{
									
								 if(b1[i-1][j+1]==3)
									b[m][n][1]++;
							}
							if((i-1)>=y)
							{
									
								 if(b1[i-1][j]==3)
									 b[m][n][2]++;
							}
							if((i-1)>=y&&(j-1)>=s)
							{
					
								 if(b1[i-1][j-1]==3)
									b[m][n][3]++;
									
							}	
							}
				   }
			   }
			


	
	   
   }	
 
 
   for(i=h3;i<=h4;i++)
			   {
				   for(j=h1;j<=h2;j++)
						{
					   if(b1[i][j]==3)
						   count++;
				   }
   }

	app=reductionfrm49to16((&b[0][0][0]),7,7,4,count);
	for(i=0;i<64;i++)
		if(app[i]>0)
			flag=1;
	return (app);
}


int least64(int c[300])
{
	int i,m;
	m=c[0];
	for(i=1;i<300;i++)
		if(c[i]<m)
			m=c[i];
		return m;
}
int greatest64(int d[300])
{
	int i,m=0;
     	 for(i=0;i<300;i++)
		 if(d[i]>m)
			 m=d[i];
		 return m;
}
void division64(int k,int a[10],float dim)
{
		float p,t,j=0;
	int q,i,prev=0;
       
	p=k/dim;
//printf("%f===  ",p);

for(i=0;i<dim;i++)
{
j=j+p;
  q=j;
t=q+0.5;
if (j>t)  a[i]= q+1-prev;
else a[i]=q-prev;
prev=prev+a[i];
}
						
	
}



float *reductionfrm49to16(int *b,int ii,int jj,int kk,float count)
{
int i,j,k,h=0;

float nc[7][7][4]={0},nc1[9][9][4]={0},nc2[4][4][4];
float *app;
app=(float*)malloc(64*sizeof(float));


//	pp->imgno=imgno;
	for(i=0;i<ii;i++)
	{
		for(j=0;j<jj;j++)
		{
			for(k=0;k<4;k++)
			{
			//	printf("required count for b[%d][%d][%d]=%d\n",i,j,k,*(b +i*jj*kk+j*kk+k));
			nc[i][j][k]=*(b +i*jj*kk+j*kk+k);
		//	printf("required count for b[%d][%d][%d]=%d\n",i,j,k,nc[i][j][k]);
			}
		//	printf("\n");
		}
	}

for(i=0;i<7;)
	{
		for(j=0;j<7;)
		{
			for(k=0;k<4;k++)
			{
              nc[i][j][k]=nc[i][j][k]*2;
						


			}j=j+2;
		}i=i+2;
}


for(i=1;i<8;i++)
{
	for(j=1;j<8;j++)
	{
		for(k=0;k<4;k++)
			nc1[i][j][k]=nc[i-1][j-1][k];
	}
}
for (i=1;i<8;i+=2)     
{
for (j=1;j<8;j+=2)
{
	for(k=0;k<4;k++)
nc2[i/2][j/2][k]=nc1[i][j][k]+ nc1[i-1][j-1][k]+nc1[i-1][j][k]+nc1[i-1][j+1][k]+nc1[i][j-1][k]+nc1[i][j+1][k]+nc1[i+1][j-1][k]+nc1[i+1][j][k]+nc1[i+1][j+1][k];
}
}
	
	
	
	
	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
				for(k=0;k<4;k++)
			{
                 nc2[i][j][k]=nc2[i][j][k]/count;
				}
		}
	}
//h=1;
for (i=0;i<4;i++)
	{
for (j=0;j<4;j++)
		{
	for(k=0;k<4;k++)
			{


				
		app[h]=nc2[i][j][k];
			  h++;
	}}
	
	}

return (app);

}

/*void writefile (float *app, int dim, int lable )
{

FILE *fp;
int i=0,h=1;
fp = fopen ("C:\\feature64upper.txt", "a+");
if (fp==NULL)  {printf ("file can't be opened");  }
//fprintf(fp,"%d",imgno);
//imgno++;
fprintf(fp,"%d ",lable);////////////specify the class tag here
for(i=0;i<dim;i++)
{
	if(app[i]==0)
	{
		h++;
		
	continue;
	}
	else
	{
fprintf (fp, "%d:%f ",h, *(app+i));

h++;
	}
}
fprintf(fp,"\n");
fclose (fp);
}
*/

float *dimension64featurelower(short **arr,int row,int col)
{
	int i,u,y1,j,t,s,r[10],c[10],y,z,m,ro,co,n,l=0,h8,h9,c1[300],c2[300],c3[300],c4[300],h1,h2,h3,h4,l1=0,l2=0,l3=0,l4=0 ;
	short **a,**b1;
	float count=0,*app;
	int b[7][7][4]={0};
	a =(short **) malloc(row * sizeof(short *));

 for (i = 0; i < row; i++) {
    a[i] = (short *)malloc(col * sizeof(short));
	}


 for(i=0;i<row;i++)
			   {
				   for(j=0;j<col;j++)
						{
					   
						a[i][j]=*(*(arr + i) + j);
				   }
				   
 }
 b1 =(short **) malloc(row * sizeof(short *));
 for (i = 0; i < row; i++) {
    b1[i] = (short *)malloc(col * sizeof(short));
	}

 for(i=0;i<row;i++)
			   {
				   for(j=0;j<col;j++)
						{
					   
						b1[i][j]=a[i][j];
				   }
				   
 }
 
for(i=0;i<300;i++)
{
c1[i]=999;
c2[i]=0;
c3[i]=999;
c4[i]=0;
}
u=col-1;
y1=row-1;
for(i=0;i<row;i++)
			   {
				   for(j=0;j<col;j++)
						
					   
						if(a[i][j]==1)
						{	c1[l1]=j;
						l1++;
						break;
				   }
}
h1=least64(c1);

for(i=0;i<row;i++)
			   {
				   for(j=u;j>=0;j--)
					   if(a[i][j]==1)
					   {	c2[l2]=j;
						l2++;
						break;
				   }
}
h2=greatest64(c2);
for(i=0;i<col;i++)
			   {
				   for(j=0;j<row;j++)
						
					   
						if(a[j][i]==1)
						{   c3[l3]=j;
						l3++;
						break;
				   }
}
h3=least64(c3);
for(i=0;i<col;i++)
			   {
				   for(j=y1;j>=0;j--)
						
					   
						if(a[j][i]==1)
						{	c4[l4]=j;
						l4++;
						break;
				   }
}
h4=greatest64(c4);
ro=h4-h3+1;
co=h2-h1+1;
//printf("%d-%d %d-%d\n",h3,h4,h1,h2);
/*for(i=h3;i<=h4;i++)
		{
			for(j=h1;j<=h2;j++)
				printf("%d",a[i][j]);
		printf("\n");
		}*/
division64(ro,r,7);//for row division
/*for(i=0;i<=6;i++)
printf("%d ",r[i]);
printf("\n");*/

division64(co,c,7);//for column division
/*for(i=0;i<=6;i++)
printf("%d ",c[i]);
printf("\n");*/
h8=h3+1;
h9=h1+1;
 for(i=h8;i<h4;i++)
			   {
				   for(j=h9;j<h2;j++)
						{

					   if(a[i][j]==0)
						   b1[i][j]=0;
						   
					   else 
							   if(a[i][j]==1){
						  if(a[i][j+1]==1&&a[i-1][j+1]==1&&a[i-1][j]==1&&a[i-1][j-1]==1&&a[i][j-1]==1&&a[i+1][j-1]==1&&a[i+1][j]==1&&a[i+1][j+1]==1)
							  b1[i][j]=1;
							   else b1[i][j]=3;}
						
				   }
			   }
 for(i=h3;i<=h4;i++)
 {
	 if(a[i][h1]==0)
		 b1[i][h1]=0;
	 else b1[i][h1]=3;
	 if(a[i][h2]==0)
		 b1[i][h2]=0;
	 else b1[i][h2]=3;
 }
 for(j=h1;j<=h2;j++)
 {
	 if(a[h3][j]==0)
		 b1[h3][j]=0;
	 else b1[h3][j]=3;
	 if(a[h4][j]==0)
		 b1[h4][j]=0;
	 else b1[h4][j]=3;
 }

			   
			   
			   
for(m=0;m<=6;m++)

   for(n=0;n<=6;n++)
   {
s=h1;
t=0;
y=h3;z=0;
for(i=0;i<m;i++)//for row
y=y+r[i];
z=y+r[m];
for(j=0;j<n;j++)//for colm
s=s+c[j];
t=s+c[n];
//printf("%d-%d %d-%d\n",z,y,t,s);
			   
			   
			   for(i=y;i<z;i++)
			   {
				   for(j=s;j<t;j++)
						{
			   if(b1[i][j]==0)
								continue;
			   else if(b1[i][j]==1) continue;

			   		else //if(b1[i][j]==3)
					{
							if((j+1)<t)
							{
									if(b1[i][j+1]==3)
									b[m][n][0]++;
							}

							if((i-1)>=y&&(j+1)<t)
							{
									
								 if(b1[i-1][j+1]==3)
									b[m][n][1]++;
							}
							if((i-1)>=y)
							{
									
								 if(b1[i-1][j]==3)
									 b[m][n][2]++;
							}
							if((i-1)>=y&&(j-1)>=s)
							{
					
								 if(b1[i-1][j-1]==3)
									b[m][n][3]++;
									
							}	
							}
				   }
			   }
			


	
	   
   }	
 
 
   for(i=h3;i<=h4;i++)
			   {
				   for(j=h1;j<=h2;j++)
						{
					   if(b1[i][j]==3)
						   count++;
				   }
   }

	app=reductionfrm49to16((&b[0][0][0]),7,7,4,count);
	return (app);
}
void writefilelower (float *app, int dim, int lable )
{

FILE *fp;
int i=0,h=1;
fp = fopen ("C:\\64tillendrow.txt", "a+");
if (fp==NULL)  {printf ("file can't be opened");  }
//fprintf(fp,"%d",imgno);
//imgno++;
fprintf(fp,"%d ",lable);////////////specify the class tag here
for(i=0;i<dim;i++)
{
	if(app[i]==0)
	{
		h++;
		
	continue;
	}
	else
	{
fprintf (fp, "%d:%f ",h, *(app+i));

h++;
	}
}
fprintf(fp,"\n");
fclose (fp);
}

void writefile400withouthead(float *app, int dim, int lable )
{

FILE *fp,*wrt;
int i=0,h=1;
fp = fopen ("C:\\HAND-print-400.txt", "a+");
//wrt=fopen("C:\\xxb.txt","a+");
if (fp==NULL)  {printf ("file can't be opened");  }
//fprintf(fp,"%d",imgno);
//imgno++;

//fprintf(wrt," \n %s         %d   \n",str1,wrod);
fprintf(fp,"%d ",lable);////////////specify the class tag here
for(i=0;i<dim;i++)
{
	if((app[i]==0))
	{
		h++;
		
	continue;
	}
	else
	{
fprintf (fp, "%d:%f ",i+1, *(app+i));

h++;
	}
}
fprintf(fp,"\n");
fclose (fp);
//fclose(wrt);
}
