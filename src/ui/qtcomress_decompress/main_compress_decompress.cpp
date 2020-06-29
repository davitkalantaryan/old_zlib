//
// file:			main_compress_decompress_test.cpp
// created on:		2020 Jun 03
// created by:		D. Kalantaryan (davit.kalantaryan@gmail.com)
// 

#include <QApplication>
#include "comress_decompress_mainwindow.hpp"

int main(int a_argc, char* a_argv[])
{
	QApplication aApp(a_argc,a_argv);
	comress_decompress::MainWindow aWnd;
	
	aWnd.show();
	
	QCoreApplication::exec();
	
	return 0;
}
