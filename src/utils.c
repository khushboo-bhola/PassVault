
#include <stdio.h>
#include "../include/utils.h"

void clearInputBuffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* Discard unwanted characters. */
    }
}
