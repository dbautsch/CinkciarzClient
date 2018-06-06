#ifndef CURRENCYINFORMATION_H
#define CURRENCYINFORMATION_H

#include <QDateTime>

typedef struct
{
    QString code;
    QString name;
    QString buyingValue;
    QString sellingValue;
    QDateTime timestampUTC;

    bool IsValid()
    {
        return
                code.length() > 0 &&
                name.length() > 0 &&
                buyingValue.length() > 0 &&
                sellingValue.length() > 0 &&
                timestampUTC.isValid();
    }
}
CurrencyInformation;

using CurrencyInformationList = QList<CurrencyInformation>;

#endif // CURRENCYINFORMATION_H
