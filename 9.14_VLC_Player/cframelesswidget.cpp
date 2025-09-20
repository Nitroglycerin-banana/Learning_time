#include "cframelesswidget.h"
#include <qt_windows.h>
#include <windows.h>
#include <windowsx.h>

CFramelessWidget::CFramelessWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowMinMaxButtonsHint*/);
    setAttribute(Qt::WA_Hover);
}

CFramelessWidget::~CFramelessWidget()
{}

//该函数实现了窗口的拖拽拉伸
bool CFramelessWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    MSG* param = static_cast<MSG*>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

        if (nX > m_nBorderWidth && nX < this->width() - m_nBorderWidth &&
            nY > m_nBorderWidth && nY < this->height() - m_nBorderWidth)
        {
            if (childAt(nX, nY) != nullptr)
                return QWidget::nativeEvent(eventType, message, result);
        }

        if ((nX > 0) && (nX < m_nBorderWidth))
            *result = HTLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
            *result = HTRIGHT;

        if ((nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOP;

        if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOM;

        if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
            && (nY < m_nBorderWidth))
            *result = HTTOPLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
            && (nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOPRIGHT;

        if ((nX > 0) && (nX < m_nBorderWidth)
            && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
            && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;

        return true;
    }//end of case
    }//end of switch

    return false;
}
