#pragma once
#include "qdialog.h"
#include "qlabel.h"
#include "qradiobutton.h"
#include "qcombobox.h"
#include "qgroupbox.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../controllers/LayerController.h"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QtWidgets\QApplication>
#include "StartupDialog.h"


//#define MAXFRAMERATE 240



namespace cacani {


	namespace ui {


		class StartupDialog;

		class StartupFPSDialog :
			public QDialog
		{

			Q_OBJECT

		public:
			StartupFPSDialog(LayerController* layerController, QString filename, StartupDialog * parent = 0);
			~StartupFPSDialog();

		private:

			StartupDialog* m_parent;
			LayerController* m_layerController;
			QRadioButton* m_radio1;
			QRadioButton* m_radio2;
			QDoubleSpinBox* m_input_fps;
			QPushButton* m_Button_Confirm;
			QPushButton* m_Button_Cancel;
			QApplication* m_app;
			double m_default_fps;

		private Q_SLOTS:
			void confirmClicked();
			void cancelClicked();
			void radio2Clicked(bool checked);

		private:
			//initialize first layer into layergroup, and set the default frame rate
			void initializeFirstLayer(LayerController* layerController, QString filename);
		};

	}
}


