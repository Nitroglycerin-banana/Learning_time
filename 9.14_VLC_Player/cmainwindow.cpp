#include "cmainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <vlcwidget.h>
#include <QDebug>
#include <QKeyEvent>
#include <QPalette>

CMainWindow::CMainWindow(QWidget * parent)
    : CFramelessWidget(parent)
{
    initUI();

}

//全屏模式相关
BOOL CALLBACK EnumVLC(HWND hwnd, LPARAM lParam)
{
    TCHAR szTitle[1024];

    int nLen = GetWindowText(hwnd, szTitle, 1024);

    if (nLen > 0)
    {
        EnableWindow(hwnd, FALSE);
        KillTimer(NULL, 1);
    }

    return TRUE;
}

//全屏模式相关
void CALLBACK TimeProc(HWND hwnd, UINT msg, UINT_PTR id, DWORD time)
{
    HWND vlcHwnd = FindWindowEx(NULL, NULL, NULL, L"王道播放器");

    if (vlcHwnd)
    {
        EnumChildWindows(vlcHwnd, EnumVLC, NULL);
    }
}

void CMainWindow::closeSlot()
{
    QMessageBox::StandardButton isExit = QMessageBox::warning(this, "提示", "你确定要退出吗？",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(isExit == QMessageBox::Yes) {
        close();
    }
}

void CMainWindow::initUI()
{
    this->setWindowTitle("王道播放器");
    _ptitleBar = new CTitleBar(this);
    VLCWidget * wbody = new VLCWidget(this);

    int nret = SetTimer(NULL, 1, 300, TimeProc);//全屏模式相关
    wbody->setMinimumSize(800, 600);

    QVBoxLayout * pvLayout = new QVBoxLayout(this);
    pvLayout->addWidget(_ptitleBar);
    pvLayout->addWidget(wbody);
    pvLayout->setContentsMargins(0, 0, 0, 0);
    pvLayout->setSpacing(0);
    connect(_ptitleBar, &CTitleBar::sigClose, this, &CMainWindow::closeSlot);
    connect(this,&CMainWindow::sighide,wbody,&VLCWidget::hide);
}

void CMainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"double clicked";
    if(this->isFullScreen()) {
        this->showNormal();
        emit sighide(false);//关闭播放列表
        //_ptitleBar->show();
    } else{
        this->showFullScreen();
        //_ptitleBar->hide();
        emit sighide(true);//打开播放列表
    }
}

//void CMainWindow::mousePressEvent(QMouseEvent *event)
//{
//    static bool condition=true;
//    if(condition)
//    {

//    }
//}

//全屏模式相关
void CMainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        this->showNormal();
    }
}
