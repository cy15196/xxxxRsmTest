#ifndef CLABSO_FT_IO_H
#define CLABSO_FT_IO_H

#include "toptestcore_global.h"
#include <QMap>
#include <QString>
#include <QDebug>

#define clabsoO(tag) ClabsoFTo(#tag).getStream()
#define csDebug clabsoO(debug)
#define csLogs clabsoO(logs)

#define CsLogs_Base csLogs << this << " : "

#define CsLogs0(info) \
    CsLogs_Base << QString(info).toUtf8().data()
#define CsLogs1(info, arg1) \
    CsLogs_Base << QString(info).arg(arg1).toUtf8().data()
#define CsLogs2(info, arg1, arg2) \
    CsLogs_Base << QString(info).arg(arg1).arg(arg2).toUtf8().data()
#define CsLogs3(info, arg1, arg2, arg3) \
    CsLogs_Base << QString(info).arg(arg1).arg(arg2).arg(arg3).toUtf8().data()
#define CsLogs4(info, arg1, arg2, arg3, arg4) \
    CsLogs_Base << QString(info).arg(arg1).arg(arg2).arg(arg3).arg(arg4).toUtf8().data()
#define CsLogs5(info, arg1, arg2, arg3, arg4, arg5) \
    CsLogs_Base << QString(info).arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5).toUtf8().data()
#define CsLogs6(info, arg1, arg2, arg3, arg4, arg5, arg6) \
    CsLogs_Base << QString(info).arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5).arg(arg6).toUtf8().data()
#define CsLogs7(info, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    CsLogs_Base << QString(info).arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5).arg(arg6).arg(arg7).toUtf8().data()
#define CsLogs8(info, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    CsLogs_Base << QString(info).arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5).arg(arg6).arg(arg7).arg(arg8).toUtf8().data()
#define CsLogs9(info, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
    CsLogs_Base << QString(info).arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5).arg(arg6).arg(arg7).arg(arg8).arg(arg9).toUtf8().data()
#define CsLogs10(info, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
    CsLogs_Base << QString(info).arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5).arg(arg6).arg(arg7).arg(arg8).arg(arg9).arg(arg10).toUtf8().data()

#define CsLogs(info, ...) VA_ARGS_MACRO_HELPER(CsLogs,1,info,__VA_ARGS__)

struct ClabsoFToPrivate;

class TOPTESTCORESHARED_EXPORT ClabsoFTo
{
    friend struct ClabsoFToPrivate;

public:
    enum Mode
    {
        Simple = 0x0,
        Intact = 0x1,
        DataTime = 0x2
    };

    // if there is no oTag or oTag is not exist ，then only the QDebug will hold the output
    ClabsoFTo();
    ClabsoFTo(const QString &oTag);

    template<typename _func_>
    ClabsoFTo(_func_ func, int mode/* = Intact | DataTime*/) :
        ClabsoFTo((virtualo *)(new o_1<_func_>(func)), mode)
    {}

    template<typename _object_>
    ClabsoFTo(_object_ *object, void (_object_::*memfunc)(const QString &), int mode/* = Intact | DataTime*/) :
        ClabsoFTo((virtualo *)(new o_2<_object_, void (_object_::*)(const QString &)>(object, memfunc)), mode)
    {}

    template<typename _object_>
    ClabsoFTo(_object_ *object, _object_ &(_object_::*memfunc)(const QString &), int mode/* = Intact | DataTime*/) :
        ClabsoFTo((virtualo *)(o_2<_object_, _object_ &(_object_::*)(const QString &)>(object, memfunc)), mode)
    {}

    ~ClabsoFTo();

    // when oTag and _func_ are both same , the old registration will be recovered
    template<typename _func_>
    static void registration(const QString &oTag, _func_ func, int mode/* = Intact | DataTime*/)
    {
        virtualo *vo = new o_1<_func_>(func);
        _registration(oTag, vo, mode);
    }

