#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

class Config
{
public:
    /**
      * @brief 单实例
      */
    static Config& getInstance();
    /**
      * @brief 配置文件路径
      */
    QString getCfgFilePath();

    QString getIp();
    int getPort();
    void setIp(QString ip);
    void setPort(int port);
    void setIpPort(QString ip, int port);
    ~Config();

private:
    Config();
    static const QString _path;  /**< 配置文件相对路径 */
    static const QString IPKEY; /**< 使用QSettings存取ip的key */
    static const QString PORTKEY; /**< 使用QSettings存取port的key */
    QSettings *settings; /**< 使用QSettings保存配置信息 */
    QString cfgFilePath; /**< 配置文件绝对路径 */
    QString ip; /** 服务端ip */
    int port; /** 服务端端口 */
};

#endif // CONFIG_H
