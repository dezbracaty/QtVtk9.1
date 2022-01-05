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
#include "CommandModel.h"
#include <queue>
#include <future>
#include "qtvtkitem.h"
class qtVtkItem;
class MyQQuickVTKRenderItem :
        public QQuickVTKRenderItem

{
public:
    void sync() override;
    void setqtvtkItem(qtVtkItem *item){qtvtkItem = item;};
    //template<class F, class... Args>
    //std::future<void>& pushCommandToQueue(F&& f, Args&&... args);
private:
    qtVtkItem *qtvtkItem;
    std::queue<std::function<void()>> CommandQueue;
public:
    template<class F, class... Args>
    std::future<void>& pushCommandToQueue(F&& f, Args&&... args)
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared< std::packaged_task<return_type()> >(
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                    );

        std::future<return_type> res = task->get_future();
        {
            CommandQueue.emplace([task]() { (*task)(); });
        }
        return res;
    }
};

#endif // MyQQuickVTKRenderItem_H
