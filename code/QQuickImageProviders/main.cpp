#include <QApplication>
#include <QQmlApplicationEngine>

#include <ImageProvider>

#include <QColor>
#include <QtDebug>

class ColorImageProcessor : public qyvlik::ImageProcessor
{
public:
    QString processName() const override;
    QImage processing(const QString &fileName, const QUrlQuery &query, QSize *size, const QSize &requestedSize) override;
};

class ColorImageProcessorFactory : public qyvlik::ImageProcessorFactory
{
public:
    QMap<QString, qyvlik::ImageProcessor *> imageProcessors() const override;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ColorImageProcessorFactory colorImageProcessorFactory;
    qyvlik::ImageProvider* imageProvider = new qyvlik::ImageProvider();
    imageProvider->registerImageProcessor(&colorImageProcessorFactory);

    QQmlApplicationEngine engine;

    // image://qyvlik/functional/file?arg0=0&arg1=1
    engine.addImageProvider("qyvlik", imageProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    int ret =  app.exec();

    return ret;
}


QString ColorImageProcessor::processName() const
{
    return "color";
}

QImage ColorImageProcessor::processing(const QString &fileName, const QUrlQuery &query, QSize *size, const QSize &requestedSize)
{
    (void)query;
    int width = 100;
    int height = 50;

    if (size)
        *size = QSize(width, height);
    QImage image(requestedSize.width() > 0 ? requestedSize.width() : width,
                 requestedSize.height() > 0 ? requestedSize.height() : height,
                 QImage::Format_ARGB32);
    image.fill(QColor(fileName).rgba());

    return image;
}

QMap<QString, qyvlik::ImageProcessor *> ColorImageProcessorFactory::imageProcessors() const
{
    QMap<QString, qyvlik::ImageProcessor *> image_processors;
    image_processors.insert("color", new ColorImageProcessor);
    return image_processors;
}
