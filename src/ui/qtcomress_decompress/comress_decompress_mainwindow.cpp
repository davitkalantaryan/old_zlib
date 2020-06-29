//
// file:			comress_decompress_mainwindow.cpp
// created on:		2020 Jun 03
// created by:		D. Kalantaryan (davit.kalantaryan@gmail.com)
// 

#include "comress_decompress_mainwindow.hpp"
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <qt_zlib_compression_routines.hpp>
#include <qt_zlib_decompress_routines.hpp>

#if 0
#define Z_NO_COMPRESSION         0
#define Z_BEST_SPEED             1
#define Z_BEST_COMPRESSION       9
#define Z_DEFAULT_COMPRESSION  (-1)
#endif

comress_decompress::MainWindow::MainWindow()
	:
	  m_inWidget("in"),
	  m_outWidget("out"),
	  m_workerThread(this)
{
	m_pWorker = nullptr;
	connect(&m_zipButton,&QPushButton::clicked,this,[this](){	
		if(m_pWorker){
			
			m_zipButton.setDisabled(true);
			m_unzipButton.setDisabled(true);
			
			QMetaObject::invokeMethod(m_pWorker,[this](){
				bool bOk=false;
				QFileInfo fileToCompress = m_inWidget.currentItemInfo();
				if(fileToCompress.filePath().size()){
					QString fileName = m_targetFileName.text();
					QString targetPath = QFileInfo(m_outWidget.currentDirectoryPath(),fileName).filePath();
					int nCompressionLevel = m_compressionLevel.currentIndex();
					nCompressionLevel=(nCompressionLevel<0)?Z_DEFAULT_COMPRESSION:(nCompressionLevel-1);
					bOk = ::qtcore::CompressFileOrFolder(fileToCompress.filePath(),targetPath,nCompressionLevel);
				}
				emit CompressFinishedSignal(bOk);
			});
		}
	});
	
	connect(&m_unzipButton,&QPushButton::clicked,this,[this](){
		if(m_pWorker){
			
			m_zipButton.setDisabled(true);
			m_unzipButton.setDisabled(true);
			
			QMetaObject::invokeMethod(m_pWorker,[this](){
				bool bOk=false;
				QFileInfo fileToDecompress = m_inWidget.currentItemInfo();
				if(fileToDecompress.size() && fileToDecompress.isFile()){
					//QString targetFolderName = m_targetFileName.text();
					//QString targetFolderPath = QFileInfo(m_outWidget.currentDirectoryPath(),targetFolderName).filePath();
					QString targetFolderPath = m_outWidget.currentDirectoryPath();
					QString fileTodecompressName = fileToDecompress.fileName();
					QString rootDecompressFileName;
					
					if(fileTodecompressName.endsWith(".zlib",Qt::CaseInsensitive)){
						rootDecompressFileName = fileTodecompressName.mid(0,fileTodecompressName.size()-5);
					}
					else{
						rootDecompressFileName = fileTodecompressName + ".decompressed";
					}
					
					bOk = ::qtcore::DecompressFile(fileToDecompress.filePath(),QFileInfo(targetFolderPath,rootDecompressFileName).filePath(),nullptr);
				}
				emit DecompressFinishedSignal(bOk);
			});
		}
	});
	
	connect(this,&MainWindow::CompressFinishedSignal,this,[this](bool a_bOk){
		qDebug()<<"compress succeed:"<<a_bOk;
		m_zipButton.setEnabled(true);
		m_unzipButton.setEnabled(true);
	});
	
	connect(this,&MainWindow::DecompressFinishedSignal,this,[this](bool a_bOk){
		qDebug()<<"decompress succeed:"<<a_bOk;
		m_zipButton.setEnabled(true);
		m_unzipButton.setEnabled(true);
	});
	
	// emit NewItemSelectedSignal(fileInfo);
	connect(&m_inWidget,&FileListView::NewItemSelectedSignal,this,[this](const QFileInfo& a_fileInfo){
		QString itemName = a_fileInfo.fileName();
		m_targetFileName.setText(itemName + ".zlib");
	});
	
	//QPushButton		m_zipButton;
	//QPushButton		m_unzipButton;
	//QLineEdit		m_targetFileName;
	//QComboBox		m_compressionLevel;
	
	m_zipButton.setText("zip");
	m_unzipButton.setText("uzip");
	m_targetFileName.setText("compr.zlib");
	
	m_compressionLevel.addItem("-1 default compression");
	m_compressionLevel.addItem("0  no compression");
	m_compressionLevel.addItem("1  best speed");
	
	for(int i(Z_BEST_SPEED+1);i<Z_BEST_COMPRESSION;++i){
		m_compressionLevel.addItem( QString::number(i) );
	}
	
	m_compressionLevel.addItem("9  best compression");
	m_compressionLevel.setCurrentIndex(0);
	
	m_mainLayout.addWidget(&m_compressionLevel,0,0,1,2);
	m_mainLayout.addWidget(&m_targetFileName,0,2,1,2);
	m_mainLayout.addWidget(&m_zipButton,0,4,1,1);
	m_mainLayout.addWidget(&m_unzipButton,0,5,1,1);
	
	m_mainLayout.addWidget(&m_inWidget,1,0,3,3);
	m_mainLayout.addWidget(&m_outWidget,1,3,3,3);
		
	setLayout(&m_mainLayout);
	
	m_workerThread.start();
}


