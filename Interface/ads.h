#ifndef ADS_H
#define ADS_H

#include <QWidget>
#include <QtWebKit/QWebView>

class Ads : public QWidget
{
    Q_OBJECT
public:
    explicit Ads(QWidget *parent = 0);

    void createComponent();

    void input();

    void connections();

    void resizeEvent(QResizeEvent *);

signals:

public slots:

    void click(QUrl url);

    bool isConnected();

    void check(bool b);

private:

    QWebView * view;

public:
    bool _isConnected;

};

#endif // ADS_H
