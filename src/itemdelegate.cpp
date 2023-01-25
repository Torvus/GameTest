#include "itemdelegate.h"

#include <QPainter>
#include <QVariant>

ItemDelegate::ItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    QItemDelegate::paint(painter, option, index);
    painter->restore();

    QRect rect = option.rect;
    QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();
    QString text = index.data(Qt::DisplayRole).toString();
    if(!text.isEmpty())
    {
        painter->drawPixmap(rect.topLeft(), pixmap);
        painter->drawText(rect, Qt::AlignBottom | Qt::AlignRight, text);
    }
}
