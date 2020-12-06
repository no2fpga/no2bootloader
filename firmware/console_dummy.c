/*
 * console_dummy.c
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdint.h>

#include "config.h"


void console_init(void)
{
}

char getchar(void)
{
	while (1);
}

int getchar_nowait(void)
{
	return -1;
}

void putchar(char c)
{
}

void puts(const char *p)
{
}

int printf(const char *fmt, ...)
{
	return 0;
}
