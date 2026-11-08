#include "list2.h"

void list_init( list_t *list, size_t capacity )
{
    list->prev =       (size_t*)calloc(capacity, sizeof(size_t));
    list->next =       (size_t*)calloc(capacity, sizeof(size_t));
    list->value = (list_elem_t*)calloc(capacity, sizeof(list_elem_t));
    if (list->prev == NULL || list->value == NULL || list->next == NULL)
    {
        return;
    }
    list->capacity = capacity;

    for (size_t i = 0; i < capacity; i++)
    {
        list->next[i] = (i + 1) % capacity;
        list->prev[i] = (i - 1) % capacity;
    }

    list->free = 1;
    list->value[0] = 0;
    list->next[0]  = 0;
    list->prev[0]  = 0;

    return;
}

void list_destroy( list_t *list )
{
    for (size_t i = 0; i < list->capacity; i++)
        list->value[i] = 0;
    free(list->value);
    free(list->next);
    free(list->prev);



    return;
}

void list_push_front( list_t *list, list_elem_t value )
{
    if (!list_free_left(list))
    {
        printf("resizing list\n");
        list_resize(list);
    }

    size_t index = list->free;

    list->free = list->next[list->free];

    list->value[index] = value;
    list->next[list->prev[0]] = index;
    list->prev[index] = list->prev[0];
    list->next[index] = 0;
    list->prev[0] = index;

    ON_LOG(LOG(myLOG_ACTION, "push" list_spec" to list front", value);
                list_draw_graph(list);)

    return;
}

void list_push_back( list_t *list, list_elem_t value )
{
    if (!list_free_left(list))
    {
        printf("resizing list\n");
        list_resize(list);
    }

    size_t index = list->free;

    list->free = list->next[list->free];

    list->value[index] = value;
    list->prev[list->next[0]] = index;
    list->next[index] = list->next[0];
    list->prev[index] = 0;
    list->next[0] = index;

    ON_LOG(LOG(myLOG_ACTION, "push" list_spec" to list back", value);
                list_draw_graph(list);)

    return;
}

void list_insert( list_t *list, list_elem_t value, size_t pos )
{
    if (!list_free_left(list))
    {
        printf("resizing list\n");
        list_resize(list);
    }

    pos = pos % list->capacity;
    size_t index = 0;
    size_t free_index = list->free;
    list->free = list->next[list->free];

    for (size_t i = 0; i < pos; i++)
        index = list->next[index];

    list->next[free_index] = list->next[index];
    list->prev[free_index] = index;
    list->value[free_index] = value;
    list->prev[list->next[index]] = free_index;
    list->next[index] = free_index;

    ON_LOG(LOG(myLOG_ACTION, "push" list_spec" to list pos %zu", value, (size_t)2);)
    list_draw_graph(list);

    return;
}

void list_pop_back( list_t *list )
{
    if (list->next[0] == 0)
        return;
    size_t second_free = list->free;
    list->free = list->next[0];

    list->value[list->next[0]] = 0;
    list->prev[list->next[list->next[0]]] = 0;
    list->next[0] = list->next[list->next[0]];

    list->next[list->free] = second_free;

    ON_LOG(LOG(myLOG_ACTION, "pop from list back");)
    list_draw_graph(list);

    return;
}

void list_pop_front( list_t *list )
{
    if (list->prev[0] == 0)
        return;
    size_t second_free = list->free;
    list->free = list->prev[0];

    list->value[list->prev[0]] = 0;
    list->next[list->prev[list->prev[0]]] = 0;
    list->prev[0] = list->prev[list->prev[0]];

    list->next[list->free] = second_free;

    ON_LOG(LOG(myLOG_ACTION, "pop from list front");
    list_draw_graph(list);)

    return;
}

void list_erase( list_t *list, size_t pos )
{
    pos = pos % list->capacity;
    size_t index = 0;

    for (size_t i = 0; i < pos; i++)
        index = list->next[index];

    if (list->next[index] == 0)
        return;
    size_t second_free = list->free;
    list->free = list->next[index];

    list->value[list->next[index]] = 0;
    list->prev[list->next[list->next[index]]] = index;
    list->next[index] = list->next[list->next[index]];

    list->next[list->free] = second_free;

    ON_LOG(LOG(myLOG_ACTION, "erase from list pos %zu", pos);)
    list_draw_graph(list);

    return;
}

