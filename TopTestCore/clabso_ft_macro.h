#ifndef CLABSO_FT_MACRO_H
#define CLABSO_FT_MACRO_H

#include <QtCore/qglobal.h>

#ifdef Q_COMPILER_NULLPTR
#  define CLABSO_NULLPTR nullptr
#else
#  define CLABSO_NULLPTR NULL
#endif

#include <QString>
#include <QStringList>
#include <QCoreApplication>

#define ClabsoDataDir QString(".cs")
#define ClabsoProjectFile QString("csp")
#define ClabsoCaseFile QString("csc")
#define ClabsoModelFile QString("csm")

#define stringEnum(enumName,...)\
public:\
    enum enumName{ \
        ##__VA_ARGS__ \
    };\
    static QString get##enumName##ToString(int num)\
    {\
        return QString(###__VA_ARGS__).remove(' ').remove('\n').section(",",num,num);   \
    }\
    static QStringList get##enumName##List()\
    {\
        return QString(###__VA_ARGS__).remove(' ').remove('\n').split(",");   \
    }\
    static enumName get##StringTo##enumName(const QString&name)\
    {\
        return (enumName) get##enumName##List().indexOf(name);\
    }\
    static QString get##enumName##Translate(int num)\
    {\
        return QCoreApplication::translate(#enumName, get##enumName##ToString(num));\
    }\
    static QStringList get##enumName##TranslateList()\
    {\
        QStringList translateList;\
        for (QString enumItem : get##enumName##List())\
        {\
            translateList << QCoreApplication::translate(#enumName, enumItem);\
        }\
        return translateList;\
    }\


#define FT_stringEnum(enumName,...)\
    enum enumName{ \
        ##__VA_ARGS__ \
    };\
    inline static QString get##enumName##ToString(int num)\
    {\
        return QString(###__VA_ARGS__).remove(' ').remove('\n').section(",",num,num);   \
    }\
    inline static QStringList get##enumName##List()\
    {\
        return QString(###__VA_ARGS__).remove(' ').remove('\n').split(",");   \
    }\
    inline static enumName get##StringTo##enumName(const QString&name)\
    {\
        return (enumName) get##enumName##List().indexOf(name);\
    }\
    inline static QString get##enumName##Translate(int num)\
    {\
        return QCoreApplication::translate(#enumName, get##enumName##ToString(num).toLatin1().data());\
    }\
    inline static QStringList get##enumName##TranslateList()\
    {\
        QStringList translateList;\
        for (QString enumItem : get##enumName##List())\
        {\
            translateList << QCoreApplication::translate(#enumName, enumItem.toLatin1().data());\
        }\
        return translateList;\
    }\
//    inline static void addTranslate()\
//    {\
//        for (QString enumItem : get##enumName##List())\
//        {\
//            QCoreApplication::translate(#enumName, enumItem.toLatin1().data());\
//        }\
//    }\

//#define FT_enumForTranslate(enumName)\
//    for (QString enumItem : get##enumName##List())\
//    {\
//        QCoreApplication::translate(#enumName, enumItem.toLatin1().data());\
//    }\

//#define FT_stringEnum(enumName,...)\
//    enum enumName{ \
//        ##__VA_ARGS__ \
//    };\
//    static QString get##enumName##ToString(int num)\
//    {\
//        QString origin(###__VA_ARGS__).remove(' ').remove('\n').section(",",num,num);\
//        if()\
//        return ;   \
//    }\
//    static QStringList get##enumName##List()\
//    {\
//        return QString(###__VA_ARGS__).remove(' ').remove('\n').split(",");   \
//    }\
//    static enumName get##StringTo##enumName(const QString&name)\
//    {\
//        return (enumName) get##enumName##List().indexOf(name.remove(' '));\
//    }\

#endif // CLABSO_FT_MACRO_H
