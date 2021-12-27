#ifndef QTVTKITEM_H
#define QTVTKITEM_H

#include <QQuickVTKRenderWindow.h>
#include <QQmlApplicationEngine>
#include <qquickwindow.h>
#include <qquickvtkrenderitem.h>
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
class qtVtkItem:public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool showFileDialog MEMBER m_showFileDialog NOTIFY showFileDialogChanged)
//    Q_PROPERTY(bool isModelSelected READ getIsModelSelected NOTIFY isModelSelectedChanged)

public:
    qtVtkItem(QQmlApplicationEngine* oeigen);
    void init() ;
    QQmlApplicationEngine* engine ;
    vtkSmartPointer<vtkCameraOrientationWidget> camOrientManipulator;
signals:
    void showFileDialogChanged();
private:
    std::shared_ptr<ProcessingEngine> m_processingEngine;
    bool m_showFileDialog = false;

};

#endif // QTVTKITEM_H
