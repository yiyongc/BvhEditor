#pragma once

#ifndef JOINTTREELISTMODEL
#define JOINTTREELISTMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "Layer.h"


namespace cacani {
	namespace data{


		class JointTreeListItem;

		class JointTreeListModel : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			explicit JointTreeListModel(Layer* data, QObject *parent = 0);

			~JointTreeListModel();

			QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
			Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
			QVariant headerData(int section, Qt::Orientation orientation,
				int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
			QModelIndex index(int row, int column,
				const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
			QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
			int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
			int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

		private:
			void setupJointModelData(const Layer* data, JointTreeListItem *parent);
			void printJoint(const Joint* curJoint, int indent, QList<JointTreeListItem*>* parents, QList<int>* indentations);

			JointTreeListItem *m_rootItem;
			Layer* m_layer;
		};

	}
}

#endif // JointTreeListModel_H