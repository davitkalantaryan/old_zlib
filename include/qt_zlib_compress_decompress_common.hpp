
// zlib_compress_decompress_common.h
// to include  ->  #include "zlib_compress_decompress_common.h"
// 2018 Feb 12

#ifndef qt_zlib_compress_decompress_common_hpp
#define qt_zlib_compress_decompress_common_hpp


#include <zlib.h>
#include <stdint.h>
#include <stddef.h>
#include <QString>

struct SRawCompressHeader{
	uint16_t	endian;
	uint16_t	isDirectory : 1;
	uint16_t	reserved16bitwise01 : 15;
	uint32_t	itemsLength;
	uint64_t	reserved64bit01;
};

struct SRawCompressedFileItem{
	uint16_t	itemPathLengthPlus1;
	uint16_t	isDirectory : 1;
	uint16_t	shouldRootDirBeAdded : 1;
	uint16_t	reserved16bitwise01 : 14;
	uint32_t	fileSize ;
};

namespace qt{

struct SCompressedFileItem{
	SRawCompressedFileItem	raw;
	QString					itemFullPath;
};

}  // namespace qt{

#define ITEM_FILE_NAME_FROM_ITEM_PTR(_itemPtr)		( reinterpret_cast<wchar_t*>(_itemPtr) + sizeof(SCompressedFileItem) )
#define ITEM_TOTAL_LENGTH_FROM_ITEM_PTR(_itemPtr)	( ((sizeof(SRawCompressedFileItem)+static_cast<size_t>(2*(_itemPtr)->itemPathLengthPlus1)+7)/8)*8 )
#define NULL_ITEM_FROM_PATH(_itemPath)				{{0,0,0,0,0},(_itemPath)}
#define INIT_CD_MAIN_HEADER(_isDir)					{1,(_isDir)?1:0,0,0,0}


#endif  // #ifndef qt_zlib_compress_decompress_common_hpp
