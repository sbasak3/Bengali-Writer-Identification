//********************************************
 /*****binarization of the image************/
//********************************************





class binarize
{
public:
	short **binarization(short **image, int length, int width);
    void write_back_file(char *out_name,short **b,int length, int width);
	int thres_val_finding(int size_r, int size_c, short **z);
    void Free(short **head,int length);

};