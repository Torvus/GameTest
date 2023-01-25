#ifndef ITEMMIMEDATA_H
#define ITEMMIMEDATA_H

#include <QMimeData>

class Item;

class ItemMimeData : public QMimeData
{
public:
    ItemMimeData(const Item &item, uint count);

    static bool canHandle(const QMimeData *mimeData);
    static QPair<Item, uint> handleMimeData(const QMimeData *mimeData);

    static QString format();
};

#endif // ITEMMIMEDATA_H
