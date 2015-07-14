#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QList>
#include <QProcess>
#include <appitem.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGridLayout *gridLayout;
    QProcess *process;
    QList<AppItem *> appList; /**< 远程应用信息列表，循环使用delete appList.takeFirst()进行释放 */

    void testGridLayout();
    void testAppItems();

public slots:
    void app_btn_clicked();
};

#endif // MAINWINDOW_H
