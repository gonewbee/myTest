#include <QtGui/QApplication>
#include "logindialog.h"
#include "webshellview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog loginDialog;
    WebShellView view;
    if (loginDialog.exec()==QDialog::Accepted) {
        view.show();
        return a.exec();
    }
    return 0;
}
