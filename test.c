#include "list.h"
#include "graphicks/histogram.h"
#include <time.h>

clock_t run_test( list_t *list, int test_amount );

const int TEST_AMOUNT = 5000;
const int MAX_TEST = 5000;

int main( void )
{
    hgram h = {};
    hist_init(&h, TEST_AMOUNT, 0, 0);

    for (int i = MAX_TEST/TEST_AMOUNT; i <= MAX_TEST; i += MAX_TEST/TEST_AMOUNT)
    {
        list_t list = {};
        list_init(&list, 10);
        clock_t sum = 0;
        for (int j = 0; j < 1; j++)
            sum += run_test(&list, i);
        printf("ended test %d %ld\n", i, sum/10);
        hist_add_bar(&h, bar((int)sum * 10, i, i + MAX_TEST/TEST_AMOUNT));
        list_destroy(&list);
    }
    hist_draw(&h);

    hist_destroy(&h);


    return 0;
}

clock_t run_test( list_t *list, int test_amount )
{
    clock_t t0 = clock();
    for (size_t i = 0; i < test_amount; i++)
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
    return t1 - t0;
}


