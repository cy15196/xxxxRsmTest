#include "clabso_ft_dpi.h"
#include <QOpenGLWindow>

ClabsoFTDpi::ClabsoFTDpi()
{

}

double ClabsoFTDpi::convertSize(double sizeValue)
{
    return (double(QOpenGLWindow().logicalDpiX()) / 96.0) * sizeValue;
}

QSize ClabsoFTDpi::convertSize(const QSize &sizeValue)
{
    return QSize(convertSize(sizeValue.width()),convertSize(sizeValue.height()));
}
