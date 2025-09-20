#include "ctitlebar.h"
#include <QHBoxLayout>
#include <windows.h>
#include <qt_windows.h>
#include <QMouseEvent>
#include <QDebug>

bool flag=true;
CTitleBar::CTitleBar(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void CTitleBar::initUI()
{
    //禁止父窗口影响子窗口样式,必须加上，否则样式不会起作用
    setAttribute(Qt::WA_StyledBackground);

    this->setFixedHeight(32+20);
    this->setStyleSheet("background-color:rgb(0,0,0)");

    _plogoLabel = new QLabel(this);
    _plogoLabel->setFixedSize(32,32);//修改logo长宽
    _plogoLabel->setStyleSheet(
            "background-image:url(:/logo.png);border:none");
    _ptitleTextLabel = new QLabel(this);
    _ptitleTextLabel->setText("嘉禾天橙国际大影院");
    _ptitleTextLabel->setFixedWidth(280);
    _ptitleTextLabel->setStyleSheet(
            "QLabel{font-family: 微软雅黑;"
                "font-weight:bold;"
                "font-size:30px;"
                "color:#FF8C00;background-color:rgb(0,0,0);}");

    _psetButton = new QPushButton(this);
    _psetButton->setFixedSize(32, 32);
    _psetButton->setStyleSheet(
            "QPushButton{background-image:url(:/orangeLocal.png);border:none}"
            "QPushButton:hover{"
                    "background-color:rgb(99,99,99);"
                    "background-image:url(:/redLocal.png);border:none;}");

    _pminButton = new QPushButton(this);
    _pminButton->setFixedSize(32, 32);
    _pminButton->setStyleSheet(
            "QPushButton{background-image:url(:/mini.png);border:none}"
            "QPushButton:hover{"
                    "background-color:rgb(99, 99, 99);"
                    "background-image:url(:/mini.png);border:none;}");

    _pmaxButton = new QPushButton(this);
    _pmaxButton->setFixedSize(33, 32);
    _pmaxButton->setStyleSheet(
            "QPushButton{background-image:url(:/max.png);border:none}"
            "QPushButton:hover{"
                    "background-color:rgb(99, 99, 99);"
                    "background-image:url(:/max.png);border:none;}");

    _pcloseButton = new QPushButton(this);
    _pcloseButton->setFixedSize(32, 32);
    _pcloseButton->setStyleSheet(
            "QPushButton{background-image:url(:/close.png);border:none}"
            "QPushButton:hover{"
                    "background-color:rgb(99, 99, 99);"
                    "background-image:url(:/close.png);border:none;}");

    QHBoxLayout * phLayout = new QHBoxLayout(this);
    phLayout->addWidget(_plogoLabel);
    phLayout->addWidget(_ptitleTextLabel);
    phLayout->addStretch();//添加一个弹簧

    phLayout->addWidget(_psetButton);

    QSpacerItem* pItem1 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
    phLayout->addSpacerItem(pItem1);

    phLayout->addWidget(_pminButton);

    QSpacerItem* pItem2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
    phLayout->addSpacerItem(pItem2);

    phLayout->addWidget(_pmaxButton);

    QSpacerItem* pItem3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
    phLayout->addSpacerItem(pItem3);

    phLayout->addWidget(_pcloseButton);
    phLayout->setContentsMargins(5,10,20,10);
    this->setLayout(phLayout);

    connect(_pminButton, &QPushButton::clicked, this, &CTitleBar::onClickedSlot);
    connect(_pmaxButton, &QPushButton::clicked, this, &CTitleBar::onClickedSlot);
    connect(_pcloseButton, &QPushButton::clicked, this, &CTitleBar::onClickedSlot);
    connect(_psetButton, &QPushButton::clicked, this, &CTitleBar::onClickedSlot);
}


void CTitleBar::mousePressEvent(QMouseEvent* event)
{
        qDebug()<<"leftbutton";
        int ret=ReleaseCapture();
        if(ret){
            QWidget* pWindow = this->window();//返回指向标题栏所属顶层窗口的指针。
            if (pWindow->isTopLevel()) //判断该窗口是否为顶层窗口（即独立窗口）
            {
                SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
            }
        }
}

void CTitleBar::onClickedSlot()
{
    QPushButton * pbtn = qobject_cast<QPushButton*>(sender());
    QWidget * pwindow = this->window();
    if(pbtn == _pminButton) {
        pwindow->showMinimized();
    } else if(pbtn == _pmaxButton) {
        if(pwindow->isMaximized()) {
            pwindow->showNormal();
            _pmaxButton->setStyleSheet(
                    "QPushButton{background-image:url(:/max.png);border:none}"
                    "QPushButton:hover{"
                            "background-color:rgb(99, 99, 99);"
                            "background-image:url(:/max.png);border:none;}");
        } else  {
            pwindow->showMaximized();
            _pmaxButton->setStyleSheet(
                    "QPushButton{background-image:url(:/max_back.png);border:none}"
                    "QPushButton:hover{"
                            "background-color:rgb(99, 99, 99);"
                            "background-image:url(:/max_back.png);border:none;}");
        }
    } else if(pbtn == _pcloseButton) {
        emit sigClose();//发射信号，通知父窗口关闭
    }
    else if(pbtn==_psetButton)
    {
        if(flag){
        _psetButton->setStyleSheet(
                "QPushButton{background-image:url(:/redLocal.png);border:none}"
                "QPushButton:hover{"
                        "background-color:rgb(99,99,99);"
                        "background-image:url(:/redLocal.png);border:none;}");
        flag=false;
        }
        else{
            _psetButton->setStyleSheet(
                    "QPushButton{background-image:url(:/orangeLocal.png);border:none}"
                    "QPushButton:hover{"
                            "background-color:rgb(99,99,99);"
                            "background-image:url(:/orangeLocal.png);border:none;}");
            flag=true;
        }
    }
}
