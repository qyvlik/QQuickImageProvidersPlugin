#include "imageprovider.h"

#include <QtDebug>


namespace qyvlik {

ImageProvider::ImageProvider():
    QQuickImageProvider(QQuickImageProvider::Image , ForceAsynchronousImageLoading)
{}

ImageProvider::~ImageProvider()
{
    // qDebug() << Q_FUNC_INFO;
    QWriteLocker l(&mLock);
    qDeleteAll(mImageProcessors.begin(), mImageProcessors.end());
    mImageProcessors.clear();
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
//    qDebug() << Q_FUNC_INFO;
    return requestImage(QUrl(id), size, requestedSize);
}

void ImageProvider::registerImageProcessor(ImageProcessorFactory *imageProcessorFactory)
{
    QWriteLocker l(&mLock);

    auto imageProcessors = imageProcessorFactory->imageProcessors();

    auto iter = imageProcessors.begin();
    auto end  = imageProcessors.end();
    while(iter != end) {

        mImageProcessors.insert(iter.key(), iter.value());

        iter++;
    }
}


/*
    QString id = "image://qyvlik/colors/red?w=1&h=1";
    QUrl url(id);
    QUrlQuery query(url);

    qDebug() << "scheme: " << url.scheme();
    qDebug() << "host: " << url.host();
    qDebug() << "path: " << url.path();

    foreach(auto iter, query.queryItems()) {
        qDebug() << iter;
    }

    //scheme:  "image"
    //host:  "qyvlik"
    //path:  "/colors/red"
    //QPair("w","1")
    //QPair("h","1")
*/
// image://qyvlik/functional/file?arg0=0&arg1=1
QImage ImageProvider::requestImage(const QUrl &url, QSize *size, const QSize &requestedSize)
{
//    qDebug() << Q_FUNC_INFO;

    QImage result;

    QString path = url.path();
    QStringList single_path = path.split("/");

    if(single_path.count() < 2) {
        return result;
    }

    QString function_name = single_path.at(0);
    QString file = single_path.at(1);
    ImageProcessor* processor = nullptr;

    {
        QReadLocker l(&mLock);
        processor = mImageProcessors.value(function_name, nullptr);
    }

    if(processor) {
        result = processor->processing(file, QUrlQuery(url), size, requestedSize);
    }
    return result;
}

//---------------------------------------ImageProcessor--------------------------------------

ImageProcessor::ImageProcessor()
{}

ImageProcessor::~ImageProcessor()
{}

//---------------------------------------ImageProcessorFactory--------------------------------------

ImageProcessorFactory::ImageProcessorFactory()
{}

ImageProcessorFactory::~ImageProcessorFactory()
{}

} // namespace qyvlik

