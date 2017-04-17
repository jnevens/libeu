/*
 * conffile.c
 *
 *  Created on: Aug 16, 2016
 *      Author: jnevens
 */
#include "../include/bus/conffile.h"

eu_config_t *eu_config_init(const char *file)
{
	return NULL;
}
void eu_config_destroy(eu_config_t *config)
{
	free(config);
}

eu_variant_t *eu_config_get_parameter(eu_config_t *config, const char *name)
{
	return NULL;
}



