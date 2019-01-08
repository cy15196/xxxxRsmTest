#ifndef FIPSITEMDELEGATE_H
#define FIPSITEMDELEGATE_H

#include "fipsitemdelegate_global.h"
#include "fipsglobalenum.h"

#include <QVariant>

class QStyledItemDelegate;

namespace Fips {



class FIPSITEMDELEGATESHARED_EXPORT FipsItemDelegate
{
public:
    FipsItemDelegate();

    ///
    /// \brief get delegate for item with given type and value
    /// \param type
    /// \param value
    /// \return pointer of delegate for item
    ///
    static QStyledItemDelegate*getDelegateForItem(ItemType itemType, const QVariant&itemValue);

};

}

#endif // FIPSITEMDELEGATE_H
