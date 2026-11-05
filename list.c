#include "list.h"

void list_init( list_t *list, size_t capacity )
{
    list->nodes = (node_t*)calloc(capacity, sizeof(node_t));
    if (list->nodes == NULL)
    {
        return;
    }
    list->capacity = capacity;

    for (size_t i = 0; i < capacity; i++)
    {
        list->nodes[i].next = (i + 1) % capacity;
        list->nodes[i].prev = (i - 1) % capacity;
    }

    list->free = 1;
    list->nodes[0].value = 0;
    list->nodes[0].next  = 0;
    list->nodes[0].prev  = 0;

    return;
}

void list_destroy( list_t *list )
{
    for (size_t i = 0; i < list->capacity; i++)
        list->nodes[i].value = 0;
    free(list->nodes);

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

    list->free = list->nodes[list->free].next;

    list->nodes[index].value = value;
    list->nodes[list->nodes[0].prev].next = index;
    list->nodes[index].prev = list->nodes[0].prev;
    list->nodes[index].next = 0;
    list->nodes[0].prev = index;

    ON_LOG(LOG(LOG_ACTION, "push" list_spec" to list front", value);
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

    list->free = list->nodes[list->free].next;

    list->nodes[index].value = value;
    list->nodes[list->nodes[0].next].prev = index;
    list->nodes[index].next = list->nodes[0].next;
    list->nodes[index].prev = 0;
    list->nodes[0].next = index;

    ON_LOG(LOG(LOG_ACTION, "push" list_spec" to list back", value);
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
    list->free = list->nodes[list->free].next;

    for (size_t i = 0; i < pos; i++)
        index = list->nodes[index].next;

    list->nodes[free_index].next = list->nodes[index].next;
    list->nodes[free_index].prev = index;
    list->nodes[free_index].value = value;
    list->nodes[list->nodes[index].next].prev = free_index;
    list->nodes[index].next = free_index;

    ON_LOG(LOG(LOG_ACTION, "push" list_spec" to list pos %zu", value, (size_t)2);)
    list_draw_graph(list);

    return;
}

void list_pop_back( list_t *list )
{
    if (list->nodes[0].next == 0)
        return;
    size_t second_free = list->free;
    list->free = list->nodes[0].next;

    list->nodes[list->nodes[0].next].value = 0;
    list->nodes[list->nodes[list->nodes[0].next].next].prev = 0;
    list->nodes[0].next = list->nodes[list->nodes[0].next].next;

    list->nodes[list->free].next = second_free;

    ON_LOG(LOG(LOG_ACTION, "pop from list back");)
    list_draw_graph(list);

    return;
}

void list_pop_front( list_t *list )
{
    if (list->nodes[0].prev == 0)
        return;
    size_t second_free = list->free;
    list->free = list->nodes[0].prev;

    list->nodes[list->nodes[0].prev].value = 0;
    list->nodes[list->nodes[list->nodes[0].prev].prev].next = 0;
    list->nodes[0].prev = list->nodes[list->nodes[0].prev].prev;

    list->nodes[list->free].next = second_free;

    ON_LOG(LOG(LOG_ACTION, "pop from list front");
    list_draw_graph(list);)

    return;
}

void list_erase( list_t *list, size_t pos )
{
    pos = pos % list->capacity;
    size_t index = 0;

    for (size_t i = 0; i < pos; i++)
        index = list->nodes[index].next;

    if (list->nodes[index].next == 0)
        return;
    size_t second_free = list->free;
    list->free = list->nodes[index].next;

    list->nodes[list->nodes[index].next].value = 0;
    list->nodes[list->nodes[list->nodes[index].next].next].prev = index;
    list->nodes[index].next = list->nodes[list->nodes[index].next].next;

    list->nodes[list->free].next = second_free;

    ON_LOG(LOG(LOG_ACTION, "erase from list pos %zu", pos);)
    list_draw_graph(list);

    return;
}

void list_resize( list_t *list )
{
    list->nodes = (node_t*)realloc(list->nodes, sizeof(node_t) * list->capacity * 2);

    for (size_t i = 0; i < list->capacity; i++)
    {
        list->nodes[list->capacity + i].next = (list->capacity + i + 1) % (2 * list->capacity);
        list->nodes[list->capacity + i].prev = (list->capacity + i - 1) % (2 * list->capacity);
    }
    list->nodes[list->free].next = list->capacity;
    //list->free = list->capacity;

    list->capacity = list->capacity * 2;

    return;
}

bool list_free_left( list_t *list )
{
    if (list->nodes[list->free].next == 0)
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
                    "bgcolor=\"transparent\";\n"
                    "rankdir=RL;\n"
                    "node [shape=record, fontname=\"monospace\"];\n"
                    "edge [color=\"#569cd6\"];\n"
                    "0 [label=\" value = NULL_ELEM | index = 0 | {<prev> prev = %zu | <next> next = %zu}\", style=\"rounded,filled\", fillcolor=\"#9fa19f\"];\n", list->nodes[0].prev, list->nodes[0].next);


    for (size_t i = list->nodes[0].next; i != 0 ; i = list->nodes[i].next)
    {
        fprintf(graph, "%zu [label=\" value = " list_spec" | index = %zu | {<prev> prev = %zu | <next> next = %zu}\", style=\"rounded,filled\", fillcolor=\"#4ce372\"];\n", i, list->nodes[i].value, i, list->nodes[i].prev, list->nodes[i].next);
    }
    //fprintf(graph, "%zu [label=\" value = " list_spec" | index = %zu | {<prev> prev = %zu | <next> next = %zu}\"];\n", (size_t)0, list->nodes[0].value, (size_t)0, list->nodes[0].prev, list->nodes[0].next);

    //fprintf(graph, "%zu:next->%zu:prev;\n", (size_t)0, list->nodes[0].next);
    fprintf(graph, "%zu:prev->%zu:next [dir=both];\n", list->nodes[0].next, (size_t)0);
    for (size_t i = list->nodes[0].next; i != 0; i = list->nodes[i].next)
    {
        fprintf(graph, "%zu:next->%zu:prev;\n", i, list->nodes[i].next);
        fprintf(graph, "%zu:prev->%zu:next;\n", list->nodes[i].next, i);
    }


    fprintf(graph, "}\n");





//printing list of free elems

    fprintf(graph, "digraph Free {\nbgcolor=\"transparent\";\nrankdir=RL;\nnode [shape=record, fontname=\"monospace\"];\nedge [color=\"#e1f35c\"];\n");


    for (size_t i = list->free; i != 0 ; i = list->nodes[i].next)
    {
        fprintf(graph, "%zu [label=\" value = FREE | index = %zu |<next> next = %zu\", style=\"rounded,filled\", fillcolor=\"#4ce372\"];\n", i, i, list->nodes[i].next);
    }

    for (size_t i = list->nodes[0].next; i != 0; i = list->nodes[i].next)
    {
        fprintf(graph, "%zu->%zu;\n", i, list->nodes[i].next);
    }


    fprintf(graph, "}");






    if (fclose(graph))
        printf("can't close graph file\n");

    char dot_cmd[1024] = {};
    snprintf(dot_cmd, sizeof(dot_cmd), "dot %s -Tsvg -o logs/%s%ld.svg", file_path, time_str, ts.tv_nsec);
    system(dot_cmd);

    ON_LOG(LOG(LOG_DEFAULT, "<img src=\"logs/%s%ld.svg\">", time_str, ts.tv_nsec);)

    return;
}
