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

#ifndef _GPIO_REG_H_
#define _GPIO_REG_H_

#define GPIONDATA(n)		(*((volatile unsigned *)(GPION_BASE(n) + 0x3FC)))
#define GPIONDIR(n)			(*((volatile unsigned *)(GPION_BASE(n) + 0x400)))
#define	GPIONIS(n)			(*((volatile unsigned *)(GPION_BASE(n) + 0x404)))
#define	GPIONIBE(n)			(*((volatile unsigned *)(GPION_BASE(n) + 0x408)))
#define	GPIONIEV(n)			(*((volatile unsigned *)(GPION_BASE(n) + 0x40C)))
#define	GPIONIE(n)			(*((volatile unsigned *)(GPION_BASE(n) + 0x410)))
#define	GPIONRIS(n)			(*((volatile unsigned *)(GPION_BASE(n) + 0x414)))
#define	GPIONMIS(n)			(*((volatile unsigned *)(GPION_BASE(n) + 0x418)))
#define	GPIONIC(n)			(*((volatile unsigned *)(GPION_BASE(n) + 0x41C)))
#define	GPIONAFSEL(n)		(*((volatile unsigned *)(GPION_BASE(n) + 0x420)))
#define	GPIONPeriphID(n)	(*((volatile unsigned *)(GPION_BASE(n) + 0xFE0)))
#define	GPIONPeriphID1(n)	(*((volatile unsigned *)(GPION_BASE(n) + 0xFE4)))
#define	GPIONPeriphID2(n)	(*((volatile unsigned *)(GPION_BASE(n) + 0xFE8)))
#define	GPIONPeriphID3(n)	(*((volatile unsigned *)(GPION_BASE(n) + 0xFEC)))
#define	GPIONPCellID0(n)	(*((volatile unsigned *)(GPION_BASE(n) + 0xFF0)))
#define	GPIONPCellID1(n)	(*((volatile unsigned *)(GPION_BASE(n) + 0xFF4)))
#define	GPIONPCellID2(n)	(*((volatile unsigned *)(GPION_BASE(n) + 0xFF8)))
#define	GPIONPCellID3(n)	(*((volatile unsigned *)(GPION_BASE(n) + 0xFFC)))

#define GPION_BASE(n)		(_gpioBaseAddr[n])

#endif
