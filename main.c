#include <stdio.h>
#include <string.h>
#include "list.h"
#include <time.h>
//TODO гистограмму такты(тесты) для двух реализаций gnuplot/raylib
//TODO вывод free

const int TEST_AMOUNT = 100;

void list_stress_test   ( list_t *list );

int main( void )
{
    ON_LOG( system("rm -rf logs");
            system("mkdir logs");
            log_file_open("log.html");
            LOG(myLOG_DEFAULT, "<pre>");)

    list_t list = {};
    list_init(&list, 5);

    list_elem_t value = 0;
    size_t pos = 0;
    printf("q - quit\n1 - add to back\n2 - add to front\n3 - dump list\n4 - draw list\n5 - add element to 2 pos\n6 - pop back\n7 - pop front\n8 - pop number\n9 - stress test list\n");
    while (1)
    {
        char c = (char)getchar();
        switch (c)
        {
            case 'q':
                list_destroy(&list);
                ON_LOG(log_file_close();
                system("xdg-open log.html");)
                return 0;
            case '1':
                scanf(list_spec, &value);

                list_push_back(&list, value);
                printf("adding %d to back\n", value);
                break;
            case '2':
                scanf(list_spec, &value);

                list_push_front(&list, value);
                printf("adding %d to front\n", value);
                break;
            case '3':
                //list_dump(&list);
                break;
            case '4':
                list_draw_graph(&list);
                break;
            case '5':
                scanf(list_spec, &value);
                list_insert(&list, value, 2);
                printf("adding %d to pos 2\n", value);
                break;
            case '6':
                list_pop_back(&list);
                break;
            case '7':
                list_pop_front(&list);
                break;
            case '8':
                scanf("%zu", &pos);
                list_erase(&list, pos);
                break;
            case '9':
                list_stress_test(&list);
                printf("end of test\n");
                break;
            default:
                break;
        }
        getchar();
    }
    return 0;
}

// void list_dump( list_t* list )
// {
//     printf("free: %4zu\n", list->free);
//     printf("   #");
//     for (size_t i = 0; i < list->capacity; i++)
//     {
//         printf("%4zu", i);
//     }
//     printf("\n");
//     printf(" val");
//     for (size_t i = 0; i < list->capacity; i++)
//     {
//         printf(list_spec, list->nodes[i].value);
//     }
//     printf("\n");
//     printf("next");
//     for (size_t i = 0; i < list->capacity; i++)
//     {
//         printf("%4zu", list->nodes[i].next);
//     }
//     printf("\n");
//     printf("prev");
//     for (size_t i = 0; i < list->capacity; i++)
//     {
//         printf("%4zu", list->nodes[i].prev);
//     }
//     printf("\n\n");
//     return;
// }
//
// void list_print( list_t* list )
// {
//     for (size_t i = list->nodes[0].next; i != 0 ; i = list->nodes[i].next)
//     {
//         printf(list_spec, list->nodes[i].value);
//     }
//     printf("\n");
//     return;
// }

void list_stress_test( list_t *list )
{
    clock_t t0 = clock();
    for (size_t i = 0; i < TEST_AMOUNT; i++)
    {
        switch (i % 6)
        {
            case 0:
                list_push_back(list, (list_elem_t)i);
                break;
            case 1:
                list_push_front(list, (list_elem_t)i);
                break;
            case 2:
                list_insert(list, (list_elem_t)i, i);
                break;
            case 3:
                list_pop_back(list);
                break;
            case 4:
                list_pop_front(list);
                break;
            case 5:
                list_erase(list, i);
                break;
            default:
                break;
        }
    }

    clock_t t1 = clock();
    printf("%ld clocks for tests\n", t1 - t0);
    return;
}