comress_decompress::MainWindow::~MainWindow()
{
	m_workerThread.quit();
	m_workerThread.wait();
}


/*////////////////////////////////////////////////////////////////////////////////////*/

comress_decompress::FileListView::FileListView(const QString& a_settingsName)
	:
	  m_settingsName(a_settingsName)
{
	QSettings aSettings;
	QString settingNameForCurrentPath = m_settingsName + "/currentPath";
	QString settingNameForRecentPaths = m_settingsName + "/recentPaths";
	QFileSystemModel *model = new QFileSystemModel;
	
	m_currentPath = aSettings.value(settingNameForCurrentPath,QDir::currentPath()).toString();
	m_recentPaths = aSettings.value(settingNameForRecentPaths,{}).toStringList();
	model->setRootPath(m_currentPath);
	m_fileWidget.setModel(model);
	m_fileWidget.setRootIndex(model->index(m_currentPath));
	
	// connections
	
	connect(&m_fileWidget,&QListView::doubleClicked,this,[this](const QModelIndex &a_index){
		QFileSystemModel* model = static_cast<QFileSystemModel*>(m_fileWidget.model());
		QFileInfo fileInfo = model->fileInfo(a_index);
		
		if(fileInfo.isDir()){
			QString filePathNew = fileInfo.filePath();
			SetNewDirectory(filePathNew,model);
		}
		
	});
	
	
	connect(&m_fileWidget,&QListView::clicked,this,[this](const QModelIndex &a_index){
		
		if(a_index!=m_prevIndex){
			QFileInfo fileInfo;
			m_prevIndex = a_index;
			
			if(a_index.isValid()){
				QFileSystemModel* model = static_cast<QFileSystemModel*>(m_fileWidget.model());
				fileInfo = model->fileInfo(a_index);
			}
			
			emit NewItemSelectedSignal(fileInfo);
		}
		
	});
	
	
	connect(&m_up,&QPushButton::clicked,this,[this](){
		QFileInfo fileInfo(m_currentPath);
		QString  parentPath = fileInfo.path();
		SetNewDirectory(parentPath,m_fileWidget.model());
	});
	
	connect(&m_back,&QPushButton::clicked,this,[this](){
		if(m_recentPaths.size()>1){
			QString newPath = m_recentPaths.at(1);
			SetNewDirectory(newPath,m_fileWidget.model());
		}
	});
	
	connect(&m_openFileDialog,&QPushButton::clicked,this,[this](){
		QString newPath = QFileDialog::getExistingDirectory(nullptr,QString(),m_currentPath);
		if(newPath!=""){
			SetNewDirectory(newPath,m_fileWidget.model());
		}
	});
	
	connect(&m_createNewDir,&QPushButton::clicked,this,[this](){
		CreateFolderDlg crtDlg;
		if(crtDlg.execNew()){
			QString newFolderName = crtDlg.newDirName();
			QFileInfo aFileInfo(m_currentPath,newFolderName);
			QString newDirPath = aFileInfo.filePath();
			QDir(newDirPath).mkpath(newDirPath);
		}
	});
	
	// end connections
	
	m_up.setText("up");
	m_back.setText("back");
	m_openFileDialog.setText("..");
	m_createNewDir.setText("create");
	
	m_mainLayout.addWidget(&m_up,0,0);
	m_mainLayout.addWidget(&m_back,0,1);
	m_mainLayout.addWidget(&m_openFileDialog,0,2);
	m_mainLayout.addWidget(&m_createNewDir,0,3);
	
	m_mainLayout.addWidget(&m_fileWidget,1,0,1,4);
	setLayout(&m_mainLayout);
}


