#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include "cframelesswidget.h"
#include "ctitlebar.h"
#include <windows.h>



class CMainWindow
: public CFramelessWidget
{
    Q_OBJECT
public:
    explicit CMainWindow(QWidget * parent= nullptr);


signals:
    void sighide(bool set);
private slots:
    void closeSlot();//与子对象CTitleBar::sigClose信号绑定
    void mouseDoubleClickEvent(QMouseEvent *event);
    //void mousePressEvent(QMouseEvent *event);//单击事件和暂停/播放绑定
    void keyPressEvent(QKeyEvent *event);

private:
    void initUI();

private:
    CTitleBar * _ptitleBar;//主窗口中包含标题栏
};

#endif // CMAINWINDOW_H
