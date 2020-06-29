//
// file:			decompress_common_in_versions_private.hpp
// created on:		2020 Jun 29
// created by:		D. Kalantaryan (davit.kalantaryan@gmail.com)
//

#ifndef DECOMPRESS_COMMON_IN_VERSION_PRIVATE_HPP
#define DECOMPRESS_COMMON_IN_VERSION_PRIVATE_HPP

#include <stdint.h>

namespace zlib { namespace privateN{

void SwapStatic(uint16_t* a_pDataToSwap);
void SwapStatic(uint32_t* a_pDataToSwap);
void SwapStatic(uint64_t* a_pDataToSwap);

}}  // namespace zlib { namespace privateN{


#endif  // #ifndef DECOMPRESS_COMMON_IN_VERSION_PRIVATE_HPP
