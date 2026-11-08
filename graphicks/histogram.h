#ifndef HIST_H
#define HIST_H

#include <stdlib.h>
#include "raylib.h"

const int WINDOW_W = 600, WINDOW_H = WINDOW_W;
const int SCALE = 100;

struct bar_t
{
    int value;
    int left, right;
};

struct hgram
{
    size_t bars_cnt, size;
    int x, y; //начало координат
    bar_t* bars;
};

hgram* hist_init    ( hgram *h, size_t size, int x, int y );
hgram* hist_add_bar ( hgram *h, bar_t bar                 );
hgram* hist_destroy ( hgram *h                            );


void hist_draw( hgram *h );
void bar_draw( bar_t bar, int x, int y );
void draw_axis( hgram *h );



bar_t bar( int value, int left, int right );

#endif
