#include "webshellview.h"
#include "ui_webshellview.h"

WebShellView::WebShellView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WebShellView)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromUtf8("网页"));
    webView = new QWebView(this);
    webView->setGeometry(0, 0, this->width(), this->height());
    webView->load(QUrl("http://www.baidu.com"));
}

WebShellView::~WebShellView()
{
    delete ui;
    delete webView;
}

/**
  * @brief 重写窗口大小变化事件，在窗口大小变化时重新设置QWebView的大小
  */
void WebShellView::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    webView->setGeometry(0, 0, this->width(), this->height());
}
