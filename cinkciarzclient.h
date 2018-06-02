#ifndef CINKCIARZCLIENT_H
#define CINKCIARZCLIENT_H

#include <QObject>

#include "currencyinformation.h"

class QNetworkAccessManager;

using CurrencyInformationList = QList<CurrencyInformation>;

class CinkciarzClient : public QObject
{
private:
    Q_OBJECT

    QNetworkAccessManager * networkManager = nullptr;

    void HttpGet(QString url);
    void ParseNetworkReply(QList<CurrencyInformation> & currenciesInformation,
                           QString data);

public:
    explicit CinkciarzClient(QNetworkAccessManager * networkManager,
                             QObject *parent = nullptr);

signals:
    void CurrenciesReady(QList<CurrencyInformation> currenciesInformation);

private slots:
    void HttpGetFinished();

public slots:
    void FetchCurrencies();
};

#endif // CINKCIARZCLIENT_H
