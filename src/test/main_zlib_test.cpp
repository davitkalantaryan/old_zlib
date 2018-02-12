
// http://www.zlib.net/zlib_how.html

#include "zlib_decompress_routines.h"
#include "zlib_compression_routines.h"

#include <stdio.h>
#include <string.h>

#if defined(MSDOS) || defined(OS2) || defined(_WIN32) || defined(__CYGWIN__)
#include <conio.h>
#  define SET_BINARY_MODE(file) _setmode(_fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#if defined(_MSC_VER) & (_MSC_VER>1400)
#pragma warning (disable:4996)
#endif

/* compress or decompress from stdin to stdout */
int main(int argc, char **argv)
{
	FILE *fpIn=NULL, *fpOut=NULL;
	int ret=1;

	if(argc<4){
		fprintf(stderr,"Provide the mode (c,d) and the input and output files names!\n");
		goto returnPoint;
	}

	

	if(strcmp("cf",argv[1])==0){
		fpOut = fopen(argv[3], "wb");
		if (!fpOut) { goto returnPoint; }
		fpIn = fopen(argv[2],"rb");
		if (!fpIn) { goto returnPoint; }
		ret = ZlibCompressFile(fpIn,fpOut,Z_DEFAULT_COMPRESSION);
	}
	else if (strcmp("df", argv[1]) == 0){
		fpOut = fopen(argv[3], "wb");
		if (!fpOut) { goto returnPoint; }
		fpIn = fopen(argv[2],"rb");
		if (!fpIn) { goto returnPoint; }
		ret = ZlibDecompressFile(fpIn,fpOut);
	}
	else if (strcmp("cd", argv[1]) == 0) {
		fpOut = fopen(argv[3], "wb");
		if (!fpOut) { goto returnPoint; }
		ret = ZlibCompressFolder(argv[2], fpOut, Z_DEFAULT_COMPRESSION);
	}
	else if (strcmp("dd", argv[1]) == 0) {
		fpIn = fopen(argv[2], "rb");
		if (!fpIn) { goto returnPoint; }
		ret=ZlibDecompressFolder(fpIn,argv[3]);
	}
	else{
		fprintf(stderr,"wrong option!\n");
		goto returnPoint;
	}

	ret = 0;
returnPoint:

	if (fpIn) { fclose(fpIn); }
	if (fpOut) { fclose(fpOut); }

	printf("return=%d\n",ret);
	//if ((ret != Z_OK)&&(ret!=0))zerr(ret);

#if defined(_WIN32) & !defined(_M_ARM)
	printf("Press any key to exit!"); fflush(stdout);
	_getch();
	printf("\n");
#endif

	return ret;
}

#if 0
/* report a zlib or i/o error */
void zerr(int ret)
{
	fputs("zpipe: ", stderr);
	switch (ret) {
	case Z_ERRNO:
		if (ferror(stdin))
			fputs("error reading stdin\n", stderr);
		if (ferror(stdout))
			fputs("error writing stdout\n", stderr);
		break;
	case Z_STREAM_ERROR:
		fputs("invalid compression level\n", stderr);
		break;
	case Z_DATA_ERROR:
		fputs("invalid or incomplete deflate data\n", stderr);
		break;
	case Z_MEM_ERROR:
		fputs("out of memory\n", stderr);
		break;
	case Z_VERSION_ERROR:
		fputs("zlib version mismatch!\n", stderr);
	}
}
#endif
