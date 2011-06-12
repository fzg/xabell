/*
 * $File: sound_device.h
 * $Date: Sun Jun 12 18:58:46 2011 +0800
 *
 * sound device class
 *
 */

#ifndef _HEADER_SOUND_DEVICE_
#define _HEADER_SOUND_DEVICE_

class SoundDevice
{
		void *m_p_handle;
		void set_params();
	public:
		typedef short int  Sample_t;
		static const Sample_t SAMPLE_VAL_MAX = (1 << 15) - 1;
		SoundDevice(unsigned int rate = 48000);
		~SoundDevice();

		void write(const Sample_t *buf, int count);
};

#endif

