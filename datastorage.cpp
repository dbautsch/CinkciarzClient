#include "datastorage.h"
#include "configuration.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#include <QDebug>

DataStorage::DataStorage(QObject *parent) : QObject(parent)
{

}

void DataStorage::StoreCurrencies50kU(CurrencyInformationList currencies)
{

}

void DataStorage::StoreCurrencies1U(CurrencyInformationList currencies)
{

}

void DataStorage::InitializeSchema()
{

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
