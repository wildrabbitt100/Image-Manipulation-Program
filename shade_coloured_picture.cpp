#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "global.h"




using namespace std;




static int do_shading( ALLEGRO_CONFIG *cfg, ALLEGRO_BITMAP *col, ALLEGRO_BITMAP *bw );


int shade_coloured_bitmap(ALLEGRO_CONFIG *cfg)
{
	unsigned char r, g, b;
	ALLEGRO_BITMAP *black_and_white;
	ALLEGRO_BITMAP *coloured;
	
	const char *colour_picture_name;
	const char *black_and_white_bitmap_name;
	
	const char *config_string;
	
	//swap_colour = al_map_rgba(mask_r, mask_g, mask_b, 255);
	
	
	/****** Load the coloured bitmap ********************************************/
	
	colour_picture_name = al_get_config_value( cfg, NULL, "colour_picture_name" );
    if(colour_picture_name == NULL)
    {
	   cout << "no value for colour_filename in config file.\n";
	   eek();
    }
    else
    {
	   cout << "Bitmap with colour on is " << colour_picture_name << "\n\n";   
    }
    
    
	cout << "Attempting to load " << colour_picture_name << "\n\n";
	
	coloured = al_load_bitmap(colour_picture_name);
   
    if(coloured == NULL)
    {
	  cout << "Loading failed.\n"; 
	  eek(); 
    }
    else
    {
	  cout << "Loaded it.\n";  
    } 
    
    
    /****** Load the black and white bitmap ********************************************/
    
    black_and_white_bitmap_name = al_get_config_value( cfg, NULL, "black_and_white_picture_name");
    if( black_and_white_bitmap_name == NULL )
    {
	   cout << "No value for bw_bitmap in config file.\n";
	   eek();
    }
    else
    {
	   cout << "Black and white bitmap is " << black_and_white_bitmap_name << "\n\n";   
    }
    
    cout << "Attempting to load " << black_and_white_bitmap_name << "\n\n";
   
    black_and_white = al_load_bitmap( black_and_white_bitmap_name );
   
    if(black_and_white == NULL)
    {
	   cout << "Loading failed.\n"; 
	   return 1;
    }
    else
    {
	   cout << "Loaded it.\n";  
    } 
    
    /*********************************************************************************************/
	
	
	int bw, bh; /* bitmap width, bitmap height */
   
    float fraction;
   
    if((bw = al_get_bitmap_width(coloured)) != al_get_bitmap_width(black_and_white) || (bh = al_get_bitmap_height(coloured)) != al_get_bitmap_height(black_and_white) )
    {
	   cout << "bitmaps are different dimensions. Quitting. \n";
	   eek();
    }
    else
    {
	   cout << "bitmap width = " << bw << "bitmap height = " << bh << "\n\n";   
    }
    
    
    if(do_shading(cfg, coloured, black_and_white) != 0)
    {
	   al_destroy_bitmap(coloured);
       al_destroy_bitmap(black_and_white);
       
       return 1;
       	
    }
    
    
    al_destroy_bitmap(coloured);
    al_destroy_bitmap(black_and_white);
    
	return 0;
}









int get_mask_colour( ALLEGRO_CONFIG *cfg, unsigned char *r, unsigned char *g, unsigned char *b )
{
   int config_int;

   const char *config_string;

   /*******************************************************************************/

   config_string = al_get_config_value( cfg, NULL, "mask_r" );
   
   if(config_string == NULL)
   {
	  cout << "No value for mask_r in config file.\n";   
	  return 1;
   }
   
   config_int = atoi( config_string );
   
   if( config_int < 0 || config_int > 255)
   {
	  cout << "value for mask_r out of range.\n";
	  return 1;
   }
   else
   {
	  //cout << "Got value for r.\n";
	  
	  *r = ( unsigned char ) config_int;
   }
   
   /*******************************************************************************/
   
   config_string = al_get_config_value( cfg, NULL, "mask_b" );
   
   if(config_string == NULL)
   {
	  cout << "No value for mask_b in config file.\n";   
	  return 1;
   }
   
   config_int = atoi( config_string );
   
   if( config_int < 0 || config_int > 255)
   {
	  cout << "value for mask_b out of range.\n";
	  return 1;
   }
   else
   {
	  *b = ( unsigned char ) config_int;
   }
   
   /*******************************************************************************/
   
  
   config_string = al_get_config_value( cfg, NULL, "mask_g" );
   
   if(config_string == NULL)
   {
	  cout << "No value for mask_g in config file.\n";   
	  return 1;
   }
   
   config_int = atoi( config_string );
   
   if( config_int < 0 || config_int > 255)
   {
	  cout << "Value for mask_g out of range.\n";
      return 1;
   }
   else
   {
	  *g = (unsigned char) config_int;   
   }


   return 0;
}




static int do_shading( ALLEGRO_CONFIG *cfg, ALLEGRO_BITMAP *col, ALLEGRO_BITMAP *bw )
{

   ALLEGRO_BITMAP *clone;
   ALLEGRO_LOCKED_REGION *lock1, *lock2;
   
   unsigned char mr, mg, mb;
   unsigned char *ptr1, *ptr2;
   int r, g, b;
   int nr, ng, nb;
   int x, y;
   int bwidth = al_get_bitmap_width(col);
   int bh = al_get_bitmap_height(col);
   int grey;
   
   unsigned int new_colour;
   
   clone = al_clone_bitmap( bw );
   
   if(clone == NULL)
   {
	  cout << "Couldn't clone black and white bitmap.\n";
	  return 1;   
   }
   
   
   get_mask_colour(cfg, &mr, &mg, &mb);
   
  
   lock1 = al_lock_bitmap( clone, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE );
   if(lock1 == NULL)
   {
	  cout << "Couldn't lock bitmap. Quitting\n";
	  eek();   
   }
   else
   {
	  cout << "Locked output bitmap.\n";   
   }
   
   lock2 = al_lock_bitmap( col, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READONLY );
   if(lock2 == NULL)
   {
	  cout << "Couldn't lock bitmap. Quitting\n";
	  eek();   
   }
   else
   {
	  cout << "Locked colour bitmap.\n";   
   }
   
   for(y = 0; y < bh; ++y)
   {
      for(x = 0; x < bwidth; ++x)
      { 
         ptr2 = ((unsigned char *) lock2->data) + lock2->pitch * y + x * 4;
         
         /* Get the r, g, b of the pixel on the colour bitmap. */
         
         r = (int) *(ptr2 + 3);
		 g = (int) *(ptr2 + 2);
		 b = (int) *(ptr2 + 1);
		 
         ptr1 = ((unsigned char *) lock1->data) + lock1->pitch * y + x * 4;
		  
		 grey = (int) *(ptr1 + 3);
		 nr = (int) ( ((float) grey / 255.0f) * (float) r);
		 ng = (int) ( ((float) grey / 255.0f) * (float) g);
		 nb = (int) ( ((float) grey / 255.0f) * (float) b);
		 
		 if(!(r == (int) mr && g == (int) mg && b == (int) mb) )
		 { 
			new_colour = (( unsigned char ) nr << 24) | (( unsigned char ) ng << 16 ) | ( (unsigned char) nb << 8 ) | 255;
		    *((unsigned int *) (ptr1)) = new_colour;
		 }
		 else
		 {
			*((unsigned int *) (ptr1)) = 0x0000ffff;
		 } 
      }
   }
   
   al_unlock_bitmap(clone);
   al_unlock_bitmap(col);
   
   al_save_bitmap( "output.png", clone );
   
   cout << "Save output bitmap (output.png).\n";
   
   return 0;
}
