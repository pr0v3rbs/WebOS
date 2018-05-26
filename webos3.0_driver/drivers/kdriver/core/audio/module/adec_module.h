/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
*/

#ifndef __ADEC_MODULE_H__
#define __ADEC_MODULE_H__

#include "common/adec_common.h"
#include "adec_module_list.h"

#define ADEC_TAG(__1ST__, __2ND__, __3RD__, __4TH__)\
	((__4TH__ << 24) | (__3RD__ << 16) | (__2ND__ << 8 ) | (__1ST__))

#define MOD_IN_PORT_MIN 0
#define MOD_IN_PORT_MAX 0xff
#define MOD_IN_PORT(__X__) (__X__)
#define MOD_IN_PORT_ORDER(__X__) (__X__)
#define VALIDATE_IN_PORT(__X__) ((__X__ & 0xffffff00) == 0x00)

#define MOD_OUT_PORT_MIN 0x100
#define MOD_OUT_PORT_MAX 0x1ff
#define MOD_OUT_PORT(__X__) ((((__X__) & 0x1f)<<3) | 0x100)
#define MOD_OUT_PORT_ORDER(__X__) (((__X__) >> 3) & 0x1f)
#define MOD_OUT_PORT_CLIENT(__X__) ((__X__) & 0x7)
#define MOD_OUT_PORT_PHYSIC(__X__) ((__X__) & 0x1f8)
#define VALIDATE_OUT_PORT(__X__) (((__X__)& 0xffffff00) == 0x100)

#define MOD_REF_PORT_MIN 0x200
#define MOD_REF_PORT_MAX 0x2ff
#define MOD_REF_PORT(__X__) (((__X__)&0xff) | 0x200)
#define MOD_REF_PORT_ORDER(__X__) ((__X__) & 0xff)
#define VALIDATE_REF_PORT(__X__) (((__X__) & 0xffffff00) == 0x200)

typedef enum _ADEC_MODULE_STATE
{
	ADEC_MOD_STATE_NOT_DEF,
	ADEC_MOD_STATE_READY,
	ADEC_MOD_STATE_RUNNING,
	ADEC_MOD_STATE_PENDING,
}ADEC_MODULE_STATE;

typedef union _AdecModuleTag
{
	int		tag;
	char	str[4];
}AdecModuleTag;

#define MOD_SET_BUFFER(__MOD__, __PORT_NUM__, __BUFFER__) \
	(__MOD__->set_buffer(__MOD__, __PORT_NUM__, __BUFFER__))

#define MOD_GET_BUFFER(__MOD__, __PORT_NUM__) \
	(__MOD__->get_buffer(__MOD__, __PORT_NUM__))

#define MOD_PROCESS(__MOD__) ((__MOD__)->process((__MOD__)))

typedef struct _AdecModule
{
	AdecModuleTag		module_tag;
	ADEC_MODULE_ID		module_id;
	char				module_name[ADEC_CNST_MODULE_NAME_LENGTH+1];

	unsigned int		num_of_output;
	unsigned int		num_of_input;
	unsigned int		num_of_ref;

	ADEC_MODULE_STATE	state;
	unsigned int		rate;

	ADEC_RESULT (*set_buffer)(
		struct _AdecModule *_module,
		int _portNum,
		void *_buffer);

	void* (*get_buffer)(
		struct _AdecModule *_module,
		int _portNum);

	ADEC_RESULT (*process)(
		struct _AdecModule *_module);

	void (*cmd)(
		void *_module,
		int _cmd,
		void *_sender,
		void *_param,
		int _paramLen);

	void (*terminator)(
		void *_module);

	void *module_specific;

}AdecModule;


#endif
