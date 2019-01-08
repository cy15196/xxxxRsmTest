#include "formlinemanager.h"
#include "ui_formlinemanager.h"

#include "topteststructdata.h"

#include <QtCharts/QLineSeries>

FormLineManager::FormLineManager(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::FormLineManager)
{
    ui->setupUi(this);
    init();
}

FormLineManager::~FormLineManager()
{
    delete ui;
}

int FormLineManager::standerButtons()
{
    return QDialogButtonBox::Ok;
}

void FormLineManager::onButtonClicked(int)
{

}

void FormLineManager::setCurrentLines(const QList<QtCharts::QLineSeries *> &lineList)
{
    currentLineList = lineList;
    updateCurrentTable();
}

void FormLineManager::on_toolButton_add_clicked()
{
    //try to add line via emit signal
    //void needToAddLine(const TopTest::LineInfo& lineInfo);

    TopTest::LineType lineType = (TopTest::LineType)ui->comboBox_lineType->currentData().toInt();
    double value = ui->doubleSpinBox_value->value();
    QPair<QPointF,QPointF> pointPair;
    pointPair.first = QPointF(ui->doubleSpinBox_point1_x->value(),
                              ui->doubleSpinBox_point1_y->value());
    pointPair.second = QPointF(ui->doubleSpinBox_point2_x->value(),
                              ui->doubleSpinBox_point2_y->value());

    emit this->needToAddLine({lineType,value,pointPair});
}

void FormLineManager::init()
{
    foreach (QDoubleSpinBox* spinBox, this->findChildren<QDoubleSpinBox*>()) {
        spinBox->setMaximum(1e5);
    }

    ui->comboBox_lineType->addItem(tr("HLine"),TopTest::HLine);
    ui->comboBox_lineType->addItem(tr("VLine"),TopTest::VLine);
//    ui->comboBox_lineType->addItem(tr("TwoPointLine"),TopTest::TwoPointLine);
    on_comboBox_lineType_currentIndexChanged(0);

    ui->doubleSpinBox_point2_x->setValue(1);
    ui->doubleSpinBox_point2_y->setValue(1);

    ui->label->setVisible(false);
    ui->tableWidget_currentList->setVisible(false);
}

void FormLineManager::updateCurrentTable()
{
    ui->tableWidget_currentList->clear();
    ui->tableWidget_currentList->reset();
    ui->tableWidget_currentList->setColumnCount(2);
    ui->tableWidget_currentList->setColumnWidth(0,50);
    ui->tableWidget_currentList->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_currentList->setHorizontalHeaderLabels({
                                                         tr("Type")
                                                         ,tr("Define")});

    foreach (QtCharts::QLineSeries* line, this->currentLineList) {
        QList<QPointF> pointList = line->points();
        Q_ASSERT(pointList.count() == 2);
        QPointF p1 = pointList.first();
        QPointF p2 = pointList.last();
        QString typeStr;
        QString defineStr;

        if(p1.x() == p2.x())
        {
            typeStr = tr("VLine");
            defineStr = QString::number(p1.x());
        }
        else if(p1.y() == p2.y())
        {
            typeStr = tr("HLine");
            defineStr = QString::number(p1.y());
        }
        else
        {
            typeStr = tr("TwoPointLine");
            defineStr = QString("(%1,%2)->(%3,%4)")
                    .arg(p1.x()).arg(p1.y())
                    .arg(p2.x()).arg(p2.y());
        }

        if(!typeStr.isEmpty() && !defineStr.isEmpty())
        {
            int row = ui->tableWidget_currentList->rowCount();
            ui->tableWidget_currentList->insertRow(row);
            ui->tableWidget_currentList->setItem(
                        row,0,new QTableWidgetItem(typeStr));
            ui->tableWidget_currentList->setItem(
                        row,1,new QTableWidgetItem(defineStr));
        }
    }

//    ui->tableWidget_currentList->insertRow(0);
}

void FormLineManager::on_comboBox_lineType_currentIndexChanged(int index)
{
    bool showTwoPoint = false;
    if(index == TopTest::TwoPointLine)
    {
        showTwoPoint = true;
    }

    ui->label_value->setVisible(!showTwoPoint);
    ui->doubleSpinBox_value->setVisible(!showTwoPoint);

    ui->label_point1->setVisible(showTwoPoint);
    ui->label_point2->setVisible(showTwoPoint);
    ui->doubleSpinBox_point1_x->setVisible(showTwoPoint);
    ui->doubleSpinBox_point1_y->setVisible(showTwoPoint);
    ui->doubleSpinBox_point2_x->setVisible(showTwoPoint);
    ui->doubleSpinBox_point2_y->setVisible(showTwoPoint);
}

void FormLineManager::on_toolButton_clear_clicked()
{
    emit this->needToClearAll();
}
