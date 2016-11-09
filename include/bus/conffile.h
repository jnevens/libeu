/*
 * conffile.h
 *
 *  Created on: Aug 16, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_BUS_CONFFILE_H_
#define INCLUDE_BUS_CONFFILE_H_

#include <bus/types.h>


config_t *config_init(const char *file);
void config_destroy(config_t *config);

variant_t *config_get_parameter(config_t *config, const char *name);



#endif /* INCLUDE_BUS_UTILS_CONFFILE_H_ */
