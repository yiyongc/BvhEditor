#include "qdialog.h"
#include "qlabel.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <qtextedit.h>
#include <qtablewidget.h>
#include <qsizepolicy.h>

namespace cacani {

	namespace ui {

		class ViewCtrlDialog :
			public QDialog
		{

			Q_OBJECT

		public:
			ViewCtrlDialog();
			~ViewCtrlDialog();

		private:
			QPushButton* m_Button_Okay;

			private Q_SLOTS:
			void okayClicked();

		};
	}
}