#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qquickwindow.h>
#include <qquickvtkrenderitem.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <QQuickVTKRenderItem.h>
#include <QQuickVTKRenderWindow.h>
#include <QQuickVTKInteractiveWidget.h>
#include <vtkAxesActor.h>
#include <vtkCubeSource.h>
#include <vtkProperty.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkPolyLine.h>
#include <vtkCameraOrientationWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkBoxRepresentation.h>
#include <vtkBoxWidget2.h>
#include <vtkTransform.h>

namespace {
class vtkBoxCallback : public vtkCommand
{
public:
  static vtkBoxCallback* New()
  {
    return new vtkBoxCallback;
  }

  vtkSmartPointer<vtkActor> m_actor;

  void SetActor(vtkSmartPointer<vtkActor> actor)
  {
    m_actor = actor;
  }

  virtual void Execute(vtkObject* caller, unsigned long, void*)
  {
    vtkSmartPointer<vtkBoxWidget2> boxWidget =
        dynamic_cast<vtkBoxWidget2*>(caller);

    vtkNew<vtkTransform> t;

    dynamic_cast<vtkBoxRepresentation*>(boxWidget->GetRepresentation())
        ->GetTransform(t);
    this->m_actor->SetUserTransform(t);
  }

  vtkBoxCallback()
  {
  }
};
} // namespace
void createLine(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2, vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkCellArray> cells)
{
    vtkSmartPointer<vtkPolyLine> line;
    line = vtkSmartPointer<vtkPolyLine>::New();
    line->GetPointIds()->SetNumberOfIds(2);

    vtkIdType id_1, id_2;
    id_1 = points->InsertNextPoint(x1, y1, z1);
    id_2 = points->InsertNextPoint(x2, y2, z2);

    line->GetPointIds()->SetId(0, id_1);
    line->GetPointIds()->SetId(1, id_2);

    cells->InsertNextCell(line);
}
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

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



    QObject* topLevel = engine.rootObjects().value(0);
    QQuickWindow* window = qobject_cast<QQuickWindow*>(topLevel);

    window->show();

    // Fetch the QQuick window using the standard object name set up in the constructor
    QQuickVTKRenderItem* qquickvtkItem = topLevel->findChild<MyQQuickVTKRenderItem*>("ConeView");
    // Create a cone pipeline and add it to the view
    vtkNew<vtkActor> actor;
    vtkNew<vtkPolyDataMapper> mapper;
    vtkNew<vtkConeSource> cone;

    vtkNew<vtkAxesActor> axes;
    double axes_length = 100.0;
    int16_t axes_label_font_size = 20;
    axes->SetTotalLength(axes_length, axes_length, axes_length);
    axes->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axes->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axes->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(axes_label_font_size);
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(axes_label_font_size);
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(axes_label_font_size);


    //add platform
    // Platform Model
    vtkNew<vtkPolyDataMapper> platformModelMapper;
    vtkNew<vtkCubeSource> m_platformModel;
    platformModelMapper->SetInputConnection(m_platformModel->GetOutputPort());
    vtkNew<vtkActor> m_platformModelActor;
    m_platformModelActor->SetMapper(platformModelMapper);
    m_platformModelActor->GetProperty()->SetColor(1, 1, 1);
    m_platformModelActor->GetProperty()->LightingOn();
    m_platformModelActor->GetProperty()->SetOpacity(1);
    m_platformModelActor->GetProperty()->SetAmbient(0.45);
    m_platformModelActor->GetProperty()->SetDiffuse(0.4);

    m_platformModelActor->PickableOff();

    // Platform Grid

    vtkNew<vtkPolyData> m_platformGrid;

    vtkSmartPointer<vtkPolyDataMapper> platformGridMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    platformGridMapper->SetInputData(m_platformGrid);

    vtkNew<vtkActor> m_platformGridActor;
    m_platformGridActor->SetMapper(platformGridMapper);
    m_platformGridActor->GetProperty()->LightingOff();
    m_platformGridActor->GetProperty()->SetColor(0.45, 0.45, 0.45);
    m_platformGridActor->GetProperty()->SetOpacity(1);
    m_platformGridActor->PickableOff();
    double m_platformWidth = 200.0;
    double m_platformDepth = 200.0;
    double m_platformHeight = 200.0;
    double m_platformThickness = 2.0;
    double m_gridBottomHeight = 0.15;
    uint16_t m_gridSize = 10;

    // Platform Grid
    vtkSmartPointer<vtkPoints> gridPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> gridCells = vtkSmartPointer<vtkCellArray>::New();

    for (int16_t i = -m_platformWidth / 2; i <= m_platformWidth / 2; i += m_gridSize)
    {
        createLine(i, -m_platformDepth / 2, m_gridBottomHeight, i, m_platformDepth / 2, m_gridBottomHeight, gridPoints, gridCells);
    }

    for (int16_t i = -m_platformDepth / 2; i <= m_platformDepth / 2; i += m_gridSize)
    {
        createLine(-m_platformWidth / 2, i, m_gridBottomHeight, m_platformWidth / 2, i, m_gridBottomHeight, gridPoints, gridCells);
    }

    m_platformGrid->SetPoints(gridPoints);
    m_platformGrid->SetLines(gridCells);
    m_platformModel->SetXLength(m_platformWidth);
    m_platformModel->SetYLength(m_platformDepth);
    m_platformModel->SetZLength(m_platformThickness);
    m_platformModel->SetCenter(0.0, 0.0, -m_platformThickness / 2);

    mapper->SetInputConnection(cone->GetOutputPort());
    actor->SetMapper(mapper);
    qquickvtkItem->renderer()->AddActor(actor);
    qquickvtkItem->renderer()->AddActor(axes);
    qquickvtkItem->renderer()->AddActor(m_platformModelActor);
    qquickvtkItem->renderer()->AddActor(m_platformGridActor);
    vtkNew<vtkCameraOrientationWidget> camOrientManipulator;
    camOrientManipulator->SetParentRenderer(qquickvtkItem->renderer());
    // Enable the widget.
    qquickvtkItem->renderer()->ResetCamera();

    qquickvtkItem->renderer()->SetBackground(0.5, 0.5, 0.7);
    qquickvtkItem->renderer()->SetBackground2(0.7, 0.7, 0.7);
    qquickvtkItem->renderer()->SetGradientBackground(true);

    qquickvtkItem->update();
    camOrientManipulator->On();
