#ifndef MyQQuickVTKRenderItem_H
#define MyQQuickVTKRenderItem_H

// Qt includes
#include <QObject>
#include <QOpenGLFunctions> // For QOpenGLFunctions
#include <QQuickItem>
// vtk includes
#include <vtkWeakPointer.h> // For vtkWeakPointer
#include <vtkGUISupportQtQuickModule.h> // for export macro
#include <vtkNew.h>                // For vtkNew
#include <vtkRenderer.h>           // For vtkRenderer
#include <vtkImageData.h>
#include <QQuickVTKRenderItem.h>

#include "qtvtkitem.h"
class qtVtkItem;
class MyQQuickVTKRenderItem :
        public QQuickVTKRenderItem

{
public:
    void sync() override;
    void setqtvtkItem(qtVtkItem *item){qtvtkItem = item;};
private:
    qtVtkItem *qtvtkItem;

};

#endif // MyQQuickVTKRenderItem_H
