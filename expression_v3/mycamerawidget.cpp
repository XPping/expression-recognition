#include "mycamerawidget.h"

#include <QGridLayout>
#include <QTimer>

MyCameraWidget::MyCameraWidget(const QCameraInfo &cameraInfo, QWidget *parent) : QWidget(parent),
    camera(0),
    imageCapture(0),
    cameraTimer(0),
    imageShowLabel(0)
{
    m_previewImage = QImage();
    // New camera capture image label
    imageShowLabel = new QLabel(this);
    imageShowLabel->setScaledContents(true);
    // Label must in top-left;
    imageShowLabel->setGeometry(QRect(0, 0, 0, 0));
    imageShowLabel->show();
    // New camera object
    camera = new QCamera(cameraInfo);
    // New camera caputre object
    imageCapture = new QCameraImageCapture(camera);
    // Connect imageCapture's imageCaptured(int, QImage) signal to slot;
    connect(imageCapture, SIGNAL(imageCaptured(int, QImage)), this, SLOT(processCapturedImage(int, QImage)));
    // Camera timer
    cameraTimer = new QTimer();
    cameraTimer->setInterval(33);
    // Camera timer timeout signal to takeCapture() slot;
    connect(cameraTimer, SIGNAL(timeout()), this, SLOT(takeCapture()));
}

MyCameraWidget::~MyCameraWidget()
{
    delete camera;
    delete imageCapture;
    delete cameraTimer;
    delete imageShowLabel;
}


QPixmap MyCameraWidget::getCapturedImage()
{
    return QPixmap::fromImage(m_previewImage);
}

void MyCameraWidget::startCamera()
{
    camera->start();
    cameraTimer->start();
}

void MyCameraWidget::stopCamera()
{
    // Close camera
    camera->stop();
    cameraTimer->stop();
}

void MyCameraWidget::takeCapture()
{
    // Send imageCapure's imageCaptured(int, QImage) signal;
    imageCapture->capture();
}

void MyCameraWidget::processCapturedImage(int requestId, const QImage &img)
{
    // Show caputred image
    Q_UNUSED(requestId);
    QImage scaledImage = img.scaled(this->size(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);
    m_previewImage = img;
    imageShowLabel->setPixmap(QPixmap::fromImage(scaledImage));

}

void MyCameraWidget::paintEvent(QPaintEvent *event)
{
    // Let image show label can zoom in/zoom out as father widget
    if(imageShowLabel != NULL){
        imageShowLabel->resize(this->size());
    }
}
