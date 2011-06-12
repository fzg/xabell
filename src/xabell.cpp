/*
 * $File: xabell.cpp
 * $Date: Sun Jun 12 23:27:58 2011 +0800
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

/* ***********************************************************************************
 * special thanks to xbelld (http://code.google.com/p/xbelld/) developed by gi1242
 * ***********************************************************************************/

#include "sfile.h"
#include "common.h"

#include <cstdio>
#include <cstdlib>

#include <X11/XKBlib.h>

static Display* open_display(int &xkb_event_code);

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s <bell file> <suppress interval (ms)>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}
	Display *dpy = NULL;
	try
	{
		int xkb_event_code;
		SoundFile sfile(argv[1], atoi(argv[2]) * 1e-3);
		dpy = open_display(xkb_event_code);
		for (; ;)
		{
			XkbEvent event;
			XNextEvent(dpy, &event.core);
			debug_output("got event type=%d (xkb_event_code=%d)\n", event.type, xkb_event_code);
			if (event.type == xkb_event_code)
				sfile.play();
		}
	}
	catch (Error &e)
	{
		fprintf(stderr, "%s: FATAL: %s\n",
				PROGRAM_NAME, e.get_msg());
	}
	if (dpy)
		XCloseDisplay(dpy);
}

Display* open_display(int &xkb_event_code)
{
    int mjr = XkbMajorVersion,
		mnr = XkbMinorVersion,
		xkb_error;
	Display *dpy = XkbOpenDisplay(NULL, &xkb_event_code, NULL, &mjr, &mnr, &xkb_error);
	if (dpy == NULL)
	{
		switch (xkb_error)
		{
			case XkbOD_BadLibraryVersion:
				throw Error("require Xkb version %d.%02d (got library %d.%02d)",
						XkbMajorVersion, XkbMinorVersion, mjr, mnr);

			case XkbOD_ConnectionRefused:
				throw Error("cannot open display");

			case XkbOD_NonXkbServer:
				throw Error("XKB extension not present");

			case XkbOD_BadServerVersion:
				throw Error("require Xkb version %d.%02d (got server %d.%02d)",
						XkbMajorVersion, XkbMinorVersion, mjr, mnr);

			default:
				throw Error("unknown error %d from XkbOpenDisplay", xkb_error);
		}
	}
    XkbSelectEvents(dpy, XkbUseCoreKbd, XkbBellNotifyMask, XkbBellNotifyMask);

	if(!XkbChangeEnabledControls(dpy, XkbUseCoreKbd, XkbAudibleBellMask, 0))
	    throw Error("cannot disable audible bell");

	return dpy;
}

