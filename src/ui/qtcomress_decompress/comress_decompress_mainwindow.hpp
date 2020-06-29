//
// file:			comress_decompress_mainwindow.hpp
// created on:		2020 Jun 03
// created by:		D. Kalantaryan (davit.kalantaryan@gmail.com)
// 

#ifndef COMPRESS_DECOMPRESS_MAINWINDOW
#define COMPRESS_DECOMPRESS_MAINWINDOW

#include <QWidget>
#include <QGridLayout>
#include <QListWidget>
#include <QTreeView>
#include <QPushButton>
#include <QStringList>
#include <QFileSystemModel>
#include <QFileInfo>
#include <QLineEdit>
#include <QComboBox>
#include <QThread>
#include <QDialog>

namespace comress_decompress{

class MainWindow;

class WorkeThread final : public ::QThread
{
	friend class MainWindow;
	WorkeThread(MainWindow* pParent);
	void run() override;
	
private:
	MainWindow*	m_pParent;
};

class Worker final : public ::QObject
{
	//
};


class FileListView final : public ::QWidget
{
	Q_OBJECT
	
public:
	FileListView(const QString& settingsName);
	const QString& currentDirectoryPath()const;
	QFileInfo currentItemInfo()const;
	
private:
	void SetNewDirectory(const QString& newDirPath, QAbstractItemModel* a_pModel);
	
private:
signals:
	void NewItemSelectedSignal( const QFileInfo& fileInfo);
	
private:
	QGridLayout		m_mainLayout;
	const QString	m_settingsName;
	QString			m_currentPath;
	QStringList		m_recentPaths;
	QPushButton		m_up;
	QPushButton		m_back;
	QPushButton		m_openFileDialog;
	QPushButton		m_createNewDir;
	QTreeView		m_fileWidget;
	QModelIndex		m_prevIndex;
};


class MainWindow final : public ::QWidget
{
	Q_OBJECT
	
	friend class WorkeThread;
public:
	MainWindow();
	~MainWindow() override;
	
private:
signals:
	void CompressFinishedSignal(bool isSuccess);
	void DecompressFinishedSignal(bool isSuccess);
	
private:
	QGridLayout		m_mainLayout;
	FileListView	m_inWidget;
	FileListView	m_outWidget;
	QPushButton		m_zipButton;
	QPushButton		m_unzipButton;
	QLineEdit		m_targetFileName;
	QComboBox		m_compressionLevel;
	
	WorkeThread		m_workerThread;
	Worker*			m_pWorker;
};


class CreateFolderDlg final : private ::QDialog{
public:
	CreateFolderDlg();
	
	bool execNew();	
	QString newDirName()const;
	
private:
	QGridLayout		m_mainLayout;
	QLineEdit		m_newDirectoryName;
	QPushButton		m_ok;
	QPushButton		m_cancel;
	bool			m_bOkPressed; 
};


}  // namespace comress_decompress{


#endif  // #ifndef COMPRESS_DECOMPRESS_MAINWINDOW