const QString& comress_decompress::FileListView::currentDirectoryPath()const
{
	return m_currentPath;
}


QFileInfo comress_decompress::FileListView::currentItemInfo()const
{
	if(m_fileWidget.currentIndex().isValid()){
		QFileSystemModel* model = static_cast<QFileSystemModel*>(m_fileWidget.model());
		return model->fileInfo(m_fileWidget.currentIndex());
	}
	
	return {};
}


void comress_decompress::FileListView::SetNewDirectory(const QString& a_newDirPath, QAbstractItemModel* a_pModel)
{
	if(a_newDirPath!=m_currentPath){
		QFileSystemModel* pModel = static_cast<QFileSystemModel*>(a_pModel);
		QSettings aSettings;
		QString settingNameForCurrentPath = m_settingsName + "/currentPath";
		QString settingNameForRecentPaths = m_settingsName + "/recentPaths";
		QStringList::iterator iter=m_recentPaths.begin(), iterEnd = m_recentPaths.end();
		m_currentPath = a_newDirPath;
		aSettings.setValue(settingNameForCurrentPath,a_newDirPath);
		for(;iter!=iterEnd;++iter){
			if(*iter == a_newDirPath){
				m_recentPaths.erase(iter);
				break;
			}
		}
		m_recentPaths.push_front(a_newDirPath);
		aSettings.setValue(settingNameForRecentPaths,m_recentPaths);
		
		pModel->setRootPath(m_currentPath);
		m_fileWidget.setRootIndex(pModel->index(m_currentPath));
	}
}


/*////////////////////////////////////////////////////////////////////////////////////*/

comress_decompress::WorkeThread::WorkeThread(MainWindow* a_pParent)
	:
	  m_pParent(a_pParent)
{
}

void comress_decompress::WorkeThread::run()
{
	Worker* pWorker = m_pParent->m_pWorker = new Worker();
	QThread::exec();
	m_pParent->m_pWorker = nullptr;
	delete pWorker;
}

/*////////////////////////////////////////////////////////////////////////////////////*/

comress_decompress::CreateFolderDlg::CreateFolderDlg()
{
	m_bOkPressed = false;
	m_ok.setText("OK");
	m_cancel.setText("CANCEL");
	m_newDirectoryName.setText("New Folder");
	
	connect(&m_ok,&QPushButton::clicked,this,[this](){
		m_bOkPressed = true;
		hide();
	});
	
	connect(&m_cancel,&QPushButton::clicked,this,[this](){
		hide();
	});
	
	m_mainLayout.addWidget(&m_newDirectoryName,0,0,1,2);
	m_mainLayout.addWidget(&m_ok,1,0,1,1);
	m_mainLayout.addWidget(&m_cancel,1,1,1,1);
	setLayout(&m_mainLayout);
}

bool comress_decompress::CreateFolderDlg::execNew()
{
	::QDialog::exec();
	return m_bOkPressed;
}


QString comress_decompress::CreateFolderDlg::newDirName()const
{
	return m_newDirectoryName.text();
}
