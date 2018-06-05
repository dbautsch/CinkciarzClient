#include "cinkciarzapplication.h"

#include "cinkciarzclient.h"
#include "configuration.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>

CinkciarzApplication::CinkciarzApplication(QObject *parent) : QObject(parent)
{
    configuration = new Configuration();

    connect(configuration,
            &Configuration::ConfigurationRead,
            this,
            &CinkciarzApplication::ConfigurationRead);

    connect(configuration,
            &Configuration::ConfigurationError,
            this,
            &CinkciarzApplication::ConfigurationError);

    networkManager = new QNetworkAccessManager();

    currencyFetchTimer = new QTimer();

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
    /* Read configuration from hard drive & wait for ConfigurationRead() slot,
     * then start client service */
    configuration->ReadConfiguration();
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

void CinkciarzApplication::ConfigurationRead()
{
    if (ClientIsRunning())
    {
        StopClient();
    }

    ApplyConfiguration(configuration);
    StartClient();
}

void CinkciarzApplication::StopClient()
{
    // Pause fetching currencies
    currencyFetchTimer->stop();
}

void CinkciarzApplication::StartClient()
{
    // Start fetching currencies
    currencyFetchTimer->start();
}

bool CinkciarzApplication::ClientIsRunning() const
{
    return currencyFetchTimer->isActive();
}

void CinkciarzApplication::ApplyConfiguration(Configuration * conf)
{
    currencyFetchTimer->setInterval(conf->intervalMsec);
}

void CinkciarzApplication::ConfigurationError(QString message)
{

}
