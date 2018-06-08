#include "datastorage.h"
#include "configuration.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include <QDebug>

DataStorage::DataStorage(QObject *parent) : QObject(parent)
{

}

void DataStorage::StoreCurrencies50kU(CurrencyInformationList currencies)
{
    StoreCurrencies(currencies, "50000");
}

void DataStorage::StoreCurrencies1U(CurrencyInformationList currencies)
{
    StoreCurrencies(currencies, "1");
}

void DataStorage::Connect()
{
    if (QSqlDatabase::isDriverAvailable(configuration->dbDriverName) == false)
    {
        qDebug() << "Database driver (" << configuration->dbDriverName << ") is not supported.";
        emit DatabaseError("Database driver is not supported.");
    }

    db = QSqlDatabase::addDatabase(configuration->dbDriverName);

    db.setDatabaseName(configuration->dbName);
    db.setHostName(configuration->dbHostname);
    db.setUserName(configuration->dbUserName);
    db.setPassword(configuration->dbPassword);

    if (db.open() == false)
    {
        emit DatabaseError(db.lastError().text());
    }
    else
    {
        emit DatabaseConnected();
    }
}

void DataStorage::StoreCurrencies(CurrencyInformationList currencies, QString units)
{
    try
    {
        db.transaction();

        foreach(CurrencyInformation currency, currencies)
        {
            StoreCurrency(currency, units);
        }

        db.commit();
    }
    catch (...)
    {
        db.rollback();
    }
}

void DataStorage::StoreCurrency(const CurrencyInformation & currencyInformation, QString units)
{
    QSqlQuery q(db);

    if (q.prepare("INSERT INTO `currencies` (currencySymbol, sellingValue, buyingValue, units, timestampUTC)\
                  VALUES (:currencySymbol, :sellingValue, :buyingValue, :units, :timestampUTC);")
        != true)
    {
        qDebug() << q.lastError().text();
        emit DatabaseError("Unable to store currency.");
        return;
    }

    q.bindValue(":currencySymbol", currencyInformation.code);
    q.bindValue(":sellingValue", currencyInformation.sellingValue);
    q.bindValue(":buyingValue", currencyInformation.buyingValue);
    q.bindValue(":units", units);
    q.bindValue(":timestampUTC", currencyInformation.timestampUTC.toString());

    if (q.exec() != true)
    {
        qDebug() << q.lastError().text();
        emit DatabaseError("Unable to store currency.");
    }
}
