#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

QT_FORWARD_DECLARE_CLASS(QSqlQuery)

class Item;

class Database : public QObject
{
    Q_OBJECT
public:
    static bool init();
    static bool setInventory(int row, int column, const Item &item, unsigned count);
    static bool removeInventory(int row, int column);
    static QSqlQuery fetchInventory();

private:
    static bool checkSqlError(const QSqlQuery &query);
};

#endif // DATABASE_H
