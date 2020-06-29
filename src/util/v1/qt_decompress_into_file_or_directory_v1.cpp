//
// file:			qt_compress_directory.cpp
// created on:		2020 Jun 04
// created by:		D. Kalantaryan (davit.kalantaryan@gmail.com)
// 

#include <common/zlib/v1/decompress_routines.hpp>
#include <zlib_compress_decompress_common_private.hpp>
#include <v1/zlib_compress_decompress_common_v1_private.hpp>
#include <vector>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QByteArray>
#include <string>
#include <QDir>
#include <decompress_common_in_versions_private.hpp>

#ifdef _WIN32
#define SYSTEM_FILE_DELIM	'\\'
#define OTHER_FILE_DELIM	'/'
#else
#define SYSTEM_FILE_DELIM	'/'
#define OTHER_FILE_DELIM	'\\'
#endif

#define DEF_CHUNK_SIZE				16384

namespace common{ namespace zlib{ namespace v1{


static bool DecompressContentOfByteArray( 
		SRawCompressHeader* a_pRawHeader, ::std::vector< SCompressedFileItem >* a_pOut, z_stream* a_strm,
		size_t* a_punMainHeaderOffset, size_t* a_punFileItemsOffset, size_t* a_punCurrentFileOffset,
		QByteArray* a_pArrayForItemsList, 
		const QString& a_decompressedFileOrFolderPath, QFile* a_pDecompressedFile, bool* a_pFinished,
		size_t* a_punCurrentIndexOfFile);


bool DecompressFile(const ::std::u32string& a_compressedFilePath, const ::std::u32string& a_decompressedFileOrFolderPath, ::std::vector< SCompressedFileItem >* a_pVector)
{
	bool bReturnFromFunc = false;
	int nReturnFromZlibRoutine;
	z_stream strm;
	QString compressedFilePath = ::QString::fromStdU32String(a_compressedFilePath);
	QString decompressedFileOrFolderPath = ::QString::fromStdU32String(a_decompressedFileOrFolderPath);
	QDir targetDir(QFileInfo(decompressedFileOrFolderPath).path());
	QFile compressedFile(compressedFilePath);
	QFile decompressedFile;
	QByteArray inputArray(DEF_CHUNK_SIZE,0);
	QByteArray arrayForItemsList;
	unsigned char* in;
	qint64	 readSize;
	::std::vector< SCompressedFileItem > outputVectorHere;
	SRawCompressHeader zipRawHeader = INIT_CD_MAIN_HEADER(0);
	size_t unMainHeaderOffset = 0;
	size_t unFileItemsOffset = 0;
	size_t unCurrentIndexOfFile=0;
	size_t unCurrentFileOffset=0;
	bool bFinished = false;
	::std::vector< SCompressedFileItem >& outputVector = a_pVector? (*a_pVector) : outputVectorHere;
	
	zipRawHeader.stats.bits.isEndianDiscovered = 0;
	
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	nReturnFromZlibRoutine = inflateInit(&strm);
	if (nReturnFromZlibRoutine != Z_OK){
		return false;
	}
	
	if(!targetDir.mkpath(targetDir.path())){
		goto returnPoint;
	}
	
	if(!compressedFile.open(QIODevice::ReadOnly)){
		goto returnPoint;
	}
	
	in = reinterpret_cast<unsigned char*>(inputArray.data());
	
	while(((readSize=compressedFile.read(reinterpret_cast<char*>(in),DEF_CHUNK_SIZE))>0)&&(!bFinished)){
		
		strm.avail_in = readSize;
		strm.next_in = in;
		
		if( !DecompressContentOfByteArray(
					&zipRawHeader,&outputVector,&strm,&unMainHeaderOffset,
					&unFileItemsOffset,&unCurrentFileOffset,&arrayForItemsList,
					decompressedFileOrFolderPath,&decompressedFile,&bFinished,&unCurrentIndexOfFile)  )
		{
			goto returnPoint;
		}
		
		if(readSize<DEF_CHUNK_SIZE){
			break;
		}
	}
	
	bReturnFromFunc=true;
returnPoint:
	deflateEnd(&strm); // if not inited then this line will not run
	return bReturnFromFunc;
}


#ifdef __INTELLISENSE__
struct SRawCompressHeader{
	uint16_t	endian;
	union{
		struct{
			uint16_t	isDirectory : 1;
			uint16_t	reserved16bitwise01 : 15;
		}bits;
		uint16_t	allBits;
	}stats;
	uint32_t	itemsHeaderSizeOrFileSize;
	uint64_t	reserved64bit01;
};
#endif

static void GetAllFileItemHeaders( const QByteArray& a_arrayForItemsList, ::std::vector< SCompressedFileItem >* a_pOut, uint32_t a_itemsHeaderSizeOrFileSize, uint16_t endian);


static bool DecompressContentOfByteArray( 
		SRawCompressHeader* a_pRawHeader, ::std::vector< SCompressedFileItem >* a_pOut, z_stream* a_strm,
		size_t* a_punMainHeaderOffset, size_t* a_punFileItemsOffset, size_t* a_punCurrentFileOffset,
		QByteArray* a_pArrayForItemsList, 
		const QString& a_decompressedFileOrFolderPath, QFile* a_pDecompressedFile, bool* a_pFinished,
		size_t* a_punCurrentIndexOfFile)
{
	int retInf;
	QByteArray outputArray(DEF_CHUNK_SIZE,0);
	unsigned char* out;
	size_t& unMainHeaderOffset = *a_punMainHeaderOffset;
	size_t& unFileItemsOffset = *a_punFileItemsOffset;
	size_t unCopySize;
	uInt decompressedStreamSize;
		
	
	/* run inflate() on input until output buffer not full */
	do {
		
		out = reinterpret_cast<unsigned char*>(outputArray.data());
		a_strm->avail_out = DEF_CHUNK_SIZE;
		a_strm->next_out = out;
		
		retInf = inflate(a_strm, Z_NO_FLUSH);
		//assert(retInf != Z_STREAM_ERROR);  /* state not clobbered */
		if(retInf == Z_STREAM_ERROR){return false;}
		switch (retInf) {
		case Z_NEED_DICT:
			retInf = Z_DATA_ERROR;     /* and fall through */
			qDebug()<<"Z_DATA_ERROR"<<retInf;
			// todo: set here fall trout supressor
			return false;
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			qDebug()<<"Z_DATA_ERROR or Z_MEM_ERROR"<<retInf;
			return false;
		}
		
		decompressedStreamSize = DEF_CHUNK_SIZE-a_strm->avail_out;
		
		while(decompressedStreamSize>0){
			
			
			if(unMainHeaderOffset<sizeof(SRawCompressHeader)){
				unCopySize = sizeof(SRawCompressHeader)-unMainHeaderOffset;
				if(decompressedStreamSize>unCopySize){
					memcpy(a_pRawHeader,out,unCopySize);
					out += unCopySize;
					unMainHeaderOffset += unCopySize;
					decompressedStreamSize -= static_cast<uInt>(unCopySize);
				}
				else{
					memcpy(a_pRawHeader,out,decompressedStreamSize);
					out += decompressedStreamSize;
					unMainHeaderOffset += decompressedStreamSize;
					decompressedStreamSize = 0;
				}
			}
			
			if(unMainHeaderOffset>=sizeof(SRawCompressHeader)){
				
				if(!a_pRawHeader->stats.bits.isEndianDiscovered){
					if(a_pRawHeader->endian!=1){
#if 0
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
#endif
						::zlib::privateN::SwapStatic(&a_pRawHeader->stats.allBits);
						::zlib::privateN::SwapStatic(&a_pRawHeader->itemsHeaderSizeOrFileSize);
						::zlib::privateN::SwapStatic(&a_pRawHeader->version);
						::zlib::privateN::SwapStatic(&a_pRawHeader->reserved16bit01);
						::zlib::privateN::SwapStatic(&a_pRawHeader->reserved32bit01);
					}
					a_pRawHeader->stats.bits.isEndianDiscovered = 1;
				}
				
				if(a_pRawHeader->stats.bits.isDirectory){
					
					if(unFileItemsOffset<a_pRawHeader->itemsHeaderSizeOrFileSize){
						char* pItemsListPtr;
						unCopySize = a_pRawHeader->itemsHeaderSizeOrFileSize - unFileItemsOffset;
						
						a_pArrayForItemsList->resize(a_pRawHeader->itemsHeaderSizeOrFileSize);
						pItemsListPtr = a_pArrayForItemsList->data();
						
						if(decompressedStreamSize>unCopySize){
							memcpy(pItemsListPtr,out,unCopySize);
							out += unCopySize;
							unFileItemsOffset += unCopySize;
							decompressedStreamSize -= static_cast<uInt>(unCopySize);
						}
						else{
							memcpy(pItemsListPtr,out,decompressedStreamSize);
							out += decompressedStreamSize;
							unFileItemsOffset += decompressedStreamSize;
							decompressedStreamSize = 0;
						}
					}  // if(unFileItemsOffset<a_pRawHeader->itemsHeaderSizeOrFileSize){
					
					if(unFileItemsOffset>=a_pRawHeader->itemsHeaderSizeOrFileSize){
						size_t unNumberOfDirectoryItems;
						size_t& unCurrentIndexOfFile = *a_punCurrentIndexOfFile;
						QString itemPath;
						
						if(a_pOut->size()<1){
							GetAllFileItemHeaders(*a_pArrayForItemsList,a_pOut,a_pRawHeader->itemsHeaderSizeOrFileSize, a_pRawHeader->endian);
						}
						
						unNumberOfDirectoryItems = a_pOut->size();
						
						if((unNumberOfDirectoryItems<1)||(unNumberOfDirectoryItems<=unCurrentIndexOfFile)){
							return false;
						}
						
						a_pOut->at(unCurrentIndexOfFile).itemFullPath.replace(OTHER_FILE_DELIM,SYSTEM_FILE_DELIM);
						if(a_pOut->at(unCurrentIndexOfFile).raw.stats.bits.shouldRootDirBeAdded){
							//itemPath = QFileInfo(a_decompressDir,a_pOut->at(unCurrentIndexOfFile).itemFullPath).filePath();
							itemPath = a_decompressedFileOrFolderPath + a_pOut->at(unCurrentIndexOfFile).itemFullPath;
						}
						else{
							itemPath = a_pOut->at(unCurrentIndexOfFile).itemFullPath;
						}
						
						if(a_pOut->at(unCurrentIndexOfFile).raw.stats.bits.isDirectory){		
							if(QDir(itemPath).mkpath(itemPath)){
								if((++unCurrentIndexOfFile)>=unNumberOfDirectoryItems){
									*a_pFinished = true;
									return true;
								}
							}
							else{
								return false;
							}
							
						}
						else{
							size_t& unCurrentFileOffset = *a_punCurrentFileOffset;
							if(!a_pDecompressedFile->isOpen()){
								QString parentDirPath = QFileInfo(itemPath).path();
								QDir(parentDirPath).mkpath(parentDirPath);
								unCurrentFileOffset = 0;
								a_pDecompressedFile->setFileName(itemPath);
								if(!a_pDecompressedFile->open(QIODevice::WriteOnly)){
									return false;
								}
							}
							
							if(unCurrentFileOffset<a_pOut->at(unCurrentIndexOfFile).raw.fileSize){
								unCopySize = a_pOut->at(unCurrentIndexOfFile).raw.fileSize - unCurrentFileOffset;
								
								if(decompressedStreamSize>unCopySize){
									a_pDecompressedFile->write(reinterpret_cast<char*>(out),unCopySize);
									out += unCopySize;
									//unCurrentFileOffset += unCopySize; // todo: think on this
									unCurrentFileOffset = 0;
									a_pDecompressedFile->close();
									if((++unCurrentIndexOfFile)>=unNumberOfDirectoryItems){
										*a_pFinished = true;
										return true;
									}
									decompressedStreamSize -= static_cast<uInt>(unCopySize);
								}
								else{
									a_pDecompressedFile->write(reinterpret_cast<char*>(out),decompressedStreamSize);
									out += decompressedStreamSize;
									unCurrentFileOffset += decompressedStreamSize;
									decompressedStreamSize = 0;
								}			
								
							}  // if(unCurrentFileOffset<a_pOut->at(unCurrentIndexOfFile).raw.fileSize){ 
							// !!!!!! 2020 Jun 08 8 lines below added newly
							else{
								unCurrentFileOffset = 0;
								a_pDecompressedFile->close();
								if((++unCurrentIndexOfFile)>=unNumberOfDirectoryItems){
									*a_pFinished = true;
									return true;
								}
							}
						}
						
						// to continue
						
					}  // if(unFileItemsOffset>=a_pRawHeader->itemsHeaderSizeOrFileSize){
					
				}  // if(a_pRawHeader->stats.bits.isDirectory){
				else{  // not directory but a file
					if(unFileItemsOffset<a_pRawHeader->itemsHeaderSizeOrFileSize){
						if(!a_pDecompressedFile->isOpen()){
							QString parentDirectoryPath = QFileInfo(a_decompressedFileOrFolderPath).path();
							QDir(parentDirectoryPath).mkpath(parentDirectoryPath); // this is even not needed, because called by caller
							a_pDecompressedFile->setFileName(a_decompressedFileOrFolderPath);
							if(!a_pDecompressedFile->open(QIODevice::WriteOnly)){
								return false;
							}
						}
						
						unCopySize = a_pRawHeader->itemsHeaderSizeOrFileSize - unFileItemsOffset;
						if(decompressedStreamSize>unCopySize){
							a_pDecompressedFile->write(reinterpret_cast<char*>(out),unCopySize);
							unFileItemsOffset += unCopySize;
							a_pDecompressedFile->close();
							*a_pFinished = true;
							return true;
						}
						else{
							a_pDecompressedFile->write(reinterpret_cast<char*>(out),decompressedStreamSize);
							out += decompressedStreamSize;
							unFileItemsOffset += decompressedStreamSize;
							decompressedStreamSize = 0;
						}
					}  // if(unFileItemsOffset<a_pRawHeader->itemsHeaderSizeOrFileSize){
				}
			}
			
			
		}  // while(decompressedStreamSize>0){
		
		//retClbk = (*a_clbk)(a_out,a_outBufferSize-a_strm->avail_out,a_userData);
		//if(retClbk<0){return retClbk;}
	
	} while (a_strm->avail_out == 0);
	
	return true;
}


#ifdef __INTELLISENSE__
struct SRawCompressedFileItem{
	uint16_t	itemPathLengthPlus1;
	union{
		struct{
			uint16_t	isDirectory : 1;
			uint16_t	shouldRootDirBeAdded : 1;
			uint16_t	reserved16bitwise01 : 14;
		}bits;
		uint16_t	allBits;
	}stats;
	uint32_t	fileSize ;
};
#endif

static void GetAllFileItemHeaders( const QByteArray& a_arrayForItemsList, ::std::vector< SCompressedFileItem >* a_pOut, uint32_t a_itemsHeaderSizeOrFileSize, uint16_t a_endian)
{
	SCompressedFileItem nextItem;
	uint32_t unOffsetInHeader = 0;
	const char* pItemsHeaderPtr = a_arrayForItemsList.data();
	const SRawCompressedFileItem* pItemPtr;
	
	while(unOffsetInHeader<a_itemsHeaderSizeOrFileSize){
		pItemPtr = reinterpret_cast<const SRawCompressedFileItem*>(pItemsHeaderPtr+unOffsetInHeader);
		nextItem.raw = *pItemPtr;
		if(a_endian!=1){
			::zlib::privateN::SwapStatic(&nextItem.raw.itemPathLengthPlus1);
			::zlib::privateN::SwapStatic(&nextItem.raw.stats.allBits);
			::zlib::privateN::SwapStatic(&nextItem.raw.fileSize);
		}
		nextItem.itemFullPath = ITEM_FILE_NAME_FROM_ITEM_PTR_V1(pItemPtr);
		a_pOut->push_back(nextItem);
		unOffsetInHeader += ITEM_TOTAL_LENGTH_FROM_ITEM_PTR_V1(&(nextItem.raw));
	}
}


}}}  // namespace common{ namespace zlib{ namespace v1{
