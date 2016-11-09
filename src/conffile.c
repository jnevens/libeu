/*
 * conffile.c
 *
 *  Created on: Aug 16, 2016
 *      Author: jnevens
 */
#include "../include/bus/conffile.h"

config_t *config_init(const char *file)
{
	return NULL;
}
void config_destroy(config_t *config)
{
	free(config);
}

variant_t *config_get_parameter(config_t *config, const char *name)
{
	return NULL;
}



