//
// file:			qt_compress_directory.cpp
// created on:		2020 Jun 04
// created by:		D. Kalantaryan (davit.kalantaryan@gmail.com)
// 

#include <qt_zlib_compression_routines.hpp>
#include <vector>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QByteArray>
#include <string>

namespace qt{


bool DecompressFile(const QString& a_compressedFilePath, const QString& a_targetFolderPath)
{
	bool bReturnFromFunc = false;
	int nReturnFromZlibRoutine;
	z_stream strm;
	
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	nReturnFromZlibRoutine = inflateInit(&strm);
	if (nReturnFromZlibRoutine != Z_OK){
		return false;
	}
	
	bReturnFromFunc=true;
returnPoint:
	deflateEnd(&strm); // if not inited then this line will not run
	return bReturnFromFunc;
}


}  // namespace qt{
