

// zlib_compression_routines.h
// to include ->  #include "zlib_compression_routines.h"
// 2018 Feb 12

#ifndef qt_zlib_compression_routines_hpp
#define qt_zlib_compression_routines_hpp

#include <qt_zlib_compress_decompress_common.hpp>
#include <QString>


namespace qt{

bool CompressFolder(const QString& a_foldePath, const QString& targetFilePath, int a_compressionLevel);

}




#endif  // #ifndef qt_zlib_compression_routines_hpp
