#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

public:
    void show();

private slots:
    void on_btn_ok_cancle_accepted();

    void on_btn_ok_cancle_rejected();

private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
