#include <stdio.h>
#include <ncurses.h>
#include "../inc/ui.h"

int main(void)
{
        tui_t tui;
        if (ui_initialize(&tui) != 0) {
                fprintf(stderr, "Error: initializing text user interface\n");
                if (tui.init) {
                        if (cleanup(&tui) != 0)
                                fprintf(
                                        stderr,
                                        "Error: failed to cleanup tui\n"
                                        "Please use reset command\n");
                }
                return -1;
        }

        if (ui_main_loop(&tui) != 0)
                fprintf(stderr, "Error: Could not run main event loop\n");

        if (ui_cleanup(&tui) != 0) {
                fprintf(stderr, "Error: failed to cleanup tui\n");
                return -1;
        }

        return 0;
}
