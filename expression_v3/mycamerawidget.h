#ifndef MYCAMERAWIDGET_H
#define MYCAMERAWIDGET_H

#include <QLabel>
#include <QWidget>

#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraImageCapture>

/*
 * Used QTimer to capture camrea image, and display
 */
class MyCameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyCameraWidget(const QCameraInfo &cameraInfo, QWidget *parent = nullptr);
    ~MyCameraWidget();
    QPixmap getCapturedImage();
signals:

public slots:
    void startCamera(); // Start Camera
    void stopCamera();  // Stop Camera
    void takeCapture();// Capture Image;
    void processCapturedImage(int requestId, const QImage &img);

protected:
    void paintEvent(QPaintEvent *event);
private:
    QCamera *camera;                       // QCamera object
    QCameraImageCapture *imageCapture;     // QCamera image capture object
    QTimer *cameraTimer;                   // QCamera timer
    QLabel *imageShowLabel;                  // Camera infomation;
    QImage m_previewImage;               // Captured preview Image;
};

#endif // MYCAMERAWIDGET_H
