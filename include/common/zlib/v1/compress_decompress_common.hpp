
// zlib_compress_decompress_common.hpp
// to include  ->  #include "zlib_compress_decompress_common.hpp"
// 2018 Feb 12

#ifndef common_zlib_v1_compress_decompress_common_hpp
#define common_zlib_v1_compress_decompress_common_hpp


#include <zlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string>

namespace common{ namespace zlib{ namespace v1{
struct SRawCompressedFileItem{
	uint16_t	itemPathLengthPlus1;
	union{
		struct{
			uint16_t	isDirectory : 1;
			uint16_t	shouldRootDirBeAdded : 1;
			uint16_t	userRWX : 3;
			uint16_t	groupRWX : 3;
			uint16_t	otherRWX : 3;
			uint16_t	reserved16bitwise01 : 5;
		}bits;
		uint16_t	allBits;
	}stats;
	uint32_t	fileSize ;
};

struct SCompressedFileItem{
	SRawCompressedFileItem		raw;
	::std::u32string			itemFullPath;
};

}}} // namespace common{ namespace zlib{ namespace v1{

#define NULL_ITEM_FROM_PATH_V1(_itemPath)				{{0,{{0,0,0,0,0,0}},0},(_itemPath)}


#endif  // #ifndef common_zlib_v1_compress_decompress_common_hpp
