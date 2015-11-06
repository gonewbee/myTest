#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QIcon>
#include <QSystemTrayIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(":/img/test.png"));
    trayIcon->show();
    LoginDialog loginDialog;
    if (loginDialog.exec()==LoginDialog::Accepted) {
        w.show();
        return a.exec();
    } else {
        return 0;
    }
}
