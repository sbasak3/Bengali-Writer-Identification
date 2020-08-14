#ifndef compo_h
#define compo_h

#include "image_info.h"
#include "script.h"

#ifndef	MyStack_defined
typedef struct MyStack
    {
	int top;
	int *i, *j;
    }stk;

#define MyStack_defined
#endif

class compo
{

  public:
	Comp_list* comAnalysis(int max, int col,short **binary,int min,int length,int width);
	Comp_measure Label(int i1, int j1, int l,short **array,int min,int max,int width);
	void Free_list2(short **head,int length);
	void Init(stk *vertex);
	void  Stack_del(stk *vertex);
	int IsEmpty(stk *vertex);
	void push(int p, int q,stk *vertex);
	void pop(stk *vertex);
	int peepi(stk *vertex) ;
	int peepj(stk *vertex);
	int Size(stk *vertex);


};

#endif