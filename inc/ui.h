#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <ncurses.h>

// TUI Configuration filepath
#ifndef CONFIG_PRIMARY
#define CONFIG_PRIMARY ".cred.ini"
#endif

#ifndef CONFIG_SECONDARY
#define CONFIG_SECONDARY ".config/cred"
#endif

// NOTE: This could be set from a configuration file itself. The configuration
// file can be placed either under $HOME or $HOME/.config/cred. The file should
// be a simple INI configuration file. If the file is not present, then default
// settings will be picked up.
typedef struct {
        bool en_line_buf; // enable/disable line buffering.
        bool en_noecho;   // enable/disable echoing of characters.
        bool en_keypad;   // enable/disable keypad, arrow keys etc.
        bool en_cursor;   // enable/disable blinking cursor.
} tui_bhv_t;

typedef struct {
        WINDOW *pane;

        // NOTE: Cols refer to x(width) and rows refer to y(height)
        int pane_width, pane_height;
        // NOTE: These are the draw positions on the terminal screen
        int pane_start_pos_x, pane_start_pos_y;
} pane_t;

typedef struct {
        // Actual screen columns and rows.
        int scr_x; // cols
        int scr_y; // rows

        bool config_primary;
        bool config_secondary;

        bool init;

        pane_t apps;
        pane_t creds;

        // terminal behavior
        tui_bhv_t behavior;
} tui_t;

/**
 * @brief initialize(void) to initialize ncurses module.
 *
 * This function will be responsible for checking the presence of the
 * configuration file. If the configuration file is present in $HOME or
 * $HOME/.config/cred in INI format, the same would be read and the
 * configuration would be loaded. Post loading the configuration the TUI will
 * be initialized.
 *
 * @param tui - pointer to the struct of type tui_t
 *
 * @return Returns 0 on success, -1 on failure.
 */
int initialize(tui_t *tui);

/**
 * @brief main_loop(...) to run the main loop for showing the window.
 *
 * This function will be showing the windows as well as responsible for
 * handling the events being triggered.
 *
 * @param tui - pointer to the struct of type tui_t
 *
 * @return Returns 0 on successful exit, -1 on any failure.
 */
int main_loop(tui_t *tui);

/**
 * @brief cleanup(...) to cleanup the resources used for the ncurses session.
 *
 * This function is responsible to for cleaning up all the resources that are
 * being changed as part of the
 */
int cleanup(tui_t *tui);

#endif
