

// zlib_compression_routines.h
// to include ->  #include "zlib_compression_routines.h"
// 2018 Feb 12

#ifndef __zlib_compression_routines_h__
#define __zlib_compression_routines_h__

#include <zlib_compress_decompress_common.h>


#ifdef __cplusplus
extern "C" {
#endif

// return 0, continues, non 0 stops
typedef int (*typeCompressCallback)(const void*buffer,int bufLen,void*userData);

int ZlibCompressBufferToCallback(
	z_stream* a_strm, int a_flush,
	void* a_out, int a_outBufferSize,
	typeCompressCallback a_clbk, void* a_userData);
int ZlibCompressBufferToFile(
	z_stream* a_strm, int a_flush,
	void* a_out, int a_outBufferSize,
	FILE *a_dest);
int ZlibCompressFileEx(
	z_stream* a_strm,
	FILE * a_source, FILE * a_dest,
	void* a_in, int a_inBufferSize,
	void* a_out, int a_outBufferSize,
	int a_nFlushInTheEnd);
int ZlibCompressFile(FILE * a_source, FILE * a_dest, int a_nCompressionLeel);
int ZlibCompressFolder(const char* a_directoryPath, FILE *a_dest, int a_level);


#ifdef __cplusplus
}
#endif



#endif  // #ifndef __zlib_compression_routines_h__
