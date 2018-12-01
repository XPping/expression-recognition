#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBuffer>
#include <QCameraInfo>
#include <QMessageBox>
#include <QThread>
#include <QtAlgorithms>

using namespace std;

Q_DECLARE_METATYPE(QCameraInfo)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    camera(0),
    m_areaChartWidget(0),
    m_detectTimer(0),
    m_tcpSocket(0)
{
    ui->setupUi(this);
    init();

    //Camera devices:
    if(QCameraInfo::availableCameras().size() != 0)
    {
        // Add available cameras into Menu--Devices, and each camera add a action
        QActionGroup *videoDevicesGroup = new QActionGroup(this);
        videoDevicesGroup->setExclusive(true);
        foreach (const QCameraInfo &cameraInfo, QCameraInfo::availableCameras()) {
            QAction *videoDeviceAction = new QAction(cameraInfo.description(), videoDevicesGroup);
            videoDeviceAction->setCheckable(true);
            videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
            if (cameraInfo == QCameraInfo::defaultCamera()) // The default camera as current used
                videoDeviceAction->setChecked(true);

            ui->menuDevices->addAction(videoDeviceAction);
        }
        // When change camera device ;
        connect(videoDevicesGroup, SIGNAL(triggered(QAction*)), SLOT(updateCameraDevice(QAction*)));
        // Open camera, MyCameraWidget() is self define camera class;
        camera = new MyCameraWidget(QCameraInfo::defaultCamera());
        ui->cameraPage->layout()->setMargin(0);
        ui->cameraPage->layout()->addWidget(camera);
        startCamera();  // Open default camera
        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
    } else{
        QMessageBox::information(NULL, "Camera Error", "Can't open any camera. Please add a camera and restart");
    }
    // Set Area Chart widget
    m_areaChartWidget = new MyAreaChartWidget();
    ui->chartWidget->layout()->addWidget(m_areaChartWidget);
    // When triggered StartCamera
    connect(ui->actionStartCamera, SIGNAL(triggered()), this, SLOT(startCamera()));
    // When triggered StopCamera
    connect(ui->actionStopCamera, SIGNAL(triggered()), this, SLOT(stopCamera()));

}

MainWindow::~MainWindow()
{
    delete m_areaChartWidget;
    delete camera;
    delete m_detectTimer;
    delete m_tcpSocket;
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    ui->imageDisplay->resize(ui->imagePage->size());
}

void MainWindow::init()
{
    ui->imageDisplay->setScaledContents(true);
    // Emotion label set font and color
    QFont ft;
    ft.setPointSize(24);
    ui->emotionLabel->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
    ui->emotionLabel->setPalette(pa);
    // Detect Timer
    m_detectTimer = new QTimer();
    m_detectTimer->setInterval(1000);
    // init QTcpSocket
    m_tcpSocket = new QTcpSocket(this);


    // Detect button Setting
    ui->startDetectBtn->setEnabled(true);
    ui->stopDetectBtn->setEnabled(false);
    connect(ui->startDetectBtn, SIGNAL(clicked()), this, SLOT(startDetect()));
    connect(ui->stopDetectBtn, SIGNAL(clicked()), this, SLOT(stopDetect()));
    connect(m_detectTimer, SIGNAL(timeout()), this, SLOT(detect()));
}

void MainWindow::connectToServer(QString host, int port)
{
    m_tcpSocket->abort();
    m_tcpSocket->connectToHost(host, port);
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

void MainWindow::updateCameraDevice(QAction *action)
{
    if(camera != NULL){
        camera->close();
        delete camera;
        camera = new MyCameraWidget(qvariant_cast<QCameraInfo>(action->data()));
        ui->cameraPage->layout()->addWidget(camera);
        camera->show();
        startCamera();
        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
    }
}

void MainWindow::startCamera()
{
    if(camera != NULL){
        camera->startCamera();
        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
    }
}

void MainWindow::stopCamera()
{
    if(camera != NULL){
        camera->stopCamera();
        m_detectTimer->stop();
        ui->actionStartCamera->setEnabled(true);
        ui->actionStopCamera->setEnabled(false);
    }
}

void MainWindow::startDetect()
{
    m_detectTimer->start();
    ui->startDetectBtn->setEnabled(false);
    ui->stopDetectBtn->setEnabled(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::stopDetect()
{
    m_detectTimer->stop();
    ui->startDetectBtn->setEnabled(true);
    ui->stopDetectBtn->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(0);
}

QVector<qreal> randomVector(int numb)
{
    QVector<qreal> vector;
    for(int i(0); i<numb; i++){
        vector.append((qreal)(qrand() % numb) / (qreal)numb);
    }
    return vector;
}

void MainWindow::detect()
{
    if(!camera->getCapturedImage().isNull()){
        // send detectd image to server. if server return detectd result, display the image
        // else wait next detect image
        sendImage(camera->getCapturedImage());
    }
}

void MainWindow::sendImage(QPixmap pix)
{
    connectToServer();

    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    pix.save(&buffer, "jpg");
    quint32 pix_len = (quint32)buffer.data().size();
    QByteArray data_array;
    data_array.append(buffer.data());
    quint32 write_len = m_tcpSocket->write(data_array);
    m_tcpSocket->write("Send End");
    // m_tcpSocket->flush();
    //qDebug()<<pix_len<<write_len;
}

void MainWindow::receiveData()
{
    QString str = m_tcpSocket->readAll();
    if(str != ""){
        // Show result in areaChart
        QVector<qreal> rect_vector;
        QVector<qreal> label_vector;
        QStringList ret_list = str.split(",");
        float sum = 0;
        int i=0;
        for(QString ret : ret_list){
            sum += ret.toFloat();
            if(i<=3) rect_vector.push_back(ret.toFloat());
            else label_vector.push_back(ret.toFloat());
            i++;
        }
        if(sum <=0) return; // no result
        // Display image
        QImage img(camera->getCapturedImage().toImage());
        // label the face rect
        for(int i=rect_vector[0]; i<=rect_vector[2]; i++){
            img.setPixelColor(i, rect_vector[1], QColor(255, 0, 0));
            img.setPixelColor(i, rect_vector[3], QColor(255, 0, 0));
        }
        for(int i=rect_vector[1]; i<=rect_vector[3]; i++){
            img.setPixelColor(rect_vector[0], i, QColor(255, 0, 0));
            img.setPixelColor(rect_vector[2], i, QColor(255, 0, 0));
        }
        // Set sclaed image
        QPixmap scaledImage = QPixmap::fromImage(img).scaled(ui->imagePage->size(),
                                        Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation);
        ui->imageDisplay->setPixmap(scaledImage);
        // Set emotion label
        qreal max_value = 0;
        for(qreal val : label_vector)
            if(max_value < val) max_value = val;
        ui->emotionLabel->setText(EMOTION_LABEL[label_vector.indexOf(max_value)]);
        // if server return result, is to say this image can be detected. So, display it
        ui->stackedWidget->setCurrentIndex(1);
        qDebug()<<label_vector<<endl;
        m_areaChartWidget->addNewPoint(label_vector);
        // Recevier detected result, stop detect timer to display result 1s
        m_detectTimer->stop();
        QTimer::singleShot(1000, this, SLOT(displayDetectedImage()));   // stop 1s
    }
}

void MainWindow::displayDetectedImage()
{
    ui->stackedWidget->setCurrentIndex(0);  // display camera
    if(ui->stopDetectBtn->isEnabled() == true)
        m_detectTimer->start();                 // restart detecte timer
}


