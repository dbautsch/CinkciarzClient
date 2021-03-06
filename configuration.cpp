#include "configuration.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>

Configuration * configuration = nullptr;

static const QString configurationFileName = "configuration.json";

Configuration::Configuration(QObject *parent) : QObject(parent)
{

}

void Configuration::ReadConfiguration()
{
    /* SLOT */

    ReadConfigurationFile();
    emit ConfigurationRead();
}

void Configuration::ReadConfigurationFile()
{
    ResetConfiguration();

    QFile jsonFile(QCoreApplication::applicationDirPath() + "/" + configurationFileName);
    if (jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&jsonFile);
        QJsonDocument configFile = QJsonDocument::fromJson(stream.readAll().toUtf8());

        if (configFile.isEmpty())
        {
            emit ConfigurationError("Configuration file is empty.");
            return;
        }

        QJsonObject obj = configFile.object();

        // Currency fetching configuration
        QJsonObject currenciesFetcherConfig = obj.value("currenciesFetcher").toObject();

        if (currenciesFetcherConfig.isEmpty() == false)
        {
            intervalMsec = currenciesFetcherConfig.value("intervalMs").toInt();

            if (currenciesFetcherConfig.value("excludedDays").isArray())
            {
                auto excludedDaysArray = currenciesFetcherConfig.value("excludedDays").toArray();

                foreach(QJsonValue arrayItem, excludedDaysArray)
                {
                    excludedDays.push_back(arrayItem.toInt());
                }
            }
        }

        // Database configuration
        QJsonObject dbConfiguration = obj.value("database").toObject();

        if (dbConfiguration.isEmpty() == false)
        {
            dbDriverName = dbConfiguration.value("driverName").toString();
            dbUserName = dbConfiguration.value("userName").toString();
            dbPassword = dbConfiguration.value("password").toString();
            dbName = dbConfiguration.value("name").toString();
            dbHostname = dbConfiguration.value("hostname").toString();
            dbPort = dbConfiguration.value("port").toInt();
        }
    }
    else
    {
        emit ConfigurationError("Unable to open configuration file.");
    }
}

void Configuration::ResetConfiguration()
{
    intervalMsec = 10000;
    excludedDays.clear();

    dbPort = 0;
}
