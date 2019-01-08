#ifndef FORMPAGEDATAVISUAL_H
#define FORMPAGEDATAVISUAL_H

#include <QWidget>
#include <dialogdatadownload.h>

namespace Ui {
class FormPageDataVisual;
}

class QMdiSubWindow;
class FormDataPage;

class FormPageDataVisual : public QWidget
{
    Q_OBJECT

public:
    explicit FormPageDataVisual(QWidget *parent = 0);
    ~FormPageDataVisual();

    static bool saveDataFile(const QString& path,
                             const TopTest::FileData &fileData,
                             const QList<QMap<QString, QVariant> > & settingFileMapList,
                             const TopTest::WaveInfo &waveInfo);


private slots:
    void onShowDialog(QWidget *w, int signal);

    void createFileSubWindow(const QString& fileName,
                             const QDateTime& recordTime,
                             const double & sampleTime,
                             const int & portNum,
                             const QByteArray& recordContent);

    void createFileSubWindow(const QString &filePath, const TopTest::RecordInfoData& infoData,
                             const QByteArray& recordContent, const QList<QMap<QString, QVariant> > &settingFileMapList, const TopTest::WaveInfo &waveInfo);

    void onSaveFileToPath(const QString& path);


    void onOpenDataFile(const QString& filePath);

    void deleteCurrentFile();

//    void applyProcess(const QString& processName);

    void onSubWindowActivated(QMdiSubWindow *window);

    inline FormDataPage* getCurrentDataPage();

    void onCurrentFileInfoChanged(const QVariantMap& currentMap);

    void onFullScreen();

    void onMonitorSwitchChanged(bool on);



private:

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

    void initToolButtonGroup();

    ///刷新按钮的可用性
    void refreshToolButtonState();

    QString formOutputString(FormDataPage *currentPage);


//    QMap<QMdiSubWindow*, TopTest::RecordInfoData> windowFileMap;

//    TopTest::RecordInfoData currentFileData;

    Ui::FormPageDataVisual *ui;

    QWidget* FullScreenWindow = 0;

    bool realMonitorOn = false;
};

#endif // FORMPAGEDATAVISUAL_H
