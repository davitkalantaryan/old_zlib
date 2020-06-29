

// qt_zlib_compression_routines.hpp
// to include ->  #include "qt_zlib_compression_routines.hpp"
// 2018 Feb 12

#ifndef common_zlib_v1_compression_routines_hpp
#define common_zlib_v1_compression_routines_hpp

#include <common/zlib/v1/compress_decompress_common.hpp>

namespace common{ namespace zlib{ namespace v1{

bool CompressFileOrFolder(const ::std::u32string& a_fileOrFolderPath, const ::std::u32string& targetFilePath, int a_compressionLevel);

}}}  // namespace common{ namespace zlib{ namespace v1{




#endif  // #ifndef common_zlib_v1_compression_routines_hpp
