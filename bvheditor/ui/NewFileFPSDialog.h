#pragma once
#include "qdialog.h"
#include "qlabel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../controllers/LayerController.h"
#include <QPushButton>
#include <QtWidgets\QApplication>


namespace cacani {


	namespace ui {

		class NewFileFPSDialog :
			public QDialog
		{

			Q_OBJECT

		public:
			NewFileFPSDialog(LayerController* layerController, int layerGroupIndex);
			~NewFileFPSDialog();


		private:

			LayerController* m_layerController;
			QPushButton* m_Button_Proj;
			QPushButton* m_Button_File;
			Layer* m_new_layer;
			double m_proj_fps;
			double m_newfile_fps;

		private Q_SLOTS:

			void buttonProjClicked();
			void buttonFileClicked();

		};

	}
}


