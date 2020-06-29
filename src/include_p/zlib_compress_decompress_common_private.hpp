
// zlib_compress_decompress_common_private.hpp
// to include  ->  #include "zlib_compress_decompress_common_private.hpp"
// 2018 Feb 12

#ifndef zlib_compress_decompress_common_private_hpp
#define zlib_compress_decompress_common_private_hpp


#include <zlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string>

struct SRawCompressHeader{
	uint16_t	endian;
	union{
		struct{
			uint16_t	isDirectory : 1;
			uint16_t	isEndianDiscovered : 1;
			uint16_t	reserved16bitwise01 : 14;
		}bits;
		uint16_t	allBits;
	}stats;
	uint32_t	itemsHeaderSizeOrFileSize;
	uint16_t	version;
	uint16_t	reserved16bit01;
	uint32_t	reserved32bit01;
};


#define INIT_CD_MAIN_HEADER(_isDir)						{1,{{(_isDir)?1:0,0,0}},0,0,0,0}


#endif  // #ifndef zlib_compress_decompress_common_private_hpp
