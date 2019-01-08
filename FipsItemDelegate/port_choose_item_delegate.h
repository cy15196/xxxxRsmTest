#ifndef PORT_CHOOSE_ITEM_DELEGATE_H
#define PORT_CHOOSE_ITEM_DELEGATE_H


#include <QStyledItemDelegate>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QObject>
#include "fipsitemdelegate_global.h"
class QTableWidget;
class QListWidget;
class QLineEdit;
class QCheckBox;

class FIPSITEMDELEGATESHARED_EXPORT BoolComboBox:public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(bool boolValue READ boolValue WRITE setBoolValue USER true)
public :
    BoolComboBox(QWidget *parent = Q_NULLPTR);
    ~BoolComboBox();

    bool boolValue() const;
    void setBoolValue(bool value);
};

class FIPSITEMDELEGATESHARED_EXPORT DoubleSpin:public QDoubleSpinBox
{
    Q_OBJECT
    Q_PROPERTY(double doubleValue READ doubleValue WRITE setDoubleValue USER true)
public :
    DoubleSpin(QWidget *parent = Q_NULLPTR);
    ~DoubleSpin();

    double doubleValue() const;
    void setDoubleValue(double value);
};

class FIPSITEMDELEGATESHARED_EXPORT MultiComboBox:public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QString valueStr READ valueStr WRITE setValueStr USER true)
public :
    MultiComboBox(QWidget *parent = Q_NULLPTR);
    ~MultiComboBox();

    QString valueStr() const;
    void setValueStr(QString list);

    void setEditorData(const QStringList &portList);
    QList<int> getPortIntList();

    void setPortStringMap(const QMap<int,QString> & portMap);

private slots:
    void onStateChanged(int state);
    void onTextChanged(const QString &text);

private:
    QListWidget* pListWidget;
    QLineEdit* pLineEdit;
    QString strSelectedText;

    bool bSelected;

    void populateList();

};

class FIPSITEMDELEGATESHARED_EXPORT PortChooseItemDelegate
        :public QStyledItemDelegate
{
    Q_OBJECT
public:
    PortChooseItemDelegate(QObject *parent =0 );
    ~PortChooseItemDelegate();

    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;


    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;

private:
    //    MultiComboBox * boxEditor;

};

#endif // PORT_CHOOSE_ITEM_DELEGATE_H
