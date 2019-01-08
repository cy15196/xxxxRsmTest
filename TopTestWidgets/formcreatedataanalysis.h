#ifndef FORMCREATEDATAANALYSIS_H
#define FORMCREATEDATAANALYSIS_H

#include <QWidget>
#include <QMap>
#include <QVariant>

namespace Ui {
class FormCreateDataAnalysis;
}

class ProjectSetting: public QWidget
{
   Q_OBJECT
public :
    explicit ProjectSetting(const QMap<QString,QVariant>& dataMap,
                            QWidget *parent = 0)
        :QWidget(parent)
    {
        init(dataMap);
    }

    virtual void init(const QMap<QString,QVariant>& dataMap){}
    virtual QMap<QString,QVariant> getDataMap()=0;

protected:

};

class FormCreateDataAnalysis : public ProjectSetting
{
    Q_OBJECT

public:
    explicit FormCreateDataAnalysis(const QMap<QString,QVariant>& dataMap,
                                    QWidget *parent = 0);

    void init(const QMap<QString,QVariant>& dataMap);
    QMap<QString,QVariant> getDataMap();

    ~FormCreateDataAnalysis();

private:
    Ui::FormCreateDataAnalysis *ui;
};

#endif // FORMCREATEDATAANALYSIS_H
