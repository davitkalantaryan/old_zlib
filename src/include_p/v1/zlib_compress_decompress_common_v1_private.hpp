
// zlib_compress_decompress_common_private.hpp
// to include  ->  #include "zlib_compress_decompress_common_private.hpp"
// 2018 Feb 12

#ifndef zlib_compress_decompress_common_v1_private_hpp
#define zlib_compress_decompress_common_v1_private_hpp


#include <zlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string>

namespace v1{

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

} // namespace v1{


#define ITEM_FILE_NAME_FROM_ITEM_PTR_V1(_itemPtr)		reinterpret_cast<const char32_t*>( reinterpret_cast<const char*>(_itemPtr) + sizeof(::common::zlib::v1::SRawCompressedFileItem) )
#define ITEM_TOTAL_LENGTH_FROM_ITEM_PTR_V1(_itemPtr)	( ((sizeof(::common::zlib::v1::SRawCompressedFileItem)+static_cast<size_t>(sizeof(char32_t)*(_itemPtr)->itemPathLengthPlus1)+7)/8)*8 )


#endif  // #ifndef zlib_compress_decompress_common_v1_private_hpp
