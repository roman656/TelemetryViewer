#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include <Core/Core.hpp>

int main(int argc, char* argv[])
{
    QGuiApplication application(argc, argv);
    QQmlApplicationEngine engine;
    Core core;

    engine.rootContext()->setContextProperty(QStringLiteral("core"), &core);
    engine.addImportPath(QStringLiteral("qrc:/"));
    engine.load(QStringLiteral("qrc:/main.qml"));

    return application.exec();
}
