#include "scanworker.h"
#include <QDebug>
#include <vector>
#include <QMessageBox>
#include <QMediaPlayer>
#include <exception>

#include <zxing/ZXing.h>
#include <zxing/Exception.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/DecodeHints.h>
#include <zxing/LuminanceSource.h>

#define SHOW_CV_WINDOW

using namespace std;
using namespace zxing;
using namespace qrcode;


typedef CvCapture *TDevice;


class OpenCVBitmapSource : public LuminanceSource
{
private:
    IplImage *m_pImage;

public:
    OpenCVBitmapSource(IplImage *image)
        : LuminanceSource(image->width, image->height), m_pImage(image)
    { }

    ~OpenCVBitmapSource()
    { }

    int getWidth() const { return m_pImage->width; }
    int getHeight() const { return m_pImage->height; }

    ArrayRef<char> getRow(int y, ArrayRef<char>) const
    {
        int offset = y * m_pImage->widthStep;
        return ArrayRef<char>(m_pImage->imageData + offset, getWidth());
    }

    ArrayRef<char> getMatrix() const
    {
        char *matrix = new char[getWidth() * getHeight()];
        for (int y = 0; y < getHeight(); y++)
            memcpy(matrix + y * getWidth(), m_pImage->imageData + y * m_pImage->widthStep, getWidth());
        auto retval = ArrayRef<char>(matrix, getWidth() * getHeight());
        delete[] matrix;

        return retval;
    }
};


ScanWorker::ScanWorker(QObject *parent):
    QObject(parent)
{
    StartCam();
}

ScanWorker::~ScanWorker() {
    StopCam();
}

ScanWorker *ScanWorker::Instance()
{
    static auto _instance = new ScanWorker();
    return _instance;
}

void ScanWorker::work() {
    forever {
        UpdateFrame();
        decodeImage();
    }
}

void ScanWorker::EndWork()
{
    StopCam();
}


bool ScanWorker::SetupCam() {
    IplImage* frame;
    int sel = 0;

    device = cvCaptureFromCAM(CV_CAP_DSHOW + sel);

    frame = cvQueryFrame(device);
    if (!frame)
        throw Exception("frame is null...");

    width  = frame->width;
    height = frame->height;
    return true;
}

void ScanWorker::StartCam()
{
    if(!SetupCam())
        throw Exception("Error setup camera");

    gray = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    if (!gray)
        throw Exception("ERROR: allocating cv image");

#ifdef SHOW_CV_WINDOW
    cvNamedWindow("Barcode Reader", CV_WINDOW_AUTOSIZE);
#endif
}


void ScanWorker::StopCam() { cvReleaseCapture(&device); }


IplImage *ScanWorker::CaptureFrame() { return cvQueryFrame(device); }


void ScanWorker::decodeImage() {
    try {
        Ref<OpenCVBitmapSource> source(new OpenCVBitmapSource(gray));
        Ref<Binarizer> binarizer(new GlobalHistogramBinarizer(source));
        Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
        Ref<Result> result(reader.decode(bitmap, DecodeHints(DecodeHints::DEFAULT_HINT)));

        emit codeScanned(QString(result->getText()->getText().c_str()));

        static auto player = new QMediaPlayer();
        if (player->state() == QMediaPlayer::StoppedState) {
            player->setMedia(QUrl::fromLocalFile("./button-3.mp3"));
            player->setVolume(100);
            player->play();
        }
    }
    catch (zxing::Exception& e)
    {
//        qDebug() << "Scanner error: " << e.what();
    }
}

void ScanWorker::UpdateFrame() {
    // Get one frame
    frame = CaptureFrame();
    if (!frame)
        throw Exception("ERROR: frame is null...");

    // CV_RGB2GRAY: convert RGB image to grayscale
    cvCvtColor(frame, gray, CV_RGB2GRAY);

    // Playing with contrast and sharpening, but it doesn't help much,
    // if the barcode is not illuminated brightly or if the webcam has no AUTO-FOCUS.

    cv::Mat image;
    cv::Mat matGray(gray);
    // add contrast
    matGray.convertTo(image, -1, 2.0, 0);	// add contrast, keep brightness

    // sharpen image
    // use a gaussian smoothing filter and subtract the smoothed version from the original image
    cv::GaussianBlur(matGray, image, cv::Size(11, 11), 10, 10);
    double alpha = 5.0;		// alpha is the weight of the first array
    cv::addWeighted(matGray, alpha, image, 1.0 - alpha, 0, image);

    *gray = matGray;

#ifdef SHOW_CV_WINDOW
    cvShowImage("Barcode Reader", gray);
#endif
}


void CreateScannerWorkerThread() {
    static auto _scanThreadInstance = QSharedPointer<QThread>();
    try {
        auto _scanThread = new QThread();
        auto _worker = ScanWorker::Instance();
        _worker->moveToThread(_scanThread);
        QObject::connect(_scanThread, SIGNAL(started()), _worker, SLOT(work()));
        QObject::connect(_worker, SIGNAL(finished()), _scanThread, SLOT(quit()));
        _scanThread->start();

        _scanThreadInstance = QSharedPointer<QThread>(_scanThreadInstance);
    } catch (std::exception &e) {
        QMessageBox msgBox;
        msgBox.setText(QString("Fatal error: cannot start scanner. Message: ") + e.what());
        msgBox.exec();
        exit(1);
    }
}
