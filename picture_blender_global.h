enum { SHADE_COLOURED_BITMAP, TRANSLUCENT_FADE, MERGE_BLACK_AND_WHITE, MAKE_NEGATIVE };

void eek();

int get_mask_colour( ALLEGRO_CONFIG *cfg, unsigned char *r, unsigned char *g, unsigned char *b );


void wait_for_keydown();

int shade_coloured_bitmap(ALLEGRO_CONFIG *cfg);

int bitmaps_same_dimensions(ALLEGRO_BITMAP *b1, ALLEGRO_BITMAP *b2);

#define make_display_target() ( al_set_target_backbuffer(al_get_current_display()) )
