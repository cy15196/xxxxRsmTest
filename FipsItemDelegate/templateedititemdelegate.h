#ifndef TEMPLATEEDITITEMDELEGATE_H
#define TEMPLATEEDITITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QString>
#include "fipsitemdelegate_global.h"
#include <QApplication>
#include <QAbstractItemView>

template <class T>
class  TemplateEditItemDelegate: public QStyledItemDelegate
{
public:
    explicit TemplateEditItemDelegate(QAbstractItemView *parent)
        :QStyledItemDelegate(parent),parentView(parent)
    {

    }
    ~TemplateEditItemDelegate()
    {

    }

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const
    {
        QStyledItemDelegate::paint(painter,option,index);
        T * edit = new T;
        parentView->setIndexWidget(index,edit);
        //    edit->initWithValue(initValue);
        //    ;

        //    QString str = index.data().toString();
        //    QApplication::style()->drawItemText(painter,
        //                                        QRect(option.rect.x() + 4,
        //                                              option.rect.y(),
        //                                              option.rect.width(),
        //                                              option.rect.height()),
        //                                        Qt::AlignLeft | Qt::AlignVCenter,
        //                                        QPalette(QColor(255, 255, 255, 0)),
        //                                        true,
        //                                        str);
    }
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        return parentView->indexWidget(index);
    }
//    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    void setModelData(QWidget *editor,
//                      QAbstractItemModel *model,
//                      const QModelIndex &index) const;

//    QVariant getInitValue() const;
//    void setInitValue(const QVariant &value);

private:
//    QVariant initValue;
    QAbstractItemView* parentView;
};







#endif // TEMPLATEEDITITEMDELEGATE_H
