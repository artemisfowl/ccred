#ifndef UTIL_H
#define UTIL_H

/**
 * @brief Path separator macros
 *
 * These macros are to be used in order to create the paths for the
 * configuration file.
 */
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define PATH_SEP '\\'
#define PATH_SEP_STR "\\"
#else
#define PATH_SEP '/'
#define PATH_SEP_STR "/"
#endif

/**
 * @brief chk_res_exists(...) checks if the resource file exists or not.
 *
 * This function will be taking in the path (as a string) and then post
 * checking it using the POSIX approach (using access(...) call) will be
 * sending out a proper response.
 *
 * @param path - string containing the path of the resource.
 *
 * @return Returns 0 if resource present, -1 if resource does not exist, -2 if
 * the path is not provided.
 */
int chk_res_exists(const char *path);

/**
 * @brief load_config(...) loads the configuration data from the specified
 * path.
 *
 * This function is responsible for reading the specified configuration file
 * and then loading the contents.
 *
 * @param path - pointer to the string containing the path of the configuration
 * file.
 *
 * @return Returns 0 on success, -1 on failure.
 */
int load_config(const char *path);

#endif
