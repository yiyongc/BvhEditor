#include "mainwindow.h"
#include <QtWidgets\QApplication>
#include <QtWidgets\qtextedit.h>
#include <QtWidgets\qsplitter.h>
#include <QVBoxLayout>
#include "../ui/FCContainer.h"
#include "../ui/SheetCanvas.h"
#include <qdir.h>
#include "../QsLog2/qslog.h"
#include <iostream>
#include "../data/JointTreeListModel.h"
#include <qtreeview.h>
#include "../ui/JointTreeWidget.h"
#include "../ui/JointEditorWidget.h"


int main(int argc, char *argv[])
{

	QApplication app(argc, argv);

	// 1. init the logging mechanism
	QsLogging::Logger& logger = QsLogging::Logger::instance();
	logger.setLoggingLevel(QsLogging::TraceLevel);
	const QString sLogPath(QDir(app.applicationDirPath()).filePath("bvh_editor_log.txt"));

	// 2. add file destination
	QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(
		sLogPath, QsLogging::EnableLogRotation, QsLogging::MaxSizeBytes(100000000), QsLogging::MaxOldLogCount(1000)));
	logger.addDestination(fileDestination);

	// 3. start logging
	QLOG_INFO() << "main:" << "Started";


	MainWindow wnd(&app);
	if (wnd.m_success){
		wnd.show();
		return app.exec();
	}
	else{ 
		return 0; 
	}

}