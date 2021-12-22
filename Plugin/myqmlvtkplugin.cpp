#include "myqmlvtkplugin.h"

// vtk includes
#include "qquickvtkrenderitem.h"
#include <vtkVersion.h>

// Qt includes
#include <QQmlEngine>
#include <QQuickVTKRenderItem.h>
//-------------------------------------------------------------------------------------------------
void MyQmlVTKPlugin::registerTypes(const char* uri)
{
  Q_ASSERT(QString::compare(uri, "VTK") == 0);

  int major = vtkVersion::GetVTKMajorVersion();
  int minor = vtkVersion::GetVTKMinorVersion();

  // Register QML metatypes
  qmlRegisterType<QQuickVTKRenderItem>(uri, major, minor, "VTKRenderItem");
}

//-------------------------------------------------------------------------------------------------
void MyQmlVTKPlugin::initializeEngine(QQmlEngine* engine, const char* uri)
{
  Q_ASSERT(QString::compare(uri, "VTK") == 0);

  QObject::connect(
    engine, &QQmlEngine::destroyed, this, &MyQmlVTKPlugin::cleanup, Qt::DirectConnection);
}

//-------------------------------------------------------------------------------------------------
void MyQmlVTKPlugin::cleanup() {}
