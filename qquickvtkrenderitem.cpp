#include "qquickvtkrenderitem.h"


void MyQQuickVTKRenderItem::sync()
{
//    std::cout<<"Syncing"<<std::endl;
    /**
     * @brief deal with command before render
     */
    CommandModel *command;
    while (!qtvtkItem->isCommandsQueueEmpty())
    {
        qtvtkItem->lockCommandsQueueMutex();

        command = qtvtkItem->getCommandsQueueFront();
        if ( !command->isReady() )
        {
            qtvtkItem->unlockCommandsQueueMutex();
            break;
        }
//        if( command->typeis() !=2 && commandTranslate->getTrans().model != nullptr && m_selectedModel != nullptr && commandTranslate->getTrans().model != m_selectedModel){
//            qtvtkItem->commandsQueuePop();
//            std::cout<<"Model Not qeual"<<command->typeis() <<std::endl;
//            qtvtkItem->unlockCommandsQueueMutex();
//            continue;
//        }
        qtvtkItem->commandsQueuePop();
        qtvtkItem->unlockCommandsQueueMutex();
        command->execute();
    }
    while(!CommandQueue.empty()){
        std::function<void()> task = std::move( this->CommandQueue.front() );
        this->CommandQueue.pop();
        task();
    }
    QQuickVTKRenderItem::sync();
}

