#ifndef HISTORYLABEL_H
#define HISTORYLABEL_H

#include <QWidget>

namespace Ui {
class HistoryLabel;
}

struct HistoryLabelPrivate;

class HistoryLabel : public QWidget
{
    Q_OBJECT
    friend struct HistoryLabelPrivate;

public:
    explicit HistoryLabel(QWidget *parent = 0);
    HistoryLabel(const QString &filePath, QWidget *parent = 0);
    ~HistoryLabel();

    void setFile(const QString &filePath);
    QString getFile() const;

    QSize sizeHint() const;
    void paintEvent(QPaintEvent *event);

signals:
    void fileLose(const QString &filePath);

private:
    Ui::HistoryLabel *ui;
    HistoryLabelPrivate *d_ptr_c;

    HistoryLabelPrivate *d_func_c() { return d_ptr_c; }
    Q_DISABLE_COPY(HistoryLabel)
};

#endif // HISTORYLABEL_H
