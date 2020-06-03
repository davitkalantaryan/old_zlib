#   
# file:			developer_tool.pro  
# created on:	2010 May 27 
# created by:	D. Kalantaryan (davit.kalantaryan@gmail.com)  
#  

TEMPLATE = lib

message("file:  developer_tool.pro  ")
include($${PWD}/../../common/common_qt/sys_common.pri)


win32:  QMAKE_CFLAGS += /FI"zlib_first_include.h"


INCLUDEPATH += $${PWD}/../../../include
INCLUDEPATH += $${PWD}/../../../src/core

SOURCES		+=	\
	$${PWD}/../../../contrib/zlib/adler32.c			\
	$${PWD}/../../../contrib/zlib/compress.c		\
	$${PWD}/../../../contrib/zlib/crc32.c			\
	$${PWD}/../../../contrib/zlib/deflate.c			\
	$${PWD}/../../../contrib/zlib/gzclose.c			\
	$${PWD}/../../../contrib/zlib/gzlib.c			\
	$${PWD}/../../../contrib/zlib/gzread.c			\
	$${PWD}/../../../contrib/zlib/gzwrite.c			\
	$${PWD}/../../../contrib/zlib/infback.c			\
	$${PWD}/../../../contrib/zlib/inffast.c			\
	$${PWD}/../../../contrib/zlib/inflate.c			\
	$${PWD}/../../../contrib/zlib/inftrees.c		\
	$${PWD}/../../../contrib/zlib/trees.c			\
	$${PWD}/../../../contrib/zlib/uncompr.c			\
	$${PWD}/../../../contrib/zlib/zutil.c

HEADERS		+=	\
