#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
#if true
    QIcon icon = QIcon(":/img/test.png");
    qDebug() << icon.isNull();
    a.setWindowIcon(icon);
    LoginDialog loginDialog;
    if (loginDialog.exec()==LoginDialog::Accepted) {
        w.show();
        return a.exec();
    } else {
        return 0;
    }
#else
    w.show();
    return a.exec();
#endif
}
