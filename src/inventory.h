#ifndef INVENTORY_H
#define INVENTORY_H

#include <QObject>

QT_FORWARD_DECLARE_CLASS(QSqlQuery);

class Item;

class InventoryPrivate;
class Inventory : public QObject
{
    Q_OBJECT
public:
    explicit Inventory(int rows, int columns, QObject *parent = nullptr);
    ~Inventory();

    int rows() const;
    int columns() const;

    Item itemAt(int row, int column) const;
    uint itemCountAt(int row, int column) const;

signals:
    void cellModified(int row, int column);

public Q_SLOTS:
    void addItem(int row, int column, const Item &item, uint count = 1);
    void removeItem(int row, int column, uint count = 1);
    void moveItems(int fromRow, int fromColumn, int toRow, int toColumn);
    void clearCell(int row, int column);

private:
    InventoryPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(Inventory)
};

#endif // INVENTORY_H
