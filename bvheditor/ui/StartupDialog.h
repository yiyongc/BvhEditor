#pragma once
#include "qdialog.h"
#include "qlabel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../controllers/LayerController.h"
#include <QPushButton>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "StartupFPSDialog.h"
#include <QtWidgets\QApplication>
#include "../mainwindow.h"

class MainWindow;


namespace cacani {


	namespace ui {

		class StartupDialog :
			public QDialog
		{

			Q_OBJECT

		public:
			StartupDialog(LayerController* layerController, MainWindow * parent = 0);
			~StartupDialog();

		signals:
			void file_updated(QString);

		public:
			MainWindow* m_parent;

		private:

			LayerController* m_layerController;
			QString m_filename;
			QLabel* m_Label_LayerName;
			QPushButton* m_Button_Browse;
			QPushButton* m_Button_Confirm;
			QPushButton* m_Button_Cancel;
			QApplication* m_app;

		private Q_SLOTS:

			void confirmClicked();
			void cancelClicked();
			void browseClicked();
			void fileUpdated(QString fileName);
		};

	}
}


