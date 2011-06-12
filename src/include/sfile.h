/*
 * $File: sfile.h
 * $Date: Sun Jun 12 23:26:04 2011 +0800
 *
 * sound file class
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

#ifndef _HEADER_SFILE_
#define _HEADER_SFILE_

#include "sdevice.h"

class SoundFile
{
		SoundDevice::Sample_t *m_buf;
		int m_nframe, m_samplerate, m_nchannel;
		double m_sp_time, m_prev_play_time;

		static void* play_thread(const SoundFile *file);
	public:
		SoundFile(const char *path, double suppress_time);
		// suppress_time: interval (seconds) during which subsequent
		// play requests are ignored
		~SoundFile();

		// play that file in a new thread
		void play();
};

#endif
