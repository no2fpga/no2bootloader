/*
 * utils.c
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdint.h>
#include <stdbool.h>

char *
hexstr(void *d, int n, bool space)
{
	static const char * const hex = "0123456789abcdef";
	static char buf[96];
	uint8_t *p = d;
	char *s = buf;
	char c;

	while (n--) {
		c = *p++;
		*s++ = hex[c >> 4];
		*s++ = hex[c & 0xf];
		if (space)
			*s++ = ' ';
	}

	s[space?-1:0] = '\0';

	return buf;
}
