#pragma once


#include <QtCore>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#ifndef _WIN32
#include <zxing/qrcode/QRCodeReader.h>
#else
#undef _WIN32
#include <zxing/qrcode/QRCodeReader.h>
#define _WIN32
#endif

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
    void StartCam();
    void StopCam();

public slots:
    void work();
    void EndWork();

signals:
    void finished();
    void codeScanned(QString code);
};

void CreateScannerWorkerThread();
