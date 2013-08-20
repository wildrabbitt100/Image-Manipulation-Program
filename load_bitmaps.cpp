#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <cstring>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "global.h"




#include "load_bitmaps.h"



using namespace std;




int get_bitmaps(ALLEGRO_BITMAP **bmp1, ALLEGRO_BITMAP **bmp2, const char *bmp1_config_key, const char *bmp2_config_key, ALLEGRO_CONFIG *cfg)
{
   
   const char *filename = NULL;
   	
   /* bmp1 : a pointer to an ALLEGRO_BITMAP pointer. */
   
   *bmp1 = NULL;
   *bmp2 = NULL;
   
   
   
   /****** Load bitmap 1. ********************************************/
	
	filename = al_get_config_value( cfg, NULL, bmp1_config_key );
    if(filename == NULL)
    {
	   cout << "No value for " << bmp1_config_key << " in config file.\n";
	   return 1;
    }
    
	cout << "Attempting to load " << filename << "\n\n";
	
	*bmp1 = al_load_bitmap(filename);
   
    if(*bmp1 == NULL)
    {
	  cout << "Loading failed.\n"; 
	  return 1;
    }
    else
    {
	  cout << "Loaded it.\n";  
    } 
	
	/****************************************************************************/
	
	
	/****** Load bitmap 2. ********************************************/
    
    filename = al_get_config_value( cfg, NULL, bmp2_config_key );
    if( filename == NULL )
    {
	   cout << "No value for " << bmp2_config_key << " in config file.\n";
	   return 1;
    }
   
    
    cout << "Attempting to load " << filename << "\n\n";
   
    *bmp2 = al_load_bitmap( filename );
   
    if(*bmp2 == NULL)
    {
	   cout << "Loading failed.\n"; 
	   return 1;
    }
    
    /*********************************************************************************************/
	
	return 0;
}




int get_one_bitmap(ALLEGRO_BITMAP **bmp, const char *config_key, ALLEGRO_CONFIG *cfg)
{
   
   const char *filename = NULL;
   	
   /* bmp1 : a pointer to an ALLEGRO_BITMAP pointer. */
   
   *bmp = NULL;
   
   /****** Load the bitmap. ********************************************/
	
   filename = al_get_config_value( cfg, NULL, config_key );
   if(filename == NULL)
   {
	  cout << "No value for " << config_key << " in config file.\n";
	  return 1;
   }
    
   cout << "Attempting to load <" << filename << ">\n\n";
	
   *bmp = al_load_bitmap(filename);
   
   cout << "Called al_load_bitmap().\n";
   
   if(*bmp == NULL)
   {
	  cout << "Loading failed.\n"; 
	  return 1;
   }
   else
   {
	  cout << "Loaded it.\n";  
   } 
    
   return 0;   
}


