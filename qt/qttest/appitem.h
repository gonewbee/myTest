#ifndef APPITEM_H
#define APPITEM_H
/**
  * @brief 自定义类，继承QPushButton。
  *
  * 在
  */

#include <QPushButton>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class AppItem : public QPushButton
{
    Q_OBJECT //如果要加slot的话，需要在h文件中加Q_OBJECT，如果加Q_OBJECT后编译出错，重新执行qmake

public:
    explicit AppItem(QWidget *parent = 0);
    ~AppItem();

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

    void downNetIcon(QString url);
private:
    QString rdpIp;
    QString rdpUser;
    QString rdpPwd;
    QString appName;
    QString appCmd;
    QString iconName; /**< 图标名称 */
    QFile *iconFile; /**< 用于保存网络下载的图标 */

    QNetworkAccessManager *accessManager;
    QNetworkReply *reply;

private slots:
    void httpReadyRead();
    void httpFinished();
};

#endif // APPITEM_H
