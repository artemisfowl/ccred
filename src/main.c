#include <stdio.h>
#include <ncurses.h>
#include "../inc/ui.h"

int main(void)
{
        tui_t tui;
        if (initialize(&tui) != 0) {
                fprintf(stderr, "Error: initializing text user interface\n");
                return -1;
        }

        if (cleanup(&tui) != 0) {
                fprintf(stderr, "Error: failed to cleanup tui\n");
                return -1;
        }
        return 0;
}
