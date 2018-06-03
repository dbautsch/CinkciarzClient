#include "cinkciarzapplication.h"

#include "cinkciarzclient.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>

CinkciarzApplication::CinkciarzApplication(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager();

    currencyFetchTimer = new QTimer();
    currencyFetchTimer->setSingleShot(true);
    currencyFetchTimer->setInterval(1); // just for now - to be read from configuration

    connect(currencyFetchTimer,
            &QTimer::timeout,
            this,
            &CinkciarzApplication::OnCurrencyFetchTimer);

    client = new CinkciarzClient(networkManager, this);

    connect(client,
            &CinkciarzClient::CurrenciesReady,
            this,
            &CinkciarzApplication::OnCurrenciesReady);

    connect(this,
            &CinkciarzApplication::FetchCurrencies,
            client,
            &CinkciarzClient::FetchCurrencies);
}

CinkciarzApplication::~CinkciarzApplication()
{
    client->deleteLater();
    networkManager->deleteLater();
}

void CinkciarzApplication::run()
{
    // Start fetching currencies
    currencyFetchTimer->start();
}

void CinkciarzApplication::finishApplication()
{
    emit finished();
}

void CinkciarzApplication::OnCurrenciesReady(CurrencyInformationList currenciesInformation50k,
                                             CurrencyInformationList currenciesInformation)
{

}

void CinkciarzApplication::OnCurrencyFetchTimer()
{
    if (client != nullptr)
    {
        emit FetchCurrencies();
    }
}
