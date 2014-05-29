#pragma once

#include <QtCore>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <zxing/qrcode/QRCodeReader.h>

typedef CvCapture *TDevice;

class ScanWorker: public QObject
{
    Q_OBJECT

    TDevice device;
    IplImage *frame, *gray;
    int width, height;

    IplImage *CaptureFrame();

    zxing::qrcode::QRCodeReader reader;
    void decodeImage();
    void UpdateFrame();

    explicit ScanWorker(QObject *parent = nullptr);
public:
    ~ScanWorker();

    static ScanWorker *Instance();

    bool SetupCam();
    void StopCam();

public slots:
    void work();
    void EndWork();

signals:
    void finished();
    void codeScanned(QString code);
};

void CreateScannerWorkerThread();
