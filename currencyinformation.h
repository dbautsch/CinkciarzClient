#ifndef CURRENCYINFORMATION_H
#define CURRENCYINFORMATION_H

typedef struct
{
    QString name;
    QString buyingValue;
    QString sellingValue;
}
CurrencyInformation;

using CurrencyInformationList = QList<CurrencyInformation>;

#endif // CURRENCYINFORMATION_H
