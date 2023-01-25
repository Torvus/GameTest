#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QDrag;
QT_END_NAMESPACE

class Inventory;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private Q_SLOTS:
    void on_newGameButton_clicked();
    void on_toolButton_clicked();

private:
    Ui::MainWindow *ui;
    Inventory *mInventory;
};
#endif // MAINWINDOW_H
