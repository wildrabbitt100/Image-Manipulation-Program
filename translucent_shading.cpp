#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "global.h"
#include "load_bitmaps.h"

#include "stdint.h"



using namespace std;









int do_translucency_shading(ALLEGRO_CONFIG *cfg)
{
	
   ALLEGRO_BITMAP *coloured, *shading, *output;
   ALLEGRO_LOCKED_REGION *col_lock, *shading_lock, *output_lock;

   int bw, bh;

   if(get_bitmaps( &coloured, &shading, "colour_picture_name", "black_and_white_picture_name", cfg) != 0)
   {
	  cout << "Couldn't load a bitmap. Cannot continue.\n\n";
	  return 1;	
   }
	
   if( bitmaps_same_dimensions( coloured, shading ) != 1)
   {
	  cout << "Bitmaps not the same dimensions.\n";
	  return 1;	
   }
	
	
   bw = al_get_bitmap_width(coloured);
   bh = al_get_bitmap_height(coloured);
	
	
   output = al_create_bitmap(bw, bh);
   if(output == NULL)
   {
	  cout << "Couldn't create the output bitmap.\n";
	  return 1;	
   }
	
   col_lock = al_lock_bitmap( coloured, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READONLY );
	
   shading_lock = al_lock_bitmap( shading, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READONLY );
	
   output_lock = al_lock_bitmap( output, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE );
    
   if(col_lock == NULL || shading_lock == NULL || output_lock == NULL)
   {
	   cout << "Couldn't lock one of the bitmaps.\n";
	   return 1;	
   }
   

    
   int x, y, r, g, b, grey, nr, ng, nb;
   unsigned char *ptr1, *ptr2, *ptr3;
   
   unsigned char mask_r, mask_g, mask_b;
   uint32_t new_colour;
   
   get_mask_colour(cfg, &mask_r, &mask_g, &mask_b);
   
   for(y = 0; y < bh; ++y)
   {
      for(x = 0; x < bw; ++x)
      { 
	     ptr2 = ((unsigned char *) col_lock->data) + col_lock->pitch * y + x * 4;
		  
	     r = (int) *(ptr2 + 3);
	     g = (int) *(ptr2 + 2);
	     b = (int) *(ptr2 + 1);
	   
	     ptr1 = ((unsigned char *) shading_lock->data) + shading_lock->pitch * y + x * 4;
		 
		 ptr3 = ((unsigned char *) output_lock->data) + output_lock->pitch * y + x * 4;
		  
	     grey = (int) *(ptr1 + 3);
	   
	
	     nr = r;
	     ng = g;
	     nb = b;
	   
	     
	     if(!(r == (int) mask_r && g == (int) mask_g && b == (int) mask_b) )
		 {   
			new_colour = (( unsigned char ) nr << 24) | (( unsigned char ) ng << 16 ) | ( (unsigned char) nb << 8 ) | ((unsigned char) grey);
		   
		    *((unsigned int *) (ptr3)) = new_colour;
			 
		 }
		 else
		 {
			*((unsigned int *) (ptr3)) = 0x000ffff;
		 } 
	  }
   }
  
   
   al_unlock_bitmap(coloured);
   al_unlock_bitmap(shading);
   al_unlock_bitmap(output);
   
   al_save_bitmap("translucent_shading_output.png", output);
   
   cout << "Saved bitmap (translucent_shading_output.png).\n";
   
   al_destroy_bitmap(coloured);
   al_destroy_bitmap(shading);
   al_destroy_bitmap(output);
	
	

   return 0;	
}



