/*
 * $File: sfile.cpp
 * $Date: Sun Jun 12 23:29:01 2011 +0800
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

#include "sfile.h"
#include "common.h"

#include <pthread.h>
#include <sndfile.h>
#include <sys/time.h>

#include <cstring>
#include <cerrno>

#define IMP_READF(type) \
	static sf_count_t sf_readf(SNDFILE *sndfile, type *ptr, sf_count_t frames) \
	{ \
		return sf_readf_##type(sndfile, ptr, frames); \
	}

IMP_READF(short)
IMP_READF(int)
IMP_READF(float)
IMP_READF(double)
#undef IMP_READF

static double get_time();

SoundFile::SoundFile(const char *path, double suppress_time) :
	m_sp_time(suppress_time), m_prev_play_time(-suppress_time)
{
	SF_INFO info;
	memset(&info, 0, sizeof(info));
	SNDFILE *fptr = sf_open(path, SFM_READ, &info);
	if (!fptr)
		throw Error("failed to open audio file (%s): %s", path,
				sf_strerror(fptr));
	m_nframe = info.frames;
	m_buf = new SoundDevice::Sample_t[info.frames * info.channels];

	int ret = sf_readf(fptr, m_buf, info.frames);
	if (ret != info.frames)
		throw Error("failed to read audio file (%s): sf_readf returned %d",
				path, ret);
	sf_close(fptr);

	m_samplerate = info.samplerate;
	m_nchannel = info.channels;
}

SoundFile::~SoundFile()
{
	delete []m_buf;
}

void SoundFile::play()
{
	if (m_sp_time)
	{
		double time = get_time();
		if (time - m_prev_play_time < m_sp_time)
		{
			debug_output("bell omitted\n");
			return;
		}
		m_prev_play_time = time;
	}
	pthread_t pt;
	int ret;
	if ((ret = pthread_create(&pt, NULL, (void* (*)(void*))SoundFile::play_thread, this)))
		throw Error("failed to create thread: %s", strerror(ret));
}

void* SoundFile::play_thread(const SoundFile *file)
{
	try
	{
		SoundDevice dev(file->m_samplerate, file->m_nchannel);
		dev.write(file->m_buf, file->m_nframe);
	} catch (Error &e)
	{
		fprintf(stderr, "%s: failed to play sound: %s\n",
				PROGRAM_NAME, e.get_msg());
	}
	return NULL;
}

double get_time()
{
	timeval tv;
	if (gettimeofday(&tv, NULL))
		throw Error("failed to get current time: %s", strerror(errno));
	return tv.tv_sec + tv.tv_usec * 1e-6;
}

