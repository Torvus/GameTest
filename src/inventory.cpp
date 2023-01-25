#include "inventory.h"
#include "item.h"
#include "database.h"

#include <QVector>
#include <QPair>
#include <QSound>
#include <QApplication>
#include <QSqlQuery>

class InventoryCell
{
public:
    InventoryCell()
        : count(0)
    {
    }
    InventoryCell(const InventoryCell &) = default;
    InventoryCell &operator=(const InventoryCell &) = default;

    static InventoryCell &staticEmpty()
    {
        static InventoryCell empty;
        empty = InventoryCell();
        return empty;
    }

    Item item;
    uint count;
};

class InventoryPrivate
{
public:
    InventoryPrivate(int r, int c)
        : rows(r)
        , columns(c)
        , eatSound(QLatin1Literal(":/sounds/eating-apple.wav"))
    {
        data.resize(rows * columns);
    }

    InventoryCell &cell(int row, int column)
    {
        if(row > -1 && row < rows && column > -1 && column < columns)
            return data[row * columns + column];
        return InventoryCell::staticEmpty();
    }
    const InventoryCell &cell(int row, int column) const
    {
        if(row > -1 && row < rows && column > -1 && column < columns)
            return data.at(row * columns + column);
        return InventoryCell::staticEmpty();
    }

    int rows;
    int columns;
    QVector<InventoryCell> data;
    QSound eatSound;
};

Inventory::Inventory(int rows, int columns, QObject *parent)
    : QObject(parent)
    , d_ptr(new InventoryPrivate(rows, columns))
{
    Q_D(Inventory);

    Database::init();

    QSqlQuery query = Database::fetchInventory();

    if(query.isActive())
    {
        while(query.next())
        {
            int row = query.value(0).toInt();
            int column = query.value(1).toInt();
            int itemType = query.value(2).toInt();
            int count = query.value(3).toInt();
            Item item((Item::Type(itemType)));
            InventoryCell &cell = d->cell(row, column);
            cell.item = item;
            cell.count = count;
        }
    }
}

Inventory::~Inventory()
{
    delete d_ptr;
}

int Inventory::rows() const
{
    Q_D(const Inventory);
    return d->rows;
}

int Inventory::columns() const
{
    Q_D(const Inventory);
    return d->columns;
}

Item Inventory::itemAt(int row, int column) const
{
    Q_D(const Inventory);
    return d->cell(row, column).item;
}

uint Inventory::itemCountAt(int row, int column) const
{
    Q_D(const Inventory);
    return d->cell(row, column).count;
}

/**
 * @brief Inventory::addItem
 * Add count items to inventory
 * @param row
 * @param column
 * @param item
 * @param count
 */
void Inventory::addItem(int row, int column, const Item &item, uint count)
{
    Q_D(Inventory);

    InventoryCell &cell = d->cell(row, column);
    if(cell.item == item)
    {
        cell.count += count;
    }
    else
    {
        cell.item = item;
        cell.count = count;
    }
    Database::setInventory(row, column, cell.item, cell.count);
    emit cellModified(row, column);
}

void Inventory::removeItem(int row, int column, uint count)
{
    Q_D(Inventory);

    if(!count)
        return;

    InventoryCell &cell = d->cell(row, column);
    if(cell.item.type() != Item::Unknown) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if(count > cell.count)
            count = cell.count;
        cell.count -= count;
        if(!cell.count)
        {
            cell.item = Item();
        }
        Database::setInventory(row, column, cell.item, cell.count);
        d->eatSound.play();
        emit cellModified(row, column);
        QApplication::restoreOverrideCursor();
    }
}

void Inventory::moveItems(int fromRow, int fromColumn, int toRow, int toColumn)
{
    Q_D(Inventory);

    InventoryCell &fromCell = d->cell(fromRow, fromColumn);
    InventoryCell &toCell = d->cell(toRow, toColumn);

    if(fromCell.item.type() == Item::Unknown)
        return;

    if(fromCell.item == toCell.item)
    {
        toCell.count += fromCell.count;
    }
    else
    {
        toCell = fromCell;
    }
    fromCell = InventoryCell();

    Database::removeInventory(fromRow, fromColumn);
    Database::setInventory(toRow, toColumn, toCell.item, toCell.count);

    emit cellModified(fromRow, fromColumn);
    emit cellModified(toRow, toColumn);
}

void Inventory::clearCell(int row, int column)
{
    Q_D(Inventory);

    InventoryCell &cell = d->cell(row, column);
    if(cell.item.type() != Item::Unknown)
    {
        cell = InventoryCell();
        Database::removeInventory(row, column);
        emit cellModified(row, column);
    }
}
