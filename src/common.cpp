/*
 * $File: common.cpp
 * $Date: Wed Mar 09 20:19:17 2011 +0800
 */

#include "common.h"

#include <cstdio>
#include <cstdarg>

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

