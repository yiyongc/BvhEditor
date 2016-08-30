#pragma once
#include "qdialog.h"
#include "qlabel.h"
#include "qcombobox.h"
#include "qgroupbox.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "../ui/sheetCanvas.h"
#include <QPushButton>


namespace cacani {


	namespace ui {

		class SelectPlayDialog :
			public QDialog
		{

			Q_OBJECT

		public:
			SelectPlayDialog(SheetCanvas* m_sheetCanvas);
			~SelectPlayDialog();

		private:

			SheetCanvas* m_sheetCanvas;
			QComboBox* m_firstFrameList;
			QComboBox* m_lastFrameList;
			QPushButton* m_Button_Play;
			QPushButton* m_Button_Cancel;

			private Q_SLOTS:
			void playClicked();
			void cancelClicked();
			void populateLastFrameList(int firstFrameNum);
		};

	}
}