/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkInteractorStyleTrackballCamera2.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkinteractorstyletrackballcamerastyle2.h"

#include "vtkCallbackCommand.h"
#include "vtkCamera.h"
#include "vtkMath.h"
#include "vtkMatrix3x3.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include <vtkPropPicker.h>
#include <vtkNamedColors.h>
#include <vtkCellPicker.h>
#include <vtkTransform.h>
#include <vtkPlane.h>
#include <vtkBoundedPlanePointPlacer.h>
vtkStandardNewMacro(vtkInteractorStyleTrackballCamera2);

//------------------------------------------------------------------------------
vtkInteractorStyleTrackballCamera2::vtkInteractorStyleTrackballCamera2()
{
    this->MotionFactor = 10.0;
    LastPickedActor = NULL;
    LastPickedProperty = vtkProperty::New();
}

//------------------------------------------------------------------------------
vtkInteractorStyleTrackballCamera2::~vtkInteractorStyleTrackballCamera2() = default;

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::OnMouseMove()
{
//    std::cout<<"On mouse move "<<std::endl;
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    switch (this->State)
    {
    case VTKIS_ENV_ROTATE:
        this->FindPokedRenderer(x, y);
        this->EnvironmentRotate();
        this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;

    case VTKIS_ROTATE:
        this->FindPokedRenderer(x, y);
        this->Rotate();
        this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;

    case VTKIS_PAN:
        this->FindPokedRenderer(x, y);
        this->Pan();
        this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;

    case VTKIS_DOLLY:
        this->FindPokedRenderer(x, y);
        this->Dolly();
        this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;

    case VTKIS_SPIN:
        this->FindPokedRenderer(x, y);
        this->Spin();
        this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;
    }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::OnLeftButtonDown()
{
    /**
     * pick the actor when leftbutton down
     **/
    // Pick from this location.
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkCellPicker> picker;
    int* clickPos = this->GetInteractor()->GetEventPosition();
    std::cout<<"On screen position :"<<clickPos[0] <<"\t"<<clickPos[1]<<std::endl;
    std::cout<<"Screen param :"<<DefaultRenderer->GetSize()[0]<<"\t"<<DefaultRenderer->GetSize()[1]<<std::endl;
    picker->Pick(clickPos[0],  clickPos[1], 0, this->GetDefaultRenderer());

    // If we picked something before, reset its property
    if (this->LastPickedActor)
    {
        this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
    }
    this->LastPickedActor = picker->GetActor();
    if (this->LastPickedActor)
    {
        // Save the property of the picked actor so that we can
        // restore it next time
        std::cout<<"We got a actor"<<std::endl;
        this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
        this->InteractionProp = vtkProp3D::SafeDownCast ( picker->GetViewProp() );
        // Highlight the picked actor by changing its properties
        this->LastPickedActor->GetProperty()->SetColor(
                    colors->GetColor3d(/*"dodger_blue"*/"cornflower").GetData());
//        this->LastPickedActor->GetProperty()->SetDiffuse(0.2);
//        this->LastPickedActor->GetProperty()->SetSpecular(0.2);
        this->LastPickedActor->GetProperty()->EdgeVisibilityOn();
        IsModelSelected = true ;
    }else
        IsModelSelected = false ;
    //part of move actor
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    this->FindPokedRenderer(x, y);
    if (this->CurrentRenderer == nullptr || picker->GetActor() == nullptr)
    {
      return;
    }

    this->GrabFocus(this->EventCallbackCommand);
    if (this->Interactor->GetControlKey())
    {
      this->StartDolly();
    }
    else
    {
      this->StartPan();
    }


}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::OnLeftButtonUp()
{
    switch (this->State)
      {
        case VTKIS_DOLLY:
          this->EndDolly();
          break;

        case VTKIS_PAN:
          this->EndPan();
          break;

        case VTKIS_SPIN:
          this->EndSpin();
          break;

        case VTKIS_ROTATE:
          this->EndRotate();
          break;
      }

      if (this->Interactor)
      {
        this->ReleaseFocus();
      }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::OnMiddleButtonDown()
{
    this->FindPokedRenderer(
                this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1]);
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    this->GrabFocus(this->EventCallbackCommand);
    this->StartPan();
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::OnMiddleButtonUp()
{
    switch (this->State)
    {
    case VTKIS_PAN:
        this->EndPan();
        if (this->Interactor)
        {
            this->ReleaseFocus();
        }
        break;
    }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::OnRightButtonDown()
{
    //remote right button dolly function

    this->FindPokedRenderer(
                this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1]);
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }


    this->GrabFocus(this->EventCallbackCommand);
    if (this->Interactor->GetShiftKey())
    {
        if (this->Interactor->GetControlKey())
        {
            this->StartDolly();
        }
        else
        {
            this->StartPan();
        }
    }
    else
    {
        if (this->Interactor->GetControlKey())
        {
            this->StartSpin();
        }
        else
        {
            this->StartRotate();
        }
    }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::OnRightButtonUp()
{
    //remote right button dolly function
    switch (this->State)
    {
    case VTKIS_DOLLY:
        this->EndDolly();
        break;

    case VTKIS_PAN:
        this->EndPan();
        break;

    case VTKIS_SPIN:
        this->EndSpin();
        break;

    case VTKIS_ROTATE:
        this->EndRotate();
        break;
    }

    if (this->Interactor)
    {
        this->ReleaseFocus();
    }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::OnMouseWheelForward()
{
    this->FindPokedRenderer(
                this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1]);
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    this->GrabFocus(this->EventCallbackCommand);
    this->StartDolly();
    double factor = this->MotionFactor * 0.2 * this->MouseWheelMotionFactor;
    this->Dolly(pow(1.1, factor));
    this->EndDolly();
    this->ReleaseFocus();
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::OnMouseWheelBackward()
{
    this->FindPokedRenderer(
                this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1]);
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    this->GrabFocus(this->EventCallbackCommand);
    this->StartDolly();
    double factor = this->MotionFactor * -0.2 * this->MouseWheelMotionFactor;
    this->Dolly(pow(1.1, factor));
    this->EndDolly();
    this->ReleaseFocus();
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::Rotate()
{
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    vtkRenderWindowInteractor* rwi = this->Interactor;

    int dx = rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0];
    int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];

    const int* size = this->CurrentRenderer->GetRenderWindow()->GetSize();

    double delta_elevation = -20.0 / size[1];
    double delta_azimuth = -20.0 / size[0];

    double rxf = dx * delta_azimuth * this->MotionFactor;
    double ryf = dy * delta_elevation * this->MotionFactor;

    vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
    camera->Azimuth(rxf);
    camera->Elevation(ryf);
    camera->OrthogonalizeViewUp();
    camera->SetViewUp(0,0,1);
    if (this->AutoAdjustCameraClippingRange)
    {
        this->CurrentRenderer->ResetCameraClippingRange();
    }

    if (rwi->GetLightFollowCamera())
    {
        this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

    rwi->Render();
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::Spin()
{
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    vtkRenderWindowInteractor* rwi = this->Interactor;

    double* center = this->CurrentRenderer->GetCenter();

    double newAngle = vtkMath::DegreesFromRadians(
                atan2(rwi->GetEventPosition()[1] - center[1], rwi->GetEventPosition()[0] - center[0]));

    double oldAngle = vtkMath::DegreesFromRadians(
                atan2(rwi->GetLastEventPosition()[1] - center[1], rwi->GetLastEventPosition()[0] - center[0]));

    vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
    camera->Roll(newAngle - oldAngle);
    camera->OrthogonalizeViewUp();

    rwi->Render();
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::Pan()
{
    if(!IsModelSelected){
        //if no model selected , the pan can move camera
        //so the whole scene will move
        if (this->CurrentRenderer == nullptr)
        {
            return;
        }

        vtkRenderWindowInteractor* rwi = this->Interactor;

        double viewFocus[4], focalDepth, viewPoint[3];
        double newPickPoint[4], oldPickPoint[4], motionVector[3];

        // Calculate the focal depth since we'll be using it a lot

        vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
        camera->GetFocalPoint(viewFocus);
        this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2], viewFocus);
        focalDepth = viewFocus[2];

        this->ComputeDisplayToWorld(
                    rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], focalDepth, newPickPoint);

        // Has to recalc old mouse point since the viewport has moved,
        // so can't move it outside the loop

        this->ComputeDisplayToWorld(
                    rwi->GetLastEventPosition()[0], rwi->GetLastEventPosition()[1], focalDepth, oldPickPoint);

        // Camera motion is reversed

        motionVector[0] = oldPickPoint[0] - newPickPoint[0];
        motionVector[1] = oldPickPoint[1] - newPickPoint[1];
        motionVector[2] = oldPickPoint[2] - newPickPoint[2];

        camera->GetFocalPoint(viewFocus);
        camera->GetPosition(viewPoint);
        camera->SetFocalPoint(
                    motionVector[0] + viewFocus[0], motionVector[1] + viewFocus[1], motionVector[2] + viewFocus[2]);

        camera->SetPosition(
                    motionVector[0] + viewPoint[0], motionVector[1] + viewPoint[1], motionVector[2] + viewPoint[2]);

        if (rwi->GetLightFollowCamera())
        {
            this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
        }

        rwi->Render();
    }else if(0){
        //this method is not used
        //if model is selected
        //the pan only move the actor which user selected
        //this part of source is from vtkInteractorStyleTrackballActor.cxx pan() code
        //and modified to suitable platform
        if (this->CurrentRenderer == nullptr || this->InteractionProp == nullptr)
        {
          return;
        }

        vtkRenderWindowInteractor* rwi = this->Interactor;

        // Use initial center as the origin from which to pan

        double* obj_center = this->InteractionProp->GetCenter();

        double disp_obj_center[3], new_pick_point[4];
        double old_pick_point[4], motion_vector[3];

        this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2], disp_obj_center);

        this->ComputeDisplayToWorld(
          rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], disp_obj_center[2], new_pick_point);

        this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0], rwi->GetLastEventPosition()[1],
          disp_obj_center[2], old_pick_point);

        motion_vector[0] = new_pick_point[0] - old_pick_point[0];
        motion_vector[1] = new_pick_point[1] - old_pick_point[1];
        motion_vector[2] = new_pick_point[2] - old_pick_point[2];
        //we need set z to origin because the models are on platform
        motion_vector[2] = 0;
        if (this->InteractionProp->GetUserMatrix() != nullptr)
        {
          vtkTransform* t = vtkTransform::New();
          t->PostMultiply();
          t->SetMatrix(this->InteractionProp->GetUserMatrix());
          t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
          this->InteractionProp->GetUserMatrix()->DeepCopy(t->GetMatrix());
          t->Delete();
        }
        else
        {
          this->InteractionProp->AddPosition(motion_vector[0], motion_vector[1], motion_vector[2]);
        }

        if (this->AutoAdjustCameraClippingRange)
        {
          this->CurrentRenderer->ResetCameraClippingRange();
        }

        rwi->Render();
        //method 1 is not good ,we use plane::intersectwithline directly.




    }else{
        //we can use this method to calculate point ,but is no needed.
//        double camPos[3],focalPoint[3];
//        vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
//        camera->GetPosition(camPos);
//        camera->GetFocalPoint(focalPoint);
//        double nearWorldPoint[4];
//        double farWorldPoint[4];
//        double tmp[3];

//        tmp[0] = displayPos[0];
//        tmp[1] = displayPos[1];
//        tmp[2] = 0.0; // near plane

//        ren->SetDisplayPoint(tmp);
//        ren->DisplayToWorld();
//        ren->GetWorldPoint(nearWorldPoint);

//        tmp[2] = 1.0; // far plane
//        ren->SetDisplayPoint(tmp);
//        ren->DisplayToWorld();
//        ren->GetWorldPoint(farWorldPoint);

//        double normal[3];
//        double origin[3];

//        this->GetProjectionNormal(normal);
//        this->GetProjectionOrigin(origin);

//        double position[3];
//        double distance;
//        if (vtkPlane::IntersectWithLine(
//              nearWorldPoint, farWorldPoint, normal, origin, distance, position))








        //Create bounding planes for projection plane
        vtkSmartPointer<vtkPlane> boundingPlanes[4];

        boundingPlanes[0] = vtkSmartPointer<vtkPlane>::New();
        boundingPlanes[0]->SetOrigin(0.0, 1000.0, 0.0);
        boundingPlanes[0]->SetNormal(0.0, -1.0, 0.0);

        boundingPlanes[1] = vtkSmartPointer<vtkPlane>::New();
        boundingPlanes[1]->SetOrigin(0.0, -1000.0, 0.0);
        boundingPlanes[1]->SetNormal(0.0, 1.0, 0.0);

        boundingPlanes[2] = vtkSmartPointer<vtkPlane>::New();
        boundingPlanes[2]->SetOrigin(1000.0, 0.0, 0.0);
        boundingPlanes[2]->SetNormal(-1.0, 0.0, 0.0);

        boundingPlanes[3] = vtkSmartPointer<vtkPlane>::New();
        boundingPlanes[3]->SetOrigin(-1000.0, 0.0, 0.0);
        boundingPlanes[3]->SetNormal(1.0, 0.0, 0.0);

        // Create projection plane parallel platform and Z coordinate from clicked position in model
        vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
        plane->SetOrigin(0.0, 0.0, 0);
        plane->SetNormal(0.0, 0.0, 1.0);

        // Set projection and bounding planes to placer
        vtkSmartPointer<vtkBoundedPlanePointPlacer> placer = vtkSmartPointer<vtkBoundedPlanePointPlacer>::New();
        placer->SetObliquePlane(plane);
        placer->SetProjectionNormalToOblique();

        placer->AddBoundingPlane(boundingPlanes[0].Get());
        placer->AddBoundingPlane(boundingPlanes[1].Get());
        placer->AddBoundingPlane(boundingPlanes[2].Get());
        placer->AddBoundingPlane(boundingPlanes[3].Get());

        double screenPos[2];
        double worldOrient[9];
        vtkRenderWindowInteractor* rwi = this->Interactor;
        screenPos[0] = rwi->GetEventPosition()[0];
        // Compensate the y-axis flip for the picking
        screenPos[1] = rwi->GetEventPosition()[1];

        int16_t withinBounds;
        std::array<double, 3> worldPos;
        withinBounds = placer->ComputeWorldPosition(this->CurrentRenderer, screenPos, worldPos.data(), worldOrient);



        // Use initial center as the origin from which to pan

        double* obj_center = this->InteractionProp->GetCenter();

        double disp_obj_center[3], new_pick_point[4];
        double old_pick_point[4], motion_vector[3];

        this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2], disp_obj_center);

        this->ComputeDisplayToWorld(
          rwi->GetEventPosition()[0], rwi->GetEventPosition()[1], disp_obj_center[2], new_pick_point);

        this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0], rwi->GetLastEventPosition()[1],
          disp_obj_center[2], old_pick_point);

        motion_vector[0] = worldPos[0] - old_pick_point[0];
        motion_vector[1] = worldPos[1] - old_pick_point[1];
        motion_vector[2] = worldPos[2] - old_pick_point[2];
        //we need set z to origin because the models are on platform
        motion_vector[2] = 0;
        if (this->InteractionProp->GetUserMatrix() != nullptr)
        {
          vtkTransform* t = vtkTransform::New();
          t->PostMultiply();
          t->SetMatrix(this->InteractionProp->GetUserMatrix());
          t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
          this->InteractionProp->GetUserMatrix()->DeepCopy(t->GetMatrix());
          t->Delete();
        }
        else
        {
          this->InteractionProp->AddPosition(motion_vector[0], motion_vector[1], motion_vector[2]);
        }

        if (this->AutoAdjustCameraClippingRange)
        {
          this->CurrentRenderer->ResetCameraClippingRange();
        }

        rwi->Render();

    }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::Dolly()
{
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    vtkRenderWindowInteractor* rwi = this->Interactor;
    double* center = this->CurrentRenderer->GetCenter();
    int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];
    double dyf = this->MotionFactor * dy / center[1];
    this->Dolly(pow(1.1, dyf));
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::Dolly(double factor)
{
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
    if (camera->GetParallelProjection())
    {
        camera->SetParallelScale(camera->GetParallelScale() / factor);
    }
    else
    {
        camera->Dolly(factor);
        if (this->AutoAdjustCameraClippingRange)
        {
            this->CurrentRenderer->ResetCameraClippingRange();
        }
    }

    if (this->Interactor->GetLightFollowCamera())
    {
        this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

    this->Interactor->Render();
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::EnvironmentRotate()
{
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }

    vtkRenderWindowInteractor* rwi = this->Interactor;

    int dx = rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0];
    int sizeX = this->CurrentRenderer->GetRenderWindow()->GetSize()[0];

    vtkNew<vtkMatrix3x3> mat;

    double* up = this->CurrentRenderer->GetEnvironmentUp();
    double* right = this->CurrentRenderer->GetEnvironmentRight();

    double front[3];
    vtkMath::Cross(right, up, front);
    for (int i = 0; i < 3; i++)
    {
        mat->SetElement(i, 0, right[i]);
        mat->SetElement(i, 1, up[i]);
        mat->SetElement(i, 2, front[i]);
    }

    double angle = (dx / static_cast<double>(sizeX)) * this->MotionFactor;

    double c = std::cos(angle);
    double s = std::sin(angle);
    double t = 1.0 - c;

    vtkNew<vtkMatrix3x3> rot;

    rot->SetElement(0, 0, t * up[0] * up[0] + c);
    rot->SetElement(0, 1, t * up[0] * up[1] - up[2] * s);
    rot->SetElement(0, 2, t * up[0] * up[2] + up[1] * s);

    rot->SetElement(1, 0, t * up[0] * up[1] + up[2] * s);
    rot->SetElement(1, 1, t * up[1] * up[1] + c);
    rot->SetElement(1, 2, t * up[1] * up[2] - up[0] * s);

    rot->SetElement(2, 0, t * up[0] * up[2] - up[1] * s);
    rot->SetElement(2, 1, t * up[1] * up[2] + up[0] * s);
    rot->SetElement(2, 2, t * up[2] * up[2] + c);

    vtkMatrix3x3::Multiply3x3(rot, mat, mat);

    // update environment orientation
    this->CurrentRenderer->SetEnvironmentUp(
                mat->GetElement(0, 1), mat->GetElement(1, 1), mat->GetElement(2, 1));
    this->CurrentRenderer->SetEnvironmentRight(
                mat->GetElement(0, 0), mat->GetElement(1, 0), mat->GetElement(2, 0));

    rwi->Render();
}

bool vtkInteractorStyleTrackballCamera2::getIsModelSelected() const
{
    return IsModelSelected;
}

//------------------------------------------------------------------------------
void vtkInteractorStyleTrackballCamera2::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "MotionFactor: " << this->MotionFactor << "\n";
}
