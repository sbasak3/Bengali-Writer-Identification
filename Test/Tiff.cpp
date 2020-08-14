
// Tiff.cpp: implementation of the Tiff class.
//
//////////////////////////////////////////////////////////////////////

#include "Tiff.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Tiff::Tiff()
{

}

Tiff::~Tiff()
{

}

void Tiff::extract_long_from_buffer(char buffer[], int lsb, int start, long *number)
{
   int i;
   union long_char_union lcu;

   if(lsb == 1){
      lcu.l_alpha[0] = buffer[start+0];
      lcu.l_alpha[1] = buffer[start+1];
      lcu.l_alpha[2] = buffer[start+2];
      lcu.l_alpha[3] = buffer[start+3];
   }  /* ends if lsb = 1 */

   if(lsb == 0){
      lcu.l_alpha[0] = buffer[start+3];
      lcu.l_alpha[1] = buffer[start+2];
      lcu.l_alpha[2] = buffer[start+1];
      lcu.l_alpha[3] = buffer[start+0];
   }  /* ends if lsb = 0      */

   *number = lcu.l_num;
}

void Tiff::extract_short_from_buffer(char buffer[], int lsb, int start, short *number)
{
    int i;
   union short_char_union lcu;

   if(lsb == 1){
      lcu.s_alpha[0] = buffer[start+0];
      lcu.s_alpha[1] = buffer[start+1];
   }  /* ends if lsb = 1 */

   if(lsb == 0){
      lcu.s_alpha[0] = buffer[start+1];
      lcu.s_alpha[1] = buffer[start+0];
   }  /* ends if lsb = 0      */

   *number = lcu.s_num;
}



int Tiff::read_line(FILE *image_file, short **array, int line_number, int ie, int le)
{
    char  *buffer, first, second;
   float a, b;
   int bytes_read, i;
   unsigned int bytes_to_read;
   union short_char_union scu;

   buffer=(char*)malloc(image_header.image_width*sizeof(char));
   for(i=0; i<image_header.image_width; i++)
      buffer[i] = '\0';

        /********************************************
        *
        *   Use the number of bits per pixel to 
        *   calculate how many bytes to read.
        *
        ********************************************/

   bytes_to_read = (le-ie)/
                   (8/image_header.bits_per_pixel);
   bytes_read    = fread(buffer, 1, bytes_to_read, 
                         image_file);

   for(i=0; i<bytes_read; i++){

        /*********************************************
        *
        *   Use unions defined in cips.h to stuff bytes
        *   into shorts.
        *
        **********************************************/

      if(image_header.bits_per_pixel == 8){
       scu.s_num          = 0;
       scu.s_alpha[0]        = buffer[i];
       array[line_number][i] = scu.s_num;
      }  /* ends if bits_per_pixel == 8 */

      if(image_header.bits_per_pixel == 4){

       scu.s_num             = 0;
       second                = buffer[i] & 0X000F;
       scu.s_alpha[0]        = second;
       array[line_number][i*2+1] = scu.s_num;

       scu.s_num             = 0;
       first                 = buffer[i] >> 4;
       first                 = first & 0x000F;
       scu.s_alpha[0]        = first;
       array[line_number][i*2] = scu.s_num;

      }  /* ends if bits_per_pixel == 4 */

   }  /*  ends loop over i  */

   free(buffer);
   return(bytes_read);
}



