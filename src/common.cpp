/*
 * $File: common.cpp
 * $Date: Sun Jun 12 23:14:23 2011 +0800
 */
/*
	This file is part of xabell

	Copyright (C) <2011>  Jiakai <jia.kai66@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

static const int ERROR_MSG_SIZE = 1024;

Error::Error(const char *fmt, ...) : m_msg(new char[ERROR_MSG_SIZE])
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(m_msg, ERROR_MSG_SIZE, fmt, ap);
	va_end(ap);
}

const char *Error::get_msg() const
{
	return m_msg;
}

Error::~Error()
{
	delete []m_msg;
}

Error::Error(const Error &)
{
	fprintf(stderr, "copy constructor of class Error called.\nexiting...\n");
	exit(EXIT_FAILURE);
}

void __debug_output(const char *file, int line, const char *func, const char *fmt, ...)
{
	fprintf(stderr, "%s: debug: %s:%d %s:\n  ", PROGRAM_NAME,
			file, line, func);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}

