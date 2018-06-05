#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>

class Configuration : public QObject
{
private:
    Q_OBJECT

    void ReadConfigurationFile();

public:
    explicit Configuration(QObject *parent = nullptr);

    int intervalMsec = 10000;

signals:
    void ConfigurationRead();
    void ConfigurationError(QString message);

public slots:
    void ReadConfiguration();
};

extern class Configuration * configuration;

#endif // CONFIGURATION_H