void Tiff::read_tiff_header(char file_name[])
{
   char buffer[12], response[80];

   FILE *image_file;

   int  bytes_read,
        closed,
        i,
        j,
        lsb,
        not_finished,
        w,
        position;

   long bits_per_pixel,
        image_length,
        image_width,
        file_size,
        image_size,
        offset_to_ifd,
        strip_offset,
        subfile,
        value,
		location;

   short entry_count,
         field_type,
         length_of_field,
         s_bits_per_pixel,
         s_image_length,
         s_image_width,
         s_strip_offset,
         tag_type;

   image_file = fopen(file_name, "rb");
   if(image_file != NULL){

        /*************************************
        *
        *   Determine if the file uses MSB
        *   first or LSB first
        *
        *************************************/

   bytes_read = fread(buffer, 1, 8, image_file);

   if(buffer[0] == 0x49 && buffer[1] == 0x49)
      lsb = 1;
   else if(buffer[0] == 0x4D && buffer[1] == 0x4D)
      lsb = 0;

        /*************************************
        *
        *   Read the offset to the IFD
        *
        *************************************/

   extract_long_from_buffer(buffer, lsb, 4, 
                            &offset_to_ifd);

   not_finished = 1;
   while(not_finished){

        /*************************************
        *
        *   Seek to the IFD and read the
        *   entry_count, i.e. the number of
        *   entries in the IFD.
        *
        *************************************/

      position   = fseek(image_file, offset_to_ifd, 
                         SEEK_SET);
      bytes_read = fread(buffer, 1, 2, image_file);
      extract_short_from_buffer(buffer, lsb, 0, 
                                &entry_count);

        /***************************************
        *
        *   Now loop over the directory entries.
        *   Look only for the tags we need.  These
        *   are:
        *     ImageLength
        *     ImageWidth
        *     BitsPerPixel(BitsPerSample)
        *     StripOffset
        *
        *****************************************/
		
	 
      for(i=0; i<entry_count; i++){
       bytes_read = fread(buffer, 1, 12, image_file);
       extract_short_from_buffer(buffer, lsb, 0, &tag_type);

       switch(tag_type){

          case 255: /* Subfile Type */
             extract_short_from_buffer(buffer, lsb, 2,
                                       &field_type);
             extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
             extract_long_from_buffer(buffer, lsb, 8, 
                                      &subfile);
             break;

          case 256: /* ImageWidth */
             extract_short_from_buffer(buffer, lsb, 2, 
                                       &field_type);
             extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
             if(field_type == 3){
              extract_short_from_buffer(buffer, lsb, 8,
                                     &s_image_width);
              image_width = s_image_width;
             }
             else
              extract_long_from_buffer(buffer, lsb, 8, 
                                       &image_width);
             break;

          case 257: /* ImageLength */
             extract_short_from_buffer(buffer, lsb, 2, 
                                       &field_type);
             extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
             if(field_type == 3){
              extract_short_from_buffer(buffer, lsb, 8,
                                    &s_image_length);
              image_length = s_image_length;
             }
             else
              extract_long_from_buffer(buffer, lsb, 8,
                                       &image_length);
             break;

          case 258: /* BitsPerSample */
             extract_short_from_buffer(buffer, lsb, 2, 
                                       &field_type);
             extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
             if(field_type == 3){
              extract_short_from_buffer(buffer, lsb, 8,
                                   &s_bits_per_pixel);
              bits_per_pixel = s_bits_per_pixel;
             }
             else
              extract_long_from_buffer(buffer, lsb, 8,
                                    &bits_per_pixel);
            /* printf("\n**********  Bit  Per  Pixel  : %ld",bits_per_pixel);
              
             scanf("%d",&w);                       */
             break;

          case 273: /* StripOffset */

             location = ftell(image_file);
			 
             extract_short_from_buffer(buffer, lsb, 2, 
                                       &field_type);
			 //printf("\n\n\t\tType = %d\n",field_type);
			 
			 extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
//			 printf("\n\n\t\tCount = %d\n",length_of_field);
			 
			 if(field_type == 3)
			 {

			  extract_short_from_buffer(buffer, lsb, 8,
                                    &s_strip_offset);
              strip_offset = s_strip_offset;
             }
             else 
			 {
				 extract_long_from_buffer(buffer, lsb, 8,
                                       &strip_offset);
				 
				 if(length_of_field != 1)
				 {
					fseek(image_file,strip_offset,SEEK_SET);
					bytes_read = fread(buffer, 1, 4, image_file);
					extract_long_from_buffer(buffer, lsb, 0, &strip_offset);
					printf("\n\n\t\tThe strip offset is = %d \n",strip_offset);
				 }
				 fseek(image_file,location,SEEK_SET);
			 }
			   
			 break;

          default:
             break;

       }  /* ends switch tag_type */

      }  /* ends loop over i directory entries */

      bytes_read = fread(buffer, 1, 4, image_file);
      extract_long_from_buffer(buffer, lsb, 0, 
                               &offset_to_ifd);
      if(offset_to_ifd == 0)
        not_finished = 0;

   }  /* ends while not_finished */


   image_header.lsb                = lsb;
   image_header.bits_per_pixel     = bits_per_pixel;
   image_header.image_length       = image_length;
   image_header.image_width        = image_width;
   image_header.strip_offset       = strip_offset;
      
   fseek(image_file,0,SEEK_END);

   file_size=ftell(image_file);

   image_size=image_length*image_width;

   //printf("File Size =%d, Image Size =%d\n",file_size,image_size);

  
   if(image_header.strip_offset<0)

    {

      printf("\n Uncompress The Image First");

      exit(1);

     } 

 /*  printf("\nStrip offset is : %d",image_header->strip_offset);*/
   closed = fclose(image_file);
   }  /* ends if file opened ok */
   else{
      printf("\n\nTIFF.C> ERROR - could not open "
             "tiff file");
   }
}

