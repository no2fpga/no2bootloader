/*
 * console.h
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

void console_init(void);

char getchar(void);
int  getchar_nowait(void);
void putchar(char c);
void puts(const char *p);
int  printf(const char *fmt, ...);
