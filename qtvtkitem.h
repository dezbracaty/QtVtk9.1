#ifndef QTVTKITEM_H
#define QTVTKITEM_H

#include <QQuickVTKRenderWindow.h>
#include <QQmlApplicationEngine>
#include <qquickwindow.h>
#include "qquickvtkrenderitem.h"
#include <QQmlContext>
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
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLWindow.h>
#include <QQuickVTKRenderItem.h>
#include "vtkinteractorstyletrackballcamerastyle2.h"
#include <QQuickVTKInteractorAdapter.h>
#include "ProcessingEngine.h"
#include "CommandModelAdd.h"
#include <queue>
class MyQQuickVTKRenderItem;
class qtVtkItem:public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(bool showFileDialog MEMBER m_showFileDialog NOTIFY showFileDialogChanged)
    Q_PROPERTY(bool isModelSelected READ getIsModelSelected WRITE setIsModelSelected NOTIFY isModelSelectedChanged)
    bool isModelSelected = false;
    bool getIsModelSelected() const;
    void setIsModelSelected(bool flag);
    Q_INVOKABLE void openModel(const QUrl &path) ;
    Q_INVOKABLE void setModelsRepresentation(const int value);
    Q_INVOKABLE void setModelsOpacity(const double value);
    Q_INVOKABLE void setGouraudInterpolation(const bool value);

public:
    qtVtkItem(QQmlApplicationEngine* oeigen);
    void init() ;
    QQmlApplicationEngine* engine ;
    vtkSmartPointer<vtkCameraOrientationWidget> camOrientManipulator;
    void addCommand(CommandModel* command);


    /** Command related function   **/
    CommandModel* getCommandsQueueFront() const;
    void commandsQueuePop();
    bool isCommandsQueueEmpty() const;
    void lockCommandsQueueMutex();
    void unlockCommandsQueueMutex();
signals:
    void showFileDialogChanged();
    void isModelSelectedChanged();

public slots:
    void modelReadyToImport(CommandModel* command);
private:
    std::shared_ptr<ProcessingEngine>       m_processingEngine;
    bool m_showFileDialog = false;
    vtkSmartPointer<vtkRenderer>          m_otherRenderer;
    std::queue<CommandModel*>           m_commandsQueue;
    std::mutex                    m_commandsQueueMutex;
    MyQQuickVTKRenderItem*              m_QQuickvtkrenderItem;
    QQuickWindow*                   qquickWindow;
};

#endif // QTVTKITEM_H