short** Tiff::read_tiff_image(char image_file_name[], int *length, int *width)
{
   char  rep[80];
   int   bytes_read,
         closed,
         position,
         i;
   
   int il=1,ie=1,ll,le;

   FILE  *image_file;
   float a;
   long  line_length, offset;

   //Head image_header;
   short ** array;

   read_tiff_header(image_file_name);
  
   ll = il + image_header.image_length ;
   le = ie + image_header.image_width ;
   
   *length = image_header.image_length ;
   *width  = image_header.image_width ; 

   array = (short **)malloc(image_header.image_length * sizeof(short *));
	for(i=0;i<image_header.image_length;i++)
		array[i]=(short *)malloc(image_header.image_width * sizeof(short));


      /***********************************************
      *
      *   Procedure:
      *   Seek to the strip offset where the data begins.
      *   Seek to the first line you want.
      *   Loop over the lines you want to read:
      *      Seek to the first element of the line.
      *      Read the line.
      *      Seek to the end of the data in that line.
      *
      ************************************************/

   image_file = fopen(image_file_name, "rb");
   if(image_file != NULL){
      position = fseek(image_file,image_header.strip_offset,SEEK_SET);
      
      position = seek_to_first_line(image_file,image_header.image_width , il);

      
      for(i=0; i<(ll-il); i++){
         offset       = (ie-1)/
                        (8/image_header.bits_per_pixel);
         position     = fseek(image_file, offset,SEEK_CUR);
         bytes_read   = read_line(image_file, array,i,ie, le);
         position     = seek_to_end_of_line(image_file,le, image_header.image_width);
         position     = fseek(image_file, 1,SEEK_CUR); 
      }  /* ends loop over i  */

    
      closed = fclose(image_file);
   }  /* ends if file opened ok */
   else{
      printf("\nRTIFF.C> ERROR - cannot open "
             "tiff file");
   }

  return(array);
}


int Tiff::seek_to_end_of_line(FILE *image_file, int le, int width)
{
   long  offset;
   int   position;

   offset   = (width-le)/
             (8/8);
   position = fseek(image_file, offset, SEEK_CUR);
   return(position);
}

int Tiff::seek_to_first_line(FILE *image_file, int width, int il)
{
  long offset;
   int  position;

/*   printf("Bit per Pixel is : %ld",image_header->bits_per_pixel);*/

  
   offset   = (il-1)*width/
             (8/8);
      /* seek from current position */
  
   position = fseek(image_file, offset, SEEK_CUR);
   return(position);
}


void Tiff::create_file_if_needed( char out_name[], short **out_image,long image_length,long image_width, long bits_per_pixel)

