#include "appitem.h"

AppItem::AppItem()
{
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
