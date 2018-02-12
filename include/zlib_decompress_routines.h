
// zlib_decompress_routines.h
// to include ->  #include "zlib_decompress_routines.h"
// 2018 Feb 12

#ifndef __zlib_decompress_routines_h__
#define __zlib_decompress_routines_h__

#include <zlib_compress_decompress_common.h>


#ifdef __cplusplus
extern "C" {
#endif

// return 0, continues, non 0 stops
typedef int(*typeDecompressCallback)(const void*buffer, int bufLen, void*userData);

int ZlibDecompressBufferToCallback(
	z_stream* a_strm,
	void* a_out, int a_outBufferSize,
	typeDecompressCallback a_clbk, void* a_userData);
int ZlibDecompressBufferToFile(
	z_stream* a_strm,
	void* a_out, int a_outBufferSize,
	FILE *a_dest);
int ZlibDecompressFile(FILE *source, FILE *dest);
int ZlibDecompressFolder(FILE *a_source, const char* a_outDirectoryPath);

#ifdef __cplusplus
}
#endif


#endif  // #ifndef __zlib_decompress_routines_h__
