#include "appitem.h"
#include <QDebug>

AppItem::AppItem(QWidget *parent) :
    QPushButton(parent)
{
    iconFile = NULL;
    accessManager = NULL;
    reply = NULL;
}

AppItem::~AppItem() {
    if (iconFile)
        delete iconFile;
    if (accessManager)
        delete accessManager;
}

QString AppItem::getRdpIp() {
    return rdpIp;
}

void AppItem::setRdpIp(QString ip) {
    rdpIp = ip;
}

QString AppItem::getRdpUser() {
    return rdpUser;
}

void AppItem::setRdpUser(QString user) {
    rdpUser = user;
}

QString AppItem::getRdpPwd() {
    return rdpPwd;
}

void AppItem::setRdpPwd(QString pwd) {
    rdpPwd = pwd;
}

QString AppItem::getAppName() {
    return appName;
}

void AppItem::setAppName(QString name) {
    appName = name;
}

QString AppItem::getAppCmd() {
    return appCmd;
}

void AppItem::setAppCmd(QString cmd) {
    appCmd = cmd;
}

void AppItem::httpReadyRead() {
    qDebug() << "httpReadyRead";
    if (iconFile)
        iconFile->write(reply->readAll());
}

void AppItem::httpFinished() {
    iconFile->flush();
    iconFile->close();
    reply->deleteLater();
    QPixmap pixmap;
    pixmap.load(iconName);
    this->setIcon(pixmap);
    this->setIconSize(QSize(pixmap.width(),pixmap.height()));
}

/**
 * @brief AppItem::downNetIcon 下载网络图标并显示
 *
 * 先判断图标文是否已存在，不存在再下载
 * @param url 图标的地址
 */
void AppItem::downNetIcon(QString url) {
    iconName = url.section('/', -1, -1); //使用'/'分割，取最后一个字符串作为文件名称
    qDebug() << "iconName::" << iconName;
    if (!QFile::exists(iconName)) {
        //图标文件不存在
        iconFile = new QFile(iconName);
        iconFile->open(QIODevice::ReadWrite);
        accessManager = new QNetworkAccessManager(this);
        reply = accessManager->get(QNetworkRequest(url));
        connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
        connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
    } else {
        QPixmap pixmap;
        pixmap.load(iconName);
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(),pixmap.height()));
    }
}
