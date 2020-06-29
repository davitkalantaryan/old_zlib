
// qt_zlib_decompress_routines.hpp
// to include ->  #include "qt_zlib_decompress_routines.hpp"
// 2018 Feb 12

#ifndef common_zlib_v1_decompress_routines_hpp
#define common_zlib_v1_decompress_routines_hpp

#include <common/zlib/v1/compress_decompress_common.hpp>
#include <vector>


namespace common{ namespace zlib{ namespace v1{

bool DecompressFile(const ::std::u32string& a_compressedFilePath, const ::std::u32string& targetFolderOrFilePath, ::std::vector< SCompressedFileItem >* a_pVector);

}}} // namespace common{ namespace zlib{ namespace v1{

#endif  // #ifndef common_zlib_v1_decompress_routines_hpp
