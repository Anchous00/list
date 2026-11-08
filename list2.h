#include <stdlib.h>
#include "logger.h"

#define list_spec "%4d"
typedef int list_elem_t;


struct list_t
{
    //TODO size_t size;
    size_t capacity;
    size_t free;
    size_t *prev;
    size_t *next;
    list_elem_t *value;
};

void list_init      ( list_t *list, size_t capacity               );
void list_destroy   ( list_t *list                                );
void list_push_front( list_t *list, list_elem_t value             );
void list_push_back ( list_t *list, list_elem_t value             );
void list_insert    ( list_t *list, list_elem_t value, size_t pos );
void list_pop_back  ( list_t *list                                );
void list_pop_front ( list_t *list                                );
void list_erase     ( list_t *list, size_t pos                    );
void list_draw_graph( list_t *list                                );
void list_resize    ( list_t *list                                );
bool list_free_left ( list_t *list                                );










