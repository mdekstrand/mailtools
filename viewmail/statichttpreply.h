#ifndef STATICHTTPREPLY_H
#define STATICHTTPREPLY_H

#include <QtNetwork>

#include <memory>

class StaticHTTPReply : public QNetworkReply
{
    Q_OBJECT
public:
    explicit StaticHTTPReply(QObject *parent = 0);

    void abort();
    bool isSequential() const;
    qint64 bytesAvailable() const;

    static StaticHTTPReply* ok(const QNetworkRequest& req,
                               const QByteArray& data, QString contentType);
    static StaticHTTPReply* notFound(const QNetworkRequest& req);
    static StaticHTTPReply* denied(QNetworkAccessManager::Operation op, const QNetworkRequest& req);

signals:

public slots:

protected slots:
    void indicateReady();

protected:
    qint64 readData(char* data, qint64 maxSize);

private:
    void setBuffer(const QByteArray& buf);
    void setBuffer(const std::string& buf);

    QByteArray bytes;
    qint64 offset;
};

#endif // STATICHTTPREPLY_H