    //when oTag ,_object_ and  _memfunc_ are all the same , the old registration will be recovered
    template<typename _object_>
    static void registration(const QString &oTag, _object_ *object, void (_object_::*memfunc)(const QString &), int mode/* = Intact | DataTime*/)
    {
        virtualo *vo = new o_2<_object_, void (_object_::*)(const QString &)>(object, memfunc);
        _registration(oTag, vo, mode);
    }

    template<typename _object_>
    static void registration(const QString &oTag, _object_ *object, _object_ &(_object_::*memfunc)(const QString &), int mode/* = Intact | DataTime*/)
    {
        virtualo *vo = new o_2<_object_, _object_ &(_object_::*)(const QString &)>(object, memfunc);
        _registration(oTag, vo, mode);
    }

    template<typename _func_>
    static void unregistration(const QString &oTag, _func_ func)
    {
        o_1<_func_> vo(func);
        _unregistration(oTag, &vo);
    }

    template<typename _object_>
    static void unregistration(const QString &oTag, _object_ *object, void (_object_::*memfunc)(const QString &))
    {
        o_2<_object_, void (_object_::*)(const QString &)> vo(object, memfunc);
        _unregistration(oTag, &vo);
    }

    template<typename _object_>
    static void unregistration(const QString &oTag, _object_ *object, _object_ &(_object_::*memfunc)(const QString &))
    {
        o_2<_object_, _object_ &(_object_::*)(const QString &)> vo(object, memfunc);
        _unregistration(oTag, &vo);
    }

    QDebug &getStream();

protected:
    class virtualo
    {
    public:
        virtual ~virtualo()
        {
        }

        virtual virtualo &operator << (const QString &data)
        {
            Q_UNUSED(data)
            return *this;
        }

        virtual bool operator == (const virtualo &other)
        {
            Q_UNUSED(other)
            return true;
        }

        bool operator != (const virtualo &other)
        {
            return !(*this == other);
        }

    protected:
        virtualo() {}
    };

    template<typename _func_>
    class o_1 : public virtualo
    {
        friend class ClabsoFTo;

    public:
        o_1(_func_ func) : func(func)
        {}
        ~o_1()
        {}

        virtualo &operator << (const QString &data)
        {
            (*func)(data);
            return *this;
        }

        bool operator == (const virtualo &other);

    private:
        _func_ func;
    };

    template<typename _object_, typename _memfunc_>
    class o_2 : public virtualo
    {
        friend class ClabsoFTo;

    public:
        o_2(_object_ *object, _memfunc_ memfunc) :
            obj(object), func(memfunc)
        {}
        ~o_2()
        {}

        virtualo &operator << (const QString &data)
        {
            (obj->*func)(data);
            return *this;
        }

        bool operator == (const virtualo &other);

    private:
        _object_ *obj;
        _memfunc_ func;
    };

private:
    ClabsoFTo(virtualo *vo, int mode);

    static void _registration(const QString &oTag, virtualo *vo, int mode);
    static void _unregistration(const QString &oTag, virtualo *vo);

    ClabsoFToPrivate *d_ptr;
    static QMap<QString, QList<QPair<virtualo *, int> > > streams;
};

template<typename _func_>
bool ClabsoFTo::o_1<_func_>::operator ==(const virtualo &other)
{
    const o_1<_func_> *other_ptr = dynamic_cast<const o_1<_func_> *>(&other);
    if (!other_ptr)
        return false;
    return func == other_ptr->func;
}

template<typename _object_, typename _memfunc_>
bool ClabsoFTo::o_2<_object_, _memfunc_>::operator ==(const virtualo &other)
{
    const o_2<_object_, _memfunc_> *other_ptr = dynamic_cast<const o_2<_object_, _memfunc_> *>(&other);
    if (!other_ptr)
        return false;
    return (obj == other_ptr->obj) && (func == other_ptr->func);
}

#endif // CLABSO_FT_IO_H
