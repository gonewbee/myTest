#include <QDir>
#include <QDebug>
#include "config.h"

/* 初始化static const的string */
const QString Config::_path = "/.helloworld/setting.ini";
const QString Config::IPKEY = "network/ip";
const QString Config::PORTKEY = "network/port";

/**
  * 构造时设置ip和port的默认值。
  * 使用QDir::homePath()获取用户根目录。
  */
Config::Config():
    ip("127.0.0.1"),
    port(80)
{
    QString homePath = QDir::homePath();
    cfgFilePath = homePath + _path;
    qDebug() << "cfg file path:" << cfgFilePath << ";" << ip << port;
    settings = new QSettings(cfgFilePath, QSettings::IniFormat);
    QString tmpIp = settings->value(IPKEY, "").toString();
    if (tmpIp=="") {
        settings->setValue(IPKEY, ip);
    } else {
        ip = tmpIp;
    }
    int tmpPort = settings->value(PORTKEY, -1).toInt();
    if (tmpPort==-1) {
        settings->setValue(PORTKEY, port);
    } else {
        port = tmpPort;
    }
}

Config::~Config() {
    qDebug() << "~Config";
    delete settings;
}

/**
  * @brief 提供单实例
  *
  * 不使用指针实例的原因是指针实例需要自己释放，可能会造成释放不及时的情况
  * 函数返回值后的"&"是引用，在使用的时候返回的是instance实例，不会创建副本
  */
Config& Config::getInstance() {
    //唯一的实例
    //可以确保被释放
    static Config instance;
    return instance;
}

/**
  * @brief 获取配置文件路径
  */
QString Config::getCfgFilePath() {
    return cfgFilePath;
}

QString Config::getIp() {
    return ip;
}

void Config::setIp(QString ip) {
    this->ip = ip;
    settings->setValue(IPKEY, ip);
}

int Config::getPort() {
    return port;
}

void Config::setPort(int port) {
    this->port = port;
    settings->setValue(PORTKEY, port);
}

void Config::setIpPort(QString ip, int port) {
    setIp(ip);
    setPort(port);
}

