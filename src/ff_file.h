/*****************************************************************************
 *  FullFAT - High Performance, Thread-Safe Embedded FAT File-System         *
 *                                                                           *
 *  Copyright(C) 2009 James Walmsley  (james@fullfat-fs.co.uk)               *
 *  Copyright(C) 2010 Hein Tibosch    (hein_tibosch@yahoo.es)                *
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
 *	@file		ff_file.h
 *	@author		James Walmsley
 *	@ingroup	FILEIO
 **/
#ifndef _FF_FILE_H_
#define _FF_FILE_H_

#include "ff_config.h"
#include "ff_types.h"
#include "ff_ioman.h"
#include "ff_dir.h"

#ifdef FF_USE_NATIVE_STDIO
#include <stdio.h>
#define FF_SEEK_SET	SEEK_SET
#define FF_SEEK_CUR	SEEK_CUR
#define FF_SEEK_END	SEEK_END
#else
#define FF_SEEK_SET	1
#define FF_SEEK_CUR	2
#define FF_SEEK_END	3
#endif

typedef struct _FF_FILE {
	FF_IOMAN		*pIoman;			///< Ioman Pointer!
	FF_T_UINT32		 Filesize;			///< File's Size.
	FF_T_UINT32		 ObjectCluster;		///< File's Start Cluster.
	FF_T_UINT32		 iChainLength;		///< Total Length of the File's cluster chain.
	FF_T_UINT32		 CurrentCluster;	///< Prevents FAT Thrashing.
	FF_T_UINT32		 AddrCurrentCluster;///< Address of the current cluster.
	FF_T_UINT32		 iEndOfChain;		///< Address of the last cluster in the chain.
	FF_T_UINT32		 FilePointer;		///< Current Position Pointer.
	//FF_T_UINT32	 AppendPointer;		///< Points to the Append from position. (The original filesize at open).
	FF_T_UINT32		 DirCluster;		///< Cluster Number that the Dirent is in.
	FF_T_UINT32		 ValidFlags;		///< Handle validation flags.
/*#ifdef	FF_REMOVABLE_MEDIA
		Invalidated : 1,
#endif
		FileDeleted : 1,
		SpareFlags : 30;*/
	FF_T_UINT16		 DirEntry;			///< Dirent Entry Number describing this file.
	FF_T_UINT8		 Mode;				///< Mode that File Was opened in.
	//FF_T_UINT8		 NumLFNs;			///< Number of LFNs associated with this file.
	struct _FF_FILE *Next;				///< Pointer to the next file object in the linked list.
} FF_FILE,
*PFF_FILE;

#define FF_VALID_FLAG_INVALID	0x00000001
#define FF_VALID_FLAG_DELETED	0x00000002

//---------- PROTOTYPES
// PUBLIC (Interfaces):

#ifdef FF_UNICODE_SUPPORT
FF_FILE *FF_Open(FF_IOMAN *pIoman, const FF_T_WCHAR *path, FF_T_UINT8 Mode, FF_ERROR *pError);
FF_T_BOOL	 FF_isDirEmpty	(FF_IOMAN *pIoman, const FF_T_WCHAR *Path);
FF_ERROR	 FF_RmFile		(FF_IOMAN *pIoman, const FF_T_WCHAR *path);
FF_ERROR	 FF_RmDir		(FF_IOMAN *pIoman, const FF_T_WCHAR *path);
FF_ERROR	 FF_Move		(FF_IOMAN *pIoman, const FF_T_WCHAR *szSourceFile, const FF_T_WCHAR *szDestinationFile);
#else
FF_FILE *FF_Open(FF_IOMAN *pIoman, const FF_T_INT8 *path, FF_T_UINT8 Mode, FF_ERROR *pError);
FF_T_BOOL	 FF_isDirEmpty	(FF_IOMAN *pIoman, const FF_T_INT8 *Path);
FF_ERROR	 FF_RmFile		(FF_IOMAN *pIoman, const FF_T_INT8 *path);
FF_ERROR	 FF_RmDir		(FF_IOMAN *pIoman, const FF_T_INT8 *path);
FF_ERROR	 FF_Move		(FF_IOMAN *pIoman, const FF_T_INT8 *szSourceFile, const FF_T_INT8 *szDestinationFile);
#endif

#ifdef FF_TIME_SUPPORT
enum {
	ETimeCreate = 1,
	ETimeMod = 2,
	ETimeAccess = 4,
	ETimeAll = 7
};
FF_ERROR FF_SetFileTime(FF_FILE *pFile, FF_SYSTEMTIME *pTime, FF_T_UINT aWhat);
#ifdef FF_UNICODE_SUPPORT
FF_ERROR FF_SetTime(FF_IOMAN *pIoman, const FF_T_WCHAR *path, FF_SYSTEMTIME *pTime, FF_T_UINT aWhat);
#else
FF_ERROR FF_SetTime(FF_IOMAN *pIoman, const FF_T_INT8 *path, FF_SYSTEMTIME *pTime, FF_T_UINT aWhat);
#endif
#endif	// FF_TIME_SUPPORT

FF_ERROR	 FF_Close		(FF_FILE *pFile);
FF_T_SINT32	 FF_GetC		(FF_FILE *pFile);
FF_T_SINT32  FF_GetLine		(FF_FILE *pFile, FF_T_INT8 *szLine, FF_T_UINT32 ulLimit);
FF_T_SINT32	 FF_Read		(FF_FILE *pFile, FF_T_UINT32 ElementSize, FF_T_UINT32 Count, FF_T_UINT8 *buffer);
FF_T_SINT32	 FF_Write		(FF_FILE *pFile, FF_T_UINT32 ElementSize, FF_T_UINT32 Count, FF_T_UINT8 *buffer);
FF_T_BOOL	 FF_isEOF		(FF_FILE *pFile);
FF_T_SINT32	 FF_BytesLeft	(FF_FILE *pFile); ///< Returns # of bytes left to read
FF_ERROR	 FF_Seek		(FF_FILE *pFile, FF_T_SINT32 Offset, FF_T_INT8 Origin);
FF_T_SINT32	 FF_PutC		(FF_FILE *pFile, FF_T_UINT8 Value);
FF_INLINE FF_T_UINT32	 FF_Tell		(FF_FILE *pFile)
{
	return pFile ? pFile->FilePointer : 0;
}

FF_T_UINT8	 FF_GetModeBits	(FF_T_INT8 *Mode);

FF_ERROR     FF_CheckValid (FF_FILE *pFile);   ///< Check if pFile is a valid FF_FILE pointer
#ifdef FF_REMOVABLE_MEDIA
FF_T_SINT32	 FF_Invalidate (FF_IOMAN *pIoman); ///< Invalidate all handles belonging to pIoman
#endif

// Private :

#endif
