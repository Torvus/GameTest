#ifndef INVENTORYVIEW_H
#define INVENTORYVIEW_H

#include <QTableView>

class Inventory;

class InventoryView : public QTableView
{
    Q_OBJECT
public:
    explicit InventoryView(QWidget *parent = nullptr);

    Inventory *inventory() const;
    void setInventory(Inventory *inventory);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // INVENTORYVIEW_H
