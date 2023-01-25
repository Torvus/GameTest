#include "itemmimedata.h"
#include "item.h"

#include <QByteArray>
#include <QDataStream>
#include <QPixmap>

ItemMimeData::ItemMimeData(const Item &item, uint count)
    : QMimeData()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << int(item.type()) << count;

    setData(format(), data);
}

bool ItemMimeData::canHandle(const QMimeData *mimeData)
{
    return mimeData->hasFormat(format());
}

QPair<Item, uint> ItemMimeData::handleMimeData(const QMimeData *mimeData)
{
    QByteArray data = mimeData->data(format());
    QDataStream stream(&data, QIODevice::ReadOnly);
    int type;
    uint count;
    stream >> type >> count;
    return qMakePair(Item(Item::Type(type)), count);
}

QString ItemMimeData::format()
{
    return QLatin1Literal("application/x-dnditem");
}
