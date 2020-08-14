#include <stdio.h>
#include <malloc.h>
#include <math.h>
int least(int c[300]);

double sqrt(double);
double atan2(double,double);
int greatest(int d[300]);
void division(int,int a[20],float);


float *dimension400feature(short **image,int row,int col)
{

	int i,j,k,count=0,m,n,rxy,r11[20],s1,t,y,z,ro,co,i1,j1,c11[20],c1[2300],c2[2300],c3[2300],c4[2300],u,y1,h1,h2,h3,h4,l1=0,l2=0,l3=0,l4=0,h11;
	float d[9][9][16]={0},nc1[11][11][16]={0},counter=0,nc2[5][5][16]={0},delv,delu,fxy,angxy,max,total=0,mean;
	float **img,**img1,*appe,s;
	appe=(float *)malloc(400*sizeof(float));

	img =(float **) malloc(row * sizeof(float *));

	for (i = 0; i < row; i++) {
		img[i] = (float *)malloc(col * sizeof(float));
	}
	
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			img[i][j]=(float)image[i][j];
		 }
	}
	
	for(i=0;i<2300;i++){
		c1[i]=999;
		c2[i]=0;
		c3[i]=999;
		c4[i]=0;
	}
	u=col-1;
	y1=row-1;

	for(i=0;i<row;i++){
		for(j=0;j<col;j++)
			if(img[i][j]==1)
			{	
				c1[l1]=j;
				l1++;
				break;
			}
	}
	h1=least(c1);

	for(i=0;i<row;i++){
		for(j=u;j>=0;j--)
			if(img[i][j]==1)
			{	
				c2[l2]=j;
				l2++;
				break;
			}
	}
	h2=greatest(c2);

	for(i=0;i<col;i++){
		for(j=0;j<row;j++)
			if(img[j][i]==1)
			{
				c3[l3]=j;
				l3++;
				break;
			}
	}
	h3=least(c3);

	for(i=0;i<col;i++){
		for(j=y1;j>=0;j--)
			if(img[j][i]==1)
			{	
				c4[l4]=j;
				l4++;
				break;
			}
	}
	h4=greatest(c4);
	ro=h4-h3+1;
	co=h2-h1+1;

	for(i=h3;i<=h4;i++){
		for (j=h1;j<=h2;j++){
			if(img[i][j]==1)
				counter++;
			}
		}

	for(k=1;k<=5;k++){
		for(i=h3;i<=h4;i++){
			for (j=h1;j<=h2;j++){
				s=0;
				s=s+img[i][j];
				if (i>=h3 && i<=h4 && j+1>=h1 && j+1<=h2)
					s=s+img[i][j+1];
				if (i+1>=h3 && i+1<=h4 && j>=h1 && j<=h2)
					s=s+img[i+1][j];
				if (i+1>=h3 && i+1<=h4 && j+1>=h1 && j+1<=h2)
					s=s+ img[i+1][j+1];
				img[i][j]=s;
			}
		}
	}


	for(i=h3;i<=h4;i++){
		for(j=h1;j<=h2;j++){
			total=total+img[i][j];
		}
	}
	count=ro*co;
	mean=total/count;
//	printf("===%f===",mean);

	for(i=h3;i<=h4;i++){
		for(j=h1;j<=h2;j++){
			img[i][j]=img[i][j]-mean;
		}
	}
		

	max=img[h3][h1];
	for(i=h3;i<=h4;i++){
		for(j=h1;j<=h2;j++){
			if(max<img[i][j])
				max=img[i][j];
			}
		}
//printf("```%f```",max);

	for(i=h3;i<=h4;i++){
		for(j=h1;j<=h2;j++){
			img[i][j]=img[i][j]/max;
		}
	}
	
	
	division(ro,r11,9);
	division(co,c11,9);
	
	img1 =(float **) malloc((ro+1) * sizeof(float *));

	for (i = 0; i < (ro+1); i++) {
		img1[i] = (float *)malloc((co+1) * sizeof(float));
	}
 

	for(i=h3,i1=0;i<=h4,i1<ro;i++,i1++){
		for(j=h1,j1=0;j<=h2,j1<co;j++,j1++){
			img1[i1][j1]=img[i][j];
		}
	}	

	for(i=0;i<ro;i++)
		img1[i][co]=(img1[i][co-1]+ img1[i][co-2])/2;

	for(j=0;j<co;j++)
		img1[ro][j]=(img1[ro-1][j]+ img1[ro-2][j])/2;

	img1[ro][co]=(img1[ro-1][co-1]+img1[ro-2][co-2])/2;


	for(m=0;m<9;m++)
	for(n=0;n<9;n++){
			s1=0;
			t=0;
			y=0;z=0;
			for(i=0;i<m;i++)//for row
				y=y+r11[i];

			z=y+r11[m];
			for(j=0;j<n;j++)//for colm
				s1=s1+c11[j];

			t=s1+c11[n];

			for(i=y;i<z;i++){
				for(j=s1;j<t;j++){
					fxy=0;
					angxy=0;
					delu=0;
					delv=0;
					delu=img1[i+1][j+1]-img1[i][j];/// this should be [i+1][j]-[i][j]
			 
					delv=img1[i+1][j]-img1[i][j+1];//////  this should be [i][j+1]-[i][j];
					if(delu==0)
						continue; 
					else{
						fxy=sqrt((delu*delu)+(delv*delv));
			 
						angxy=(atan2(delv,delu)*180)/3.142857;
						angxy=angxy+180;
			 
						rxy=angxy/22.5;
			
						d[m][n][rxy]=d[m][n][rxy]+fxy;}
					}
				}
			}//end for i=y


		for(i=0;i<9;)
		{
			for(j=0;j<9;)
			{
				for(k=0;k<16;k++)
				{
					d[i][j][k]=d[i][j][k]*2;
						
				}j=j+2;
			}
			i=i+2;
		}

		for(i=1;i<10;i++)
		{
			for(j=1;j<10;j++)
			{
				for(k=0;k<16;k++)
					nc1[i][j][k]=d[i-1][j-1][k];
			}
		}
		for (i=1;i<10;i+=2)     
		{
			for (j=1;j<10;j+=2)
			{
				for(k=0;k<16;k++)
				nc2[i/2][j/2][k]=nc1[i][j][k]+nc1[i-1][j-1][k]+nc1[i-1][j][k]+nc1[i-1][j+1][k]+nc1[i][j-1][k]+nc1[i][j+1][k]+nc1[i+1][j-1][k]+nc1[i+1][j][k]+nc1[i+1][j+1][k];
			}
		}

		h11=0;
		for(i=0;i<5;i++)
			for(j=0;j<5;j++)
				for(k=0;k<16;k++)
				{
					appe[h11]=nc2[i][j][k]/counter;
					h11++;
				}

		for (i = 0; i < row; i++)
			free(img[i]);
		for (i = 0; i < (ro+1); i++) 
			free(img1[i]);

		return (appe);

	}//end for(n=0;n<9;n++)




int least(int c[300])
{
	int i,m;
	m=c[0];
	for(i=1;i<300;i++)
		if(c[i]<m)
			m=c[i];
		return m;
}


int greatest(int d[300])
{
	int i,m=0;
     	 for(i=0;i<300;i++)
		 if(d[i]>m)
			 m=d[i];
		 return m;
}

void writefile (float *app, int dim, int lable )
{

FILE *fp;
int i=0,h=1;
fp = fopen ("C:\\bangla-character\\feature400.txt", "a");
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

void division(int k,int a[10],float dim)
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