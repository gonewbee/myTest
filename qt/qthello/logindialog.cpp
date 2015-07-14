#include <QMessageBox>
#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromUtf8("登陆"));
    setting = new SettingDialog(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
    delete setting;
}

void LoginDialog::on_btn_login_clicked()
{
    if (ui->edit_username->text()=="zsy" && ui->edit_password->text()=="123456") {
        this->accept();
    } else {
        QMessageBox::warning(this, QString::fromUtf8("警告"), QString::fromUtf8("账号或密码错误"), QMessageBox::Yes);
        ui->edit_username->clear();
        ui->edit_password->clear();
        ui->edit_username->setFocus();
    }
}

void LoginDialog::on_btn_exit_clicked()
{
    this->close();
}

void LoginDialog::on_btn_setting_clicked()
{
    setting->show();
}
