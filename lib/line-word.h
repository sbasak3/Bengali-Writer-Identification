typedef struct Line_list
     {
       int max;
       int min;
       int avg;
       struct Line_list *next;
     }ln;  
typedef struct Result
     {
        int Line_start;
        int Line_end;
        int Boundary_start;
        int Boundary_end;
        int Line_width;
        int Line_boundary;
        int decision_flag ;
        int valid_line    ;       
        struct Result *next;
     }ln_width; 
     
typedef struct Word
     {
        int word_start    ;
        int word_end      ;
        int word_valid    ;
        int word_split    ;
        struct Word *next ;
     }word_seg ;
