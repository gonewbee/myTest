#include <QDebug>
#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "config.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromUtf8("设置"));
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

/**
  * 每次显示的时候重新获取ip和port
  */
void SettingDialog::show() {
    Config &config = Config::getInstance();
    ui->edit_ip->setText(config.getIp());
    ui->edit_port->setText(QString::number(config.getPort(), 10));
    QDialog::show();
}

void SettingDialog::on_btn_ok_cancle_accepted()
{
    Config::getInstance().setIpPort(ui->edit_ip->text(), ui->edit_port->text().toInt());
    this->close();
}

void SettingDialog::on_btn_ok_cancle_rejected()
{
    this->close();
}
