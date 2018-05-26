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

const PE_REG_PARAM_T ifc_l_atv_ntsc_default_m14b0[] =
{
	/* pe0d ifc, 0x3B14~0x3B68 */
	{0x3B14, 0x00E8D800}, // IFC_CTRL_0
	{0x3B18, 0x0C004C20}, // IFC_CTRL_1
	{0x3B1C, 0x00000000}, // IFC_CTRL_2
	{0x3B20, 0x11006E20}, // IFC_CTRL_3
	{0x3B24, 0x00031203}, // IFC_CTRL_4
	{0x3B28, 0x00006616}, // IFC_CTRL_5
	{0x3B2C, 0x00000000}, // IFC_CTRL_6
	{0x3B30, 0x00000000}, // IFC_CTRL_7
	{0x3B34, 0x24000001}, // IFC_CTRL_8
	{0x3B38, 0x00000000}, // IFC_CTRL_9
	{0x3B3C, 0x00000000}, // IFC_CTRL_10
	{0x3B40, 0x00000000}, // IFC_CTRL_11
	{0x3B44, 0x80040000}, // IFC_CTRL_12
	{0x3B48, 0x00000000}, // IFC_CTRL_13
	{0x3B4C, 0x00000000}, // IFC_CTRL_14
	{0x3B50, 0x064011FF}, // IFC_CTRL_15
	{0x3B54, 0x001002A2}, // IFC_CTRL_16
	{0x3B58, 0x8500DCFF}, // IFC_CTRL_17
	{0x3B5C, 0x00000000}, // IFC_CTRL_18
	{0x3B60, 0x21000000}, // IFC_CTRL_19
	{0x3B64, 0x00000000}, // IFC_CTRL_20
	{0x3B68, 0x0080B000}  // IFC_CTRL_21
};
