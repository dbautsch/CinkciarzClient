#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>

class Configuration : public QObject
{
private:
    Q_OBJECT

    void ReadConfigurationFile();
    void ResetConfiguration();

public:
    explicit Configuration(QObject *parent = nullptr);

    // Currency fetching data
    int intervalMsec = 10000;
    QList<int> excludedDays;

    // Database configuration
    QString dbDriverName;
    QString dbUserName;
    QString dbPassword;
    QString dbName;
    QString dbHostname;
    int dbPort = 0;

signals:
    void ConfigurationRead();
    void ConfigurationError(QString message);

public slots:
    void ReadConfiguration();
};

extern class Configuration * configuration;

#endif // CONFIGURATION_H
