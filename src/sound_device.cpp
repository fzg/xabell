/*
 * $File: sound_device.cpp
 * $Date: Sun Jun 12 18:58:58 2011 +0800
 *
 * sound device class
 *
 */

#include "sound_device.h"
#include "common.h"

#include <alsa/asoundlib.h>

static const char *PCM_DEVICE_NAME = "fileout";

#define GET_HANDLE() static_cast<snd_pcm_t*>(this->m_p_handle)

SoundDevice::SoundDevice(unsigned int rate) : m_p_handle(NULL)
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
					1,	// channels
					rate,
					1,	// allow resampling
					0	// required overall latency in us 
					)) < 0)
		throw Error("failed to set playback parameters: %s",
				snd_strerror(err));

	m_p_handle = handle;
}

void SoundDevice::write(const Sample_t *buf, int count)
{
	while (count)
	{
		snd_pcm_sframes_t frames = snd_pcm_writei(GET_HANDLE(),
				buf, count);
		if (frames < 0)
			frames = snd_pcm_recover(GET_HANDLE(), frames, 0);
		if (frames < 0)
			throw Error("snd_pcm_writei() failed: %s", snd_strerror(frames));

		if (!frames)
			throw Error("zero write!");

		count -= frames;
		buf += frames;
	}
}

SoundDevice::~SoundDevice()
{
	if (m_p_handle)
		snd_pcm_close(GET_HANDLE());
}

