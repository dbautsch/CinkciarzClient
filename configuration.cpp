#include "configuration.h"

#include <QJsonDocument>
#include <QJsonObject>
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
        QJsonObject currenciesFetcherConfig = obj.value("currenciesFetcher").toObject();

        if (currenciesFetcherConfig.isEmpty() == false)
        {
            intervalMsec = currenciesFetcherConfig.value("intervalMs").toVariant().toInt();
        }
    }
    else
    {
        emit ConfigurationError("Unable to open configuration file.");
    }
}
