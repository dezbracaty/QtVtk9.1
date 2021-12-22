#ifndef MYQMLVTKPLUGIN_H
#define MYQMLVTKPLUGIN_H

#include <QtQml/QQmlExtensionPlugin>

class MyQmlVTKPlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  typedef QQmlExtensionPlugin Superclass;

  Q_PLUGIN_METADATA(IID "org.kitware.VTK")

public:
  /**
   * Constructor
   */
  MyQmlVTKPlugin() = default;

  /**
   * Destructor
   */
  virtual ~MyQmlVTKPlugin() = default;

  /**
   * Register QML types provided by VTK
   */
  void registerTypes(const char* uri);

  /**
   * Initialize the extension using the QQmlEngine
   *
   * \sa cleanup
   */
  void initializeEngine(QQmlEngine* engine, const char* uri);

protected Q_SLOTS:
  /**
   * Destroy any singleton instances that were created during initializeEngine
   *
   * \sa initializeEngine
   */
  void cleanup();

private:
  Q_DISABLE_COPY(MyQmlVTKPlugin);
};

#endif // MYQMLVTKPLUGIN_H
