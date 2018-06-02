#ifndef CINKCIARZAPPLICATION_H
#define CINKCIARZAPPLICATION_H

#include <QObject>

#include "currencyinformation.h"

class CinkciarzClient;

class QNetworkAccessManager;
class QTimer;

class CinkciarzApplication : public QObject
{
private:
    Q_OBJECT

    CinkciarzClient * client = nullptr;
    QNetworkAccessManager * networkManager = nullptr;
    QTimer * currencyFetchTimer = nullptr;

public:
    explicit CinkciarzApplication(QObject *parent = nullptr);
    ~CinkciarzApplication();

signals:
    void finished();
    void FetchCurrencies();

private slots:
    void OnCurrenciesReady(QList<CurrencyInformation> currenciesInformation);
    void OnCurrencyFetchTimer();

public slots:
    void run();
    void finishApplication();
};

#endif // CINKCIARZAPPLICATION_H
