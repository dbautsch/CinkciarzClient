#ifndef CINKCIARZAPPLICATION_H
#define CINKCIARZAPPLICATION_H

#include <QObject>

#include "currencyinformation.h"

class CinkciarzClient;
class Configuration;

class QNetworkAccessManager;
class QTimer;

class CinkciarzApplication : public QObject
{
private:
    Q_OBJECT

    CinkciarzClient * client = nullptr;
    QNetworkAccessManager * networkManager = nullptr;
    QTimer * currencyFetchTimer = nullptr;

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

private slots:
    void OnCurrenciesReady(CurrencyInformationList currenciesInformation50k,
                           CurrencyInformationList currenciesInformation);
    void OnCurrencyFetchTimer();
    void ConfigurationRead();
    void ConfigurationError(QString message);

public slots:
    void run();
    void finishApplication();
};

#endif // CINKCIARZAPPLICATION_H
