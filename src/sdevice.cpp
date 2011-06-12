/*
 * $File: sdevice.cpp
 * $Date: Sun Jun 12 20:24:54 2011 +0800
 *
 * sound device class
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

#include "sdevice.h"
#include "common.h"

#include <alsa/asoundlib.h>

#include <limits>

static const char *PCM_DEVICE_NAME = "default";

#define GET_HANDLE() static_cast<snd_pcm_t*>(this->m_p_handle)

const SoundDevice::Sample_t
	SoundDevice::SAMPLE_VAL_MAX = std::numeric_limits<Sample_t>::max();

SoundDevice::SoundDevice(unsigned int rate, int nchannel) :
	m_p_handle(NULL)
{
	snd_pcm_t *handle;
	int err;
	if ((err = snd_pcm_open(
					&handle,
					PCM_DEVICE_NAME,
					SND_PCM_STREAM_PLAYBACK,
					0)) < 0)
		throw Error("failed to open playback device: %s",
				snd_strerror(err));

	if ((err = snd_pcm_set_params(
					handle,
					SND_PCM_FORMAT_S16_LE,
					SND_PCM_ACCESS_RW_INTERLEAVED,
					nchannel,
					rate,
					1,	// allow resampling
					0	// required overall latency in us 
					)) < 0)
		throw Error("failed to set playback parameters: %s",
				snd_strerror(err));

	m_p_handle = handle;
}

void SoundDevice::write(const Sample_t *buf, int nframe)
{
	while (nframe)
	{
		snd_pcm_sframes_t ret = snd_pcm_writei(GET_HANDLE(),
				buf, nframe);
		if (ret < 0)
			ret = snd_pcm_recover(GET_HANDLE(), ret, 0);
		if (ret < 0)
			throw Error("snd_pcm_recover() failed: %s", snd_strerror(ret));

		if (!ret)
			throw Error("zero write!");

		nframe -= ret;
		buf += ret;
	}
}

SoundDevice::~SoundDevice()
{
	if (m_p_handle)
		snd_pcm_close(GET_HANDLE());
}

