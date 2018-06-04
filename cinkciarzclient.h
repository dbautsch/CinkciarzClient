#ifndef CINKCIARZCLIENT_H
#define CINKCIARZCLIENT_H

#include <QObject>

#include "currencyinformation.h"

class QNetworkAccessManager;

class CinkciarzClient : public QObject
{
private:
    Q_OBJECT

    QNetworkAccessManager * networkManager = nullptr;

    void HttpGet(QString url);

    void ParseNetworkReply(CurrencyInformationList &currenciesInformation50k,
                           CurrencyInformationList & currenciesInformation,
                           QString data);

    void ParseCurrencyData(QString currencyData, CurrencyInformationList & currencyInformation);

    void Get50kUnitsCurrencyData(QString data, QString & currencyData);

    void Get1UnitCurrencyData(QString data, QString & currencyData);

    static QString ExtractElementFromString(QString data,
                                            QString first,
                                            QString second,
                                            QString third,
                                            int startFrom = 0,
                                            int * firstOccurencePos = nullptr);

    bool GetNextCurrencyInformation(QString data,
                                    int previousLocation,
                                    CurrencyInformation & currencyInformation,
                                    int & lastPos);

public:
    explicit CinkciarzClient(QNetworkAccessManager * networkManager,
                             QObject *parent = nullptr);

signals:
    void CurrenciesReady(CurrencyInformationList currenciesInformation50k,
                         CurrencyInformationList currenciesInformation);

private slots:
    void HttpGetFinished();

public slots:
    void FetchCurrencies();
};

#endif // CINKCIARZCLIENT_H
