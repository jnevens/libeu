/*
 * conffile.h
 *
 *  Created on: Aug 16, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EU_CONFFILE_H_
#define INCLUDE_EU_CONFFILE_H_

#include <stdbool.h>

typedef int (eu_config_file_line_parcer_cb_t)(const char *arg, const char *val, void *arguments);

// basic config file parser
bool eu_config_file_parse(eu_config_file_line_parcer_cb_t cb, const char *config_file, void *arguments);


#endif /* INCLUDE_EU_UTILS_CONFFILE_H_ */
