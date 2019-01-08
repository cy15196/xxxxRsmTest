#include "portmanagermainwindow.h"
#include "ui_portmanagermainwindow.h"

#include "toptestportmanager.h"

#include <Windows.h>
#include <Dbt.h>
#include <QDebug>

PortManagerMainWindow::PortManagerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PortManagerMainWindow)
{
    ui->setupUi(this);
}

PortManagerMainWindow::~PortManagerMainWindow()
{
    delete ui;
}


bool PortManagerMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{

    if ("windows_dispatcher_MSG" == eventType
            || "windows_generic_MSG" == eventType)

        {
            MSG * msg = reinterpret_cast<MSG*>(message);
//            qDebug()<<"windows  message of type "<<eventType<<QString::number(msg->message,16);
            if(msg->message == WM_DEVICECHANGE )
            {

                PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
                switch(msg->wParam)
                {
                case DBT_DEVICEARRIVAL://检测到新设备
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {

                        qDebug() << "DBT_DEVICEARRIVAL";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_DEVICEQUERYREMOVE://请求移除设备，可能失败  此时刷新不会让移动设备消失
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        qDebug() << "DBT_DEVICEQUERYREMOVE";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_DEVICEQUERYREMOVEFAILED://去除中断
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        qDebug() << "DBT_DEVICEQUERYREMOVEFAILED";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_DEVICEREMOVEPENDING://即将删除，仍然有效
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        qDebug() << "DBT_DEVICEREMOVEPENDING";
                    }
                    break;

                case DBT_DEVICEREMOVECOMPLETE://设备不见了
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        qDebug() << "DBT_DEVICEREMOVECOMPLETE";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_CUSTOMEVENT:
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_HANDLE)
                    {
                        qDebug() << "DBT_CUSTOMEVENT";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_DEVNODES_CHANGED:
                {
                    qDebug() << "DBT_DEVNODES_CHANGED";
                    TopTestPortManager::instance()->refreshAviablePortList();
                }
                   break;

                default:
                    qDebug() << msg->wParam;
                }
//                outputDrives();
          }
    }
    return QWidget::nativeEvent(eventType,message,result);
}
