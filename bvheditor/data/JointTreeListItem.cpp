#include <QStringList>

#include "JointTreeListItem.h"

namespace cacani {
	namespace data{

		JointTreeListItem::JointTreeListItem(const QList<QVariant> &data, JointTreeListItem *parent)
		{
			m_parentItem = parent;
			m_itemData = data;
			
		}

		JointTreeListItem::~JointTreeListItem()
		{
			qDeleteAll(m_childItems);
		}

		void JointTreeListItem::appendChild(JointTreeListItem *item)
		{
			m_childItems.append(item);
		}

		JointTreeListItem *JointTreeListItem::child(int row)
		{
			return m_childItems.value(row);
		}

		int JointTreeListItem::childCount() const
		{
			return m_childItems.count();
		}

		int JointTreeListItem::columnCount() const
		{
			return m_itemData.count();
		}

		QVariant JointTreeListItem::data(int column) const
		{
			return m_itemData.value(column);
		}

		JointTreeListItem *JointTreeListItem::parentItem()
		{
			return m_parentItem;
		}

		int JointTreeListItem::row() const
		{
			if (m_parentItem)
				return m_parentItem->m_childItems.indexOf(const_cast<JointTreeListItem*>(this));

			return 0;
		}

	}
}