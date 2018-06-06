#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QObject>
#include <QtSql/QSqlDatabase>

#include "currencyinformation.h"

class DataStorage : public QObject
{
private:
    Q_OBJECT

    QSqlDatabase db;

public:
    explicit DataStorage(QObject *parent = nullptr);

signals:
    void DatabaseConnected();
    void DatabaseError(QString message);

public slots:
    void Connect();
    void StoreCurrencies50kU(CurrencyInformationList currencies);
    void StoreCurrencies1U(CurrencyInformationList currencies);
};

#endif // DATASTORAGE_H
