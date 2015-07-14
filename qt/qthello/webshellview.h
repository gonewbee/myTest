#ifndef WEBSHELLVIEW_H
#define WEBSHELLVIEW_H

#include <QMainWindow>
#include <QWebView>

namespace Ui {
class WebShellView;
}

class WebShellView : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit WebShellView(QWidget *parent = 0);
    ~WebShellView();

protected:
    void resizeEvent(QResizeEvent *);

private:
    Ui::WebShellView *ui;
    QWebView *webView;
};

#endif // WEBSHELLVIEW_H
