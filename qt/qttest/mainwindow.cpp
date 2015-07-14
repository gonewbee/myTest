#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gridLayout = new QGridLayout();/* 使用网格布局 */
    process = new QProcess(this);
    this->setWindowTitle(QString::fromUtf8("云应用"));
    testAppItems();
}

/**
 * @brief MainWindow::~MainWindow
 *
 * 循环调用appList.takeFirst()用于释放appList资源
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete gridLayout;
    delete process;
    while (!appList.isEmpty())
        delete appList.takeFirst();
}

/**
 * @brief MainWindow::app_btn_clicked
 *
 * 当多个按键对应一个slot时，可以通过dynamic_cast<AppItem *>(sender())获取到发送消息的对象。
 */
void MainWindow::app_btn_clicked() {
    AppItem *appitem = dynamic_cast<AppItem *>(sender());
    QString args = "/v:" + appitem->getRdpIp() + " /u:" + appitem->getRdpUser() + " /p:" + appitem->getRdpPwd()
            + " /app:\"" + appitem->getAppName() + "\"" + " /drive:home,/";
    QString app = "/home/zsy/Workspace/down/FreeRDP/client/X11/xfreerdp " + args;
    qDebug() << app;
    process->close();
    process->start(app);
}

void MainWindow::testAppItems() {
    AppItem *appitem = new AppItem();
    appitem->setRdpIp("192.168.1.74");
    appitem->setRdpUser("administrator");
    appitem->setRdpPwd("Aa123!");
    appitem->setAppName("||WINWORD");
    appitem->setText("WINWORD");
    appList.append(appitem);
    connect(appitem, SIGNAL(clicked()), this, SLOT(app_btn_clicked()));

    AppItem *appitem2 = new AppItem();
    appitem2->setRdpIp("192.168.1.74");
    appitem2->setRdpUser("administrator");
    appitem2->setRdpPwd("Aa123!");
    appitem2->setAppName("||EXCEL");
    appitem2->setText("EXCEL");
    appList.append(appitem2);
    connect(appitem2, SIGNAL(clicked()), this, SLOT(app_btn_clicked()));

    gridLayout->setSpacing(2);
    gridLayout->setMargin(2);
    int i = 0;
    /* 使用QListIterator遍历QList */
    QListIterator<AppItem *> it(appList);
    while (it.hasNext()) {
        gridLayout->addWidget(it.next(), 0, i++);
    }
    QWidget* widget = new QWidget(this);
    widget->setLayout(gridLayout);
    this->setCentralWidget(widget);
}

/**
 * @brief 测试网格布局
 *
 * QMainWindow不能直接setLayout(gridLayout)，要setCentralWidget(widget)后再widget->setLayout(gridLayout)
 */
void MainWindow::testGridLayout() {
    const QString texts[] = {"1", "2","3","4","5","6","7","8","9"};
    this->resize(250, 100);
    gridLayout->setSpacing(2);
    gridLayout->setMargin(2);
    for(int i=0, k = 0; i < 3;i++,k+=3)
    {
        for(int j = 0; j < 3;j++)
        {
            QLabel *label = new QLabel(texts[k+j]);
            label->setFrameStyle(QFrame::Panel + QFrame::Sunken);
            label->setMinimumSize(55,0);
            label->setAlignment(Qt::AlignCenter);
            gridLayout->addWidget(label,i,j);
        }
    }
    QWidget* widget = new QWidget(this);
    widget->setLayout(gridLayout);
    this->setCentralWidget(widget);
}