void list_resize( list_t *list )
{
    list->prev =       (size_t*)realloc(list->prev, 2 * list->capacity * sizeof(size_t));
    list->next =       (size_t*)realloc(list->next, 2 * list->capacity * sizeof(size_t));
    list->value = (list_elem_t*)realloc(list->value, 2 * list->capacity * sizeof(list_elem_t));

    for (size_t i = 0; i < list->capacity; i++)
    {
        list->next[list->capacity + i] = (list->capacity + i + 1) % (2 * list->capacity);
        list->prev[list->capacity + i] = (list->capacity + i - 1) % (2 * list->capacity);
    }
    list->next[list->free] = list->capacity;
    //list->free = list->capacity;

    list->capacity = list->capacity * 2;

    return;
}

bool list_free_left( list_t *list )
{
    if (list->next[list->free] == 0)
        return false;
    return true;
}

void list_draw_graph( list_t *list )
{
    time_t t = time(NULL);
    struct tm *time_info = localtime(&t);


    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);  // или timespec_get(&ts, TIME_UTC)


    char time_str[64] = {};
    strftime(time_str, sizeof(time_str), "%d%m%Y%H%M%S", time_info);
    char file_path[256] = {};
    snprintf(file_path, sizeof(file_path), "logs/%s%ld.gv", time_str, ts.tv_nsec);

    FILE *graph = fopen(file_path, "w");
    if (graph == NULL)
    {
        printf("can't open graph file\n");
        return;
    }


//printing list
    fprintf(graph,  "digraph List{\n"
                    "sortv=1;"
                    "bgcolor=\"transparent\";\n"
                    "rankdir=RL;\n"
                    "node [shape=record, fontname=\"Courier\"];\n"
                    "edge [color=\"#569cd6\"];\n"
                    "0 [label=\" value = NULL_ELEM | index = 0 | {<prev> prev = %zu | <next> next = %zu}\", style=\"rounded,filled\", fillcolor=\"#9fa19f\"];\n",
                     list->prev[0], list->next[0]);


    for (size_t i = list->next[0]; i != 0 ; i = list->next[i])
    {
        fprintf(graph, "%zu [label=\" value = " list_spec" | index = %zu | {<prev> prev = %zu | <next> next = %zu}\", style=\"rounded,filled\", fillcolor=\"#4ce372\"];\n",
         i, list->value[i], i, list->prev[i], list->next[i]);
    }
    //fprintf(graph, "%zu [label=\" value = " list_spec" | index = %zu | {<prev> prev = %zu | <next> next = %zu}\"];\n", (size_t)0, list->nodes[0].value, (size_t)0, list->nodes[0].prev, list->nodes[0].next);

    //fprintf(graph, "%zu:next->%zu:prev;\n", (size_t)0, list->nodes[0].next);
    fprintf(graph, "%zu:prev->%zu:next [dir=both];\n", list->next[0], (size_t)0);
    for (size_t i = list->next[0]; i != 0; i = list->next[i])
    {
        fprintf(graph, "%zu:next->%zu:prev;\n", i, list->next[i]);
        fprintf(graph, "%zu:prev->%zu:next;\n", list->next[i], i);
    }


    fprintf(graph, "}\n");





//printing list of free elems

    fprintf(graph,  "digraph Free {\n"
                    "sortv=2;"
                    "bgcolor=\"transparent\";\n"
                    "rankdir=RL;\n"
                    "node [shape=record, fontname=\"Courier\"];\n"
                    "edge [color=\"#cacbc0\"];\n");


    for (size_t i = list->free; i != 0 ; i = list->next[i])
    {
        fprintf(graph, "%zu [label=\" value = FREE | index = %zu |<next> next = %zu\", style=\"rounded,filled\", fillcolor=\"#afea26\"];\n", i, i, list->next[i]);
    }

    for (size_t i = list->free; list->next[i] != 0; i = list->next[i])
    {
        fprintf(graph, "%zu->%zu;\n", i, list->next[i]);
    }


    fprintf(graph, "}");






    if (fclose(graph))
        printf("can't close graph file\n");

    char dot_cmd[1024] = {};
    //snprintf(dot_cmd, sizeof(dot_cmd), "gvpack -u -g logs/%s%ld.gv | dot -Tsvg -o logs/%s%ld.svg", time_str, ts.tv_nsec, time_str, ts.tv_nsec);
    snprintf(dot_cmd, sizeof(dot_cmd), "ccomps -x logs/%s%ld.gv | dot | gvpack -array_u2 -m 240 | neato -n2 -Tsvg -o logs/%s%ld.svg", time_str, ts.tv_nsec, time_str, ts.tv_nsec);

    system(dot_cmd);

    ON_LOG(LOG(myLOG_DEFAULT, "<img src=\"logs/%s%ld.svg\">", time_str, ts.tv_nsec);)

    return;
}
