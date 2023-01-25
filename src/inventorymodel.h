#ifndef INVENTORYMODEL_H
#define INVENTORYMODEL_H

#include <QAbstractTableModel>

class Inventory;
class InventoryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit InventoryModel(QObject *parent = nullptr);

    Inventory *inventory() const;
    void setInventory(Inventory *inventory);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    Qt::DropActions supportedDropActions() const override;
    Qt::DropActions supportedDragActions() const override;

private Q_SLOTS:
    void onCellModified(int row, int column);

private:
    Inventory *mInventory;
};

#endif // INVENTORYMODEL_H
