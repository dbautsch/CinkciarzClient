#include "cinkciarzclient.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

static const QString currenciesUrl = "https://cinkciarz.pl/kantor/kursy-walut-cinkciarz-pl";

CinkciarzClient::CinkciarzClient(QNetworkAccessManager *networkManager,
                                  QObject *parent)
    : QObject(parent),
      networkManager(networkManager)
{

}

void CinkciarzClient::FetchCurrencies()
{
    /*
     * (SLOT)
     */

    HttpGet(currenciesUrl);
}

void CinkciarzClient::HttpGet(QString url)
{
    auto * reply = networkManager->get(QNetworkRequest(QUrl(url)));

    connect(reply,
            &QNetworkReply::finished,
            this,
            &CinkciarzClient::HttpGetFinished);
}

void CinkciarzClient::HttpGetFinished()
{
    auto * reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString data = reply->readAll();
    reply->deleteLater();

    QList<CurrencyInformation> currenciesInformation;
    ParseNetworkReply(currenciesInformation, data);

    emit CurrenciesReady(currenciesInformation);
}

void CinkciarzClient::ParseNetworkReply(QList<CurrencyInformation> & currenciesInformation,
                                        QString data)
{

}
