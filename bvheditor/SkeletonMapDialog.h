#pragma once
#ifndef SKELETONMAPDIALOG_H
#define SKELETONMAPDIALOG_H

#include "qdialog.h"
#include "qlabel.h"
#include "qcombobox.h"
#include "qgroupbox.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "../data/Layer.h"
#include "../data/LayerGroup.h"
#include "qmessagebox.h"
#include <unordered_map>

class SkeletonMapDialog : public QDialog {
	Q_OBJECT

	public:
		SkeletonMapDialog(cacani::data::LayerGroup* layerGroup, unordered_multimap<int, pair<int, int>> *m, int selectedVert, int imageIndex);
		~SkeletonMapDialog();

	private:
		unordered_multimap<int, pair<int, int>>* m_map;
		cacani::data::LayerGroup* m_layerGroup;
		QComboBox* m_joints;
		QPushButton* m_Button_Confirm;
		QPushButton* m_Button_Cancel;
		int selected;
		int selectedImage;
		pair<int, int> m_pair;

		private Q_SLOTS:
			void confirmClicked();
			void cancelClicked();
};


#endif  //SKELETONMAPDIALOG_H