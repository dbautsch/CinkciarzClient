#include "cinkciarzapplication.h"

#include "cinkciarzclient.h"
#include "configuration.h"
#include "datastorage.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>

CinkciarzApplication::CinkciarzApplication(QObject *parent) : QObject(parent)
{
    ////
    configuration = new Configuration();

    connect(configuration,
            &Configuration::ConfigurationRead,
            this,
            &CinkciarzApplication::ConfigurationRead);

    connect(configuration,
            &Configuration::ConfigurationError,
            this,
            &CinkciarzApplication::ConfigurationError);

    ///
    storage = new DataStorage();

    connect(this,
            &CinkciarzApplication::StoreCurrencies50kU,
            storage,
            &DataStorage::StoreCurrencies50kU);

    connect(this,
            &CinkciarzApplication::StoreCurrencies1U,
            storage,
            &DataStorage::StoreCurrencies1U);

    connect(storage,
            &DataStorage::DatabaseConnected,
            this,
            &CinkciarzApplication::DatabaseConnected);

    connect(storage,
            &DataStorage::DatabaseError,
            this,
            &CinkciarzApplication::DatabaseError);

    connect(this,
            &CinkciarzApplication::StorageConnect,
            storage,
            &DataStorage::Connect);

    ////
    networkManager = new QNetworkAccessManager();

    currencyFetchTimer = new QTimer();

    connect(currencyFetchTimer,
            &QTimer::timeout,
            this,
            &CinkciarzApplication::OnCurrencyFetchTimer);

    ////
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
    emit StoreCurrencies1U(currenciesInformation);
    emit StoreCurrencies50kU(currenciesInformation50k);
}

void CinkciarzApplication::OnCurrencyFetchTimer()
{
    QDateTime now = QDateTime::currentDateTime();
    auto dayOfWeek = now.date().dayOfWeek();

    if (configuration->excludedDays.contains(dayOfWeek))
    {
        /* Ignore event, current day is excluded from fetching currencies */
        return;
    }

    if (client != nullptr)
    {
        emit FetchCurrencies();
    }
}

void CinkciarzApplication::ConfigurationRead()
{
    emit StorageConnect();
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
    qDebug() << "Configuration error: " << message;

    finishApplication();
}

void CinkciarzApplication::DatabaseError(QString message)
{
    qDebug() << "Database error: " << message;

    finishApplication();
}

void CinkciarzApplication::DatabaseConnected()
{
    if (ClientIsRunning())
    {
        StopClient();
    }

    ApplyConfiguration(configuration);
    StartClient();
}
