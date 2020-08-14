
// Tiff.h: interface for the Tiff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIFF_H__611141EF_5BA4_46B3_917A_154ADA63BC82__INCLUDED_)
#define AFX_TIFF_H__611141EF_5BA4_46B3_917A_154ADA63BC82__INCLUDED_
#include"image_info.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Tiff  
{
private:
	Head image_header ;
  public:
	  void create_file_if_needed_un_char(char *f_name,unsigned char **line_array,int length,int width);
	  short**	save_file(char f_name[], short **image, int length, int width);
	  short	**write_file(int flag, short **image, int length, int width);
	  int does_not_exist(char file_name[]);
	  void round_off_image_size(struct tiff_header_struct *image_header, int *length, int *width);
	  void insert_long_into_buffer(unsigned char buffer[], int start,long number);
	  void insert_short_into_buffer(unsigned char buffer[], int start, short number);
	  int  write_line(FILE *image_file, short **array, int line_number, long image_width, long bits_per_pixel, int ie, int le);
	  void write_array_into_tiff_image(char image_file_name[], short **array,long image_length, long image_width, long bits_per_pixel );
	  void create_allocate_tiff_file(char file_name[],long image_length,long image_width, long bits_per_pixel,short** image);
	  void extract_long_from_buffer(char buffer[], int lsb, int start, long *number);
	  int seek_to_first_line(FILE *image_file, int width, int il); 
	  void extract_short_from_buffer(char buffer[], int lsb, int start, short *number);
      int read_line(FILE *image_file, short **array, int line_number, int ie, int le); 
	  void read_tiff_header(char file_name[]);
	  short** read_tiff_image(char image_file_name[], int *length, int *width);
      int seek_to_end_of_line(FILE *image_file, int le, int width);
	  void create_file_if_needed( char out_name[], short **out_image,long image_length,long image_width, long bits_per_pixel);

	Tiff();
	virtual ~Tiff();

};

#endif // !defined(AFX_TIFF_H__611141EF_5BA4_46B3_917A_154ADA63BC82__INCLUDED_)

// Tiff.h: interface for the Tiff class.
//
//////////////////////////////////////////////////////////////////////

