#include "clabso_ft_io.h"
#include <QDateTime>
#include <mutex>
#include <thread>
#include <QFile>
#include <toptestcore.h>
#include <QTextStream>
#include <QDir>
#include <QApplication>
//#include "clabso_ft_settingmanager.h"

QMap<QString, QList<QPair<ClabsoFTo::virtualo *, int> > > ClabsoFTo::streams;

struct ClabsoFToPrivate
{
    ClabsoFToPrivate() : stream(&buf)
    {
        stream.setAutoInsertSpaces(false);
    }

    QList<QPair<ClabsoFTo::virtualo *, int> > streams;
    QString buf;
    QDebug stream;

    static std::mutex mutex;
    static bool inited;
    static QString appOpenDateTime;
    static void init()
    {
        if (inited) return;
        std::lock_guard<std::mutex> lock(mutex);
        if (inited) return;
        ClabsoFTo::registration("logs", &[] (const QString &data) {
//            if (!cSettings->getMakeLogs())
//                return;
//            if (ClabsoCore->getLogsPath().isEmpty())
//                return;
            QString filePath = QDir::homePath()+"/Documents/TopTest/logs";
            if (!filePath.endsWith("/"))
                filePath += "/";
            if (!QDir(filePath).exists())
                QDir().mkpath(filePath);
            filePath += "Logs_";
            filePath += qApp->applicationName();
            filePath += "_";
            filePath += qApp->applicationVersion().replace('.','_');
            filePath += "_";
            filePath += appOpenDateTime;
            qDebug() << filePath;
            QFile file(filePath);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "open error :" << file.errorString();
                return;
            }
            QTextStream s(&file);
            s << data << endl;
            file.close();
        }, ClabsoFTo::Intact | ClabsoFTo::DataTime);
        inited = true;
    }
};

std::mutex ClabsoFToPrivate::mutex;
bool ClabsoFToPrivate::inited = false;
QString ClabsoFToPrivate::appOpenDateTime = QDateTime::currentDateTime().toString(Qt::ISODate).replace('-','_').replace('T','_').replace(':','_');

ClabsoFTo::ClabsoFTo() :
    ClabsoFTo("")
{

}

ClabsoFTo::ClabsoFTo(const QString &oTag) :
    d_ptr(new ClabsoFToPrivate)
{
    ClabsoFToPrivate::init();
    d_ptr->streams = streams.value(oTag);
}

ClabsoFTo::~ClabsoFTo()
{
    QString dataTime = "[" + QDateTime::currentDateTime().toString(Qt::ISODateWithMs) + "] ";
    for (QPair<virtualo *, int> out : d_ptr->streams)
    {   
        switch (out.second & 0x1)
        {
        case Intact:
            {
                if(d_ptr->buf.isEmpty())
                    continue;

                if (out.second & 0x2)
                    *(out.first) << dataTime + d_ptr->buf;
                else
                    *(out.first) << d_ptr->buf;
            }
            break;
        case Simple:
            {
                if(d_ptr->buf.section("##", 0, 0).isEmpty())
                    continue;

                if (out.second & 0x2)
                    *(out.first) << dataTime + d_ptr->buf.section("##", 0, 0);
                else
                    *(out.first) << d_ptr->buf.section("##", 0, 0);
            }
            break;
        default:
            break;
        }
    }
    qDebug() << (dataTime + d_ptr->buf).toUtf8().data();
}

QDebug &ClabsoFTo::getStream()
{
    return d_ptr->stream;
}

ClabsoFTo::ClabsoFTo(ClabsoFTo::virtualo *vo, int mode) :
    d_ptr(new ClabsoFToPrivate)
{
    ClabsoFToPrivate::init();
    QPair<ClabsoFTo::virtualo *, int> p;
    p.first = vo;
    p.second = mode;
    d_ptr->streams.clear();
    d_ptr->streams.append(p);
}

void ClabsoFTo::_registration(const QString &oTag, ClabsoFTo::virtualo *vo, int mode)
{
    if (!streams.keys().contains(oTag))
        streams.insert(oTag, QList<QPair<virtualo *, int> >());
    QList<QPair<virtualo *, int> > &os = streams[oTag];
    bool has = false;
    for (int i=0; i<os.count(); ++i)
    {
        if (*vo == *(os[i].first))
        {
            has = true;
            os[i].second = mode;
            break;
        }
    }
    if (!has)
    {
        QPair<virtualo *, int> vo_;
        vo_.first = vo;
        vo_.second = mode;
        os.append(vo_);
    }
}

void ClabsoFTo::_unregistration(const QString &oTag, ClabsoFTo::virtualo *vo)
{
    if (!streams.keys().contains(oTag))
        return;
    QList<QPair<virtualo *, int> > &os = streams[oTag];
    for (int i=0; i<os.count(); ++i)
    {
        if (*vo == *(os[i].first))
        {
            delete os[i].first;
            os.removeAt(i);
            return;
        }
    }
}
