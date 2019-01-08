#include "fipsitemdelegate.h"


#include "list_item_delegate.h"
#include "spin_item_delegate.h"
#include "color_item_delegate.h"
#include "vector_item_delegate.h"
#include "dimension_item_delegate.h"
#include "color_list_item_delegate.h"
#include "map_item_delegate.h"
#include "validator_item_delegate.h"
#include "presort_item_delegate.h"


namespace Fips {



FipsItemDelegate::FipsItemDelegate()
{

}

QStyledItemDelegate *FipsItemDelegate::getDelegateForItem(ItemType itemType, const QVariant &itemValue)
{
    switch(itemType)
    {
    case NoType:
        break;
    case BoolItem:
        break;
    case StringItem:
        break;

    case IntItem:
    {
        ValidatorItemDelegate*delegate=new ValidatorItemDelegate;
        delegate->applyIntValidtor();
        return delegate;
    }
        break;
    case DoubleItem:
    {
        ValidatorItemDelegate*delegate=new ValidatorItemDelegate;
        delegate->applyDoubleValidtor();
        return delegate;
    }
        break;


    case StringEnumItem:
    {
        ListItemDelegate*delegate=new ListItemDelegate;
        delegate->setOptions(itemValue.toStringList());
        return delegate;
    }
        break;

    case IntEnumItem:
    {
        PresortItemDelegate<int>*delegate=new PresortItemDelegate<int>;
        QList<QPair<QString,int> > pairList;
        foreach (QVariant valueItem, itemValue.toList()) {
            QPair <QString,int> itemPair;
            itemPair.first=QString::number(valueItem.toInt());
            itemPair.second=valueItem.toInt();
            pairList.append(itemPair);
        }
        delegate->setPairList(pairList);
        return delegate;
    }
        break;
    case DoubleEnumItem:
    {
        PresortItemDelegate<double>*delegate=new PresortItemDelegate<double>;
        QList<QPair<QString,double> > pairList;
        foreach (QVariant valueItem, itemValue.toList()) {
            QPair <QString,double> itemPair;
            itemPair.first=QString::number(valueItem.toDouble());
            itemPair.second=valueItem.toDouble();
            pairList.append(itemPair);
        }
        delegate->setPairList(pairList);
        return delegate;
    }
        break;

    case SpinItem:
    {
        QVariantList valueList=itemValue.toList();
        if(valueList.count()<4)
            return 0;
        ///             double  Minimum
        ///             double  Maximum
        ///             double  SingalStep
        ///             int     Decimals
        ///             string  Prefix
        ///             string  Suffix
        ///             string  SpecialValueText
        SpinItemDelegate*delegate=new SpinItemDelegate;
        QString prefix,suffix,specialValueText;
        switch(valueList.count())
        {
        case 5:
            prefix=valueList[4].toString();
        case 6:
            suffix=valueList[5].toString();
        case 7:
            specialValueText=valueList[6].toString();
            break;
        default:
            break;
        }
        delegate->setSpinBox(
                    valueList[0].toDouble(),
                    valueList[1].toDouble(),
                    valueList[2].toDouble(),
                    valueList[3].toInt(),
                    prefix,
                    suffix,
                    specialValueText);
        return delegate;
    }
        break;

    case DimensionItem:
    {
        return new DimensionItemDelegate;
    }
        break;

    case ColorItem:
        return new ColorItemDelegate;
        break;
    case ColorEnumItem:
    {
        ColorListItemDelegate*delegate=new ColorListItemDelegate;
        delegate->setColorList(itemValue.toStringList());
        return delegate;
    }
        break;
    default:
        break;
    }

    return 0;

}


}
