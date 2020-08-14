typedef struct iso_char
{
	int  pos_i;
	int  pos_j;
	struct iso_char *next;
	
}ichar;



struct Character
{
	int startcolumn;
	int stopcolumn;
	int startrow;
	int endrow;
	int lineindex;
	int wordindex;
	ichar *pixels;
	struct Character *next;
};

typedef struct Character character;



struct word
{
	int start;
	int stop;
	char type;
	char condition;
	struct word *next;
	struct word * previous;
};

typedef struct word wrd;

struct line
{
	int startline;
	int endline;
	struct lie * next;
};

//typedef struct line  ln;

typedef struct Comp_link
 {
   int	x , y ;
   int	direction ;
   char	st_pt;
   struct Comp_link *next;
 }c_node ;

struct Pitch
{
	int block_start;
	int block_stop;
	struct Pitch *next;
};

typedef struct Pitch pitch;