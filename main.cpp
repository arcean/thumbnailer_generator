#include <QApplication>
#include "header.h"

int
main (int argc, char* argv[]) {
    if(argc < 2) {
        qDebug() << "You have to give at least one uri";
        return 1;
    }

    QUrl uri = ThumbGetter::parameterToUri(argv[1]);
    QString mime = ThumbGetter::parameterToMime(argv[1]);
    if(mime.isEmpty()) {
        qDebug() << "Given URI:" << QString(argv[1]) << "has not supported file extension!";
        return 2;
    }

    qDebug() << "Requesting for uri =" << uri.toString() << "which has MIME Type =" << mime;

    QApplication qapp(argc, argv);
    ThumbGetter getter(&qapp, uri, mime);
    return getter.run();
}
