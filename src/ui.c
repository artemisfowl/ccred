#include "../inc/ui.h"
#include "../inc/util.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

/**
 * @brief chkinit(void) to check if initialization succeeded or not.
 *
 * This static function checks if the ncurses module has been initialized or
 * not, in case it is not initialized, it returns -1, else it returns 0.
 *
 * @return Returns -1 if not initialized, 0 if initialization was successful.
 */
static int chkinit(void)
{
        return stdscr ? 0 : -1;
}

int initialize(tui_t *tui)
{
        if (!tui) {
                fprintf(stderr, "Error: TUI container instance not provided\n");
                tui->init = false;
                return -1;
        }

        char config_path[PATH_MAX];
        memset(config_path, '\0', PATH_MAX);
        sprintf(config_path, "%s%c%s",
                        getenv("HOME"),
                        PATH_SEP,
                        CONFIG_PRIMARY);
        printf("Configuration (primary) path: %s\n", config_path);
        tui->config_primary =
                chk_res_exists(config_path) == 0 ? true : false;
        if (!tui->config_primary) {
                memset(config_path, '\0', PATH_MAX);
                sprintf(config_path, "%s%c%s",
                                getenv("HOME"),
                                PATH_SEP,
                                CONFIG_SECONDARY);
                printf("Configuration (secondary) path: %s\n", config_path);
                tui->config_secondary =
                        chk_res_exists(config_path) == 0 ? true : false;
        }

        initscr();
        if (chkinit() != 0) {
                fprintf(stderr, "Error: Failed to initialize ncurses\n");
                tui->init = false;
                return -1;
        }

        // NOTE: None of the configurations are present, use default settings
        if (!tui->config_primary && !tui->config_secondary) {
                cbreak();
                noecho();
                keypad(stdscr, TRUE);
                curs_set(0);

                // setting up the behavior
                tui->behavior.en_line_buf = true;
                tui->behavior.en_noecho = true;
                tui->behavior.en_keypad = true;
                tui->behavior.en_cursor = true;
        } else {
                if (tui->config_primary || tui->config_secondary) {
                        if (tui->config_primary) {
                                // NOTE: Load the primary configuration
                                printf("Loading primary configuration...\n");
                        } else if (tui->config_secondary) {
                                // NOTE: Load the secondary configuration
                                printf("Loading secondary configuration...\n");
                        }
                }
        }

        getmaxyx(stdscr, tui->scr_y, tui->scr_x);

        // FIXME: Add the code for setting up the panes
        tui->apps.pane = newwin(tui->scr_y, tui->scr_x / 2, 0, 0);
        tui->creds.pane = newwin(tui->scr_y,
                        tui->scr_x - (tui->scr_x / 2), 0, 0);

        tui->init = true;

        return 0;
}

int cleanup(tui_t *tui) {
        if (!tui) {
                fprintf(stderr, "Error: TUI container instance empty\n");
                return -1;
        }

        if (tui->init) {
                // FIXME: Add the checks for the delwin(...) call
                delwin(tui->apps.pane);
                delwin(tui->creds.pane);
                if (endwin() != OK) {
                        fprintf(
                                stderr,
                                "Error: Could not cleanup ncurses module\n"
                        );
                        return -1;
                }
        }

        return 0;
}
