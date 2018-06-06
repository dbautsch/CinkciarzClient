#ifndef CINKCIARZAPPLICATION_H
#define CINKCIARZAPPLICATION_H

#include <QObject>

#include "currencyinformation.h"

class CinkciarzClient;
class Configuration;
class DataStorage;

class QNetworkAccessManager;
class QTimer;

class CinkciarzApplication : public QObject
{
private:
    Q_OBJECT

    CinkciarzClient * client = nullptr;
    QNetworkAccessManager * networkManager = nullptr;
    QTimer * currencyFetchTimer = nullptr;
    DataStorage * storage = nullptr;

    void StopClient();
    void StartClient();
    void ApplyConfiguration(Configuration * conf);
    bool ClientIsRunning() const;

public:
    explicit CinkciarzApplication(QObject *parent = nullptr);
    ~CinkciarzApplication();

signals:
    void finished();
    void FetchCurrencies();
    void StoreCurrencies50kU(CurrencyInformationList currenciesInformation50k);
    void StoreCurrencies1U(CurrencyInformationList currenciesInformation);
    void StorageConnect();

private slots:
    void OnCurrenciesReady(CurrencyInformationList currenciesInformation50k,
                           CurrencyInformationList currenciesInformation);
    void OnCurrencyFetchTimer();
    void ConfigurationRead();
    void ConfigurationError(QString message);

    void DatabaseError(QString message);
    void DatabaseConnected();

public slots:
    void run();
    void finishApplication();
};

#endif // CINKCIARZAPPLICATION_H
