#include "JointTreeListItem.h"
#include "JointTreeListModel.h"

#include <QStringList>
#include "../QsLog2/qslog.h"


namespace cacani {
	namespace data{


		JointTreeListModel::JointTreeListModel(Layer* data, QObject *parent)
		{
			QList<QVariant> rootData;
			rootData << "Joints";
			m_rootItem = new JointTreeListItem(rootData);
			m_layer = data;
			setupJointModelData(data, m_rootItem);
			
		}


		JointTreeListModel::~JointTreeListModel()
		{
			delete m_rootItem;
		}


		int JointTreeListModel::columnCount(const QModelIndex &parent) const
		{
			if (parent.isValid())
				return static_cast<JointTreeListItem*>(parent.internalPointer())->columnCount();
			else
				return m_rootItem->columnCount();
		}

		QVariant JointTreeListModel::data(const QModelIndex &index, int role) const
		{
			if (!index.isValid())
				return QVariant();

			if (role != Qt::DisplayRole)
				return QVariant();

			JointTreeListItem *item = static_cast<JointTreeListItem*>(index.internalPointer());

			return item->data(index.column());
		}

		Qt::ItemFlags JointTreeListModel::flags(const QModelIndex &index) const
		{
			if (!index.isValid())
				return 0;

			return QAbstractItemModel::flags(index);
		}

		QVariant JointTreeListModel::headerData(int section, Qt::Orientation orientation,
			int role) const
		{
			if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
				return m_rootItem->data(section);

			return QVariant();
		}

		QModelIndex JointTreeListModel::index(int row, int column, const QModelIndex &parent)
			const
		{
			if (!hasIndex(row, column, parent))
				return QModelIndex();

			JointTreeListItem *parentItem;

			if (!parent.isValid())
				parentItem = m_rootItem;
			else
				parentItem = static_cast<JointTreeListItem*>(parent.internalPointer());

			JointTreeListItem *childItem = parentItem->child(row);
			if (childItem)
				return createIndex(row, column, childItem);
			else
				return QModelIndex();
		}

		QModelIndex JointTreeListModel::parent(const QModelIndex &index) const
		{
			if (!index.isValid())
				return QModelIndex();

			JointTreeListItem *childItem = static_cast<JointTreeListItem*>(index.internalPointer());
			JointTreeListItem *parentItem = childItem->parentItem();

			if (parentItem == m_rootItem)
				return QModelIndex();

			return createIndex(parentItem->row(), 0, parentItem);
		}

		int JointTreeListModel::rowCount(const QModelIndex &parent) const
		{
			JointTreeListItem *parentItem;
			if (parent.column() > 0)
				return 0;

			if (!parent.isValid())
				parentItem = m_rootItem;
			else
				parentItem = static_cast<JointTreeListItem*>(parent.internalPointer());

			return parentItem->childCount();
		}

		void JointTreeListModel::setupJointModelData(const Layer* data, JointTreeListItem *parent)
		{
			QLOG_INFO() << "JointTreeListModel:" << "setupJointModelData() called";

			QList<JointTreeListItem*>* parents = new QList<JointTreeListItem*>;
			QList<int>* indentations = new QList<int>;

			*parents << parent;
			*indentations << 0;
			const Joint* curJoint = data->GetJoint(0);
			int indent = 0;

			printJoint(curJoint, indent, parents, indentations);
		}

		
		void JointTreeListModel::printJoint(const Joint* curJoint, int indent, QList<JointTreeListItem*>* parents, QList<int>* indentations){
			//this limit is set because of the special condition at the end of the bone hierarchy
			int limit = curJoint->children.size();
			if (limit == 0) limit = 1;
			for (int i = 0; i < limit; i++){
	            //only list it out at the first time traversing the node
				if (i == 0){
					if (parents->last()->childCount() > 0) {
						*parents << parents->last()->child(parents->last()->childCount() - 1);
						indent++;
						*indentations << indent;
					}
					QList<QVariant> columnData;
					columnData << QString::fromStdString(curJoint->name);
					parents->last()->appendChild(new JointTreeListItem(columnData, parents->last()));
				}
				//proceed and recur at its children
				if (curJoint->children.size()>0) printJoint(curJoint->children.at(i), indent, parents, indentations);
			}
			parents->pop_back();
			indentations->pop_back();
		}
	}
}