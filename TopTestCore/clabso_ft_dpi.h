#ifndef CLABSOFTDPI_H
#define CLABSOFTDPI_H

#include "toptestcore_global.h"
#include <QSize>

class TOPTESTCORESHARED_EXPORT ClabsoFTDpi
{
public:
    ClabsoFTDpi();

    static inline double convertSize(double sizeValue);
    static inline QSize convertSize(const QSize& sizeValue);
};

#endif // CLABSOFTDPI_H
