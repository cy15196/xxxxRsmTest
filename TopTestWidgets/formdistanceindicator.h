#ifndef FORMDISTANCEINDICATOR_H
#define FORMDISTANCEINDICATOR_H

#include <QWidget>

#include "toptestwidgets_global.h"


class FormDistanceIndicatorPrivate;

class TOPTESTWIDGETSSHARED_EXPORT FormDistanceIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit FormDistanceIndicator(QWidget *parent = 0);
    ~FormDistanceIndicator();

    void setWholeDiatance(const double & value);

private:
    FormDistanceIndicatorPrivate* d_ptr;

};

#endif // FORMDISTANCEINDICATOR_H
