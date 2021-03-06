/*
 * $File: common.h
 * $Date: Sun Jun 12 23:18:16 2011 +0800
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

#ifndef _HEADER_COMMON_
#define _HEADER_COMMON_

#define PROGRAM_NAME	"xabell"

class Error
{
	char *m_msg;
	void operator = (const Error &)
	{}
public:
	Error(const Error &);
	Error(const char *fmt, ...) __attribute__((format(printf, 2, 3)));
	const char *get_msg() const;
	~Error();
};


#ifdef _DEBUG_BUILD_
extern void __debug_output(const char *file, int line, const char *func,
		const char *fmt, ...) __attribute__((format(printf, 4, 5)));
#define debug_output(fmt, ...) \
	__debug_output(__FILE__, __LINE__, __PRETTY_FUNCTION__, fmt, ## __VA_ARGS__)
#else
#define debug_output(...)
#endif

#endif // _HEADER_COMMON_

