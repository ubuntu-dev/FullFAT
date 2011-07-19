/*****************************************************************************
 *  FullFAT - High Performance, Thread-Safe Embedded FAT File-System         *
 *                                                                           *
 *  Copyright(C) 2009 James Walmsley  (james@fullfat-fs.co.uk)               *
 *	Copyright(C) 2010 Hein Tibosch    (hein_tibosch@yahoo.es)                *
 *                                                                           *
 *  See RESTRICTIONS.TXT for extra restrictions on the use of FullFAT.       *
 *                                                                           *
 *  Removing this notice is illegal and will invalidate this license.        *
 *****************************************************************************
 *  See http://www.fullfat-fs.co.uk/ for more information.                   *
 *  Or  http://fullfat.googlecode.com/ for latest releases and the wiki.     *
 *****************************************************************************
 * As of 19-July-2011 FullFAT has abandoned the GNU GPL License in favour of *
 * the more flexible Apache 2.0 license. See License.txt for full terms.     *
 *                                                                           *
 *            YOU ARE FREE TO USE FULLFAT IN COMMERCIAL PROJECTS             *
 *****************************************************************************/

/**
 *	@file		ff_crc.h
 *	@author		James Walmsley
 *	@ingroup	CRC
 *
 **/

#ifndef _FF_CRC_H_
#define _FF_CRC_H_

#include "ff_types.h"
#include "ff_config.h"

FF_T_UINT8	FF_GetCRC8	(FF_T_UINT8 *pbyData, FF_T_UINT32 stLength);
FF_T_UINT16 FF_GetCRC16	(FF_T_UINT8 *pbyData, FF_T_UINT32 stLength);
FF_T_UINT32 FF_GetCRC32	(FF_T_UINT8 *pbyData, FF_T_UINT32 stLength);

#endif

