#include "clabso_ft_dpi.h"
#include <QOpenGLWindow>

ClabsoFTDpi::ClabsoFTDpi()
{

}

double ClabsoFTDpi::convertSize(double sizeValue)
{
    return (double(QOpenGLWindow().logicalDpiX()) / 96.0) * sizeValue;
}
