#ifndef DIALOGDATADOWNLOAD_H
#define DIALOGDATADOWNLOAD_H

#include <QDialog>
#include <QMap>
#include <QDateTime>
#include "dialogbase.h"

#include "topteststructdata.h"

namespace Ui {
class DialogDataDownload;
}

class QAbstractButton;

class DialogDataDownload : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogDataDownload(QWidget *parent = 0);
    ~DialogDataDownload();

    void resetUnwriteChanges();

    int standerButtons();

    int getPortNum() const;
    void setPortNum(int value);

    void updateStateAndProgress(const QString& name, int progress = -1 );

signals:
    ///emit this signal to tell the main data visual to create new curve file
    void fileRecordDownloadSuccess(const QString& fileName,
                                   const QDateTime& recordTime,
                                   const double & sampleTime,
                                   const int & portNum,
                                   const QByteArray& recordContent);

//    void fileRecordDownloadSuccess(const QString& fileName,
//                                   const QDateTime& recordTime,
//                                   const double & sampleTime,
//                                   const int & portNum,
//                                   const QByteArray& recordContent);

private slots:
    void deleteFirstRecord();
    void refreshDataRecord();

    void downloadRecords();

    void onButtonClicked(int role);

    void clearRecord();

    void onFileDataReady(const QByteArray &fileContent);

    void downloadOneFile();

    void onFileProgressUpdate(const int& addLength);

private:

    QList<int> getChoosePortList();

    QByteArray filterChoosePorts(QByteArray originData,
                                 const int& portNum,
                                 const QList<int> & choosePortList);

    void init();
    void addRecordInfo(const QDateTime& testDateTime,
                       const double & sampleInteralTime,
                       const int & portNum,
                       const int & dataLength);

    void addRecordInfo(const QString& testDateTime,
                       const QString& sampleInteralTime,
                       const int & dataLength);

    Ui::DialogDataDownload *ui;

    QMap<int,TopTest::RecordInfoData> rowRecordDataMap;

    static DialogBase* linkWidgetTemp;

    int portNum;

//    int currentLength = 1;

    enum ReadFileMode{
        None,
        ReadRecord,
        ReadFileContent
    };

    ReadFileMode mode = None;
    QPair<TopTest::RecordInfoData,QList<int>> currentFilePair;
    QByteArray currentFileContent;
    QList<QPair<TopTest::RecordInfoData,QList<int>>> downloadFileQueueList ;
};

#endif // DIALOGDATADOWNLOAD_H
