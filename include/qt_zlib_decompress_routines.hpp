
// qt_zlib_decompress_routines.hpp
// to include ->  #include "qt_zlib_decompress_routines.hpp"
// 2018 Feb 12

#ifndef qt_zlib_decompress_routines_hpp
#define qt_zlib_decompress_routines_hpp

#include <qt_zlib_compress_decompress_common.hpp>


namespace qt{

bool DecompressFile(const QString& a_compressedFilePath, const QString& targetFolderOrFilePath);

}

#endif  // #ifndef qt_zlib_decompress_routines_hpp
