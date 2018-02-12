
// zlib_compress_decompress_common.cpp
// 2018 Feb 12

#include "zlib_compress_decompress_common.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

SCompressDecompressHeader* ZlibCreateCompressDecompressHeader(uint32_t a_headerSize, uint32_t a_numberOfItems)
{
	SCompressDecompressHeader* pCompressDecompressHeader = (SCompressDecompressHeader*)malloc(a_headerSize);
	if (!pCompressDecompressHeader) { return pCompressDecompressHeader; }
	pCompressDecompressHeader->numberOfItems = a_numberOfItems;
	pCompressDecompressHeader->wholeHeaderSizeInBytes = a_headerSize;
	return pCompressDecompressHeader;
}


void DestroyCompressDecompressHeader(SCompressDecompressHeader* a_header)
{
	free(a_header);
}


#ifdef __cplusplus
}
#endif

