#include "../inc/ui.h"
#include "../inc/util.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

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
        tui->init = true;

        // NOTE: None of the configurations are present, use default settings
        if (!tui->config_primary && !tui->config_secondary) {
                cbreak();
                noecho();
                keypad(stdscr, TRUE);
                curs_set(0);
                nodelay(stdscr, TRUE);

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

        // NOTE: This is for the application pane
        tui->apps.pane_height = tui->scr_y - 2;
        tui->apps.pane_width = (tui->scr_x * 35) / 200;
        tui->apps.pane_start_pos_y = 0;
        tui->apps.pane_start_pos_x = 0;
        tui->apps.pane = newwin(
                        tui->apps.pane_height, tui->apps.pane_width,
                        tui->apps.pane_start_pos_y, tui->apps.pane_start_pos_x);
        if (!tui->apps.pane) {
                fprintf(stderr, "Error: Could not create application window\n");
                return -1;
        }


        // NOTE: This is for the credentials pane
        tui->creds.pane_height = tui->scr_y - 2;
        tui->creds.pane_width = tui->scr_x - tui->apps.pane_width;
        tui->creds.pane_start_pos_y = 0;
        tui->creds.pane_start_pos_x = tui->apps.pane_width;
        tui->creds.pane = newwin(
                        tui->creds.pane_height, tui->creds.pane_width,
                        tui->creds.pane_start_pos_y,
                        tui->creds.pane_start_pos_x);
        if (!tui->creds.pane) {
                fprintf(stderr, "Error: Could not create credentials window\n");
                return -1;
        }

        tui->scroll_offset = 0;
        tui->key = '\0';
        tui->run_loop = true;

        return 0;
}

/**
 * @brief tui_refresh(...) to refresh the windows created.
 *
 * This function will be refreshing the windows created as part of the program.
 *
 * @param tui - pointer to the struct of type tui_t
 */
static void tui_refresh(tui_t *tui)
{
        if (!tui) {
                fprintf(stderr, "Error: TUI container instance empty\n");
                return;
        }

        wrefresh(tui->apps.pane);
        wrefresh(tui->creds.pane);
}

int main_loop(tui_t *tui)
{
        if (!tui) {
                fprintf(stderr, "Error: TUI container instance empty\n");
                return -1;
        }

        while (tui->run_loop) {
                wclear(tui->apps.pane);
                wclear(tui->creds.pane);

                box(tui->apps.pane, 0, 0);
                box(tui->creds.pane, 0, 0);

                tui_refresh(tui);

                tui->key = getch();
                if (tui->key != ERR) {
                        switch (tui->key) {
                                case 'q':
                                        tui->run_loop = false;
                                        break;
                        }
                }
                refresh();
                usleep(16000);
        }
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
