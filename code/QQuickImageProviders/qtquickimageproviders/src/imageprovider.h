#ifndef QYVLIK_IMAGEPROVIDER_H
#define QYVLIK_IMAGEPROVIDER_H

#include <QtQuick/QQuickImageProvider>
#include <QtCore/QReadWriteLock>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QMap>

namespace qyvlik {

class ImageProvider;
class ImageProcessor;
class ImageProcessorFactory;
// class ImageProcessorFactoryBuilder;

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();

    ~ImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    void registerImageProcessor(ImageProcessorFactory* imageProcessorFactory);

protected:
    // image://qyvlik/functional/file?arg0=0&arg1=1
    // thread-safe
    virtual QImage requestImage(const QUrl& url, QSize* size, const QSize& requestedSize);

private:
    QReadWriteLock mLock;
    QMap<QString, ImageProcessor*> mImageProcessors;
};

class ImageProcessor
{
public:
    explicit ImageProcessor();

    virtual ~ImageProcessor();

    virtual QString processName() const = 0;

    // thread-safe
    virtual QImage processing(const QString& fileName, const QUrlQuery& query, QSize* size, const QSize& requestedSize) = 0;
};

class ImageProcessorFactory
{
public:
    explicit ImageProcessorFactory();
    virtual ~ImageProcessorFactory();
    virtual QMap<QString, ImageProcessor*> imageProcessors() const = 0;
};


} // namespace qyvlik

#endif // QYVLIK_IMAGEPROVIDER_H
