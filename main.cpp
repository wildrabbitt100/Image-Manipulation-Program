#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "global.h"
#include "merge_black_and_white.h"
#include "translucent_shading.h"

#include "make_negative.h"

ALLEGRO_DISPLAY *display;

ALLEGRO_EVENT_QUEUE *queue;


using namespace std;


void eek()
{
	
   system("PAUSE");
   exit(1);
   
}






int initialise()
{
	
   al_init();
   
   al_install_keyboard();
   
   al_init_primitives_addon();
   
   al_init_font_addon();
   
   al_init_image_addon();
   
   if( al_init_ttf_addon() == false)
   {
      cout << "couldn't init true type font addon.\n";
      eek();
   }
   else
   {
      cout << "initialised truetype addon.\n";
   }
   
   
   return 0;
}





int main( int argc, char *argv[] )
{
   ALLEGRO_EVENT event;
   ALLEGRO_TIMER *timer;
   ALLEGRO_FONT *alexis;
   
   
   
   ALLEGRO_CONFIG *config_file;
   
   int i;
   
   unsigned char mask_r, mask_g, mask_b;
   
   int config_int;
   
   const char *config_string;
   
   const char *colour_picture_name;
   
   
   unsigned char *ptr1;
   unsigned char *ptr2;
  
   
   unsigned int new_colour;
   
   int nr, ng, nb, na;
   
   int grey;
   
   
   
   int job_type = 0;
   
   
   initialise();
  
   cout << "Initialised allegro.\n\n";
   
   config_file = al_load_config_file( "configuration.cfg" );
   if( config_file == NULL )
   {
	  cout << "Couldn't load config file named configuration.cfg .\n\n";
	  eek();
   }
   else
   {
	  cout << "Loaded configuration file.\n\n";   
   }
   
   
   alexis = al_load_ttf_font("Alpha Romanie G98.ttf", 24, 0);
   if(alexis == NULL)
   {
      cout << "Couldn't load \"Alpha Romanie G98.ttf\". quitting\n\n";
   }
   else
   {
      cout << "Loaded font.\n";
   }
   
   cout << "Creating display.\n\n";
   
   
   
   display = al_create_display(1024, 768);
   if(display == NULL)
   {
	  cout << "Couldn't create the display.\n\n";
	  
	  /* to do : clear up */
	  
	  return 0;   
   }
   
   
   al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
   
   
   
   
   /************************************************************************************/
   /************ Get the type of job to do. ********************************************/
   /************************************************************************************/
   /************************************************************************************/
   
   

   
   config_string = al_get_config_value( config_file, NULL, "job_type" );
   
   if(config_string == NULL)
   {
	  cout << "Config string NULL for \"job_type\" key." << "\n\n";
	  
	  return 1;
   }
   else if( strcmp(config_string, "translucent_fade") == 0)
   {
	   job_type = TRANSLUCENT_FADE;   
	   cout << "Doing a fade job type.\n";
	   
   }
   else if( strcmp(config_string, "shade") == 0)
   {
	   job_type = SHADE_COLOURED_BITMAP;   
	   
	   cout << "Doing a shade (coloured bitmap) job.";
   }
   else if( strcmp(config_string, "bwmerge") == 0)
   {
	   
	   job_type = MERGE_BLACK_AND_WHITE;  
	   
	   cout << "Doing a merge black and white pictures job.\n\n";
   }
   else if( strcmp(config_string, "make_negative") == 0 )
   {
	  job_type = MAKE_NEGATIVE;
	  
	  cout << "Doing a make negative job.\n"; 
   }
   else
   {
	   cout << "string not recognised for job_type.\n";
	   return 1;   
   }
   
   
   
   timer = al_create_timer(1.0 / 60.0);
   
   queue = al_create_event_queue();

   al_register_event_source( queue, al_get_keyboard_event_source() );
   al_register_event_source( queue, al_get_timer_event_source(timer) );
   
  
 
   int redraw = 1;
   
   int quit = 0;
   
   int wait = 0;
   int key_up_time;
   
   
   al_start_timer(timer);
   
   int x, y;
   
   
   
   switch(job_type)
   {
	   
      case MERGE_BLACK_AND_WHITE:
         if( merge_black_and_white( config_file ) != 0 )
         {
			eek();
			return 0;
	     }
         break;
      case SHADE_COLOURED_BITMAP:
         cout << "1111.\n";
         if( shade_coloured_bitmap( config_file ) != 0 )
         {
			eek();
			return 0; 
	     }
         break;
      case TRANSLUCENT_FADE:
         
         
         if( do_translucency_shading(config_file) != 0)
         {
			eek();
			return 1; 
		 }
         return 0;
         break;
      case MAKE_NEGATIVE:
         if( make_negative(config_file) != 0 )
         {
			eek();
		    return 1;
		 }
		 return 0;
		 break;
   } 
   
   cout << "here.\n";
   
   
   
   /*
	  
   }
   
   
   
   al_unlock_bitmap( clone );
   al_unlock_bitmap( colour_bitmap );
   */
   
   cout << "Unlocked bitmaps.\n";
   
   
   
   
   eek();
   
   
   do
   {
      if( al_get_timer_count(timer) > key_up_time + 10 )
	  {
	     wait = 0;
	  }
   
 
      while( !al_is_event_queue_empty( queue ) )
	  {
	     al_get_next_event(queue, &event);
	     
	
	     switch( event.type )
		 {
		    case ALLEGRO_EVENT_TIMER:
			   redraw = 1;
			   break;
		    case ALLEGRO_EVENT_KEY_DOWN:
			   
			   cout << "key down event.\n";
			   
			   if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			   {
			      quit = 1;
			   }
			   
			   break;
		 
		 }
	
	  

	  
	     if(al_is_event_queue_empty(queue) && redraw)
	     {
	        al_set_target_backbuffer(al_get_current_display());
	        
	        
		    al_draw_textf( alexis, al_map_rgb(0, 0, 0), 10, 10, 0, "Hello %d", 0);
		 
		
		 
	
		    //cout << "flipped.\n";
		 
            al_flip_display();
		    redraw = 0;
         }
   
      } 
   
   } while(quit == 0);



   cout << "returning 0 from main. \n\n";
   eek();   
   return 0;
}







void wait_for_keydown(void)
{
   ALLEGRO_EVENT event;
   ALLEGRO_EVENT_QUEUE *q = al_create_event_queue();
   
   	
   
   al_register_event_source( q, al_get_keyboard_event_source());
   
   
	
 
   do
   {
      al_get_next_event( q, &event );  
	   
   } while( event.type != ALLEGRO_EVENT_KEY_DOWN );
   
   al_destroy_event_queue(q);

}




int bitmaps_same_dimensions(ALLEGRO_BITMAP *b1, ALLEGRO_BITMAP *b2)
{
   return ((al_get_bitmap_width(b1) == al_get_bitmap_width(b2)) && (al_get_bitmap_height(b1) == al_get_bitmap_height(b2)));	
	
}

