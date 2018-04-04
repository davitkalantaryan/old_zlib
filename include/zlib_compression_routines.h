

// zlib_compression_routines.h
// to include ->  #include "zlib_compression_routines.h"
// 2018 Feb 12

#ifndef __zlib_compression_routines_h__
#define __zlib_compression_routines_h__

#include <zlib_compress_decompress_common.h>

#define		FIND_FILE_SIZE_LATER	-2018


#ifdef __cplusplus
extern "C" {
#endif

typedef struct SFileItemList
{
	SFileItem*		item;
	SFileItemList*	next;
	FILE*			file;
}SFileItemList;
typedef struct SCompressList{ SFileItemList	*first, *last; }SCompressList;

#define Init_SCompressList(_list)	do{(_list)->first=(_list)->last=NULL;}while(0)

// return 0, continues, non 0 stops
typedef int (*typeCompressCallback)(const void*buffer,int bufLen,void*userData);

SFileItemList*	ZlibCreateListItemCompress(const char* a_cpcFileName,
	uint16_t fileNameLen,int a_isDir, const char* a_fullPath);

int ZlibCompressBufferToCallback(
	z_stream* a_strm, int a_flush,
	void* a_out, int a_outBufferSize,
	typeCompressCallback a_clbk, void* a_userData);
int ZlibCompressBufferToFile(
	z_stream* a_strm, int a_flush,
	void* a_out, int a_outBufferSize,
	FILE *a_dest);
int ZlibCompressFileRawEx(
	z_stream* a_strm,
	FILE * a_source, FILE * a_dest,
	void* a_in, int a_inBufferSize,
	void* a_out, int a_outBufferSize,
	int a_nFlushInTheEnd);
int ZlibCompressFileRaw(FILE * a_source, FILE * a_dest, int a_nCompressionLeel);
int ZlibCompressFolderEx(const SCompressList* a_list, uint16_t a_headerSize, uint16_t a_numberOfItems, FILE *a_dest, int a_level);
int ZlibCompressFolder(const char* a_directoryPath, FILE *a_dest, int a_level);


#ifdef __cplusplus
}
#endif



#endif  // #ifndef __zlib_compression_routines_h__