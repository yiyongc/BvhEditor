#pragma once

#ifndef JOINTTREELISTITEM_H
#define JOINTTREELISTITEM_H

#include <QList>
#include <QVariant>

namespace cacani {
	namespace data{

		class JointTreeListItem
		{
		public:
			explicit JointTreeListItem(const QList<QVariant> &data, JointTreeListItem *parentItem = 0);
			~JointTreeListItem();

			void appendChild(JointTreeListItem *child);

			JointTreeListItem *child(int row);
			int childCount() const;
			int columnCount() const;
			QVariant data(int column) const;
			int row() const;
			JointTreeListItem *parentItem();

		private:
			QList<JointTreeListItem*> m_childItems;
			QList<QVariant> m_itemData;
			JointTreeListItem *m_parentItem;
		};

	}

}

#endif // JointTreeListItem_H