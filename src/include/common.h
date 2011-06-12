/*
 * $File: common.h
 * $Date: Sun Jun 12 19:00:23 2011 +0800
 */

#ifndef _HEADER_COMMON_
#define _HEADER_COMMON_

class Error
{
	char *m_msg;
	Error(const Error &) {}
	Error& operator = (const Error &)
	{}
public:
	Error(const char *fmt, ...) __attribute__((format(printf, 2, 3)));
	const char *get_msg() const;
	~Error();
};

#endif

