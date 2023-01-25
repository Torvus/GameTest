#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "itemmimedata.h"
#include "item.h"
#include "itemdelegate.h"
#include "inventory.h"
#include "inventorymodel.h"

#include <QStackedWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDrag>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mInventory(new Inventory(3, 3, this))
{
    ui->setupUi(this);
    setAcceptDrops(true);
    ui->inventoryView->setItemDelegate(new ItemDelegate(this));
    ui->appleLabel->setProperty("item", qVariantFromValue(Item(Item::Apple)));
    ui->inventoryView->setInventory(mInventory);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::mousePressEvent
 * Start copy Drag'n'Drop
 * @param event
 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget *source = childAt(event->pos());
    if(!source)
        return;

    if(QLabel *label = qobject_cast<QLabel*>(source))
    {
        Item item = label->property("item").value<Item>();

        QDrag *drag = new QDrag(this);
        drag->setMimeData(new ItemMimeData(item, 1));
        drag->setPixmap(QPixmap(item.imagePath()));
        drag->setHotSpot(event->pos() - label->pos());
        drag->start(Qt::CopyAction);
    }
}

/**
 * @brief MainWindow::on_newGameButton_clicked
 * Show game board
 */
void MainWindow::on_newGameButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page2);
}

/**
 * @brief MainWindow::on_toolButton_clicked
 * Show main menu
 */
void MainWindow::on_toolButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page1);
}

