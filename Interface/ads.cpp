#include "ads.h"
#include <QDesktopServices>
#include <QUrl>

Ads::Ads(QWidget *parent) :
    QWidget(parent)
{
    createComponent();
    connections();
    input();
}

void Ads::createComponent()
{
    view = new QWebView(this);
}

void Ads::input()
{
    view->load(QUrl("http://www.jaguati.com.br/ads/pedyrum.php"));
    view->show();
    view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

void Ads::connections()
{
    connect(view,SIGNAL(linkClicked(QUrl)),SLOT(click(QUrl)));
    connect(view,SIGNAL(loadFinished(bool)),SLOT(check(bool)));
}

void Ads::click(QUrl url)
{
    QDesktopServices::openUrl(url);
}

bool Ads::isConnected()
{
    return _isConnected;
}

void Ads::check(bool b)
{
    _isConnected = b;
}

void Ads::resizeEvent(QResizeEvent *)
{
    view->setGeometry(0,0,this->width(),this->height());
}

