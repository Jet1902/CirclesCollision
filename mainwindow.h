#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onWorldUpdated(int msecTaken);
    void on_pushAdd10_clicked();
    void on_pushAdd100_clicked();
    void on_pushClear_clicked();

    void on_pushUseQuadTree_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    QLabel *m_dtLabel = nullptr;
    QLabel *m_numBallsLabel = nullptr;
};

#endif // MAINWINDOW_H
