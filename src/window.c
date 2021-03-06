﻿#include "window.h"

static void WindowUpdateTexture( window_t * w ) {
	Uint32 * dst;
	int row, col;
	void * pixels;
	int pitch;
	if ( SDL_LockTexture( w->texture, NULL, &pixels, &pitch ) < 0 ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError() );
		SDL_Quit();
	}
	Uint8 * ptr = w->framebuffer;
	for ( row = 0; row < w->height; ++row ) {
		dst = (Uint32*)( (Uint8*)pixels + row * pitch );
		for ( col = 0; col < w->width; ++col ) {
			Uint8 b = *ptr++;
			Uint8 g = *ptr++;
			Uint8 r = *ptr++;
			Uint8 a = *ptr++;
			*dst++ = ( ( b << 24 ) | ( g << 16 ) | ( r << 8 ) | a );
		}
	}
	SDL_UnlockTexture( w->texture );
}

static Uint8 * WindowInitFramebuffer( window_t * w ) {
	size_t sz = w->width * w->height * w->bpp * sizeof( Uint8 );
	Uint8 * buffer = (Uint8*)malloc( sz );
	if ( buffer == NULL) return NULL;
	memset( buffer, 0, sz );
	return buffer;
}

window_t * WindowInit( int width, int height, int bpp ) {

	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
		printf( "SDL_Init Error: %s", SDL_GetError() );
		return NULL;
	}

	SDL_Window * sdlwindow = SDL_CreateWindow(	"Software OpenGL renderer",
												SDL_WINDOWPOS_CENTERED,
												SDL_WINDOWPOS_CENTERED,
												width, height,
												SDL_WINDOW_ALLOW_HIGHDPI );

	if ( sdlwindow == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create SDL window: %s", SDL_GetError() );
		SDL_DestroyWindow( sdlwindow );
		SDL_Quit();
		return NULL;
	}

	SDL_Renderer * renderer = SDL_CreateRenderer( sdlwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	if ( renderer == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError() );
		SDL_DestroyRenderer( renderer );
		SDL_Quit();
		return NULL;
	}

	SDL_Texture * texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, width, height );

	if ( texture == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError() );
		SDL_DestroyTexture( texture );
		SDL_Quit();
		return NULL;
	}

	window_t * mainwindow = (window_t*)malloc( sizeof( window_t ) );

	if ( mainwindow == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate window\n" );
		SDL_Quit();
		return NULL;
	}

	mainwindow->width	= width;
	mainwindow->height= height;
	mainwindow->bpp		= bpp;
	mainwindow->pitch	= width * bpp;

	Uint8 * framebuffer = WindowInitFramebuffer( mainwindow );

	if ( framebuffer == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate framebuffer: %s\n", SDL_GetError() );
		SDL_Quit();
		return NULL;
	}

	mainwindow->framebuffer = framebuffer;
	mainwindow->sdlwindow	= sdlwindow;
	mainwindow->renderer	= renderer;
	mainwindow->texture		= texture;

	return mainwindow;
}

void WindowDestroy( window_t * w ) {
	SDL_DestroyRenderer( w->renderer );
	SDL_DestroyTexture( w->texture );
	SDL_DestroyWindow( w->sdlwindow );
	free( w->framebuffer );
	SDL_Quit();
}

void WindowUpdate( window_t * w ) {
	SDL_RenderClear( w->renderer );
	SDL_RenderCopy( w->renderer, w->texture, NULL, NULL );
	SDL_RenderPresent( w->renderer );
	WindowUpdateTexture( w );
}

void WindowDrawPoint( window_t * w, int x, int y, Uint8 r, Uint8 g, Uint8 b ) {
	// Fonction à implementer
	Uint32* pixel = (Uint32*)w->framebuffer;
	pixel[y*(w->width) + x] = ( (0xFF << 24) | (r << 16) | (g << 8) | b );
}

void WindowDrawClearColor( window_t * w, Uint8 r, Uint8 g, Uint8 b ) {
	// Fonction à implementer
	int col;
	int lg;
	for(col=0; col<w->width; col++)
		for(lg=0; lg<w->height; lg++)
			WindowDrawPoint(w, col, lg, r, g, b);
}

void WindowDrawLine( window_t * w, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b ) {

  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    WindowDrawPoint(w, x0, y0, r, g, b);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}


/*
void WindowDrawLine( window_t * w, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b ) {
	// Fonction à implementer
	int col, lg, xA, xB, yA, yB, i;
	xA = (x0 < x1) ? x0 : x1;
	yA = (y0 < y1) ? y0 : y1;
	xB = (x0 > x1) ? x0 : x1;
	yB = (y0 > y1) ? y0 : y1;

	float coeff = ((yB-yA)/(xB-xA));

	if (abs(coeff) > 1){
		if (coeff > 0){
				for(i = xA; i< xB; i++){
					WindowDrawPoint(w, i, i*coeff, r, g, b);
				}
		}
		else{
				for(i = xA; i< xB; i++){
					WindowDrawPoint(w, i, i*coeff, r, g, b);
				}
		}
	}

	else{
		if (coeff > 0){
				for(i = yA; i< yB; i++){
					WindowDrawPoint(w, i*coeff, i, r, g, b);
				}
		}
		else{
				for(i = yA; i< yB; i++){
					WindowDrawPoint(w, i*coeff, i, r, g, b);
				}
		}
	}
}
*/





void WindowDrawTriangle( window_t * w ) {
	// Fonction à implementer
}
