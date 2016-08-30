#include "ViewCtrlDialog.h"

namespace cacani{

	namespace ui{

		ViewCtrlDialog::ViewCtrlDialog() :
			m_Button_Okay(new QPushButton("Okay Thanks!"))
		{
			//Setup link for Okay Button
			connect(m_Button_Okay, SIGNAL(clicked(bool)), this, SLOT(okayClicked()));

			setWindowTitle("Control Keys of BVH Editor");

			//Information
			QLabel* info = new QLabel("The following table shows the shortcut key controls of the BVH Editor.");
			//info->setReadOnly(true);
			
			//Keys and actions
			QLabel* key1 = new QLabel("Mouse Left Click and Drag");
			QLabel* action1 = new QLabel("Rotation about X and Y axis.");
			
			//QTableWidget
			QTableWidget* keyTable = new QTableWidget;
			keyTable->setRowCount(7);
			keyTable->setColumnCount(2);
			keyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

			//Header of Table
			QStringList tableLabels; 
			tableLabels << "Shortcut Keys" << "Actions";
			keyTable->setHorizontalHeaderLabels(tableLabels);
			

			keyTable->setItem(0, 0, new QTableWidgetItem("Mouse Left Click + Drag"));
			keyTable->setItem(0, 1, new QTableWidgetItem("Rotation about X and Y axis"));
			keyTable->setItem(1, 0, new QTableWidgetItem("Mouse Right Click + Drag"));
			keyTable->setItem(1, 1, new QTableWidgetItem("Rotation about X and Z axis"));
			keyTable->setItem(2, 0, new QTableWidgetItem("Scroll Button Press"));
			keyTable->setItem(2, 1, new QTableWidgetItem("Play/Pause Current BVH Layer"));
			keyTable->setItem(3, 0, new QTableWidgetItem("Scroll Wheel Up"));
			keyTable->setItem(3, 1, new QTableWidgetItem("Zoom In"));
			keyTable->setItem(4, 0, new QTableWidgetItem("Scroll Wheel Down"));
			keyTable->setItem(4, 1, new QTableWidgetItem("Zoom Out"));
			keyTable->setItem(5, 0, new QTableWidgetItem("Double Click Left Mouse Button"));
			keyTable->setItem(5, 1, new QTableWidgetItem("Restart Animation from First Frame"));
			keyTable->setItem(6, 0, new QTableWidgetItem("Double Click Right Mouse Button"));
			keyTable->setItem(6, 1, new QTableWidgetItem("Return to Original Camera View"));



			//Word wrapping for Table
			keyTable->resizeColumnsToContents();
			keyTable->resizeRowsToContents();

			//Combining all widgets
			QVBoxLayout* dialogLayout = new QVBoxLayout;
			dialogLayout->addWidget(info);
			dialogLayout->addWidget(keyTable);
			dialogLayout->addWidget(m_Button_Okay);
			
			setLayout(dialogLayout);
			resize(420, 260);

		}


		ViewCtrlDialog::~ViewCtrlDialog() {}

		void ViewCtrlDialog::okayClicked() {
			close();
		}
	}
}