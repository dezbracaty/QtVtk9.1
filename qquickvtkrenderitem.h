#ifndef MyQQuickVTKRenderItem_H
#define MyQQuickVTKRenderItem_H

// Qt includes
#include <QObject>
// Qt includes
#include <QOpenGLFunctions> // For QOpenGLFunctions
#include <QQuickItem>
// vtk includes
#include "vtkWeakPointer.h" // For vtkWeakPointer
#include "vtkGUISupportQtQuickModule.h" // for export macro
#include "vtkNew.h"                // For vtkNew
#include "vtkRenderer.h"           // For vtkRenderer
#include <vtkImageData.h>
#include <QQuickVTKRenderItem.h>
class MyQQuickVTKRenderItem :
        public QQuickVTKRenderItem

{
    MyQQuickVTKRenderItem ();
    ~MyQQuickVTKRenderItem () = default;
};

#endif // MyQQuickVTKRenderItem_H
