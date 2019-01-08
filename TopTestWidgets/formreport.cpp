#include "formreport.h"
#include "ui_formreport.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QPrintEngine>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QPageSetupDialog>

#include "clabso_ft_dpi.h"

FormReport::FormReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReport)
{
    ui->setupUi(this);

    QSize icoSize(ClabsoFTDpi::convertSize(20),ClabsoFTDpi::convertSize(20));
    ui->toolButton_edit->setIconSize(icoSize);
    ui->toolButton_print->setIconSize(icoSize);
}

FormReport::~FormReport()
{
    delete ui;
}

void FormReport::on_toolButton_print_clicked()
{
    doPrintPreview();
//    doPrint();
}

void FormReport::doPrint()
{
    // 创建打印机对象
     QPrinter printer;
     // 创建打印对话框
     QString printerName = printer.printerName();
     if( printerName.size() == 0)
         return;
//     printer.setOutputFormat(QPrinter::PdfFormat);
     QPrintDialog dlg(&printer, this);
     dlg.addEnabledOption(QAbstractPrintDialog::PrintPageRange);

     //如果编辑器中有选中区域，则打印选中区域
//     if (ui->textEdit->textCursor().hasSelection())
//         dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);
     // 如果在对话框中按下了打印按钮，则执行打印操作
     if (dlg.exec() == QDialog::Accepted)
     {
        ui->textEdit->print(&printer);
        // print the existing document by absoult path
//        printFile("D:/myRSM.doc");
     }
}

void FormReport::doPrintPreview()
{
    QPrinter printer;
    // 创建打印预览对话框
    QPrintPreviewDialog preview(&printer, this);
    // 当要生成预览页面时，发射paintRequested()信号
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
                  this,SLOT(printPreview(QPrinter*)));
    preview.exec();
}

void FormReport::printPreview(QPrinter *printer)
{
    ui->textEdit->print(printer);
}

void FormReport::createPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出PDF文件"), QString(), "*.pdf");
      if (!fileName.isEmpty()) {
          // 如果文件后缀为空，则默认使用.pdf
          if (QFileInfo(fileName).suffix().isEmpty())
              fileName.append(".pdf");
          QPrinter printer;
          // 指定输出格式为pdf
          printer.setOutputFormat(QPrinter::PdfFormat);
          printer.setOutputFileName(fileName);
          ui->textEdit->print(&printer);
      }
}

void FormReport::setUpPage()
{
    QPrinter printer;
    QPageSetupDialog pageSetUpdlg(&printer, this);
    if (pageSetUpdlg.exec() == QDialog::Accepted)
    {
        printer.setOrientation(QPrinter::Landscape);
    }
    else
    {
        printer.setOrientation(QPrinter::Portrait);
    }
}
