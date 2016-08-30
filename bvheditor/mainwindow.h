#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <iostream>

//QT includes
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qsplitter.h>
#include <QVBoxLayout>
#include <qdir.h>
#include <qtreeview.h>
#include <qerrormessage.h>
#include <qfileinfo.h>

//Other Includes
#include "../data/JointTreeListModel.h"
#include "../QsLog2/qslog.h"
#include "data/ImageFile.h"

//OpenToonz
#include "OpenToonz/meshbuilder.h"
#include "OpenToonz/gutil.h"
#include "OpenToonz/meshutils.h"
#include "OpenToonz/MeshOptionDialog.h"

//UI includes
#include "../ui/FCContainer.h"
#include "../ui/SheetCanvas.h"
#include "../ui/JointTreeWidget.h"
#include "../ui/JointEditorWidget.h"
#include "../ui/ImportDialog.h"
#include "../ui/ExportDialog.h"
#include "../ui/StartupDialog.h"
#include "../ui/SelectPlayDialog.h"
#include "../ui/ViewCtrlDialog.h"
//#include "../ui/InsertFrameDialog.h"
#include "../ui/DeleteSelectedDialog.h"

#include "../ui/ImageListWidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QApplication* app, QWidget *parent = 0);
	~MainWindow();
	void startup();


	private Q_SLOTS:
		//BVH File related
		void importNewFile();
		void exportFile();
		
		//Canvas Related
		void zoomIn();
		void zoomOut();
		void playAnimation();
		void revertView();
		void restartAnimation();
		void stopAnimation();
		void goToTPose();
		void groundModel();

		//UI related
		void viewToolBar(bool isView);
		void togglePlayOnce(bool isChecked);
		void playSelected();
		void viewControls();

		//FrameWidget related
		void addFrames();
		void addOne();
		void addFive();
		void addTen();
		void deleteCurrentFrame();
		void deleteAllFrames();
		void deleteSelectedFrames();
		
		//Image file related
		void importNewImage();
		void createMesh();
		void toggleViewImage();
		void toggleViewMesh();
		void deleteImage();
		void deleteAllImages();


private:
	LayerGroup* m_root;
	QWidget* m_FCCWidget;
	LayerGroup* m_LayerGroup;
	ViewSettings* m_ViewSettings;
	LayerController* m_LayerController;
	CACStateMachine* m_CACStateMachine;
	SheetCanvas* m_SheetCanvas;
	LayerSelector* m_LayerSelector;
	FCContainer* m_FCContainer;
	JointTreeListModel* m_JointTreeListModel;
	JointTreeWidget* m_JointTreeWidget;
	JointEditorWidget* m_JointEditorWidget;
	QApplication* m_app;
	QToolBar* toolbar;
	ImageListWidget* m_ImageListWidget;

	//View image, may be commented out in the future (remove function to view in import image)
	QLabel* myView;

	//Image Related
	QString imageFilePath;
	QString imageFileName;
	QImage* importedImage;
	TRaster32P convertedImage;
	TMeshImageP meshImage;
	void convertImage();

public:
	bool m_success;
};

#endif // MAINWINDOW_H
