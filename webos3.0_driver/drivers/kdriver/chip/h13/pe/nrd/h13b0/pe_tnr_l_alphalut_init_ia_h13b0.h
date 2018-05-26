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

const PE_REG_PARAM_T tnr_l_alphalut_init_ia_h13b0[] =
{
	{0x04D0, 0x00001000},	// # [12]AI enable [2:0]Indirect Address
	{0x04D4, 0xFF583220},	// # Data MSB
	{0x04D8, 0x1A100908},	// # Data LSB (Trigger) Addr=0: L
	{0x04D4, 0xFFFFFFFF},	// # Data MSB
	{0x04D8, 0xFFFFFFFF},	// # Data LSB (Trigger) Addr=0: H
	{0x04D4, 0xFF65412E},	// # Data MSB
	{0x04D8, 0x24110C04},	// # Data LSB (Trigger) Addr=1: L
	{0x04D4, 0xFFFFFFFF},	// # Data MSB
	{0x04D8, 0xFFFFFFFF},	// # Data LSB (Trigger) Addr=1: H
	{0x04D4, 0xFFA15F41},	// # Data MSB
	{0x04D8, 0x31180C04},	// # Data LSB (Trigger) Addr=2: L
	{0x04D4, 0xFFFFFFFF},	// # Data MSB
	{0x04D8, 0xFFFFFFFF},	// # Data LSB (Trigger) Addr=2: H
	{0x04D4, 0xFFFB8658},	// # Data MSB
	{0x04D8, 0x411E0804},	// # Data LSB (Trigger) Addr=3: L
	{0x04D4, 0xFFFFFFFF},	// # Data MSB
	{0x04D8, 0xFFFFFFFF},	// # Data LSB (Trigger) Addr=3: H
	{0x04D4, 0xFFFB9664},	// # Data MSB
	{0x04D8, 0x4D280C04},	// # Data LSB (Trigger) Addr=4: L
	{0x04D4, 0xFFFFFFFF},	// # Data MSB
	{0x04D8, 0xFFFFFFFF},	// # Data LSB (Trigger) Addr=4: H
	{0x04D4, 0xFFFCAE77},	// # Data MSB
	{0x04D8, 0x59320E04},	// # Data LSB (Trigger) Addr=5: L
	{0x04D4, 0xFFFFFFFF},	// # Data MSB
	{0x04D8, 0xFFFFFFFF},	// # Data LSB (Trigger) Addr=5: H
	{0x04D4, 0xFFFDC799},	// # Data MSB
	{0x04D8, 0x6F3B1304},	// # Data LSB (Trigger) Addr=6: L
	{0x04D4, 0xFFFFFFFF},	// # Data MSB
	{0x04D8, 0xFFFFFFFF},	// # Data LSB (Trigger) Addr=6: H
	{0x04D4, 0xFFFDC799},	// # Data MSB
	{0x04D8, 0x6F3B1304},	// # Data LSB (Trigger) Addr=7: L
	{0x04D4, 0xFFFFFFFF},	// # Data MSB
	{0x04D8, 0xFFFFFFFF},	// # Data LSB (Trigger) Addr=7: H
	{0x04D4, 0x807F7C76},	// # Data MSB
	{0x04D8, 0x6F4F4C41},	// # Data LSB (Trigger) Addr=8: L
	{0x04D4, 0x80808080},	// # Data MSB
	{0x04D8, 0x80808080},	// # Data LSB (Trigger) Addr=8: H
	{0x04D4, 0x807F7C76},	// # Data MSB
	{0x04D8, 0x6F4F4C38},	// # Data LSB (Trigger) Addr=9: L
	{0x04D4, 0x80808080},	// # Data MSB
	{0x04D8, 0x80808080},	// # Data LSB (Trigger) Addr=9: H
	{0x04D4, 0x807D786F},	// # Data MSB
	{0x04D8, 0x61373531},	// # Data LSB (Trigger) Addr=A: L
	{0x04D4, 0x80808080},	// # Data MSB
	{0x04D8, 0x80808080},	// # Data LSB (Trigger) Addr=A: H
	{0x04D4, 0x807D786F},	// # Data MSB
	{0x04D8, 0x5D303028},	// # Data LSB (Trigger) Addr=B: L
	{0x04D4, 0x80808080},	// # Data MSB
	{0x04D8, 0x80808080},	// # Data LSB (Trigger) Addr=B: H
	{0x04D4, 0x807D7A6F},	// # Data MSB
	{0x04D8, 0x60292821},	// # Data LSB (Trigger) Addr=C: L
	{0x04D4, 0x80808080},	// # Data MSB
	{0x04D8, 0x80808080},	// # Data LSB (Trigger) Addr=C: H
	{0x04D4, 0x807D776D},	// # Data MSB
	{0x04D8, 0x5C282821},	// # Data LSB (Trigger) Addr=D: L
	{0x04D4, 0x80808080},	// # Data MSB
	{0x04D8, 0x80808080},	// # Data LSB (Trigger) Addr=D: H
	{0x04D4, 0x807D776D},	// # Data MSB
	{0x04D8, 0x5A28211F},	// # Data LSB (Trigger) Addr=E: L
	{0x04D4, 0x80808080},	// # Data MSB
	{0x04D8, 0x80808080},	// # Data LSB (Trigger) Addr=E: H
	{0x04D4, 0x807A776F},	// # Data MSB
	{0x04D8, 0x6128211F},	// # Data LSB (Trigger) Addr=F: L
	{0x04D4, 0x80808080},	// # Data MSB
	{0x04D8, 0x80808080}	// # Data LSB (Trigger) Addr=F: H
};