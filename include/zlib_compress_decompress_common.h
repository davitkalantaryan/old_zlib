
// zlib_compress_decompress_common.h
// to include  ->  #include "zlib_compress_decompress_common.h"
// 2018 Feb 12

#ifndef __zlib_compress_decompress_common_h__
#define __zlib_compress_decompress_common_h__

#include <stdint.h>
#include <zlib.h>
#include <stdio.h>

#define LAST_STRING_IN_THE_FILE		"End"
#define LAST_STRING_IN_THE_FILE_LEN	4

#define DEF_CHUNK_SIZE				16384


#ifdef __cplusplus
extern "C"{
#endif

// padding is 8
typedef struct SFileItem
{
	uint32_t fileSize;
	uint16_t folderNum, fileNameLen/* increased to 8* */;
}SFileItem;

typedef struct SCompressDecompressHeader
{
	uint32_t wholeHeaderSizeInBytes;
	uint32_t numberOfItems;
}SCompressDecompressHeader;

#define LEN_FROM_ITEM(_item)	(  sizeof(SFileItem)+(_item)->fileNameLen    )
#define ITEM_NAME(_item)		(  ((char*)(_item))+sizeof(SFileItem)  )

SCompressDecompressHeader* ZlibCreateCompressDecompressHeader(uint32_t headerSize, uint32_t numberOfItems);
void DestroyCompressDecompressHeader(SCompressDecompressHeader* header);


#ifdef __cplusplus
}
#endif


#endif  // #ifndef __zlib_compress_decompress_common_h__
