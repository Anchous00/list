#include "histogram.h"

hgram* hist_init( hgram *h, size_t size, int x, int y )
{
    h->x = x;
    h->y = y;
    h->size = size;
    h->bars_cnt = 0;
    h->bars = (bar_t*)calloc(size, sizeof(bar_t));
    if (h->bars == NULL)
    {
        //TODO ON_LOG(TRACELOG(LOG_WARNING, "histrogram initialization: can't allocate memory");)
        return NULL;
    }

    return h;
}

hgram* hist_destroy( hgram *h )
{
    free(h->bars);

    return h;
}

hgram* hist_add_bar( hgram *h, bar_t bar )
{
    h->bars[h->bars_cnt++] = bar;

    return h;
}

void draw_axis( hgram *h )
{
    DrawLine(h->x, WINDOW_H - h->y, h->x, 0, RED);
    DrawLine(h->x, WINDOW_H - h->y, WINDOW_W, WINDOW_H - h->y, RED);

    for (int j = 0; j < WINDOW_H; j += SCALE)
    {
        DrawText(TextFormat("%i", j), h->x + 10, WINDOW_H - j - h->y, 30, LIGHTGRAY);
        DrawText(TextFormat("%i", j), h->x + j, WINDOW_H - h->y - 60, 30, LIGHTGRAY);
    }

    return;
}

void bar_draw( bar_t bar, int x, int y )
{

    DrawRectangle(x + bar.left, WINDOW_H - y - bar.value, bar.right - bar.left, bar.value, BLUE);
    DrawRectangleLines(x + bar.left, WINDOW_H - y - bar.value, bar.right - bar.left, bar.value, GRAY);

    return;
}

void hist_draw( hgram *h )
{
    SetTraceLogLevel(LOG_NONE);

    InitWindow(WINDOW_W, WINDOW_H, "EBEYSHIY SWAG");
    SetTargetFPS(0);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        for (size_t i = 0; i < h->bars_cnt; i++)
        {
            bar_draw(h->bars[i], h->x, h->y);
        }
        draw_axis(h);

        EndDrawing();
    }
    CloseWindow();
}

bar_t bar( int value, int left, int right )
{
    bar_t b = {.value = value, .left = left, .right = right};

    return b;
}
