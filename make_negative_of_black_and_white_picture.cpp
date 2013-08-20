#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdint.h>
#include "global.h"
#include "load_bitmaps.h"
#include "make_negative.h"




using namespace std;





int make_negative(ALLEGRO_CONFIG *cfg)
{

   ALLEGRO_BITMAP *in, *output;
   unsigned char mr, mg, mb; /* mask r, mask g, mask b. */
   ALLEGRO_LOCKED_REGION *lock = NULL;
   ALLEGRO_LOCKED_REGION *output_lock = NULL;
   int bw, bh, x, y;
   unsigned char *in_ptr, *output_ptr, grey;
   
   bw = bh = x = y = 0;
   in = output = NULL;
   
   if(get_one_bitmap(&in, "black_and_white_picture_name", cfg) != 0)
   {
	  return 1; 
   }
   
   bw = al_get_bitmap_width(in);
   bh = al_get_bitmap_height(in);
   
   cout << "bw = " << bw << "bh = " << bh << "\n";
   
   if(bw == 0 || bh == 0)
   {
	  cout << "One of the bitmap dimensions is 0.\n";   
	  return 1;
   }
   
   if(get_mask_colour(cfg, &mr, &mg, &mb) != 0)
   {
	  return 1;   
   }
   
   lock = al_lock_bitmap( in, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READONLY );
   if(lock == NULL)
   {
	  cout << "Couldn't lock the bitmap.\n\n";   
	  return 1;
   }
   
   output = al_create_bitmap(bw, bh);
   if(output == NULL)
   {
	  cout << "Couldn't create the output bitmap.\n\n";
	  return 1;   
	   
   }
   
   output_lock = al_lock_bitmap( output, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_WRITEONLY );
   if(output_lock == NULL)
   {
	  cout << "Couldn't lock the bitmap.\n\n";   
	  return 1;
   }
   
  
   
   
   for(y = 0; y < bh; ++y )
   {
	  for(x = 0; x < bw; ++x )
      {
		 in_ptr = ((unsigned char *) lock->data) + y * lock->pitch + x * 4;
		 output_ptr = ((unsigned char *) output_lock->data) + y * output_lock->pitch + x * 4;
		
		 
		 grey = 255 - *(in_ptr + 3);
		 
		 * ((uint32_t *) output_ptr)  = ( grey << 24 ) | ( grey << 16 ) | ( grey << 8 ) | 255;
		 
      }
   }
   
   al_unlock_bitmap( output );
   al_unlock_bitmap( in );
   
   
	
   al_save_bitmap( "negative_version_output.png", output);

   cout << "Saved bitmap.\n";

   al_destroy_bitmap( output );
   al_destroy_bitmap( in );
	
	
	
	
   return 0;
}
