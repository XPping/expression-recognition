#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include "mycamerawidget.h"
#include "myareachartwidget.h"
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();

    void connectToServer(QString host="127.0.0.1", int port=6666);

private slots:
    // Camera
    void updateCameraDevice(QAction *action);       // When changing camera device
    void startCamera();                             // Start camera
    void stopCamera();                              // Stop camera;

    // Detect
    void startDetect();                             // Start detect, that is start m_detectTimer
    void stopDetect();
    void detect();

    // Send image to server
    void sendImage(QPixmap pix);
    // Receive data from server
    void receiveData();
    // Display detectd image
    void displayDetectedImage();
protected:
    void paintEvent(QPaintEvent *event);
private:
    Ui::MainWindow *ui;
    MyCameraWidget *camera;
    MyAreaChartWidget *m_areaChartWidget;
    QTimer *m_detectTimer;

    QTcpSocket *m_tcpSocket;

    QString EMOTION_LABEL[7] = {"Anger", "Disgust", "Fear", "Happy", "Neutral", "Sad", "Surprise"};
};

#endif // MAINWINDOW_H
