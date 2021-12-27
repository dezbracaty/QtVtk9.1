#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "qtvtkitem.h"
#include <vtkAutoInit.h>
#include <QtQuickControls2/qquickstyle.h>
#include <QIcon>
#include <QApplication>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
//VTK_MODULE_INIT(vtkRenderingFreeType)
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    app.setOrganizationName("XunshiSoonSolid");
    app.setOrganizationDomain("soonsolid.com");
    app.setApplicationName("XunshiApplication");

    QQmlApplicationEngine engine;
    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qt-project.org/imports/");

//    qDebug() << "QML2_IMPORT_PATH:" << engine.importPathList()<<QCoreApplication::applicationDirPath() + "/qt-project.org/imports/";
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    qmlRegisterType<QQuickVTKRenderWindow>("VTKPLUS", 9, 1, "VTKRenderWindow");
    qmlRegisterType<QQuickVTKRenderItem>("VTKPLUS", 9, 1, "VTKRenderItem");
    qmlRegisterType<QQuickVTKInteractiveWidget>("VTKPLUS", 9, 1, "VTKWidget");



    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);



    qtVtkItem qtvtkItem(&engine);
    return app.exec();
}
