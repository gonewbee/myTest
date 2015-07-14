#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
#if true
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
