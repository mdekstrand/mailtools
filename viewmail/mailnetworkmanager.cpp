#include "mailnetworkmanager.h"
#include "statichttpreply.h"

#include <QtNetwork>
#include <QtDebug>

MailNetworkManager::MailNetworkManager(QNetworkAccessManager* old, QObject *parent) :
    QNetworkAccessManager(parent), f_remoteEnabled(false)
{
    setCache(old->cache());
    setCookieJar(old->cookieJar());
    setProxy(old->proxy());
    setProxyFactory(old->proxyFactory());
}

bool
MailNetworkManager::remoteEnabled() const
{
    return f_remoteEnabled;
}

void
MailNetworkManager::remoteEnabled(bool enabled)
{
    f_remoteEnabled = enabled;
    emit remoteEnabledChanged(enabled);
}

HTMLMailMessage*
MailNetworkManager::activeMessage()
{
    return theMessage;
}

void
MailNetworkManager::activeMessage(HTMLMailMessage* msg)
{
    theMessage = msg;
    emit messageLoaded(msg);
}

QNetworkReply*
MailNetworkManager::createRequest(Operation op, const QNetworkRequest& req, QIODevice* outgoingData)
{
    QUrl url = req.url();
    QString scheme = url.scheme();
    QNetworkReply* reply = NULL;
    if (scheme == "cid") {
        if (op == GetOperation) {
            qDebug() <<"getting component for" <<url;
            vmime::ref<vmime::bodyPart> part;
            if (url.path() == "ROOT") {
                part = theMessage->getBody();
            } else {
                part = theMessage->getRelatedPart(url.path());
            }
            if (part) {
                reply = makeReply(req, part);
            } else {
                qDebug() <<url <<"not found";
                reply = StaticHTTPReply::notFound(req);
            }
        } else {
            qDebug() <<"operation" <<op <<"not supported for CID URLs";
            reply = StaticHTTPReply::denied(op, req);
        }
    } else if (f_remoteEnabled && (url.scheme() == "http" || url.scheme() == "https")) {
        qDebug() <<"delegating request for" <<url;
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    } else {
        qDebug() <<"denying remote request for" <<url;
        reply = StaticHTTPReply::denied(op, req);
        emit blockedRequest(req);
    }
    connect(reply, SIGNAL(finished()), SLOT(replyFinished()));
    return reply;
}

void
MailNetworkManager::replyFinished()
{
    QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
    qDebug() <<"reply" <<reply <<"finished";
    emit finished(reply);
}

QNetworkReply*
MailNetworkManager::makeReply(const QNetworkRequest& req, vmime::ref<vmime::bodyPart> content)
{
    auto ctype = content->getHeader()->ContentType()->getValue()->generate();
    QString mediaType = QString::fromStdString(ctype);

    // FIXME Don't copy the data quite so much
    vmime::string data;
    {
        vmime::utility::outputStreamStringAdapter os(data);
        content->getBody()->getContents()->extract(os);
    }
    qDebug() <<"making reply of length" <<data.size() <<"and type" <<mediaType;
    QByteArray buffer(data.c_str(), data.size());

    return StaticHTTPReply::ok(req, buffer, mediaType);
}
