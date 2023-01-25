#include "inventoryview.h"
#include "inventory.h"
#include "inventorymodel.h"
#include "itemmimedata.h"
#include "item.h"

#include <QMouseEvent>
#include <QDrag>
#include <QDebug>

InventoryView::InventoryView(QWidget *parent)
    : QTableView(parent)
{
    setModel(new InventoryModel(this));
}

Inventory *InventoryView::inventory() const
{
    InventoryModel *inventoryModel = qobject_cast<InventoryModel*>(model());
    return inventoryModel ? inventoryModel->inventory() : nullptr;
}

void InventoryView::setInventory(Inventory *inventory)
{
    if(InventoryModel *inventoryModel = qobject_cast<InventoryModel*>(model()))
    {
        inventoryModel->setInventory(inventory);
        QSize cellSize = model()->data(QModelIndex(), Qt::SizeHintRole).toSize();
        if(!cellSize.isEmpty())
        {
            setMinimumHeight(cellSize.height() * inventory->rows());
            setMinimumWidth(cellSize.width() * inventory->columns());
        }
    }
}

/**
 * @brief InventoryView::mouseMoveEvent
 * Start moving Drag'n'Drop
 * @param event
 */
void InventoryView::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if(!index.isValid() || event->buttons() != Qt::LeftButton)
    {
        QTableView::mouseMoveEvent(event);
    }
    else if(Inventory *inv = inventory())
    {
        Item item = inv->itemAt(index.row(), index.column());
        uint count = inv->itemCountAt(index.row(), index.column());
        if(item.type() == Item::Unknown || !count)
        {
            QTableView::mouseMoveEvent(event);
        }
        else
        {
            QDrag *drag = new QDrag(this);
            drag->setMimeData(new ItemMimeData(item, count));
            drag->setPixmap(QPixmap(item.imagePath()));
            drag->setHotSpot(event->pos() - visualRect(index).topLeft());
            inv->clearCell(index.row(), index.column());

            if(!drag->exec(Qt::MoveAction))
            {
                inv->addItem(index.row(), index.column(), item, count);
            }
        }
    }
}

/**
 * @brief InventoryView::contextMenuEvent
 * Eat item from inventory
 * @param event
 */
void InventoryView::contextMenuEvent(QContextMenuEvent *event)
{
    if(Inventory *inv = inventory())
    {
        QModelIndex index = indexAt(event->pos());
        if(index.isValid())
        {
            inv->removeItem(index.row(), index.column());
        }
    }
}
