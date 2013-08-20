#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdint.h>
#include "global.h"
#include "merge_black_and_white.h"






using namespace std;



#define BLEND_MODE_AVERAGE 0
#define BLEND_MODE_DARKEST 1



int merge_black_and_white(ALLEGRO_CONFIG *cfg)
{
	unsigned char mr, mg, mb; /*                 */
	ALLEGRO_BITMAP *bw1;
	ALLEGRO_BITMAP *bw2;
	const char *bw1_name;
	const char *bw2_name;
	const char *config_string;
	ALLEGRO_LOCKED_REGION *lock1;
	ALLEGRO_LOCKED_REGION *lock2;
	ALLEGRO_LOCKED_REGION *lock3;
	ALLEGRO_BITMAP *output;
	int blend_type;
	int x, y;
	unsigned char *ptr1, *ptr2, *ptr3;
	int r1, g1, b1, r2, g2, b2, a1, a2;
	unsigned char new_r, new_g, new_b;
	unsigned char new_gray;
	
	uint32_t new_colour;
	
	
	/***** Load the first black and white bitmap. *****************************************/
	
	bw1_name = al_get_config_value( cfg, NULL, "bw_picture1" );
    if(bw1_name == NULL)
    {
	   cout << "No value for \"bw_picture1\" in config file.\n";
	   eek();
    }
    else
    {
	   cout << "Name of bitmap 1 is " << bw1_name << "\n\n";   
    }
    
    
	cout << "Attempting to load " << bw1_name << "\n\n";
	
	bw1 = al_load_bitmap(bw1_name);
   
    if(bw1 == NULL)
    {
	  cout << "Loading failed.\n"; 
	  eek(); 
    }
    else
    {
	  cout << "Loaded it.\n";  
    } 
	
	/***** Load the second black and white bitmap. ****************************************/
	
    
    bw2_name = al_get_config_value( cfg, NULL, "bw_picture2");
    if( bw2_name  == NULL )
    {
	   cout << "No value for \"bw_picture2\" in config file.\n";
	   eek();
    }
    else
    {
	   cout << "Name of bitmap 2 is " << bw2_name << "\n\n";   
    }
    
    cout << "Attempting to load " << bw2_name << "\n\n";
   
    bw2 = al_load_bitmap( bw2_name );
   
    if(bw2 == NULL)
    {
	   cout << "Loading failed.\n"; 
	   return 1;
    }
    else
    {
	   cout << "Loaded it.\n";  
    } 
    
    /*********************************************************************************************/
	
	
	
	int bw = al_get_bitmap_width( bw1 );
	int bh = al_get_bitmap_height( bw1 );
	
	if( bw != al_get_bitmap_width(bw2) || bh != al_get_bitmap_height(bw2) )
	{
	   
	   cout << "Bitmaps are not the same dimensions.\n";
		
	   return 1;	
    }
    else
    {
	    cout << "bw1 : " << bw << " * " << bh << "\n\n bw2 : " << al_get_bitmap_width(bw2) << " * " << al_get_bitmap_height(bw2) << "\n\n";	
	}
	
	
	
	
	
	lock1 = al_lock_bitmap( bw1, ALLEGRO_PIXEL_FORMAT_RGBA_8888,  ALLEGRO_LOCK_READONLY );
	
	if(lock1 == NULL)
	{
	   cout << "Couldn't lock bitmap 1.\n";
	   return 1;
    }
	
	cout << " locked bitmap 1.\n\n";
	
	lock2 = al_lock_bitmap( bw2, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READONLY );
	
	if(lock2 == NULL)
	{
		cout << "Couldn't lock bitmap 1.\n";
		return 1;
	}
	
	cout << " locked bitmap 2.\n\n";
	
	
	
	
	output = al_create_bitmap( bw, bh );
	if(output == NULL)
	{
	   cout << "Couldn't create the new bitmap.\n";
	   return 1;	
    }
	
	
	cout << "output bitmap : " << al_get_bitmap_width(output)  << " * " << al_get_bitmap_height(output) << "\n\n";
	
	cout << "Getting blend type.\n";
	
	/* Using bw1_name out of context here instead of using another variable. */
	
	bw1_name = al_get_config_value( cfg, NULL, "black_and_white_merge_blend_type" );
	
	
	if( strcmp( bw1_name, "average" ) == 0 )
	{
		blend_type = BLEND_MODE_AVERAGE;	
		
		cout << "Blend type is : BLEND_MODE_AVERAGE.\n";
	}
	else if( strcmp( bw1_name, "darkest" ) == 0 )
	{
		blend_type = BLEND_MODE_DARKEST;
		
		cout << "Blend type is : BLEND_MODE_DARKEST.\n";
	}
	else
	{
	   cout << "No blend mode specified in configuration file.\n\n";
	   return 1;	
	}
	
	
	lock3 = al_lock_bitmap( output, ALLEGRO_PIXEL_FORMAT_RGBA_8888,  ALLEGRO_LOCK_READWRITE );
	if(lock3 == NULL)
	{
	   cout << "Couldn't lock output bitmap.\n\n";
	   return 1;	
	}
	
	
	cout << "pitch members of locks :";
	cout << "lock1->pitch = " << lock1->pitch << "\n";
	cout << "lock2->pitch = " << lock2->pitch << "\n";
	cout << "lock3->pitch = " << lock3->pitch << "\n";
    
    /***************************************************************************************************/
    /****************** Get the colour which is the mask colour. ***************************************/
    /***************************************************************************************************/
    
    
    if( get_mask_colour(cfg, &mr, &mg, &mb) != 0 )
	{
	   	cout << "Couldn't get mask colour from configuration file.\n\n";
	   	return 1;
	}
	else
	{
	    cout << "mask colour is " << (int) mr << " " << (int) mg << " " << (int) mb << "\n\n";	
	}
	
	/***************************************************************************************************/
	
	cout << "processing bitmaps.\n\n";
	
	for( y = 0; y < bh; ++y )
	{
	   for( x = 0; x < bw; ++x )
	   {	
		  //cout << "y = " << y << "x = " << x << "\n\n";
		  
		  ptr1 = ((unsigned char *) lock1->data) + lock1->pitch * y + x * 4;
		  ptr2 = ((unsigned char *) lock2->data) + lock2->pitch * y + x * 4;
		  ptr3 = ((unsigned char *) lock3->data) + lock3->pitch * y + x * 4;
		 
		  
		  r1 = (int) *(ptr1 + 3);
		  g1 = (int) *(ptr1 + 2);
		  b1 = (int) *(ptr1 + 1);
	      a1 = (int) *(ptr1 + 0);
	      
	      r2 = (int) *(ptr2 + 3);
	      g2 = (int) *(ptr2 + 2);
	      b2 = (int) *(ptr2 + 1);
	      a2 = (int) *(ptr2 + 0);
	      
	      /*
	      if(a1 != 255 || a2 != 255)
	      {
			 cout << "Alpha of at least one pixel on bitmap is not 255. Aborting in case this matters.\n\n";
			 cout << "x = " << x << "y = " << y << " a1 = " << a1 << " a2 = " << a2 << "\n\n";
			 return 1;  
		  }
	      else 
	      * 
	      */
	      /*
	      if( (r1 != g1 || g1 != b1 || r1 != b1) )
	      {
			 cout << "A pixel on bitmap 1 is not grey.\n\n";  
			 return 1;
		  }
		  else if( (r2 != g2 || g2 != b2 || r2 != b2) )
	      {
			 cout << "A pixel on bitmap 2 is not grey.\n\n";  
			 return 1;
		  }
		  */
		  
		  switch(blend_type)
		  {
		     case BLEND_MODE_AVERAGE:
		     
		        new_gray = (unsigned char) ((((float) r1 + (float) r2) / 2.0) + 0.5);
		    
		        break;
		     case BLEND_MODE_DARKEST:
		        
		        new_gray = (r1 < r2) ?  (unsigned char) r1 : (unsigned char) r2;
		       
	            break;
	      }
	      
	      //cout << "new_gray = " << (int) new_gray << "\n";
	      
	      new_colour = ((new_gray) << 24) | ((new_gray) << 16) | ((new_gray) << 8) | 255;
	      
	      if( (r1 == mr && g1 == mg && b1 == mb) || (r2 == mr && g2 == mg && b2 == mb) )
	      {
			 /* Put */
			  
			  //cout << "white.\n";
			 *((uint32_t *) ptr3) = 0x0000ffff;
	      }
	      else
	      {
			 //cout << "r1 = " << r1 << "\n"; 
			 //cout << "r2 = " << r2 << "\n";
			 //cout << "new_gray = " << ((int) new_gray) << "\n"; 
			 //wait_for_keydown();
			
	         *((uint32_t *) ptr3) = new_colour;
	      }
	   }
	}
	
	cout << "unlocking bitmaps.\n";
	
	
	al_unlock_bitmap( bw1 );
	al_unlock_bitmap( bw2 );
	al_unlock_bitmap( output );
	
	
	
	al_save_bitmap("output.png", output);
	
}
