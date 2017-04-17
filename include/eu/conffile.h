/*
 * conffile.h
 *
 *  Created on: Aug 16, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EU_CONFFILE_H_
#define INCLUDE_EU_CONFFILE_H_

#include <eu/types.h>


eu_config_t *eu_config_init(const char *file);
void eu_config_destroy(eu_config_t *config);

eu_variant_t *eu_config_get_parameter(eu_config_t *config, const char *name);



#endif /* INCLUDE_EU_UTILS_CONFFILE_H_ */
