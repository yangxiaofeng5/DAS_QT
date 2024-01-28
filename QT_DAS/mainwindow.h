#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot/qcustomplot.h>

#include <QTimer>
#include <dynamicdraw.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void RealtimeDataShow(QCustomPlot *customPlot);
    DynamicDraw* CustomDraw_ch1;
    DynamicDraw* CustomDraw_ch2;

private slots:
    void realtimeDataSlot();

    void on_open_device_Button_toggled(bool checked);

    void on_start_collect_Button_toggled(bool checked);

    void on_read_all_commend_Button_clicked();

    void on_write_all_commend_Button_clicked();

    void on_read_rate_clock_Button_clicked();

    void on_write_rate_clock_Button_clicked();

    void on_read_triger_length_Button_clicked();

    void on_write_triger_length_Button_clicked();

    void on_soft_triger_Button_clicked(bool checked);

    void on_graphics_clean_Button_clicked();

    void on_ch1_checkBox_toggled(bool checked);

    void on_ch2_checkBox_toggled(bool checked);

    void on_message_clean_Button_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *dataTimer;
signals:
    void starting(int thread_flag);
    //QVector mData;
};
#endif // MAINWINDOW_H
