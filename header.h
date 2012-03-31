#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QTime>
// include header with Thumbnailer class declaration
#include <thumbnailer/Thumbnailer>

class ThumbGetter : public QObject {
Q_OBJECT
private:
    ThumbGetter() : QObject(), qapp(0), uri(QUrl()), mimeType(QString()){}
    const QApplication* qapp;
    const QUrl& uri;
    const QString& mimeType;
    Thumbnails::Thumbnailer* thumbler;
    int retcode;
    QTime timer;
    void printTime() {
        int i = timer.elapsed();
        float f = (float)(timer.elapsed()) / 1000.0f;
        qDebug(" ### %d ms elapsed (%.2f [s])", i, f);
    }
protected:
public:
    ThumbGetter(QApplication* qapp, const QUrl& uri, const QString& mime) :
        QObject (),
        qapp(qapp),
        uri(uri),
        mimeType(mime),
        thumbler(new Thumbnails::Thumbnailer()),
        retcode(0),
        timer(QTime::currentTime())
    {
        // connect all signals from Thumbnailer
        connect(thumbler, SIGNAL(started()), this, SLOT(started()));
        connect(thumbler, SIGNAL(finished(int)), this, SLOT(finished(int)));
        connect(thumbler, SIGNAL(thumbnail(QUrl,QUrl,QPixmap,QString)), this, SLOT(thumbnail(QUrl,QUrl)));
        connect(thumbler, SIGNAL(defaultThumbnail(QUrl,QUrl,QPixmap,QString)), this, SLOT(defaultThumbnail(QUrl,QUrl)));
        connect(thumbler, SIGNAL(error(QString,QUrl)), this, SLOT(error(QString,QUrl)));
        connect(thumbler, SIGNAL(dequeued(QUrl)), this, SLOT(dequeued(QUrl)));
    }

    static QUrl parameterToUri(char* param) {
        QString tmp(param);
        if(tmp.startsWith("file://")) return QUrl(tmp);
        if(tmp.startsWith("/")) return QUrl(QString("file://") + tmp);
        return QUrl(QString("file://") + QDir::currentPath() + QDir::separator() + tmp);
    }
    static QString parameterToMime(char* param) {
        static QHash<QString, QString> mimes;
        if(mimes.count() <= 0) {
            mimes.insert(".jpeg", "image/jpeg");
            mimes.insert(".jpg",  "image/jpeg");
            mimes.insert(".gif",  "image/gif");
            mimes.insert(".png",  "image/png");
            mimes.insert(".bmp",  "image/bmp");
            mimes.insert(".tif",  "image/tiff");
            mimes.insert(".tiff", "image/tiff");

            mimes.insert(".avi",  "video/x-msvideo");
            mimes.insert(".mpg",  "video/mpeg");
            mimes.insert(".mpeg", "video/mpeg");
            mimes.insert(".mpga", "video/mpeg");
            mimes.insert(".mpe",  "video/mpeg");
            mimes.insert(".mpa",  "video/mpeg");
            mimes.insert(".mp2",  "video/mpeg");
            mimes.insert(".mp4",  "video/mp4");
            mimes.insert(".3gp",  "video/3gpp");
        }

        QString tmp(param);
        foreach(QString mime, mimes.keys()) {
            if(tmp.endsWith(mime, Qt::CaseInsensitive)) {
                return mimes[mime];
            }
        }

        return QString();
    }

    int run() {
        if(NULL == thumbler) {
            qDebug() << "Could not create Thumbnailer object, Error!";
            return 3;
        }

        QList<QUrl> uris; uris << uri;
        QStringList mimes; mimes << mimeType;

        timer.restart();
        thumbler->request(uris, mimes, false, QString("video-grid"), QString(""));

        qapp->exec();
        return retcode;
    }

public Q_SLOTS:
        void started() {
            printTime();
            qDebug() << "* Requests started:";
        }
        void finished(int left) {
            printTime();
            int ms = timer.elapsed();
            qDebug() << "* Requests finished. Requests left:" << left;
            qDebug("Whole request took %d milliseconds (%.2f [s])", ms, ((float)ms)/1000.0f);
            // do not exit application, wait for user action (CTRL+c)
            //qapp->exit(retcode);
        }
        void thumbnail(const QUrl& src, const QUrl& thumbnail) {
            printTime();
            qDebug() << "   - got" << __FUNCTION__ << "signal:";
            qDebug() << "         param:src       = " << src.toString();
            qDebug() << "         param:thumbnail = " << thumbnail.toString();

            QString file = thumbnail.toString();
            file.replace(QString("file://"), QString(""));
            qDebug() << "         file:" + file << (QFile::exists(file) ? "exists." : "does NOT exist!");
            retcode = 0;
        }
        void defaultThumbnail(const QUrl& src, const QUrl& thumbnail) {
            printTime();
            qDebug() << "   - got" << __FUNCTION__ << "signal:";
            qDebug() << "         param:src       = " << src.toString();
            qDebug() << "         param:thumbnail = " << thumbnail.toString();
            retcode = 10;
        }
        void error(const QString& message, const QUrl& src) {
            printTime();
            qDebug() << "   - got" << __FUNCTION__ << "signal:";
            qDebug() << "         param:message   = " << message;
            qDebug() << "         param:src       = " << src.toString();
            retcode = 13;
        }
        void dequeued(const QUrl& src) {
            printTime();
            qDebug() << "   - got" << __FUNCTION__ << "signal:";
            qDebug() << "         param:src       = " << src.toString();
            retcode = 14;
        }
};
