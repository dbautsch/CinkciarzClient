#include "cinkciarzclient.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>
#include <QCoreApplication>

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

    CurrencyInformationList currenciesInformation50k;
    CurrencyInformationList currenciesInformation;

    ParseNetworkReply(currenciesInformation50k,
                      currenciesInformation,
                      data);

    emit CurrenciesReady(currenciesInformation50k, currenciesInformation);
}

void CinkciarzClient::ParseNetworkReply(CurrencyInformationList &currenciesInformation50k,
                                        CurrencyInformationList & currenciesInformation,
                                        QString data)
{
#ifdef DUMP_RESPONSES
    QFile f(QCoreApplication::applicationDirPath() + "/response.html");

    if (f.open(QIODevice::WriteOnly | QIODevice::Text) == false)
    {
        qDebug() << "Unable to create response.html file";
    }
    else
    {
        QTextStream s(&f);
        s << data;
        s.flush();
        f.close();
    }
#endif // DUMP_RESPONSES defined

    if (data.length() == 0)
    {
        return;
    }

    QString currencyData50k, currencyData;

    Get50kUnitsCurrencyData(data, currencyData50k);
    ParseCurrencyData(currencyData50k, currenciesInformation50k);

    Get1UnitCurrencyData(data, currencyData);
    ParseCurrencyData(currencyData, currenciesInformation);
}

void CinkciarzClient::ParseCurrencyData(QString currencyData,
                                        CurrencyInformationList & currencyInformation)
{
    int previousLocation = -1;
    int lastPos;
    CurrencyInformation info;

    while (GetNextCurrencyInformation(currencyData,
                                      previousLocation,
                                      info,
                                      lastPos))
    {
        currencyInformation.push_back(info);
        previousLocation = lastPos;
    }

}

void CinkciarzClient::Get50kUnitsCurrencyData(QString data, QString & currencyData)
{
    currencyData = ExtractElementFromString(data,
                                            "<div role=\"tabpanel\"",
                                            "id=\"for-50k\">",
                                            "</tbody>");
}

void CinkciarzClient::Get1UnitCurrencyData(QString data, QString & currencyData)
{
    currencyData = ExtractElementFromString(data,
                                            "<div role=\"tabpanel\"",
                                            "id=\"for-1\">",
                                            "</tbody>");
}

QString CinkciarzClient::ExtractElementFromString(QString data,
                                                  QString first,
                                                  QString second,
                                                  QString third,
                                                  int startFrom,
                                                  int *firstOccurencePos)
{
    auto firstPos = data.indexOf(first, startFrom);

    if (firstPos < 0)
    {
        return QString();
    }

    if (firstOccurencePos != nullptr)
    {
        *firstOccurencePos = firstPos;
    }

    auto secondPos = data.indexOf(second, firstPos + first.length());

    if (secondPos < 0)
    {
        return QString();
    }

    auto thirdPos = data.indexOf(third, secondPos + second.length());

    if (thirdPos < 0)
    {
        return QString();
    }

    return data.mid(secondPos + second.length(),
                    thirdPos - (secondPos + second.length()));
}

bool CinkciarzClient::GetNextCurrencyInformation(QString data,
                                                 int previousLocation,
                                                 CurrencyInformation & currencyInformation,
                                                 int &lastPos)
{
    if (previousLocation < 0)
    {
        previousLocation = 0;
    }

    int occurencePos;

    currencyInformation.name = (ExtractElementFromString(data,
                                                         "<td data-label=\"Nazwa\">",
                                                         "\">",
                                                         "</a>",
                                                         previousLocation,
                                                         &occurencePos)).trimmed();
    previousLocation = occurencePos;

    currencyInformation.code = (ExtractElementFromString(data,
                                                         "\"Kod waluty\">",
                                                         "</strong>",
                                                         "</td>",
                                                         previousLocation,
                                                         &occurencePos)).trimmed();

    previousLocation = occurencePos;

    currencyInformation.buyingValue = (ExtractElementFromString(data,
                                                                "data-buy=\"true",
                                                                "\">",
                                                                "</td>",
                                                                previousLocation,
                                                                &occurencePos)).trimmed();

    previousLocation = occurencePos;

    currencyInformation.sellingValue = (ExtractElementFromString(data,
                                                                 "data-sell=\"true",
                                                                 "\">",
                                                                 "</td>",
                                                                 previousLocation,
                                                                 &occurencePos)).trimmed();


    lastPos = occurencePos;

    return currencyInformation.IsValid();
}
