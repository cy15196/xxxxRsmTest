#ifndef FORMGROUPLCD_H
#define FORMGROUPLCD_H

#include <QWidget>
#include <QMap>
class FormNoLCD;

class FormGroupLCD : public QWidget
{
    Q_OBJECT

public:
    explicit FormGroupLCD(QWidget *parent = 0);
    ~FormGroupLCD();

    void showLCDValueList(const QList<double>& portValues);

    int getColumnCount() const;
    void setColumnCount(int value);

    int getPortNum() const;
    void setPortNum(int value);

private:

    int portNum;
    int rowCount;
    int columnCount;

    QMap<int ,FormNoLCD*> noToLCDMap;

};

#endif // FORMGROUPLCD_H
