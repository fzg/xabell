/*
 * $File: sdevice.h
 * $Date: Sun Jun 12 20:18:17 2011 +0800
 *
 * sound device class
 *
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

#ifndef _HEADER_SDEVICE_
#define _HEADER_SDEVICE_

class SoundDevice
{
		void *m_p_handle;
		void set_params();
	public:
		typedef short int  Sample_t;
		static const Sample_t SAMPLE_VAL_MAX;
		SoundDevice(unsigned int rate, int nchannel);
		~SoundDevice();

		void write(const Sample_t *buf, int nframe);
};

#endif