//    camOrientManipulator->SetCurrentRenderer(qquickvtkItem->renderer());
    vtkNew<vtkRenderWindowInteractor> iRen;
    QQuickVTKRenderWindow* qquickvtkWindow = topLevel->findChild<QQuickVTKRenderWindow*>("RenderWindowQt");
    vtkNew<vtkBoxWidget2> boxWidget;
    boxWidget->SetInteractor(iRen);
    boxWidget->GetRepresentation()->SetPlaceFactor(1); // Default is 0.5
    boxWidget->GetRepresentation()->PlaceWidget(m_platformModelActor->GetBounds());

    // Set up a callback for the interactor to call so we can manipulate the actor
    vtkNew<vtkBoxCallback> boxCallback;
    boxCallback->SetActor(m_platformModelActor);
    boxWidget->AddObserver(vtkCommand::InteractionEvent, boxCallback);

    boxWidget->On();
//    iRen->SetRenderWindow(qquickvtkWindow->renderWindow());
//    iRen->Initialize();
//    iRen->Start();


//    // Fetch the QQuick window using the standard object name set up in the constructor
//    QQuickVTKRenderItem* qquickvtkItem2 = topLevel->findChild<MyQQuickVTKRenderItem*>("ConeView2");
//    // Create a cone pipeline and add it to the view
//    vtkNew<vtkActor> actor2;
//    vtkNew<vtkPolyDataMapper> mapper2;
//    vtkNew<vtkConeSource> cone2;
//    mapper2->SetInputConnection(cone2->GetOutputPort());
//    actor2->SetMapper(mapper2);
//    qquickvtkItem2->renderer()->AddActor(actor2);
//    qquickvtkItem2->renderer()->ResetCamera();
//    qquickvtkItem2->renderer()->SetBackground(0.5, 0.5, 0.7);
//    qquickvtkItem2->renderer()->SetBackground2(0.7, 0.7, 0.7);
//    qquickvtkItem2->renderer()->SetGradientBackground(true);
//    qquickvtkItem2->update();

    return app.exec();
}
