#ifndef APPITEM_H
#define APPITEM_H
/**
  * @brief 自定义类，继承QPushButton。
  *
  * 在
  */

#include <QPushButton>

class AppItem : public QPushButton
{
public:
    AppItem();

    QString getRdpIp();
    void setRdpIp(QString ip);
    QString getRdpUser();
    void setRdpUser(QString user);
    QString getRdpPwd();
    void setRdpPwd(QString pwd);
    QString getAppName();
    void setAppName(QString name);
    QString getAppCmd();
    void setAppCmd(QString cmd);


private:
    QString rdpIp;
    QString rdpUser;
    QString rdpPwd;
    QString appName;
    QString appCmd;
};

#endif // APPITEM_H
