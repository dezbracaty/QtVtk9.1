#ifndef COMMANDMODEL_H
#define COMMANDMODEL_H

#include <vtkRenderer.h>

class CommandModel
{
public:
	CommandModel(){}
	virtual ~CommandModel(){}

	virtual bool isReady() const = 0;
	virtual void execute() = 0;
    virtual unsigned typeis() const = 0 ;

protected:
    vtkRenderer *m_vtkRenderer;
};

#endif // COMMANDMODEL_H
