#include "qtvtkitem.h"

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
qtVtkItem::qtVtkItem(QQmlApplicationEngine* oeigen)
{
    engine = std::move(oeigen);
}

void  qtVtkItem::init()
{
    QObject* topLevel = engine->rootObjects().value(0);
    // Create classes instances
    m_processingEngine = std::shared_ptr<ProcessingEngine>(new ProcessingEngine());
    // Expose C++ classes to QML


    qquickWindow = qobject_cast<QQuickWindow*>(topLevel);
    qquickWindow->show();
    QQuickVTKRenderWindow* QvtkRwd = new QQuickVTKRenderWindow();
    // Fetch the QQuick window using the standard object name set up in the constructor
    m_QQuickvtkrenderItem = topLevel->findChild<MyQQuickVTKRenderItem*>("ConeView");
    m_otherRenderer = m_QQuickvtkrenderItem->renderer();
    m_QQuickvtkrenderItem->setqtvtkItem(this);
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

//    m_QQuickvtkrenderItem->renderer()->AddActor(actor);
    m_QQuickvtkrenderItem->renderer()->AddActor(axes);
    m_QQuickvtkrenderItem->renderer()->AddActor(m_platformModelActor);
    m_QQuickvtkrenderItem->renderer()->AddActor(m_platformGridActor);
    camOrientManipulator = vtkSmartPointer<vtkCameraOrientationWidget>::New() ;
    // Enable the widget.
    m_QQuickvtkrenderItem->renderer()->ResetCamera();

    m_QQuickvtkrenderItem->renderer()->SetBackground(0.5, 0.5, 0.7);
    m_QQuickvtkrenderItem->renderer()->SetBackground2(0.7, 0.7, 0.7);
    m_QQuickvtkrenderItem->renderer()->SetGradientBackground(true);

//    m_QQuickvtkrenderItem->update();
//    camOrientManipulator->SetCurrentRenderer(m_QQuickvtkrenderItem->renderer());
//    vtkNew<vtkRenderWindowInteractor> iRen;
//    QQuickVTKRenderWindow* qquickvtkWindow = topLevel->findChild<QQuickVTKRenderWindow*>("RenderWindowQt");
//    vtkNew<vtkBoxWidget2> boxWidget;
//    boxWidget->SetInteractor(iRen);
//    boxWidget->GetRepresentation()->SetPlaceFactor(1); // Default is 0.5
//    boxWidget->GetRepresentation()->PlaceWidget(m_platformModelActor->GetBounds());

//    // Set up a callback for the interactor to call so we can manipulate the actor
//    vtkNew<vtkBoxCallback> boxCallback;
//    boxCallback->SetActor(m_platformModelActor);
//    boxWidget->AddObserver(vtkCommand::InteractionEvent, boxCallback);

//    boxWidget->On();







    // Interactor Style
    vtkSmartPointer<vtkInteractorStyleTrackballCamera2> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera2>::New();
//    style->SetDefaultRenderer(m_QQuickvtkrenderItem->renderer());
//    style->SetMotionFactor(10.0);
    vtkNew<vtkRenderWindowInteractor> iRen;
    m_QQuickvtkrenderItem->renderWindow()->renderWindow()->GetInteractor()->SetInteractorStyle(style);
    style->SetDefaultRenderer(m_QQuickvtkrenderItem->renderer());
    style->setQtItem(this);
    QQuickVTKInteractiveWidget* interactorWidget = new QQuickVTKInteractiveWidget(m_QQuickvtkrenderItem->renderWindow());
    interactorWidget->setWidget(camOrientManipulator);
    interactorWidget->setEnabled(true);
    m_QQuickvtkrenderItem->addWidget(interactorWidget);
    camOrientManipulator->SetParentRenderer(m_QQuickvtkrenderItem->renderer());
    camOrientManipulator->SetInteractor(m_QQuickvtkrenderItem->renderWindow()->renderWindow()->GetInteractor());
    m_QQuickvtkrenderItem->update();
    camOrientManipulator->EnabledOn();
    m_QQuickvtkrenderItem->renderWindow()->interactorAdapter()->SetEnableHover(true);
    camOrientManipulator->On();

    double m_camPositionX = -237.885;
    double m_camPositionY = -392.348;
    double m_camPositionZ = 369.477;

    m_QQuickvtkrenderItem->renderer()->GetActiveCamera()->SetPosition(m_camPositionX, m_camPositionY, m_camPositionZ);
    m_QQuickvtkrenderItem->renderer()->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);
    m_QQuickvtkrenderItem->renderer()->GetActiveCamera()->SetFocalPoint(0.0, 0.0, 0.0);

}

void qtVtkItem::openModel(const QUrl &path)
{
    QUrl localFilePath;
    if (path.isLocalFile())
    {
        // Remove the "file:///" if present
        localFilePath = path.toLocalFile();
    }
    else
    {
        localFilePath = path;
    }
    qDebug() << "QVTKFramebufferObjectItem::addModelFromFile"<<localFilePath;
    CommandModelAdd *command = new CommandModelAdd(m_otherRenderer, m_processingEngine, localFilePath);

//    connect(command, &CommandModelAdd::ready, m_otherRenderer, &vtkRenderer::Render);
////    connect(command, &CommandModelAdd::done, this, &QVTKFramebufferObjectItem::addModelFromFileDone);

    command->start();

    addCommand(command);
}


void qtVtkItem::setModelsRepresentation(const int value)
{
    m_QQuickvtkrenderItem->pushCommandToQueue([=](){
        m_processingEngine->setModelsRepresentation(value);
        qquickWindow->resetOpenGLState();
        });
}

void qtVtkItem::setModelsOpacity(const double value)
{
    m_QQuickvtkrenderItem->pushCommandToQueue([=](){
        m_processingEngine->setModelsOpacity(value);
        qquickWindow->resetOpenGLState();
        });
}

void qtVtkItem::setGouraudInterpolation(const bool value)
{
    m_QQuickvtkrenderItem->pushCommandToQueue([=](){
        m_processingEngine->setModelsGouraudInterpolation(value);
        qquickWindow->resetOpenGLState();
        });
}

void qtVtkItem::addCommand(CommandModel *command)
{
    m_commandsQueueMutex.lock();
    m_commandsQueue.push(command);
    m_commandsQueueMutex.unlock();

    //    update();
}

bool qtVtkItem::getIsModelSelected() const
{
//    if( m_otherRenderer )
//        return vtkInteractorStyleTrackballCamera2::SafeDownCast( m_otherRenderer->GetRenderWindow()->GetInteractor()->GetInteractorStyle() )
//                ->getIsModelSelected();
    return isModelSelected;
}

void qtVtkItem::setIsModelSelected(bool flag)
{
    isModelSelected = flag;
    emit isModelSelectedChanged();
}

CommandModel *qtVtkItem::getCommandsQueueFront() const
{
    return m_commandsQueue.front();
}

void qtVtkItem::commandsQueuePop()
{
    m_commandsQueue.pop();
}

bool qtVtkItem::isCommandsQueueEmpty() const
{
    return m_commandsQueue.empty();
}

void qtVtkItem::lockCommandsQueueMutex()
{
    m_commandsQueueMutex.lock();
}

void qtVtkItem::unlockCommandsQueueMutex()
{
    m_commandsQueueMutex.unlock();
}

void qtVtkItem::modelReadyToImport(CommandModel* command)
{
    command->execute();
}


