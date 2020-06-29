#   
# file:			zcompressdecompress.pro  
# created on:	2010 May 27 
# created by:	D. Kalantaryan (davit.kalantaryan@gmail.com)  
#  


message("file:  developer_tool.pro  ")
include($${PWD}/../../common/common_qt/sys_common.pri)
include($${PWD}/../../common/common_qt/zlib.pri)

QT += widgets

#win32: debug: CONFIG += console

win32 {
} else {
}

INCLUDEPATH += $${PWD}/../../../include
INCLUDEPATH += $${PWD}/../../../src/include_p

SOURCES		+=	\
	$${PWD}/../../../src/ui/qtcomress_decompress/main_compress_decompress.cpp			\
	$${PWD}/../../../src/ui/qtcomress_decompress/comress_decompress_mainwindow.cpp		\
	$${PWD}/../../../src/util/v1/qt_compress_file_or_directory_v1.cpp					\
	$${PWD}/../../../src/util/v1/qt_decompress_into_file_or_directory_v1.cpp			\
	$${PWD}/../../../src/util/qt_compress_file_or_directory.cpp							\
	$${PWD}/../../../src/util/qt_decompress_into_file_or_directory.cpp					\
	$${PWD}/../../../src/util/decompress_common_in_versions.cpp

HEADERS		+=	\
	$${PWD}/../../../src/ui/qtcomress_decompress/comress_decompress_mainwindow.hpp		\
	$${PWD}/../../../include/qt_zlib_compress_decompress_common.hpp						\
	$${PWD}/../../../include/qt_zlib_compression_routines.hpp							\
	$${PWD}/../../../include/qt_zlib_decompress_routines.hpp							\
	$${PWD}/../../../include/common/zlib/v1/compress_decompress_common.hpp				\
	$${PWD}/../../../include/common/zlib/v1/compression_routines.hpp					\
	$${PWD}/../../../include/common/zlib/v1/decompress_routines.hpp						\
	$${PWD}/../../../src/include_p/zlib_compress_decompress_common_private.hpp			\
	$${PWD}/../../../src/include_p/v1/zlib_compress_decompress_common_v1_private.hpp	\
	$${PWD}/../../../src/include_p/decompress_common_in_versions_private.hpp

RESOURCES	+= \
	$${PWD}/../../../src/resources/zlib_rcs.qrc


OTHER_FILES +=	\
	$${PWD}/../../../src/util/qt_compress_file_or_directory.cpp				\
	$${PWD}/../../../src/util/qt_decompress_into_file_or_directory.cpp
