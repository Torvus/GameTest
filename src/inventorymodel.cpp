#include "inventorymodel.h"
#include "inventory.h"
#include "item.h"
#include "itemmimedata.h"

#include <QPixmap>
#include <QDebug>

InventoryModel::InventoryModel(QObject *parent)
    : QAbstractTableModel(parent)
    , mInventory(nullptr)
{
}

Inventory *InventoryModel::inventory() const
{
    return mInventory;
}

void InventoryModel::setInventory(Inventory *inventory)
{
    if(inventory == mInventory)
        return;
    beginResetModel();
    if(mInventory)
    {
        disconnect(mInventory, &Inventory::cellModified, this, &InventoryModel::onCellModified);
    }
    mInventory = inventory;
    if(mInventory)
    {
        connect(mInventory, &Inventory::cellModified, this, &InventoryModel::onCellModified);
    }
    endResetModel();
}

int InventoryModel::rowCount(const QModelIndex &parent) const
{
    if (!mInventory || parent.isValid())
        return 0;

    return mInventory->rows();
}

int InventoryModel::columnCount(const QModelIndex &parent) const
{
    if (!mInventory || parent.isValid())
        return 0;

    return mInventory->columns();
}

QVariant InventoryModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::SizeHintRole)
        return QSize(128, 128);

    if (!mInventory || !index.isValid())
        return QVariant();

    Item item = mInventory->itemAt(index.row(), index.column());
    if(item.type() != Item::Unknown)
    {

        switch(role)
        {
        case Qt::DisplayRole:
            return QString::number(mInventory->itemCountAt(index.row(), index.column()));

        case Qt::DecorationRole:
            return qVariantFromValue(QPixmap(item.imagePath()));

        case Qt::ToolTipRole:
            return item.name();

        default:
            break;
        }
    }

    return QVariant();
}

/**
 * @brief InventoryModel::onCellModified
 * Map inventory cell modification and emit dataChange signal
 * @param row
 * @param column
 */
void InventoryModel::onCellModified(int row, int column)
{
    QModelIndex idx = index(row, column);
    emit dataChanged(idx, idx);
}

/**
 * @brief InventoryModel::mimeTypes
 * @return Supported Mime types
 */
QStringList InventoryModel::mimeTypes() const
{
    return QStringList(ItemMimeData::format());
}

QMimeData *InventoryModel::mimeData(const QModelIndexList &indexes) const
{
    if(!mInventory || indexes.size() != 1)
        return nullptr;

    QModelIndex index = indexes.first();
    Item item = mInventory->itemAt(index.row(), index.column());
    if(item.type() == Item::Unknown)
        return nullptr;

    return new ItemMimeData(item, mInventory->itemCountAt(index.row(), index.column()));
}

bool InventoryModel::canDropMimeData(const QMimeData *data, Qt::DropAction, int, int, const QModelIndex &parent) const
{
    if(!mInventory || !parent.isValid())
        return false;
    return ItemMimeData::canHandle(data);
}

bool InventoryModel::dropMimeData(const QMimeData *data, Qt::DropAction, int, int, const QModelIndex &parent)
{
    if(!mInventory || !parent.isValid())
        return false;

    QPair<Item, uint> pair = ItemMimeData::handleMimeData(data);
    mInventory->addItem(parent.row(), parent.column(), pair.first, pair.second);
    return true;
}

Qt::DropActions InventoryModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions InventoryModel::supportedDragActions() const
{
    return Qt::MoveAction;
}


Qt::ItemFlags InventoryModel::flags(const QModelIndex &index) const
{
    if(!mInventory)
        return Qt::ItemFlags();

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
    Item item = index.isValid() ? mInventory->itemAt(index.row(), index.column()) : Item();
    if(item.type() != Item::Unknown)
        flags |= Qt::ItemIsDragEnabled;

    return flags;
}
