#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_btn_login_clicked();

    void on_btn_cancle_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
