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

const PE_REG_PARAM_T ifc_l_init_h15a0[] =
{
	/* nd dnr ifc, 0x7214~0x7268 */
	{0x7214, 0x00E8D800}, // IFC_CTRL_0
	{0x7218, 0x0C003820}, // IFC_CTRL_1
	{0x721C, 0x00000000}, // IFC_CTRL_2
	{0x7220, 0x11006E20}, // IFC_CTRL_3
	{0x7224, 0x00031203}, // IFC_CTRL_4
	{0x7228, 0x00006616}, // IFC_CTRL_5
	{0x722C, 0x00000000}, // IFC_CTRL_6
	{0x7230, 0x00000000}, // IFC_CTRL_7
	{0x7234, 0x24000000}, // IFC_CTRL_8
	{0x7238, 0x00000000}, // IFC_CTRL_9
	{0x723C, 0x00000000}, // IFC_CTRL_10
	{0x7240, 0x00000000}, // IFC_CTRL_11
	{0x7244, 0x80040000}, // IFC_CTRL_12
	{0x7248, 0x00000000}, // IFC_CTRL_13
	{0x724C, 0x00000000}, // IFC_CTRL_14
	{0x7250, 0x064011FF}, // IFC_CTRL_15
	{0x7254, 0x001002A2}, // IFC_CTRL_16
	{0x7258, 0x8500DCFF}, // IFC_CTRL_17
	{0x725C, 0x00000000}, // IFC_CTRL_18
	{0x7260, 0x21000000}, // IFC_CTRL_19
	{0x7264, 0x00000000}, // IFC_CTRL_20
	{0x7268, 0x0080B000}  // IFC_CTRL_21
};
