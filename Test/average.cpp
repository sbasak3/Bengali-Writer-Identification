#include  "image_info.h"
#include "script.h"


int Length( Comp_list *Result )
 {
   int Comp_length , Comp_width ;
   int Sum_length = 0 ;
   int Avg_length ;
   int s ;
   int j = 0;
   
   //printf("\n\n AVERAGE LENGTH *******************\n");

   for(s=0;Result!=NULL;Result=Result->next,s++)
      {
        Comp_length=Result->Comp_data.maxRow - Result->Comp_data.minRow + 3;
        Comp_width =Result->Comp_data.maxCol - Result->Comp_data.minCol + 3;
                    
        Sum_length = Sum_length  + Comp_length ; 
      }
      
     Avg_length = Sum_length / s ; 
     
    // printf("\n Avg Length = %d",Avg_length);
     
     return( Avg_length );
 }    
