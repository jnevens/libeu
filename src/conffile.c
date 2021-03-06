/*
 * conffile.c
 *
 *  Created on: Aug 16, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eu/conffile.h"

// basic parser
bool eu_config_file_parse(eu_config_file_line_parcer_cb_t cb, const char *config_file, void *arguments)
{
	FILE * fp ;
	char bufr[256];
	char *var = NULL;
	char *val = NULL;
	bool rv = true;

	fp = fopen(config_file, "r");
	if (!fopen) {
		eu_log_err("Failed to open config file: %m\n");
		return false;
	} else {

		while (!feof(fp)) {
			fgets(bufr, sizeof(bufr), fp);

			if (bufr[0] == '#')
				continue;
			if (bufr[0] == '\n')
				continue;
			if (bufr[strlen(bufr) - 1] == '\n')
				bufr[strlen(bufr) - 1] = '\0';
			if ((var = strtok(bufr, "=")) == NULL)
				continue;
			if ((val = strtok(NULL, "=")) == NULL)
				continue;
			if (strlen(var) < 1)
				continue;
			if (strlen(val) < 1)
				continue;

			cb(var, val, arguments);
		}
	}

	fclose(fp);

	return rv;
}

