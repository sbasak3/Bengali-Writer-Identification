#if !defined(script_h)
#define script_h
//#include "compo.h"

typedef struct Stored
    {
       int x;
       int y;
    }st_st;

typedef struct Pair
    {
       st_st Left;
       st_st Right;
       int mark;
    }within;

typedef struct Scan
    {
       st_st Rightscan;
       st_st Leftscan;
    }scn;              
    
typedef struct Reservoir
    {
      char flow;  /*  top Flow of the Reservoir */
      int  height; /*  Height of the Reservoir */
      int  Maximum_level; /* Row at which flow occurs from the Reservoir */
      int  Bheight;  /* The height of the Bounding Box of the Reservoir */
      int  width;    /*  The width of the Reservoir */           
      int  Minimum_col;
      int  Bwidth;   /*  Width of the Bounding Box of the Reservoir */
      int  area;
      char Status;
      int  bottom_column ; 
      int  top_column   ;
      int left_column   ;
      int right_column  ;
      int  xpos         ;
    }Res_det;
        
 

typedef struct Information
    { 
      int  xmin;
      int  xmax;
      int  xpos;
      int  ymin;
      int  ymax;
      char state;
      int Area;
      int top_column ;
      int left_column;
      int right_column; 
    }info;
    
typedef struct F_Control
    {
      int x1;
      int y1;
      int x2;
      int y2;
    }F_Info;                    
 
typedef struct Reservoir_mstr
   {
     int maximum;
     char res_flow;
     int max_height;
     int mincol;
     int max_width;
     int res_area;
     int bottom_column ;
     int top_column ;          
     int left_column  ;
     int right_column ;
     int xpos       ; 
   }C_mstr;
 
typedef struct Component_Measure
{
     int minRow;
     int maxRow;
     int minCol;
     int maxCol;
     short **Comp_array;
   }Comp_measure;
     
typedef struct Component_size
   {
      int minR;     
      int maxR;
      int minC;
      int maxC;
      C_mstr *Comp_res;
      int Total_no;
   }Comp_info;   
   
typedef struct Component_Line
   {
       Comp_measure Comp_data;
       struct Component_Line *next;
   }Comp_list;       

        
typedef struct Component_flow
   {
       C_mstr *Reservoir_height;
       int     Component_no;
   }C_flow;
   

typedef struct Line_Pixel
  {
    int Pixel_height;   
    int Pixel_width;
  }Pix;
    
                
typedef struct Bangla
  {
    int equispace;
    struct Bangla *next;
  }bangla_line;
               

typedef struct Matra
  {
    int Top_Matra      ;
    int Bottom_Matra   ;
    int Matra_width    ;
    int Matra_row      ;
    struct Matra *next ;                     
  }bangla_matra ;  


typedef struct Comp_convexity
 {
     int convex_row ;
     char status    ;
     struct Comp_convexity *next ;
 }convex_node;
          
#endif              