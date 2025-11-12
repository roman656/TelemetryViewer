#ifndef TV_CORE_CORE_HPP
#define TV_CORE_CORE_HPP

#include <QVariant>

class Core final : public QObject
{
    Q_OBJECT

signals:
    void telemetryLoaded(QVariantList telemetry);

public slots:
    void loadTelemetry(const QUrl& fileQUrl);
};

#endif // TV_CORE_CORE_HPP