{
   int    length, width;
   struct tiff_header_struct image_header;
   char ans ;

   /*if(does_not_exist(out_name))
     {
        printf("\n\n output file does not exist %s",
               out_name);*/
           
        create_allocate_tiff_file(out_name, image_length,
 image_width, bits_per_pixel, out_image);
                                
        write_array_into_tiff_image(out_name,
out_image, image_length,
 image_width , bits_per_pixel);
                         
        //printf("\n\n File Created ......\n\n");                  
                                                 
     /*}   ends if does_not_exist */
   /*else
     {
        printf("\n\n %s :: This File already exists , Do you want to replace it ?( y/n )\n\n",
               out_name);
        fflush(stdin);
        
        scanf("%c",&ans);
        
        if( ans == 'y' || ans == 'Y' )
         {
                      
           create_allocate_tiff_file(out_name, image_length,
 image_width, bits_per_pixel, out_image);
                                  
           write_array_into_tiff_image(out_name,
out_image, image_length,
 image_width , bits_per_pixel );
                       
           printf("\n\n File Created ......\n\n"); 
         }
     }*/          
                 
   
}

void Tiff::create_allocate_tiff_file(char file_name[], long image_length, long image_width, long bits_per_pixel, short **image)
{
     unsigned char  buffer[12], end_buf ;
   FILE  *image_file;
   int   bytes_written,
         i,
         j,
         l,
         w;

   long  k;
   char *long_buffer ; 

   int pos , q ;

   int end_size ;

   int diff;   

   

      /***************************************
      *
      *   Create the image file in binary mode
      *   for both reading and writing.
      *
      ****************************************/

   long_buffer = new char[50];
   
   image_file = fopen(file_name, "wb");

   
      /***************************************
      *
      *   Write out the first 8 bytes of the
      *   header.  The meaning of the
      *   bytes (HEX) is:
      *      0-1 = 49 49 - LSB first
      *      2-3 = 2A 00 - version #
      *      4-7 = 08 00 00 00 - go to offset
      *           8 for the first
      *           Image File
      *           Directory
      *
      ****************************************/

   
   buffer[0] = 0x49;
   buffer[1] = 0x49;
   buffer[2] = 0x2A;
   buffer[3] = 0x00;
   buffer[4] = 0x08;
   buffer[5] = 0x00;
   buffer[6] = 0x00;
   buffer[7] = 0x00;

   
   bytes_written = fwrite(buffer, 1, 8, image_file);

   
   //printf("\n wrote %d bytes", bytes_written);

   
      /***************************************
      *
      *   Write out the first 2 bytes of the
      *   Image File Directory.  These tell
      *   the number of entries in the IFD.
      *
      ****************************************/

   
   buffer[0] = 0x12;
   buffer[1] = 0x00;
   bytes_written = fwrite(buffer, 1, 2, image_file);

   
   //printf("\n wrote %d bytes", bytes_written);

      /***************************************
      *
      *   Write out the entries into the
      *   Image File Directory.
      *
      ****************************************/


      /* New Subfile Type */
   buffer[0]  = 0xFE;
   buffer[1]  = 0x00;
   buffer[2]  = 0x03;
   buffer[3]  = 0x00;
   buffer[4]  = 0x01;
   buffer[5]  = 0x00;
   buffer[6]  = 0x00;
   buffer[7]  = 0x00;
   buffer[8]  = 0x00;
   buffer[9]  = 0x00;
   buffer[10] = 0x00;
   buffer[11] = 0x00;
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


   
      /* Subfile Type */
   buffer[0]  = 0xFF;
   buffer[1]  = 0x00;
   buffer[2]  = 0x03;
   buffer[3]  = 0x00;
   buffer[4]  = 0x01;
   buffer[5]  = 0x00;
   buffer[6]  = 0x00;
   buffer[7]  = 0x00;
   buffer[8]  = 0x01;
   buffer[9]  = 0x00;
   buffer[10] = 0x00;
   buffer[11] = 0x00;
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Image Width */
   insert_short_into_buffer(buffer, 0, 256);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 
                     image_width);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Image Length */
   insert_short_into_buffer(buffer, 0, 257);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 
                     image_length);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Bits Per Sample */
   insert_short_into_buffer(buffer, 0, 258);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 
                     bits_per_pixel);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Compression - None */
   insert_short_into_buffer(buffer, 0, 259);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 1);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Photometric Interpretation */
      /* set to 1 because BLACK is ZERO */
   insert_short_into_buffer(buffer, 0, 262);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 1);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Strip Offset */
      /* start after software name at 296 */
   insert_short_into_buffer(buffer, 0, 273);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8,296 );

   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Samples per Pixel */
   insert_short_into_buffer(buffer, 0, 277);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 1);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* clear buffer */
   for(i=0; i<12; i++) buffer[i] = 0x00;

      /* Rows Per Strip 1 strip for the entire image */
      /* use 2E32 - 1, which is max */
   insert_short_into_buffer(buffer, 0, 278);
   insert_short_into_buffer(buffer, 2, 4);
   insert_short_into_buffer(buffer, 4, 1);
   insert_long_into_buffer(buffer, 8, 4294967295);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Strip Byte Counts */
      /* this = image width times length */
   insert_short_into_buffer(buffer, 0, 279);
   insert_short_into_buffer(buffer, 2, 4);
   insert_short_into_buffer(buffer, 4, 1);
   insert_long_into_buffer(buffer, 8, 
    (long)(image_length *
           image_width));
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Min Sample Value */
   insert_short_into_buffer(buffer, 0, 280);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 0);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Max Sample Value */
   insert_short_into_buffer(buffer, 0, 281);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   if(bits_per_pixel == 8)
      insert_short_into_buffer(buffer, 8, 255);
   else
      insert_short_into_buffer(buffer, 8, 15);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* X Resolution */
      /* Store the 8 bytes for this value
         starting at 230 */
   insert_short_into_buffer(buffer, 0, 282);
   insert_short_into_buffer(buffer, 2, 5);
   insert_long_into_buffer(buffer, 4, 1);
   insert_long_into_buffer(buffer, 8, 230);
   
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Y Resolution */
      /* Store the 8 bytes for this value
         starting at 238 */
   insert_short_into_buffer(buffer, 0, 283);
   insert_short_into_buffer(buffer, 2, 5);
   insert_long_into_buffer(buffer, 4, 1);
   insert_long_into_buffer(buffer, 8, 238);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


  

     /* clear buffer */
   for(i=0; i<12; i++) buffer[i] = 0x00;

      /* Planer Configuration */
      /* chunky */
   insert_short_into_buffer(buffer, 0, 284);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 1);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Resolution Unit */
      /* inches */
   insert_short_into_buffer(buffer, 0, 296);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 2);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Software */
      /* Put this a 246, 50 bytes */
   insert_short_into_buffer(buffer, 0, 305);
   insert_short_into_buffer(buffer, 2, 2);
   insert_short_into_buffer(buffer, 4, 50);
   insert_short_into_buffer(buffer, 8, 246);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Offset to next IFD (0 means no more IFD's) */
   for(i=0; i<12; i++)
    buffer[i] = 0x00;
   bytes_written = fwrite(buffer, 1, 4, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* clear buffer */
   for(i=0; i<12; i++) 
    buffer[i] = 0x00;

    
      /* Now store the X Resolution
         first long is numerator
         second long is denominator */
   insert_long_into_buffer(buffer, 0, 300L);
   insert_long_into_buffer(buffer, 4, 1L);
   bytes_written = fwrite(buffer, 1, 8, image_file);
   //printf("\n wrote %d bytes", bytes_written);


      /* Now store the Y Resolution
         first long is numerator
         second long is denominator */
   insert_long_into_buffer(buffer, 0, 300L);
   insert_long_into_buffer(buffer, 4, 1L);
   bytes_written = fwrite(buffer, 1, 8, image_file);
   //printf("\n wrote %d bytes", bytes_written);


 
    
      /* Now store the software tag */
   for(i=0; i<50; i++) long_buffer[i] = '\0';
   strcpy(long_buffer,
   "C Image Processing System ");
   long_buffer[46] = '\0';
   long_buffer[47] = '\0';
   long_buffer[48] = '\0';
   long_buffer[49] = '\0';
   bytes_written = fwrite(long_buffer, 1, 50, 
                          image_file);
   //printf("\n wrote %d bytes", bytes_written);
   //printf("\n%s", long_buffer);

   

   

   end_size = ftell(image_file);
   
   //printf("\n Data starts from : %d Byte\n",end_size); 
   

   
      /***************************************
      *
      *   Now write the image data.
      *
      ****************************************/



   /*k = 1; image_length * image_width;
*/

  /* if(bits_per_pixel == 8)
      k = k/2;
   else
      k = k/4;
   k++;
*/

   

    
   /*for(i=0; i<image_length; i++)
      for(j=0; j<image_width; j++)
        image[i][j] = 255;
*/

/*   j = sizeof(short) *image_length * image_width;
 */

  fclose(image_file);
}



void Tiff::write_array_into_tiff_image(char image_file_name[], short **array, long image_length, long image_width, long bits_per_pixel)
{
  char  *buffer;
   FILE  *image_file;
   int   bytes_written,
         closed,
         i,
          position,
         ll, le, il, ie,  
         written;

   float a;

   long  line_length,
         offset;

    

      /***********************************************
      *
      *   Procedure:
      *   Seek to the strip offset where the data begins.
      *   Seek to the first line you want.
      *   Loop over the lines you want to write.
      *      Seek to the first element of the line.
      *      Write the line.
      *      Seek to the end of the data in that line.
      *
      ************************************************/

   
   il = 1 ;
   
   ie = 1 ;  
  
   ll = 1 + image_length ;
   
   le = 1 + image_width  ;
   
   buffer=(char*)malloc(image_width*sizeof(char));
   image_file = fopen(image_file_name, "r+b");
   position   = fseek(image_file,
 296, SEEK_SET);
   
   position   = seek_to_first_line(image_file,
   image_width,il);

   for(i=0; i<(ll-il); i++){
      offset        = (ie-1)/
(8/bits_per_pixel);
      
      position      = fseek(image_file, offset, 
SEEK_CUR);
      
      bytes_written = write_line(image_file, array,i, image_width, bits_per_pixel, ie, le);
      
      position      = seek_to_end_of_line(image_file,
 le, image_width);
      
      position      = fseek(image_file, 1, SEEK_CUR);
      
   }  /* ends loop over i  */

   closed = fclose(image_file);
}



int Tiff::write_line(FILE *image_file, short **array, int line_number, long image_width, long bits_per_pixel, int ie, int le)
{
     char     *buffer, first, second;
   float    a;
   int      bytes_written, i;
   unsigned int bytes_to_write;
   union    short_char_union scu;

    

   buffer=(char*)malloc(image_width*sizeof(char)); 
   for(i=0; i<image_width; i++)
      buffer[i] = '\0';

   bytes_to_write = (le-ie)/
                    (8/bits_per_pixel);

   for(i=0; i<bytes_to_write; i++){

        /**********************************************
        *
        *   Use unions defined in cips.h to stuff shorts
        *   into bytess.
        *
        **********************************************/

      if(bits_per_pixel == 8){
       scu.s_num = 0;
       scu.s_num = array[line_number][i];
       buffer[i] = scu.s_alpha[0];
      }  /* ends if bits_per_pixel == 8 */


      if(bits_per_pixel == 4){

       scu.s_num = 0;
       scu.s_num = array[line_number][i*2];
       first     = scu.s_alpha[0] << 4;

       scu.s_num = 0;
       scu.s_num = array[line_number][i*2];
       second    = scu.s_alpha[0] & 0X000F;

       buffer[i] = first | second;
      }  /* ends if bits_per_pixel == 4 */

   }  /*  ends loop over i  */


   bytes_written = fwrite(buffer, 1, bytes_to_write, 
                          image_file);

   return(bytes_written);
}



void Tiff::insert_short_into_buffer(unsigned char buffer[], int start, short number)
{
       union short_char_union lsu;

    lsu.s_num       = number;
    buffer[start+0] = lsu.s_alpha[0];
    buffer[start+1] = lsu.s_alpha[1];
}



void Tiff::insert_long_into_buffer(unsigned char buffer[], int start, long number)
{
  union long_char_union lsu;

    lsu.l_num       = number;
    buffer[start+0] = lsu.l_alpha[0];
    buffer[start+1] = lsu.l_alpha[1];
    buffer[start+2] = lsu.l_alpha[2];
    buffer[start+3] = lsu.l_alpha[3];
}


void Tiff::round_off_image_size(tiff_header_struct *image_header, int *length, int *width)
{
   *length = (ROWS-10 + image_header->image_length)/ROWS;
   *width  = (COLS-10 + image_header->image_width)/COLS;
}


int Tiff::does_not_exist(char file_name[])
{
  FILE *image_file;
   int  result;

   result = 1;
   image_file = fopen(file_name, "rb");
   if(image_file != NULL){
      result = 0;
      fclose(image_file);
   }
   return(result);
}

short**	Tiff::write_file(int flag, short **image, int length, int width)
{

	int i,j;
	if(flag==0)
	{
		for( i = 0 ; i < length ; i++)
		{
			for( j = 0  ; j < width  ; j++ )
			{
				if(image[i][j]==1)
					image[i][j]=0;
				else
					if(image[i][j]==0)
						image[i][j]=255;
			}
		}
	}
	else 
	{
		if(flag==1)
		{
			for( i = 0 ; i < length ; i++)
			{
				for( j = 0  ; j < width  ; j++ )
				{
					if(image[i][j]==0)
						image[i][j]=1;
					else
						if(image[i][j]==255)
							image[i][j]=0;
				}
			}
		
		}
		else
		{
			if(flag==9)
			{
				for( i = 0 ; i < length ; i++)
				{
					for( j = 0  ; j < width  ; j++ )
					{
						if(image[i][j]==0)
							image[i][j]=1;
						else
							image[i][j]=0;
					}
				}
			
			}
			else
			{
				if(flag==5)
				{
					for( i = 0 ; i < length ; i++)
					{
						for( j = 0  ; j < width  ; j++ )
						{
							if(image[i][j]!=0)
								image[i][j]=1;
						
						}
					}
				}
				else
				{
					for( i = 0 ; i < length ; i++)
					{
						for( j = 0  ; j < width  ; j++ )
						{
							if(image[i][j]==255)
								image[i][j]=0;
							else
								image[i][j]=1;
						}
					}
				}
				
			}
		}
	}
	return (image);
}

short**	Tiff::save_file(char f_name[], short **image, int length, int width)
{
	if(length <1 || width <1 )
		return image;
	write_file(0, image,length, width)	;
	create_file_if_needed(f_name,image,length, width,8);
	write_file(1, image,length, width)	;
	return image;
}

void Tiff::create_file_if_needed_un_char(char *f_name, unsigned char **line_array, int length, int width)
{
	short **image;
	int i,j;
//	if(flag==1)
//	{
		image=(short**)malloc(length*sizeof(short*));
		for(i=0;i<length;i++)
		{
			image[i]=(short*)malloc(width*sizeof(short));
			for (j=0;j<width;j++)
			{
			
				if(line_array[i][j]==1)
					image[i][j]=0;
				else
					if(line_array[i][j]==10 )
						image[i][j]=127;
					else
						image[i][j]=255;
			}
			//printf("\n");
		}
//	}
/*	if(flag==0)
	{
		image=(short**)malloc(length*sizeof(short*));
		for(i=0;i<length;i++)
		{
			image[i]=(short*)malloc(width*sizeof(short));
			for (j=0;j<width;j++)
			{
			
				if(line_array[i][j]==1)
					image[i][j]=255;
				else
					image[i][j]=0;
			}
			//printf("\n");
		}
	}*/
	
	create_file_if_needed(f_name,image,length,width,8);
}
