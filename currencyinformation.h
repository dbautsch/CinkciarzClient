#ifndef CURRENCYINFORMATION_H
#define CURRENCYINFORMATION_H

typedef struct
{
    QString code;
    QString name;
    QString buyingValue;
    QString sellingValue;

    bool IsValid()
    {
        return
                code.length() > 0 &&
                name.length() > 0 &&
                buyingValue.length() > 0 &&
                sellingValue.length() > 0;
    }
}
CurrencyInformation;

using CurrencyInformationList = QList<CurrencyInformation>;

#endif // CURRENCYINFORMATION_H
