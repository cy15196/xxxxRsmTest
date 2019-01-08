#ifndef CHECKTOOLBUTTON_ITEM_DELEGATE_H
#define CHECKTOOLBUTTON_ITEM_DELEGATE_H


#include "fipsitemdelegate_global.h"

#include <QStyledItemDelegate>

class QTableWidget;

class FIPSITEMDELEGATESHARED_EXPORT ToolButtonItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ToolButtonItemDelegate(QTableWidget* table);
    ~ToolButtonItemDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
//    QWidget *createEditor(QWidget *parent,
//                          const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;
//    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    void setModelData(QWidget *editor,
//                      QAbstractItemModel *model,
//                      const QModelIndex &index) const;

    QTableWidget *getTable() const;
    void setTable(QTableWidget *value);
    QString getIconPathUnchecked() const;
    void setIconPathUnchecked(const QString &value);

    QString getIconPathChecked() const;
    void setIconPathChecked(const QString &value);

    bool getCheckable() const;
    void setCheckable(bool value);

private slots:
    void onToolButtonClicked(bool checked);

private:

    bool checkable;

    QTableWidget* table;
    QString iconPathUnchecked;
    QString iconPathChecked;

};

#endif // CHECKTOOLBUTTON_ITEM_DELEGATE_H
