#include "database.h"
#include "item.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QApplication>

bool Database::init()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("database.sqlite");
    if(!database.open())
    {
        QMessageBox::critical(0, tr("Error"), tr("Database open error"));
        return false;
    }
    else
    {
        QSqlQuery query;
        query.exec(QLatin1Literal("CREATE TABLE IF NOT EXISTS items ("
                                  "id INTEGER PRIMARY KEY, "
                                  "name TEXT NOT NULL)"));
        if(checkSqlError(query))
            return false;

        query.exec(QLatin1Literal("CREATE TABLE IF NOT EXISTS inventory("
                                  "row INTEGER, "
                                  "column INTEGER, "
                                  "item INTEGER NOT NULL, "
                                  "count INTEGER NOT NULL, "
                                  "PRIMARY KEY(row, column), "
                                  "FOREIGN KEY(item)"
                                  " REFERENCES items(id)"
                                  " ON DELETE CASCADE"
                                  " ON UPDATE NO ACTION)"));
        if(checkSqlError(query))
            return false;

        query.exec(QString(QLatin1Literal("INSERT OR REPLACE INTO items(id, name) VALUES(%1, '%2')"))
                   .arg(Item::Apple)
                   .arg(Item(Item::Apple).name()));
        if(checkSqlError(query))
            return false;

    }
    return true;
}

bool Database::setInventory(int row, int column, const Item &item, unsigned count)
{
    if(!QSqlDatabase::database().isOpen())
        return false;
    if(item.type() == Item::Unknown || !count)
        return removeInventory(row, column);

    QString sql = QLatin1Literal("INSERT OR REPLACE INTO inventory(row, column, item, count) "
                                 "VALUES(%1, %2, %3, %4)");
    sql = sql
            .arg(row)         // %1
            .arg(column)      // %2
            .arg(item.type()) // %3
            .arg(count);      // %4

    QSqlQuery query;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool ok = query.exec(sql);
    QApplication::restoreOverrideCursor();
    if(!ok)
    {
        checkSqlError(query);
    }
    return ok;
}

bool Database::removeInventory(int row, int column)
{
    if(!QSqlDatabase::database().isOpen())
        return false;
    QString sql = QLatin1Literal("DELETE FROM inventory WHERE row = %1 AND column = %2");
    sql = sql
            .arg(row)     // %1
            .arg(column); // %2

    QSqlQuery query;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool ok = query.exec(sql);
    QApplication::restoreOverrideCursor();
    if(!ok)
    {
        checkSqlError(query);
    }
    return ok;
}

QSqlQuery Database::fetchInventory()
{
    if(QSqlDatabase::database().isOpen())
    {
        QSqlQuery query;
        if(query.exec(QLatin1Literal("SELECT row, column, item, count FROM inventory")))
            return query;
    }
    return QSqlQuery();
}

bool Database::checkSqlError(const QSqlQuery &query)
{
    QSqlError error = query.lastError();
    if(error.isValid())
    {
        QMessageBox::warning(nullptr, tr("Warning"),
                             tr("SQL error: %1\nQuery: %2").arg(error.driverText(),
                                                                query.executedQuery()),
                             QMessageBox::Close);
        return true;
    }
    return false;
}
