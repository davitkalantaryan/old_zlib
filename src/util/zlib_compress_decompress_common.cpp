
// zlib_compress_decompress_common.cpp
// 2018 Feb 12

#include "zlib_compress_decompress_common.h"
#include <stdlib.h>
#include <search.h>
#include <memory.h>

#ifdef __cplusplus
extern "C" {
#endif

SCompressDecompressHeader* ZlibCreateCompressDecompressHeader(uint32_t a_headerSize, uint32_t a_typeOfCompressedContent,uint32_t a_numberOfItems)
{
	SCompressDecompressHeader* pCompressDecompressHeader = (SCompressDecompressHeader*)malloc(a_headerSize);
	if (!pCompressDecompressHeader) { return pCompressDecompressHeader; }
	pCompressDecompressHeader->version = ZLIB_COMPR_DECOMPR_VERSION;
	pCompressDecompressHeader->numberOfItems = a_numberOfItems;
	pCompressDecompressHeader->wholeHeaderSizeInBytes = a_headerSize;
	pCompressDecompressHeader->typeOfCompressedContent = a_typeOfCompressedContent;
	return pCompressDecompressHeader;
}


SCompressDecompressHeader* ZlibCreateAndCopyComprDecomprHeader(const SCompressDecompressHeader* a_orgin, int a_nAll)
{
	SCompressDecompressHeader* pCompressDecompressHeader = (SCompressDecompressHeader*)malloc(a_orgin->wholeHeaderSizeInBytes);
	size_t unSize = a_nAll ? a_orgin->wholeHeaderSizeInBytes : sizeof(SCompressDecompressHeader);

	if (!pCompressDecompressHeader) { return pCompressDecompressHeader; }
	memcpy(pCompressDecompressHeader, a_orgin, unSize);
	return pCompressDecompressHeader;
}


void DestroyCompressDecompressHeader(SCompressDecompressHeader* a_header)
{
	free(a_header);
}


SDiskCompDecompHeader* DZlibCreateHeaderForDiscCompression(uint64_t a_diskSize, uint32_t a_diskInfoSize, const DRIVE_LAYOUT_INFORMATION_EX* a_driveInfo)
{
	uint16_t unWholeHeaderSizeInBytes = (uint16_t)a_diskInfoSize+sizeof(SDiskCompDecompHeader);
	SDiskCompDecompHeader* pDiskCompDecompHeader = (SDiskCompDecompHeader*)malloc(unWholeHeaderSizeInBytes);

	if (!pDiskCompDecompHeader) { return pDiskCompDecompHeader; }

	pDiskCompDecompHeader->version = ZLIB_COMPR_DECOMPR_VERSION;
	pDiskCompDecompHeader->wholeHeaderSizeInBytes = unWholeHeaderSizeInBytes;
	pDiskCompDecompHeader->isAnyPartAvalible = 0;
	pDiskCompDecompHeader->diskSize = a_diskSize;

	if(a_diskInfoSize){memcpy(DISK_INFO_FROM_ITEM(pDiskCompDecompHeader),a_driveInfo, a_diskInfoSize);}
	return pDiskCompDecompHeader;
}


SDiskCompDecompHeader* DZlibResizeHeaderForDiscDecompression(SDiskCompDecompHeader** a_initial)
{
	SDiskCompDecompHeader* pDiskCompDecompHeader = (SDiskCompDecompHeader*)realloc(*a_initial, (*a_initial)->wholeHeaderSizeInBytes);
	if(pDiskCompDecompHeader){*a_initial= pDiskCompDecompHeader;}
	return pDiskCompDecompHeader;
}


static int CompareStat(const void *a_arg1, const void *a_arg2)
{
	return (int)(   ((PARTITION_INFORMATION_EX*)a_arg1)->StartingOffset.QuadPart - ((PARTITION_INFORMATION_EX*)a_arg2)->StartingOffset.QuadPart   );
}


void SortDiscCompressDecompressHeader(SDiskCompDecompHeader* a_header)
{
	DRIVE_LAYOUT_INFORMATION_EX* pInfo = DISK_INFO_FROM_ITEM(a_header);
		
	if(pInfo->PartitionCount){
		PARTITION_INFORMATION_EX* pMax;
		PARTITION_INFORMATION_EX* pPartitions = pInfo->PartitionEntry;
		int64_t llnMax, llnNew;
		DWORD i;

		qsort(pPartitions, pInfo->PartitionCount, sizeof(PARTITION_INFORMATION_EX), &CompareStat);
		
		a_header->isAnyPartAvalible = 0;
		pMax = NULL;
		llnMax = MINIMUM_DISK_SIZE_TO_COMPRESS;
		
		for (i = 0; i < pInfo->PartitionCount;++i){
			llnNew = pPartitions[i].StartingOffset.QuadPart + pPartitions[i].PartitionLength.QuadPart;
			if(llnNew>llnMax){
				if(pMax){pMax->RewritePartition = 0;}
				else{a_header->isAnyPartAvalible=1;}
				pPartitions[i].RewritePartition = 1;
				pMax = &pPartitions[i];
				llnMax = llnNew;
			}
			else{pPartitions[i].RewritePartition = 0;}
		}
	}

}


void DestroyHeaderForDiscCompression(SDiskCompDecompHeader* a_header)
{
	free(a_header);
}


#ifdef __cplusplus
}
#endif
