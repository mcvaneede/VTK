/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestRotationalExtrusionFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .SECTION Thanks
// This test was written by Philippe P�bay, Kitware SAS 2011

#include "vtkCamera.h"
#include "vtkLineSource.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRotationalExtrusionFilter.h"
#include "vtkTestUtilities.h"

//----------------------------------------------------------------------------
int TestRotationalExtrusion( int argc, char * argv [] )
{
  // Create a line source
  vtkNew<vtkLineSource> line;
  line->SetPoint1( 0., 1., 0. );
  line->SetPoint2( 0., 1., 2. );
  line->SetResolution( 10 );

  // Create 3/4 of a cylinder by rotational extrusion
  vtkNew<vtkRotationalExtrusionFilter> lineSweeper;
  lineSweeper->SetResolution( 20 );
  lineSweeper->SetInputConnection( line->GetOutputPort() );
  lineSweeper->SetAngle( 270 );

  vtkNew<vtkPolyDataNormals> normals;
  normals->SetInputConnection( lineSweeper->GetOutputPort() );

  // Create mapper for surface representation
  vtkNew<vtkPolyDataMapper> cylMapper;
  cylMapper->SetInputConnection( normals->GetOutputPort() );
  cylMapper->SetResolveCoincidentTopologyPolygonOffsetParameters( 0, 1 );
  cylMapper->SetResolveCoincidentTopologyToPolygonOffset();

  // Create mapper for wireframe representation
  vtkNew<vtkPolyDataMapper> cylMapperW;
  cylMapperW->SetInputConnection( lineSweeper->GetOutputPort() );
  cylMapperW->SetResolveCoincidentTopologyPolygonOffsetParameters( 1, 1 );
  cylMapperW->SetResolveCoincidentTopologyToPolygonOffset();

  // Create actor for surface representation
  vtkNew<vtkActor> cylActor;
  cylActor->SetMapper( cylMapper.GetPointer() );
  cylActor->GetProperty()->SetRepresentationToSurface();
  cylActor->GetProperty()->SetInterpolationToFlat();
  cylActor->GetProperty()->SetColor( 1., 0.3882, .2784 ); // tomato

  // Create actor for wireframe representation
  vtkNew<vtkActor> cylActorW;
  cylActorW->SetMapper( cylMapperW.GetPointer() );
  cylActorW->GetProperty()->SetRepresentationToWireframe();
  cylActorW->GetProperty()->SetColor( 0., 0., 0.);
  cylActorW->GetProperty()->SetAmbient( 1. );
  cylActorW->GetProperty()->SetDiffuse( 0. );
  cylActorW->GetProperty()->SetSpecular( 0. );

  // Create a renderer, add actors to it
  vtkNew<vtkRenderer> ren1;
  ren1->AddActor( cylActor.GetPointer() );
  ren1->AddActor( cylActorW.GetPointer() );
  ren1->SetBackground( 1., 1., 1. );

  // Create a renderWindow
  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer( ren1.GetPointer() );
  renWin->SetSize( 300, 300 );
  renWin->SetMultiSamples( 0 );
  
  vtkNew<vtkCamera> camera;
  camera->SetClippingRange( 0.576398, 28.8199 );
  camera->SetFocalPoint( 0.0463079, -0.0356571, 1.01993 );
  camera->SetPosition( -2.47044, 2.39516, -3.56066 );
  camera->SetViewUp( 0.607296, -0.513537, -0.606195 );
  ren1->SetActiveCamera( camera.GetPointer() );

  // Create interactor
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow( renWin.GetPointer() );

  // Render and test
  renWin->Render();

  int retVal = vtkRegressionTestImage( renWin.GetPointer() );
  if ( retVal == vtkRegressionTester::DO_INTERACTOR)
    {
    iren->Start();
    }

  return !retVal;
}
