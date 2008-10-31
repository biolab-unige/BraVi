#include "MainWidget.h"

#include <iostream>

#include "vtkGenericMedicalImageReader.h"
#include "ReadImageDialog.h"

#include <vtkRenderWindow.h>

#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QFileDialog>
#include <QDir>

#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>

#include <vtkImageReslice.h>


#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>


#include <vtkTransform.h>



MainWidget::MainWidget ( QWidget *parent ) :
  QMainWindow (parent),
  m_RenderingWidget_volume(NULL),
  m_RenderingWidget_axial(NULL),
  m_RenderingWidget_sagittal(NULL),
  m_RenderingWidget_coronal(NULL),
  m_RenderingWidget_slice(NULL),
  isVolumeLoaded(false),
  isOrientationMarkerLoaded(false)
{
#ifdef SV_DANZ
  setWindowTitle("Inchinati dinnanzi alla potenza di seegViewer");
#else // SV_DANZ
  setWindowTitle("SeegViewer");
#endif // SV_DANZ

  QFont font("Arial", 7);
  setFont(font);

  CreateRenderingWidgets();


  for(int i=0; i<NUM_SURFACES; i++)
  {
    m_SurfaceWidget[i] = new SurfaceWidget(this, i+1);
    m_SurfaceWidget[i]->addRenderer(m_Rend_volume);
  }

  CreateLayout();
  CreateImageViewer();
  CreateMenu(); //Importante! Create menu va chiamata dopo la creazione del layout o la connessione dei segnali causa un segmentation fault

  ResetControlsVolume();
  DisableControlsVolume(true);

  RenderAll();
}


MainWidget::~MainWidget()
{
  CloseAll();

  m_ImageViewer_axial->Delete();
  m_ImageViewer_coronal->Delete();
  m_ImageViewer_sagittal->Delete();
  m_ImageViewer_slice->Delete();

  if ( m_RenderingWidget_volume )
    delete m_RenderingWidget_volume;
  m_Rend_volume->Delete();

  if ( m_RenderingWidget_axial )
    delete m_RenderingWidget_axial;

  if ( m_RenderingWidget_sagittal )
    delete m_RenderingWidget_sagittal;

  if ( m_RenderingWidget_coronal )
    delete m_RenderingWidget_coronal;

  if ( m_RenderingWidget_slice )
    delete m_RenderingWidget_slice;
}


/// Crea il menu dell'applicazione
void MainWidget::CreateMenu()
{
  m_Action_File_LoadVolume = new QAction ( tr ( "Load &Volume" ), this );
  m_Action_File_LoadVolume->setShortcut ( tr ( "Ctrl+V" ) );
  m_Action_File_LoadVolume->setStatusTip ( tr ( "Load Volume (Ctrl+V)" ) );
  connect ( m_Action_File_LoadVolume, SIGNAL ( triggered() ), this, SLOT ( loadVolume() ) );

  m_Action_File_CloseVolume = new QAction ( tr ( "&Close Volume" ), this );
  m_Action_File_CloseVolume->setShortcut ( tr ( "Ctrl+C" ) );
  m_Action_File_CloseVolume->setStatusTip ( tr ( "Close Volume (Ctrl+C)" ) );
  connect ( m_Action_File_CloseVolume, SIGNAL ( triggered() ), this, SLOT ( CloseVolume() ) );

  m_Action_File_CloseAll = new QAction ( tr ( "Close &All" ), this );
  m_Action_File_CloseAll->setShortcut ( tr ( "Ctrl+A" ) );
  m_Action_File_CloseAll->setStatusTip ( tr ( "Close All (Ctrl+A)" ) );
  connect ( m_Action_File_CloseAll, SIGNAL ( triggered() ), this, SLOT ( CloseAll() ) );

  m_Action_File_Quit = new QAction ( tr ( "&Quit" ), this );
  m_Action_File_Quit->setShortcut ( tr ( "Ctrl+Q" ) );
  m_Action_File_Quit->setStatusTip ( tr ( "Quit (Ctrl+Q)" ) );
  connect ( m_Action_File_Quit, SIGNAL ( triggered() ), this, SLOT ( fileQuit() ) );

  m_Menu_File = this->menuBar()->addMenu ( tr ( "&File" ) );
  m_Menu_File->addAction ( m_Action_File_LoadVolume );
  m_Menu_File->addAction ( m_Action_File_CloseVolume );
  m_Menu_File->addSeparator();

  for(int i=0; i<NUM_SURFACES; i++)
    m_Menu_File->addMenu(m_SurfaceWidget[i]->getMenu());

  m_Menu_File->addSeparator();

  m_Menu_File->addAction ( m_Action_File_CloseAll );
  m_Menu_File->addAction ( m_Action_File_Quit );

#ifndef SV_DICEVIEWER
  //Operations Menu
  m_Action_Operations_Filter_Convert = new QAction ( tr ( "Convert Filter" ), this);
  m_Action_Operations_Filter_Convert->setStatusTip ( tr ( "Esegui Conversione" ) );
  connect ( m_Action_Operations_Filter_Convert, SIGNAL ( triggered() ), this, SLOT ( filterConvert() ) );

  m_Action_Operations_Filter_ConvertSeries = new QAction ( tr ( "Convert Series Filter" ), this);
  m_Action_Operations_Filter_ConvertSeries->setStatusTip ( tr ( "Esegui Conversione Serie" ) );
  connect ( m_Action_Operations_Filter_ConvertSeries, SIGNAL ( triggered() ), this, SLOT ( filterConvertSeries() ) );

  m_Action_Operations_Filter_Pad = new QAction ( tr ( "Pad Filter" ), this);
  m_Action_Operations_Filter_Pad->setStatusTip ( tr ( "Esegui Padding" ) );
  connect ( m_Action_Operations_Filter_Pad, SIGNAL ( triggered() ), this, SLOT ( filterPad() ) );

  m_Action_Operations_Filter_Rescale = new QAction ( tr ( "Rescale Filter" ), this);
  m_Action_Operations_Filter_Rescale->setStatusTip ( tr ( "Esegui Rescale" ) );
  connect ( m_Action_Operations_Filter_Rescale, SIGNAL ( triggered() ), this, SLOT ( filterRescale() ) );

  m_Action_Operations_Filter_RescaleInt2UChar = new QAction ( tr ( "Rescale Int to Unsigned Char" ), this);
  m_Action_Operations_Filter_RescaleInt2UChar->setStatusTip ( tr ( "Esegui Rescale Int to Unsigned Char" ) );
  connect ( m_Action_Operations_Filter_RescaleInt2UChar, SIGNAL ( triggered() ), this, SLOT ( filterRescaleInt2UChar() ) );

  m_Action_Operations_Filter_RescaleUShort2UChar = new QAction ( tr ( "Rescale Unsigned Short to Unsigned Char" ), this);
  m_Action_Operations_Filter_RescaleUShort2UChar->setStatusTip ( tr ( "Esegui Rescale Unsigned Short to Unsigned Char" ) );
  connect ( m_Action_Operations_Filter_RescaleUShort2UChar, SIGNAL ( triggered() ), this, SLOT ( filterRescaleUShort2UChar() ) );

  m_Action_Operations_Filter_Orient = new QAction ( tr ( "Orient Filter" ), this);
  m_Action_Operations_Filter_Orient->setStatusTip ( tr ( "Esegui Orient -> RAS" ) );
  connect ( m_Action_Operations_Filter_Orient, SIGNAL ( triggered() ), this, SLOT ( filterOrient() ) );

  m_Action_Operations_Filter_Sigmoid = new QAction ( tr ( "Sigmoid Filter" ), this);
  m_Action_Operations_Filter_Sigmoid->setStatusTip ( tr ( "Esegui Sigmoid" ) );
  connect ( m_Action_Operations_Filter_Sigmoid, SIGNAL ( triggered() ), this, SLOT ( filterSigmoid() ) );

  m_Action_Operations_Filter_Derivative = new QAction ( tr ( "Derivative Filter" ), this);
  m_Action_Operations_Filter_Derivative->setStatusTip ( tr ( "Esegui Derivata Parziale" ) );
  connect ( m_Action_Operations_Filter_Derivative, SIGNAL ( triggered() ), this, SLOT ( filterDerivative() ) );

  m_Action_Operations_Filter_GradientMagnitude = new QAction ( tr ( "Gradient Magnitude Filter" ), this);
  m_Action_Operations_Filter_GradientMagnitude->setStatusTip ( tr ( "Esegui Gradient Magnitude" ) );
  connect ( m_Action_Operations_Filter_GradientMagnitude, SIGNAL ( triggered() ), this, SLOT ( filterGradientMagnitude() ) );

  m_Action_Operations_Filter_Hessian = new QAction ( tr ( "Hessian Filter" ), this);
  m_Action_Operations_Filter_Hessian->setStatusTip ( tr ( "Esegui Hessiana" ) );
  connect ( m_Action_Operations_Filter_Hessian, SIGNAL ( triggered() ), this, SLOT ( filterHessian() ) );

  m_Action_Operations_Filter_BinaryThreshold = new QAction ( tr ( "Binary Threshold Filter" ), this);
  m_Action_Operations_Filter_BinaryThreshold->setStatusTip ( tr ( "Esegui Binary Threshold" ) );
  connect ( m_Action_Operations_Filter_BinaryThreshold, SIGNAL ( triggered() ), this, SLOT ( filterBinaryThreshold() ) );

  m_Action_Operations_Filter_Maximum = new QAction ( tr ( "Maximum Filter" ), this);
  m_Action_Operations_Filter_Maximum->setStatusTip ( tr ( "Esegui Maximum" ) );
  connect ( m_Action_Operations_Filter_Maximum, SIGNAL ( triggered() ), this, SLOT ( filterMaximum() ) );

  m_Action_Operations_Filter_Minimum = new QAction ( tr ( "Minimum Filter" ), this);
  m_Action_Operations_Filter_Minimum->setStatusTip ( tr ( "Esegui Minimum" ) );
  connect ( m_Action_Operations_Filter_Minimum, SIGNAL ( triggered() ), this, SLOT ( filterMinimum() ) );

  m_Action_Operations_Filter_Test = new QAction ( tr ( "Test Filter" ), this);
  m_Action_Operations_Filter_Test->setStatusTip ( tr ( "Esegui filtro di test" ) );
  connect ( m_Action_Operations_Filter_Test, SIGNAL ( triggered() ), this, SLOT ( filterTest() ) );

  m_Menu_Operations_Filter = new QMenu ( tr ( "&Filters" ) );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Convert );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_ConvertSeries );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Pad );
  m_Menu_Operations_Filter->addSeparator();
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Rescale );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_RescaleInt2UChar );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_RescaleUShort2UChar );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Orient );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Sigmoid );
  m_Menu_Operations_Filter->addSeparator();
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Derivative );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_GradientMagnitude );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Hessian );
  m_Menu_Operations_Filter->addSeparator();
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_BinaryThreshold );
  m_Menu_Operations_Filter->addSeparator();
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Maximum );
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Minimum );
  m_Menu_Operations_Filter->addSeparator();
  m_Menu_Operations_Filter->addAction ( m_Action_Operations_Filter_Test );

  m_Action_Operations_Segmentation_connectedThreshold = new QAction ( tr ( "Connected Threshold Filter" ), this);
  m_Action_Operations_Segmentation_connectedThreshold->setStatusTip ( tr ( "Esegui Connected Threshold" ) );
  connect ( m_Action_Operations_Segmentation_connectedThreshold, SIGNAL ( triggered() ), this, SLOT ( segmentationConnectedThreshold() ) );

  m_Action_Operations_Segmentation_fastMarching = new QAction ( tr ( "Fast Marching Filter" ), this);
  m_Action_Operations_Segmentation_fastMarching->setStatusTip ( tr ( "Esegui Fast Marching" ) );
  connect ( m_Action_Operations_Segmentation_fastMarching, SIGNAL ( triggered() ), this, SLOT ( segmentationFastMarching() ) );

  m_Action_Operations_Segmentation_geodesicActiveContour = new QAction ( tr ( "Geodesic Active Contour Filter" ), this);
  m_Action_Operations_Segmentation_geodesicActiveContour->setStatusTip ( tr ( "Esegui Geodesic Active Contour" ) );
  connect ( m_Action_Operations_Segmentation_geodesicActiveContour, SIGNAL ( triggered() ), this, SLOT ( segmentationGeodesicActiveContour() ) );

  m_Action_Operations_Segmentation_geodesicActiveContourFull = new QAction ( tr ( "Geodesic Active Contour Filter (Full)" ), this);
  m_Action_Operations_Segmentation_geodesicActiveContourFull->setStatusTip ( tr ( "Esegui Geodesic Active Contour (Full)" ) );
  connect ( m_Action_Operations_Segmentation_geodesicActiveContourFull, SIGNAL ( triggered() ), this, SLOT ( segmentationGeodesicActiveContourFull() ) );

  m_Action_Operations_Segmentation_Bet = new QAction ( tr ( "Brain Extraction Tool" ), this);
  m_Action_Operations_Segmentation_Bet->setStatusTip ( tr ( "Esegui BET (Brain Extraction Tool)" ) );
  connect ( m_Action_Operations_Segmentation_Bet, SIGNAL ( triggered() ), this, SLOT ( segmentationBet() ) );


  m_Menu_Operations_Segmentation = new QMenu (tr ( "&Segmentations" ) );
  m_Menu_Operations_Segmentation->addAction ( m_Action_Operations_Segmentation_connectedThreshold );
  m_Menu_Operations_Segmentation->addSeparator();
  m_Menu_Operations_Segmentation->addAction ( m_Action_Operations_Segmentation_fastMarching );
  m_Menu_Operations_Segmentation->addAction ( m_Action_Operations_Segmentation_geodesicActiveContour );
  m_Menu_Operations_Segmentation->addAction ( m_Action_Operations_Segmentation_geodesicActiveContourFull );
  m_Menu_Operations_Segmentation->addSeparator();
  m_Menu_Operations_Segmentation->addAction ( m_Action_Operations_Segmentation_Bet );


  m_Action_Operations_Skeletonization_BinaryThinning = new QAction ( tr ( "Binary Thinning Filter" ), this);
  m_Action_Operations_Skeletonization_BinaryThinning->setStatusTip ( tr ( "Esegui Binary Thinning" ) );
  connect ( m_Action_Operations_Skeletonization_BinaryThinning, SIGNAL ( triggered() ), this, SLOT ( skeletonizationBinaryThinning() ) );

  m_Action_Operations_Skeletonization_BinaryPruning = new QAction ( tr ( "Binary Pruning Filter" ), this);
  m_Action_Operations_Skeletonization_BinaryPruning->setStatusTip ( tr ( "Esegui Binary Pruning" ) );
  connect ( m_Action_Operations_Skeletonization_BinaryPruning, SIGNAL ( triggered() ), this, SLOT ( skeletonizationBinaryPruning() ) );

  m_Action_Operations_Skeletonization_BinaryErode = new QAction ( tr ( "Binary Erode Filter" ), this);
  m_Action_Operations_Skeletonization_BinaryErode->setStatusTip ( tr ( "Esegui Binary Erode" ) );
  connect ( m_Action_Operations_Skeletonization_BinaryErode, SIGNAL ( triggered() ), this, SLOT ( skeletonizationBinaryErode() ) );

  m_Action_Operations_Skeletonization_BinaryDilate = new QAction ( tr ( "Binary Dilate Filter" ), this);
  m_Action_Operations_Skeletonization_BinaryDilate->setStatusTip ( tr ( "Esegui Binary Dilate" ) );
  connect ( m_Action_Operations_Skeletonization_BinaryDilate, SIGNAL ( triggered() ), this, SLOT ( skeletonizationBinaryDilate() ) );

  m_Action_Operations_Skeletonization_DanielssonDistanceMap = new QAction ( tr ( "Danielsson Distance Map Filter" ), this);
  m_Action_Operations_Skeletonization_DanielssonDistanceMap->setStatusTip ( tr ( "Esegui Danielsson Distance Map" ) );
  connect ( m_Action_Operations_Skeletonization_DanielssonDistanceMap, SIGNAL ( triggered() ), this, SLOT ( skeletonizationDanielssonDistanceMap() ) );

  m_Action_Operations_Skeletonization_electrodeAxisExtraction = new QAction ( tr ( "Electrode Axis Extraction Filter" ), this);
  m_Action_Operations_Skeletonization_electrodeAxisExtraction->setStatusTip ( tr ( "Esegui Electrode Axis Extraction" ) );
  connect ( m_Action_Operations_Skeletonization_electrodeAxisExtraction, SIGNAL ( triggered() ), this, SLOT ( skeletonizationElectrodeAxisExtraction() ) );

  m_Action_Operations_Skeletonization_electrodeAxisExtraction2 = new QAction ( tr ( "Electrode Axis Extraction Filter 2" ), this);
  m_Action_Operations_Skeletonization_electrodeAxisExtraction2->setStatusTip ( tr ( "Esegui Electrode Axis Extraction 2" ) );
  connect ( m_Action_Operations_Skeletonization_electrodeAxisExtraction2, SIGNAL ( triggered() ), this, SLOT ( skeletonizationElectrodeAxisExtraction2() ) );

  m_Action_Operations_Skeletonization_electrodeAxisExtraction3 = new QAction ( tr ( "Electrode Axis Extraction Filter 3" ), this);
  m_Action_Operations_Skeletonization_electrodeAxisExtraction3->setStatusTip ( tr ( "Esegui Electrode Axis Extraction 3" ) );
  connect ( m_Action_Operations_Skeletonization_electrodeAxisExtraction3, SIGNAL ( triggered() ), this, SLOT ( skeletonizationElectrodeAxisExtraction3() ) );

  m_Action_Operations_Skeletonization_skelInsert = new QAction ( tr ( "Skel Insert Filter" ), this);
  m_Action_Operations_Skeletonization_skelInsert->setStatusTip ( tr ( "Skel Insert Filter" ) );
  connect ( m_Action_Operations_Skeletonization_skelInsert, SIGNAL ( triggered() ), this, SLOT ( skelInsert() ) );

  m_Menu_Operations_Skeletonization = new QMenu (tr ( "S&keletonization" ) );
  m_Menu_Operations_Skeletonization->addAction ( m_Action_Operations_Skeletonization_BinaryThinning );
  m_Menu_Operations_Skeletonization->addAction ( m_Action_Operations_Skeletonization_BinaryPruning );
  m_Menu_Operations_Skeletonization->addAction ( m_Action_Operations_Skeletonization_BinaryErode );
  m_Menu_Operations_Skeletonization->addAction ( m_Action_Operations_Skeletonization_BinaryDilate );
  m_Menu_Operations_Skeletonization->addAction ( m_Action_Operations_Skeletonization_DanielssonDistanceMap );
  m_Menu_Operations_Skeletonization->addAction ( m_Action_Operations_Skeletonization_electrodeAxisExtraction );
  m_Menu_Operations_Skeletonization->addAction ( m_Action_Operations_Skeletonization_electrodeAxisExtraction2 );
  m_Menu_Operations_Skeletonization->addAction ( m_Action_Operations_Skeletonization_electrodeAxisExtraction3 );
  m_Menu_Operations_Skeletonization->addAction ( m_Action_Operations_Skeletonization_skelInsert );

  m_Action_Operations_Registration_ApplyRigidTransform = new QAction ( tr ( "Apply Rigid Transform Filter" ), this);
  m_Action_Operations_Registration_ApplyRigidTransform->setStatusTip ( tr ( "Applica una trasformazione rigida Filter" ) );
  connect ( m_Action_Operations_Registration_ApplyRigidTransform, SIGNAL ( triggered() ), this, SLOT ( registrationApplyRigidTransform() ) );

  m_Menu_Operations_Registration = new QMenu (tr ( "R&egistration" ) );
  m_Menu_Operations_Registration->addAction ( m_Action_Operations_Registration_ApplyRigidTransform );


  m_Menu_Operations = this->menuBar()->addMenu ( tr ( "&Operations" ) );

  m_Menu_Operations->addMenu (m_Menu_Operations_Filter);
  m_Menu_Operations->addMenu (m_Menu_Operations_Segmentation);
  m_Menu_Operations->addMenu (m_Menu_Operations_Skeletonization);
  m_Menu_Operations->addMenu (m_Menu_Operations_Registration);
#endif // SV_DICEVIEWER


#ifndef SV_SIMPLEINTERFACE
  // Window Menu
  m_Menu_Window_Volume = new QAction ( tr ( "&Volume Window"), this);
  m_Menu_Window_Volume->setStatusTip ( tr ( "Show Volume Window" ) );
  connect ( m_Menu_Window_Volume, SIGNAL ( triggered() ), m_VolumeMdiSubWindow, SLOT ( show() ) );

  m_Menu_Window_Slices = new QAction ( tr ( "&Slices Window"), this);
  m_Menu_Window_Slices->setStatusTip ( tr ( "Show Slices Window" ) );
  connect ( m_Menu_Window_Slices, SIGNAL ( triggered() ),  m_SlicesMdiSubWindow, SLOT ( show() ) );

  m_Menu_Window_Tile = new QAction ( tr ( "&Tile Windows"), this);
  m_Menu_Window_Tile->setStatusTip ( tr ( "Tile Windows" ) );
  connect ( m_Menu_Window_Tile, SIGNAL ( triggered() ), m_MdiArea, SLOT ( tileSubWindows() ) );

  m_Menu_Window_Cascade = new QAction ( tr ( "&Cascade Windows"), this);
  m_Menu_Window_Cascade->setStatusTip ( tr ( "Cascade Windows" ) );
  connect ( m_Menu_Window_Cascade, SIGNAL ( triggered() ), m_MdiArea, SLOT ( cascadeSubWindows() ) );

  m_Menu_Window = this->menuBar()->addMenu (tr( "&Window" ));
  m_Menu_Window->addAction(m_Menu_Window_Volume);

  m_Menu_Window->addAction(m_Menu_Window_Slices);
  m_Menu_Window->addSeparator();
  m_Menu_Window->addAction(m_Menu_Window_Tile);
  m_Menu_Window->addAction(m_Menu_Window_Cascade);
  m_Menu_Window->addSeparator();

  m_Menu_Window->addAction(m_orthoOptionsDockWidget->toggleViewAction());
  m_Menu_Window->addAction(m_sliceOptionsDockWidget->toggleViewAction());

  for(int i=0; i<NUM_SURFACES; i++)
    m_Menu_Window->addAction(m_surfaceOptionsDockWidget[i]->toggleViewAction());

#endif // SV_SIMPLEINTERFACE


#ifndef SV_DICEVIEWER
  // Help Menu
  m_Menu_Help_Handbook = new QAction(tr("&Handbook"), this);
  m_Menu_Help_Handbook->setStatusTip(tr("Show handbook"));
  connect(m_Menu_Help_Handbook, SIGNAL(triggered()), this, SLOT(Handbook()));

  m_Menu_Help_AboutQt = new QAction(tr("About &Qt"), this);
  m_Menu_Help_AboutQt->setStatusTip(tr("Show the Qt library's about box"));
  connect(m_Menu_Help_AboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  m_Menu_Help_About = new QAction(tr("&About"), this);
  m_Menu_Help_About->setStatusTip(tr("Show about box"));
  connect(m_Menu_Help_About, SIGNAL(triggered()), this, SLOT(About()));

  m_Menu_Help = this->menuBar()->addMenu (tr( "&Help" ));
  m_Menu_Help->addAction(m_Menu_Help_Handbook);
  m_Menu_Help->addSeparator();

  m_Menu_Help->addAction(m_Menu_Help_AboutQt);
  m_Menu_Help->addAction(m_Menu_Help_About);
#endif // SV_DICEVIEWER



}

void MainWidget::SetCameraVolumeFront()
{
  vtkCamera* camera = m_Rend_volume->GetActiveCamera();
  camera->SetPosition(0,1,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);

  m_Rend_volume->ResetCameraClippingRange();
  m_Rend_volume->ResetCamera();
  m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::SetCameraVolumeBack()
{
  vtkCamera* camera = m_Rend_volume->GetActiveCamera();
  camera->SetPosition(0,-1,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);

  m_Rend_volume->ResetCameraClippingRange();
  m_Rend_volume->ResetCamera();
  m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::SetCameraVolumeLeft()
{
  vtkCamera* camera = m_Rend_volume->GetActiveCamera();
  camera->SetPosition(-1,0,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);

  m_Rend_volume->ResetCameraClippingRange();
  m_Rend_volume->ResetCamera();
  m_Rend_volume->Render();
  m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::SetCameraVolumeRight()
{
  vtkCamera* camera = m_Rend_volume->GetActiveCamera();
  camera->SetPosition(1,0,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);

  m_Rend_volume->ResetCameraClippingRange();
  m_Rend_volume->ResetCamera();
  m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::SetCameraVolumeTop()
{
  vtkCamera* camera = m_Rend_volume->GetActiveCamera();
  camera->SetPosition(0,0,1);
  camera->SetViewUp(0,1,0);
  camera->SetFocalPoint(0,0,0);

  m_Rend_volume->ResetCameraClippingRange();
  m_Rend_volume->ResetCamera();
  m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::SetCameraVolumeBottom()
{
  vtkCamera* camera = m_Rend_volume->GetActiveCamera();
  camera->SetPosition(0,0,-1);
  camera->SetViewUp(0,1,0);
  camera->SetFocalPoint(0,0,0);

  m_Rend_volume->ResetCameraClippingRange();
  m_Rend_volume->ResetCamera();
  m_RenderingWidget_volume->GetRenderWindow()->Render();
}


/// \brief Crea i rendering widgets
void MainWidget::CreateRenderingWidgets()
{
    m_RenderingWidget_volume = new QVTKSquareWidget;
    m_RenderingWidget_volume->setPalette ( QPalette ( QColor ( Qt::black ) ) );
    m_RenderingWidget_volume->setMinimumSize( 300, 300 );
    m_Rend_volume = vtkRenderer::New();

//     vtkCamera* camera = m_Rend_volume->MakeCamera();
//     camera->SetPosition(0,1,0);
//     camera->SetViewUp(0,0,1);
//     camera->SetFocalPoint(0,0,0);
// 
//     m_Rend_volume->SetActiveCamera(camera);
// //    m_Rend_volume->ResetCamera();
// //    m_Rend_volume->ResetCameraClippingRange();
//     camera->Delete();

    m_RenderingWidget_volume->GetRenderWindow()->AddRenderer ( m_Rend_volume );
/*
        double bounds[6];
        double center[3];
        vtkCamera* camera = m_RenderingWidget_volume->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
        double pos[3];
        camera->GetPosition(pos);
        std::cerr << "POSITION: [" << pos[0] << ", " << pos[1] << ", " << pos[2] << "]" << std::endl;
        std::cerr << "CENTER:   [" << center[0] << ", " << center[1] << ", " << center[2] << "]" << std::endl;

        camera->SetPosition(center[0], -center[1]*5, center[2]);
        camera->OrthogonalizeViewUp();
        m_Volume3DActor_volume->GetBounds(bounds);

        center[0] = (bounds[0] + bounds[1])/2.0;
        center[1] = (bounds[2] + bounds[3])/2.0;
        center[2] = (bounds[4] + bounds[5])/2.0;
*/

    m_RenderingWidget_axial = new QVTKSquareWidget;
    m_RenderingWidget_axial->setPalette ( QPalette ( QColor ( Qt::black ) ) );

    m_RenderingWidget_sagittal = new QVTKSquareWidget;
    m_RenderingWidget_sagittal->setPalette ( QPalette ( QColor ( Qt::black ) ) );

    m_RenderingWidget_coronal = new QVTKSquareWidget;
    m_RenderingWidget_coronal->setPalette ( QPalette ( QColor ( Qt::black ) ) );

    m_RenderingWidget_slice = new QVTKSquareWidget;
    m_RenderingWidget_slice->setPalette ( QPalette ( QColor ( Qt::black ) ) );
}

/// \brief Crea il layout con i controlli per la proiezione assiale
void MainWidget::CreateAxialControls()
{
    // Proiezione assiale
    m_Slider_axial = new QSlider ( Qt::Horizontal );
    m_Slider_axial->setSingleStep ( 1 );
    m_Slider_axial->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_SpinBox_axial = new QSpinBox ();
    m_SpinBox_axial->setSingleStep ( 1 );
    m_SpinBox_axial->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_HBox_axial = new QHBoxLayout;
    m_HBox_axial->setContentsMargins(0,0,0,0);
    m_HBox_axial->setSpacing(2);
    m_HBox_axial->addWidget ( m_Slider_axial );
    m_HBox_axial->addWidget ( m_SpinBox_axial );
    m_ComboBox_axial = new QComboBox( this );
    m_ComboBox_axial->addItem( tr("No") );
    m_ComboBox_axial->addItem( tr("Box") );
    m_ComboBox_axial->addItem( tr("View Slice") );
    m_ComboBox_axial->addItem( tr("Crop +") );
    m_ComboBox_axial->addItem( tr("Crop -") );

    m_Layout_controls_axial = new QVBoxLayout;
    m_Layout_controls_axial->setContentsMargins(0,0,0,0);
    m_Layout_controls_axial->setSpacing(1);
    m_Layout_controls_axial->addLayout ( m_HBox_axial );
    m_Layout_controls_axial->addWidget ( m_ComboBox_axial );

    m_GroupBox_controls_axial = new QGroupBox( tr("Axial Controls") );
    m_GroupBox_controls_axial->setLayout(m_Layout_controls_axial);
}

/// \brief Crea il layout con i controlli per la proiezione sagittale
void MainWidget::CreateSagittalControls()
{
    // Proiezione sagittale
    m_Slider_sagittal = new QSlider ( Qt::Horizontal );
    m_Slider_sagittal->setSingleStep ( 1 );
    m_Slider_sagittal->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_SpinBox_sagittal = new QSpinBox ();
    m_SpinBox_sagittal->setSingleStep ( 1 );
    m_SpinBox_sagittal->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_HBox_sagittal = new QHBoxLayout;
    m_HBox_sagittal->setContentsMargins(0,0,0,0);
    m_HBox_sagittal->setSpacing(2);
    m_HBox_sagittal->addWidget ( m_Slider_sagittal );
    m_HBox_sagittal->addWidget ( m_SpinBox_sagittal );
    m_ComboBox_sagittal = new QComboBox( this );
    m_ComboBox_sagittal->addItem( tr("No") );
    m_ComboBox_sagittal->addItem( tr("Box") );
    m_ComboBox_sagittal->addItem( tr("View Slice") );
    m_ComboBox_sagittal->addItem( tr("Crop +") );
    m_ComboBox_sagittal->addItem( tr("Crop -") );

    m_Layout_controls_sagittal = new QVBoxLayout;
    m_Layout_controls_sagittal->setContentsMargins(0,0,0,0);
    m_Layout_controls_sagittal->setSpacing(1);
    m_Layout_controls_sagittal->addLayout ( m_HBox_sagittal );
    m_Layout_controls_sagittal->addWidget ( m_ComboBox_sagittal );

    m_GroupBox_controls_sagittal = new QGroupBox( tr("Sagittal Controls") );
    m_GroupBox_controls_sagittal->setLayout(m_Layout_controls_sagittal);
}

/// \brief Crea il layout con i controlli per la proiezione coronale
void MainWidget::CreateCoronalControls()
{
    // Proiezione coronale
    m_Slider_coronal = new QSlider ( Qt::Horizontal );
    m_Slider_coronal->setSingleStep ( 1 );
    m_Slider_coronal->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_SpinBox_coronal = new QSpinBox ();
    m_SpinBox_coronal->setSingleStep ( 1 );
    m_SpinBox_coronal->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_HBox_coronal = new QHBoxLayout;
    m_HBox_coronal->setContentsMargins(0,0,0,0);
    m_HBox_coronal->setSpacing(2);
    m_HBox_coronal->addWidget ( m_Slider_coronal );
    m_HBox_coronal->addWidget ( m_SpinBox_coronal );
    m_ComboBox_coronal = new QComboBox( this );
    m_ComboBox_coronal->addItem( tr("No") );
    m_ComboBox_coronal->addItem( tr("Box") );
    m_ComboBox_coronal->addItem( tr("View Slice") );
    m_ComboBox_coronal->addItem( tr("Crop +") );
    m_ComboBox_coronal->addItem( tr("Crop -") );

    m_Layout_controls_coronal = new QVBoxLayout;
    m_Layout_controls_coronal->setContentsMargins(0,0,0,0);
    m_Layout_controls_coronal->setSpacing(1);
    m_Layout_controls_coronal->addLayout ( m_HBox_coronal );
    m_Layout_controls_coronal->addWidget ( m_ComboBox_coronal );

    m_GroupBox_controls_coronal = new QGroupBox( tr("Coronal Controls") );
    m_GroupBox_controls_coronal->setLayout(m_Layout_controls_coronal);
}

/// \brief Crea il layout con i controlli per la proiezione generica
void MainWidget::CreateSliceControls()
{
    // Proiezione qualsiasi
    m_Label_slice_X = new QLabel("X");
    m_Slider_slice_X = new QSlider ( Qt::Horizontal );
    m_Slider_slice_X->setSingleStep ( 1 );
    m_Slider_slice_X->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_SpinBox_slice_X = new QSpinBox();
    m_SpinBox_slice_X->setSingleStep ( 1 );
    m_SpinBox_slice_X->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_Label_slice_Y = new QLabel("Y");
    m_Slider_slice_Y = new QSlider ( Qt::Horizontal );
    m_Slider_slice_Y->setSingleStep ( 1 );
    m_Slider_slice_Y->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_SpinBox_slice_Y = new QSpinBox ();
    m_SpinBox_slice_Y->setSingleStep ( 1 );
    m_SpinBox_slice_Y->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_Label_slice_Z = new QLabel("Z");
    m_Slider_slice_Z = new QSlider ( Qt::Horizontal );
    m_Slider_slice_Z->setSingleStep ( 1 );
    m_Slider_slice_Z->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_SpinBox_slice_Z = new QSpinBox ();
    m_SpinBox_slice_Z->setSingleStep ( 1 );
    m_SpinBox_slice_Z->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );

    m_Label_slice_alpha = new QLabel("alpha");
    m_Slider_slice_alpha = new QSlider ( Qt::Horizontal );
    m_Slider_slice_alpha->setSingleStep ( 1 );
    m_Slider_slice_alpha->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_SpinBox_slice_alpha = new QSpinBox ();
    m_SpinBox_slice_alpha->setSingleStep ( 1 );
    m_SpinBox_slice_alpha->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_Label_slice_beta = new QLabel("beta");
    m_Slider_slice_beta = new QSlider ( Qt::Horizontal );
    m_Slider_slice_beta->setSingleStep ( 1 );
    m_Slider_slice_beta->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_SpinBox_slice_beta = new QSpinBox ();
    m_SpinBox_slice_beta->setSingleStep ( 1 );
    m_SpinBox_slice_beta->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_Label_slice_gamma = new QLabel("gamma");
    m_Slider_slice_gamma = new QSlider ( Qt::Horizontal );
    m_Slider_slice_gamma->setSingleStep ( 1 );
    m_Slider_slice_gamma->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );
    m_SpinBox_slice_gamma = new QSpinBox ( );
    m_SpinBox_slice_gamma->setSingleStep ( 1 );
    m_SpinBox_slice_gamma->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );



    m_ComboBox_slice = new QComboBox( this );
    m_ComboBox_slice->addItem( tr("No") );
    m_ComboBox_slice->addItem( tr("Box") );
    m_ComboBox_slice->addItem( tr("View Slice") );

    m_Layout_controls_slice = new QGridLayout();
    m_Layout_controls_slice->setContentsMargins(0,0,0,0);
    m_Layout_controls_slice->setSpacing(2);
    m_Layout_controls_slice->setAlignment ( Qt::AlignTop | Qt::AlignLeft );
    m_Layout_controls_slice->setOriginCorner ( Qt::TopLeftCorner );

    m_Layout_controls_slice->addWidget ( m_Label_slice_X,         0, 0, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Slider_slice_X,        0, 1, 1, 1);
    m_Layout_controls_slice->addWidget ( m_SpinBox_slice_X,       0, 2, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Label_slice_Y,         1, 0, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Slider_slice_Y,        1, 1, 1, 1);
    m_Layout_controls_slice->addWidget ( m_SpinBox_slice_Y,       1, 2, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Label_slice_Z,         2, 0, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Slider_slice_Z,        2, 1, 1, 1);
    m_Layout_controls_slice->addWidget ( m_SpinBox_slice_Z,       2, 2, 1, 1);
#ifdef SV_SLICECONTROLS_2COL
    m_Layout_controls_slice->addWidget ( m_Label_slice_alpha,     0, 3, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Slider_slice_alpha,    0, 4, 1, 1);
    m_Layout_controls_slice->addWidget ( m_SpinBox_slice_alpha,   0, 5, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Label_slice_beta,      1, 3, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Slider_slice_beta,     1, 4, 1, 1);
    m_Layout_controls_slice->addWidget ( m_SpinBox_slice_beta,    1, 5, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Label_slice_gamma,     2, 3, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Slider_slice_gamma,    2, 4, 1, 1);
    m_Layout_controls_slice->addWidget ( m_SpinBox_slice_gamma,   2, 5, 1, 1);
    m_Layout_controls_slice->addWidget ( m_ComboBox_slice,        3, 0, 1, 6);
#else // SV_SLICECONTROLS_2COL
    m_Layout_controls_slice->addWidget ( m_Label_slice_alpha,     3, 0, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Slider_slice_alpha,    3, 1, 1, 1);
    m_Layout_controls_slice->addWidget ( m_SpinBox_slice_alpha,   3, 2, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Label_slice_beta,      4, 0, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Slider_slice_beta,     4, 1, 1, 1);
    m_Layout_controls_slice->addWidget ( m_SpinBox_slice_beta,    4, 2, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Label_slice_gamma,     5, 0, 1, 1);
    m_Layout_controls_slice->addWidget ( m_Slider_slice_gamma,    5, 1, 1, 1);
    m_Layout_controls_slice->addWidget ( m_SpinBox_slice_gamma,   5, 2, 1, 1);
    m_Layout_controls_slice->addWidget ( m_ComboBox_slice,        6, 0, 1, 3);
#endif // SV_SLICECONTROLS_2COL


    m_GroupBox_controls_slice = new QGroupBox( tr("Slice Controls") );
    m_GroupBox_controls_slice->setLayout(m_Layout_controls_slice);
}


/// \brief Crea i widget e sistema il tutto dentro la finestra
void MainWidget::CreateLayout()
{
  // Volume
  m_Layout_view_volume = new QVBoxLayout;
  m_Layout_view_volume->setContentsMargins(0,0,0,0);
  m_Layout_view_volume->setSpacing(1);
  m_Layout_view_volume->addWidget ( m_RenderingWidget_volume, Qt::AlignHCenter );
  m_GroupBox_view_volume = new QGroupBox( tr("3D View") );
  m_GroupBox_view_volume->setLayout(m_Layout_view_volume);

  // Axial
  CreateAxialControls();

  m_Layout_view_axial = new QVBoxLayout;
  m_Layout_view_axial->setContentsMargins(0,0,0,0);
  m_Layout_view_axial->setSpacing(1);
  m_Layout_view_axial->addWidget ( m_RenderingWidget_axial, Qt::AlignHCenter );

#ifdef SV_NEUROLOGICALCONVENTION
  m_GroupBox_view_axial = new QGroupBox( tr("Axial View (Neurological Conv.)") );
#else // SV_NEUROLOGICALCONVENTION
  m_GroupBox_view_axial = new QGroupBox( tr("Axial View (Radiological Conv.)") );
#endif

  m_GroupBox_view_axial->setLayout(m_Layout_view_axial);


  // Sagittal
  CreateSagittalControls();

  m_Layout_view_sagittal = new QVBoxLayout;
  m_Layout_view_sagittal->setContentsMargins(0,0,0,0);
  m_Layout_view_sagittal->setSpacing(1);
  m_Layout_view_sagittal->addWidget ( m_RenderingWidget_sagittal, Qt::AlignHCenter );

  m_GroupBox_view_sagittal = new QGroupBox( tr("Sagittal View") );

  m_GroupBox_view_sagittal->setLayout(m_Layout_view_sagittal);


  // Coronal
  CreateCoronalControls();

  m_Layout_view_coronal = new QVBoxLayout;
  m_Layout_view_coronal->setContentsMargins(0,0,0,0);
  m_Layout_view_coronal->setSpacing(1);
  m_Layout_view_coronal->addWidget ( m_RenderingWidget_coronal, Qt::AlignHCenter);
#ifdef SV_NEUROLOGICALCONVENTION
  m_GroupBox_view_coronal = new QGroupBox( tr("Coronal View (Neurological Conv.)") );
#else // SV_NEUROLOGICALCONVENTION
  m_GroupBox_view_coronal = new QGroupBox( tr("Coronal View (Radiological Conv.)") );
#endif
  m_GroupBox_view_coronal->setLayout(m_Layout_view_coronal);

  // Ortho controls
#ifdef SV_ORTHOCONTROLS_3COL
  m_Layout_controls_ortho = new QHBoxLayout();
#else // SV_ORTHOCONTROLS_3COL
  m_Layout_controls_ortho = new QVBoxLayout();
#endif // SV_ORTHOCONTROLS_3COL

  m_Layout_controls_ortho->setContentsMargins(0,0,0,0);
  m_Layout_controls_ortho->setSpacing(1);

  m_Layout_controls_ortho->addWidget(m_GroupBox_controls_axial);
  m_Layout_controls_ortho->addWidget(m_GroupBox_controls_sagittal);
  m_Layout_controls_ortho->addWidget(m_GroupBox_controls_coronal);

  m_Widget_controls_ortho = new QWidget();
  m_Widget_controls_ortho->setBackgroundRole ( QPalette::Window );
  m_Widget_controls_ortho->setLayout(m_Layout_controls_ortho);

  // Slice
  CreateSliceControls();

  m_Layout_view_slice = new QVBoxLayout();
  m_Layout_view_slice->setContentsMargins(0,0,0,0);
  m_Layout_view_slice->setSpacing(1);

  m_Layout_view_slice->addWidget(m_RenderingWidget_slice);

  m_GroupBox_view_slice = new QGroupBox ( tr("Slice View") );
  m_GroupBox_view_slice->setLayout(m_Layout_view_slice);

  // Slices
  m_Layout_view_allslices = new QGridLayout();
  m_Layout_view_allslices->setAlignment ( Qt::AlignTop | Qt::AlignLeft );
  m_Layout_view_allslices->setOriginCorner ( Qt::TopLeftCorner );
  m_Layout_view_allslices->setContentsMargins(0,0,0,0);
  m_Layout_view_allslices->setSpacing(1);
  m_Layout_view_allslices->addWidget ( m_GroupBox_view_axial,    0, 0, 1, 1 );
  m_Layout_view_allslices->addWidget ( m_GroupBox_view_sagittal, 0, 1, 1, 1 );
  m_Layout_view_allslices->addWidget ( m_GroupBox_view_coronal,  1, 0, 1, 1 );
  m_Layout_view_allslices->addWidget ( m_GroupBox_view_slice,    1, 1, 1, 1 );
  m_Widget_allslices = new QWidget();
  m_Widget_allslices->setBackgroundRole ( QPalette::Window );
  m_Widget_allslices->setLayout( m_Layout_view_allslices );

#ifndef SV_SIMPLEINTERFACE
  // MdiArea
  m_MdiArea = new QMdiArea;
  setCentralWidget( m_MdiArea );

  m_SlicesMdiSubWindow =  new QMdiSubWindow();
  m_SlicesMdiSubWindow->setWidget(m_Widget_allslices);
  m_SlicesMdiSubWindow->setWindowTitle("Slices");
  m_MdiArea->addSubWindow(m_SlicesMdiSubWindow);

  m_VolumeMdiSubWindow =  new QMdiSubWindow();
  m_VolumeMdiSubWindow->setWidget(m_GroupBox_view_volume);
  m_VolumeMdiSubWindow->setWindowTitle("Volume");
  m_MdiArea->addSubWindow(m_VolumeMdiSubWindow);

//    m_MdiArea->tileSubWindows();

  // Dock widget Ortho Slices
  m_orthoOptionsDockWidget = new QDockWidget( tr("Ortho Slices") );
  m_orthoOptionsDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  m_orthoOptionsDockWidget->setWidget(m_Widget_controls_ortho);

  addDockWidget(Qt::BottomDockWidgetArea, m_orthoOptionsDockWidget);

  // Dock widget Slices
  m_sliceOptionsDockWidget = new QDockWidget( tr("Generic Slice") );
  m_sliceOptionsDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  m_sliceOptionsDockWidget->setWidget(m_GroupBox_controls_slice);

  addDockWidget(Qt::BottomDockWidgetArea, m_sliceOptionsDockWidget);

  // Dock widgets Surface Options
  for(int i=0; i<NUM_SURFACES; i++)
  {
    m_surfaceOptionsDockWidget[i] = new QDockWidget( tr("Surface Options") );
    m_surfaceOptionsDockWidget[i]->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    m_surfaceOptionsDockWidget[i]->setWidget(m_SurfaceWidget[i]);

    addDockWidget(Qt::BottomDockWidgetArea, m_surfaceOptionsDockWidget[i]);
  }

  for (int i=NUM_SURFACES-1; i>0; i--)
    tabifyDockWidget(m_surfaceOptionsDockWidget[i], m_surface2OptionsDockWidget[i-1]);
  tabifyDockWidget(m_surfaceOptionsDockWidget[0], m_sliceOptionsDockWidget);
  tabifyDockWidget(m_sliceOptionsDockWidget, m_orthoOptionsDockWidget);


#else // SV_SIMPLEINTERFACE
  m_Layout_main = new QGridLayout();
  m_Layout_main->setAlignment ( Qt::AlignTop | Qt::AlignLeft );
  m_Layout_main->setOriginCorner ( Qt::TopLeftCorner );
  m_Layout_main->setContentsMargins(0,0,0,0);
  m_Layout_main->setSpacing(2);

  m_Layout_main->addWidget ( m_GroupBox_view_volume,       0,              0, 1,              1 );
  m_Layout_main->addWidget ( m_Widget_allslices,           0,              1, 1+NUM_SURFACES, 1 );
  for(int i=0; i<NUM_SURFACES; i++)
    m_Layout_main->addWidget ( m_SurfaceWidget[i],         i+1,            0, 1,              1 );
  m_Layout_main->addWidget ( m_Widget_controls_ortho,      1+NUM_SURFACES, 1, 1,              1 );
  m_Layout_main->addWidget ( m_GroupBox_controls_slice,    1+NUM_SURFACES, 0, 1,              1 );


  m_Layout_main->setColumnStretch(0,2);
  m_Layout_main->setColumnStretch(1,3);
  m_Layout_main->setRowStretch(0,1);


  m_Widget_main = new QWidget();
  m_Widget_main->setBackgroundRole ( QPalette::Window );
  m_Widget_main->setLayout(m_Layout_main);
  setCentralWidget( m_Widget_main );


#endif // SV_SIMPLEINTERFACE


  statusBar()->showMessage("");

#ifdef SV_DANZ
  QLabel* danz = new QLabel("Danz" );
  QFont danzFont("Helvetica", 6, QFont::StyleItalic);
  danz->setFont(danzFont);
  danz->setAlignment(Qt::AlignRight|Qt::AlignBottom);
  danz->setDisabled(true);
  statusBar()->addPermanentWidget ( danz );
#endif //SV_DANZ
}

void MainWidget::CreateImageViewer()
{
  m_ImageViewer_axial = vtkImageViewer2::New();
  m_ImageViewer_axial->SetSize ( 180,180 );

  m_ImageViewer_sagittal = vtkImageViewer2::New();
  m_ImageViewer_sagittal->SetSize ( 180,180 );

  m_ImageViewer_coronal = vtkImageViewer2::New();
  m_ImageViewer_coronal->SetSize ( 180,180 );

  m_ImageViewer_slice = vtkImageViewer2::New();
  m_ImageViewer_slice->SetSize ( 180,180 );
}



/// \brief Renders all windows
void MainWidget::RenderAll()
{
  m_RenderingWidget_volume->GetRenderWindow()->Render();
  m_RenderingWidget_axial->GetRenderWindow()->Render();
  m_RenderingWidget_sagittal->GetRenderWindow()->Render();
  m_RenderingWidget_coronal->GetRenderWindow()->Render();
  m_RenderingWidget_slice->GetRenderWindow()->Render();
}


#include <vtkImagePermute.h>


void MainWidget::loadVolume()
{
  ReadImageDialog dialog(this, "Load Volume");
  if ( dialog.exec() )
  {
    // Se c'è un volume già aperta lo chiude
    if (isVolumeLoaded)
      CloseVolume();

    // Se non abbiamo un valido OpenGL context viene creato
    RenderAll();

    // Legge l'immagine
    vtkGenericMedicalImageReader* reader = vtkGenericMedicalImageReader::New();
    if (dialog.useDirectory())
      reader->SetDirectoryName(dialog.getDirectory().c_str());
    else
      reader->SetFileName (dialog.getFilename().c_str());
    reader->SetImageType(dialog.getImageType());

    reader->Update();

    m_Volume3DActor_volume = Volume3DActor::New();
    m_Volume3DActor_volume->SetInput ( reader->GetOutput() );

    m_VolumeMapper_volume = vtkVolumeMapper::SafeDownCast ( m_Volume3DActor_volume->GetMapper() );
    if ( m_VolumeMapper_volume == NULL )
      std::cerr << "ERROR: if ((m_VolumeMapper_volume = NULL)" << std::endl;


    m_OutlineActor_volume = boxActor ( reader->GetOutput() );
    m_OutlineActor_volume->PokeMatrix(m_Volume3DActor_volume->GetMatrix());

    m_Rend_volume->AddActor ( m_Volume3DActor_volume );
    m_Rend_volume->AddActor ( m_OutlineActor_volume );


    m_VolumeMapper_volume->GetBounds ( m_VolumeBounds );
    m_VolumeMapper_volume->CroppingOn();
    m_VolumeMapper_volume->SetCroppingRegionFlagsToSubVolume();

    m_Rend_volume->ResetCamera();

    CropVolume();

    SetCameraVolumeLeft();


// Proiezione Assiale
    m_SliceActor_axial = SliceActor::New();
    m_SliceActor_axial->SetInput ( reader->GetOutput() );
    m_SliceActor_axial->getResliceAxes()->DeepCopy(axialElements);
    m_SliceActor_axial->SetVisibility ( false );

    m_ImageViewer_axial->SetInputConnection( m_SliceActor_axial->getImageReslice()->GetOutputPort() );
    m_RenderingWidget_axial->SetRenderWindow ( m_ImageViewer_axial->GetRenderWindow() );
    m_ImageViewer_axial->SetupInteractor ( m_RenderingWidget_axial->GetRenderWindow()->GetInteractor() );
    m_Slider_axial->setMinimum ( m_SliceActor_axial->getMinZ() );
    m_Slider_axial->setMaximum ( m_SliceActor_axial->getMaxZ() );
    m_SpinBox_axial->setMinimum ( m_SliceActor_axial->getMinZ() );
    m_SpinBox_axial->setMaximum ( m_SliceActor_axial->getMaxZ() );

    m_OutlineActor_axial = boxActor ( m_ImageViewer_axial->GetImageActor()->GetInput() );
    m_OutlineActor_axial->GetProperty()->SetColor ( 1.0, 0.0, 0.0 );
#ifdef WIN32 //Nascosti in Windows perchè buggati
    m_OutlineActor_axial->SetVisibility(false);
#endif // WIN32
    m_ImageViewer_axial->GetRenderer()->AddActor ( m_OutlineActor_axial );
    m_ImageViewer_axial->GetRenderer()->ResetCamera();
    m_OutlineActor_volume_axial = boxActor ( m_SliceActor_axial->GetInput() );
    m_OutlineActor_volume_axial->GetProperty()->SetColor ( 1.0, 0.0, 0.0 );
    m_OutlineActor_volume_axial->PokeMatrix(m_SliceActor_axial->getResliceAxes());

    setSlice_axial(0);

    m_Rend_volume->AddActor ( m_SliceActor_axial );
    m_Rend_volume->AddActor ( m_OutlineActor_volume_axial );


// Proiezione Sagittale
    m_SliceActor_sagittal = SliceActor::New();
    m_SliceActor_sagittal->SetInput ( reader->GetOutput() );
    m_SliceActor_sagittal->getResliceAxes()->DeepCopy(sagittalElements);
    m_SliceActor_sagittal->SetVisibility ( false );

    m_ImageViewer_sagittal->SetInputConnection ( m_SliceActor_sagittal->getImageReslice()->GetOutputPort() );
    m_RenderingWidget_sagittal->SetRenderWindow ( m_ImageViewer_sagittal->GetRenderWindow() );
    m_ImageViewer_sagittal->SetupInteractor ( m_RenderingWidget_sagittal->GetRenderWindow()->GetInteractor() );
    m_Slider_sagittal->setMinimum ( m_SliceActor_sagittal->getMinX() );
    m_Slider_sagittal->setMaximum ( m_SliceActor_sagittal->getMaxX() );
    m_SpinBox_sagittal->setMinimum ( m_SliceActor_sagittal->getMinX() );
    m_SpinBox_sagittal->setMaximum ( m_SliceActor_sagittal->getMaxX() );

    m_OutlineActor_sagittal = boxActor ( m_ImageViewer_sagittal->GetImageActor()->GetInput() );
    m_OutlineActor_sagittal->GetProperty()->SetColor ( 0.0, 1.0, 0.0 );
    m_ImageViewer_sagittal->GetRenderer()->AddActor ( m_OutlineActor_sagittal );
    m_ImageViewer_sagittal->GetRenderer()->ResetCamera();
#ifdef WIN32 //Nascosti in Windows perchè buggati
    m_OutlineActor_sagittal->SetVisibility(false);
#endif // WIN32
    m_OutlineActor_volume_sagittal = boxActor ( m_SliceActor_sagittal->GetInput() );
    m_OutlineActor_volume_sagittal->GetProperty()->SetColor ( 0.0, 1.0, 0.0 );
    m_OutlineActor_volume_sagittal->PokeMatrix(m_SliceActor_sagittal->getResliceAxes());

    setSlice_sagittal(0);

    m_Rend_volume->AddActor ( m_SliceActor_sagittal );
    m_Rend_volume->AddActor ( m_OutlineActor_volume_sagittal );


// Proiezione Coronale
    m_SliceActor_coronal = SliceActor::New();
    m_SliceActor_coronal->SetInput ( reader->GetOutput() );
    m_SliceActor_coronal->getResliceAxes()->DeepCopy(coronalElements);
    m_SliceActor_coronal->SetVisibility ( false );

    m_ImageViewer_coronal->SetInputConnection ( m_SliceActor_coronal->getImageReslice()->GetOutputPort() );
    m_RenderingWidget_coronal->SetRenderWindow ( m_ImageViewer_coronal->GetRenderWindow() );
    m_ImageViewer_coronal->SetupInteractor ( m_RenderingWidget_coronal->GetRenderWindow()->GetInteractor() );
    m_Slider_coronal->setMinimum ( m_SliceActor_coronal->getMinY() );
    m_Slider_coronal->setMaximum ( m_SliceActor_coronal->getMaxY() );
    m_SpinBox_coronal->setMinimum ( m_SliceActor_coronal->getMinY() );
    m_SpinBox_coronal->setMaximum ( m_SliceActor_coronal->getMaxY() );

    m_OutlineActor_coronal = boxActor ( m_ImageViewer_coronal->GetImageActor()->GetInput() );
    m_OutlineActor_coronal->GetProperty()->SetColor ( 0.0, 0.0, 1.0 );
#ifdef WIN32 //Nascosti in Windows perchè buggati
    m_OutlineActor_coronal->SetVisibility(false);
#endif // WIN32
    m_ImageViewer_coronal->GetRenderer()->AddActor ( m_OutlineActor_coronal );
    m_ImageViewer_coronal->GetRenderer()->ResetCamera();
    m_OutlineActor_volume_coronal = boxActor ( m_SliceActor_coronal->GetInput() );
    m_OutlineActor_volume_coronal->GetProperty()->SetColor ( 0.0, 0.0, 1.0 );
    m_OutlineActor_volume_coronal->PokeMatrix(m_SliceActor_coronal->getResliceAxes());

    setSlice_coronal(0);

    m_Rend_volume->AddActor ( m_SliceActor_coronal );
    m_Rend_volume->AddActor ( m_OutlineActor_volume_coronal );


// Slice generica
    m_SliceActor_slice = SliceActor::New();
    m_SliceActor_slice->SetInput ( reader->GetOutput() );
    m_SliceActor_slice->getResliceAxes()->DeepCopy(axialElements);
    m_SliceActor_slice->setX( m_SliceActor_slice->getCenterX() );
    m_SliceActor_slice->setY( m_SliceActor_slice->getCenterY() );
    m_SliceActor_slice->setZ( m_SliceActor_slice->getCenterZ() );
    m_SliceActor_slice->SetVisibility ( false );

    m_ImageViewer_slice->SetInputConnection ( m_SliceActor_slice->getImageReslice()->GetOutputPort() );
    m_RenderingWidget_slice->SetRenderWindow ( m_ImageViewer_slice->GetRenderWindow() );
    m_ImageViewer_slice->SetupInteractor ( m_RenderingWidget_slice->GetRenderWindow()->GetInteractor() );
    m_Slider_slice_X->setMinimum ( m_SliceActor_slice->getMinX() );
    m_Slider_slice_X->setMaximum ( m_SliceActor_slice->getMaxX() );
    m_Slider_slice_X->setValue ( m_SliceActor_slice->getCenterX() );
    m_Slider_slice_Y->setMinimum ( m_SliceActor_slice->getMinY() );
    m_Slider_slice_Y->setMaximum ( m_SliceActor_slice->getMaxY() );
    m_Slider_slice_Y->setValue ( m_SliceActor_slice->getCenterY() );
    m_Slider_slice_Z->setMinimum ( m_SliceActor_slice->getMinZ() );
    m_Slider_slice_Z->setMaximum ( m_SliceActor_slice->getMaxZ() );
    m_Slider_slice_Z->setValue ( m_SliceActor_slice->getCenterZ() );
    m_Slider_slice_alpha->setMaximum ( 360 );
    m_Slider_slice_beta->setMaximum ( 360 );
    m_Slider_slice_gamma->setMaximum ( 360 );
    m_SpinBox_slice_X->setMinimum ( m_SliceActor_slice->getMinX() );
    m_SpinBox_slice_X->setMaximum ( m_SliceActor_slice->getMaxX() );
    m_SpinBox_slice_X->setValue ( m_SliceActor_slice->getCenterX() );
    m_SpinBox_slice_Y->setMinimum ( m_SliceActor_slice->getMinY() );
    m_SpinBox_slice_Y->setMaximum ( m_SliceActor_slice->getMaxY() );
    m_SpinBox_slice_Y->setValue ( m_SliceActor_slice->getCenterY() );
    m_SpinBox_slice_Z->setMinimum ( m_SliceActor_slice->getMinZ() );
    m_SpinBox_slice_Z->setMaximum ( m_SliceActor_slice->getMaxZ() );
    m_SpinBox_slice_Z->setValue ( m_SliceActor_slice->getCenterZ() );
    m_SpinBox_slice_alpha->setMaximum ( 360 );
    m_SpinBox_slice_beta->setMaximum ( 360 );
    m_SpinBox_slice_gamma->setMaximum ( 360 );

    m_OutlineActor_slice = boxActor ( m_ImageViewer_slice->GetImageActor()->GetInput() );
    m_OutlineActor_slice->GetProperty()->SetColor ( 0.0, 1.0, 1.0 );
    m_ImageViewer_slice->GetRenderer()->AddActor ( m_OutlineActor_slice );
    m_ImageViewer_slice->GetRenderer()->ResetCamera();
#ifdef WIN32 //Nascosti in Windows perchè buggati
    m_OutlineActor_slice->SetVisibility(false);
#endif // WIN32
    m_OutlineActor_volume_slice = boxActor ( m_SliceActor_slice->GetInput() );
    m_OutlineActor_volume_slice->GetProperty()->SetColor ( 0.0, 1.0, 1.0 );
    m_OutlineActor_volume_slice->PokeMatrix(m_SliceActor_slice->getResliceAxes());

    m_Rend_volume->AddActor ( m_SliceActor_slice );
    m_Rend_volume->AddActor ( m_OutlineActor_volume_slice );


    DisableControlsVolume(false);
    ConnectControlsVolume();

// Setta luminosità e contrasto delle immagini a seconda del numero di bit contenuti
    double range[2];
    reader->GetOutput()->GetScalarRange(range);

    double diff = range[1] - range[0];
    //double shift = -range[0];

    m_ImageViewer_axial->SetColorLevel ( diff/2 );
    m_ImageViewer_axial->SetColorWindow ( diff );

    m_ImageViewer_sagittal->SetColorLevel ( diff/2 );
    m_ImageViewer_sagittal->SetColorWindow ( diff );

    m_ImageViewer_coronal->SetColorLevel ( diff/2 );
    m_ImageViewer_coronal->SetColorWindow ( diff );

    m_ImageViewer_slice->SetColorLevel ( diff/2 );
    m_ImageViewer_slice->SetColorWindow ( diff );

    m_Volume3DActor_volume->SetUnsignedChar();
    m_SliceActor_axial->SetRange(range[0], range[1]);
    m_SliceActor_sagittal->SetRange(range[0], range[1]);
    m_SliceActor_coronal->SetRange(range[0], range[1]);
    m_SliceActor_slice->SetRange(range[0], range[1]);


    isVolumeLoaded = true;
    statusBar()->showMessage("Volume Loaded", 2000);

    showOrientationMarker();
    RenderAll();
    reader->Delete();
  }
}



// Close Volume
void MainWidget::CloseVolume()
{
    if (isVolumeLoaded)
    {
        statusBar()->showMessage("Closing Volume...");

        //Nasconde tutto per evitare brutti effetti grafici
        m_ImageViewer_axial->GetImageActor()->SetVisibility ( false );
        m_ImageViewer_sagittal->GetImageActor()->SetVisibility ( false );
        m_ImageViewer_coronal->GetImageActor()->SetVisibility ( false );
        m_ImageViewer_slice->GetImageActor()->SetVisibility ( false );
        m_OutlineActor_axial->SetVisibility ( false );
        m_OutlineActor_sagittal->SetVisibility ( false );
        m_OutlineActor_coronal->SetVisibility ( false );
        m_OutlineActor_slice->SetVisibility ( false );

        RenderAll();

        //Disconnessione dei segnali per evitare che possano essere chiamate le funzioni
        ResetControlsVolume(); //Reset va prima del disconnect per rimuovere gli eventuali piani di taglio
        DisconnectControlsVolume();
        DisableControlsVolume(true);

// Eliminazione degli actors dalla finestra principale
        m_Rend_volume->RemoveActor(m_OutlineActor_volume);
        m_Rend_volume->RemoveActor(m_OutlineActor_volume_axial);
        m_Rend_volume->RemoveActor(m_OutlineActor_volume_sagittal);
        m_Rend_volume->RemoveActor(m_OutlineActor_volume_coronal);
        m_Rend_volume->RemoveActor(m_OutlineActor_volume_slice);
        m_Rend_volume->RemoveActor(m_Volume3DActor_volume);

// Deallocazione memoria actors della finestra principale
        m_OutlineActor_volume->Delete();
        m_OutlineActor_volume_axial->Delete();
        m_OutlineActor_volume_sagittal->Delete();
        m_OutlineActor_volume_coronal->Delete();
        m_OutlineActor_volume_slice->Delete();
        m_Volume3DActor_volume->Delete();

        m_OutlineActor_volume = NULL;
        m_OutlineActor_volume_axial = NULL;
        m_OutlineActor_volume_sagittal = NULL;
        m_OutlineActor_volume_coronal = NULL;
        m_OutlineActor_volume_slice = NULL;
        m_Volume3DActor_volume = NULL;

// Eliminazione degli actors dalle finestre delle slice
        m_RenderingWidget_axial->SetRenderWindow(NULL);
        m_RenderingWidget_sagittal->SetRenderWindow(NULL);
        m_RenderingWidget_coronal->SetRenderWindow(NULL);
        m_RenderingWidget_slice->SetRenderWindow(NULL);

// Deallocazione memoria actors delle finestre delle slice
        m_ImageViewer_axial->Delete();
        m_ImageViewer_coronal->Delete();
        m_ImageViewer_sagittal->Delete();
        m_ImageViewer_slice->Delete();

        m_ImageViewer_axial = NULL;
        m_ImageViewer_coronal = NULL;
        m_ImageViewer_sagittal = NULL;
        m_ImageViewer_slice = NULL;

        m_SliceActor_axial->Delete();
        m_SliceActor_sagittal->Delete();
        m_SliceActor_coronal->Delete();
        m_SliceActor_slice->Delete();

        m_SliceActor_axial = NULL;
        m_SliceActor_sagittal = NULL;
        m_SliceActor_coronal = NULL;
        m_SliceActor_slice = NULL;

        m_OutlineActor_axial->Delete();
        m_OutlineActor_sagittal->Delete();
        m_OutlineActor_coronal->Delete();
        m_OutlineActor_slice->Delete();

        m_OutlineActor_axial = NULL;
        m_OutlineActor_sagittal = NULL;
        m_OutlineActor_coronal = NULL;
        m_OutlineActor_slice = NULL;

        CreateImageViewer();

        isVolumeLoaded = false;
        statusBar()->showMessage("Volume Closed", 2000);

/*
#ifdef SV_3_SURFACES
        if (!isSurfaceLoaded && !isSurface2Loaded && !isSurface3Loaded)
#else // SV_3_SURFACES
        if (!isSurfaceLoaded)
#endif // SV_3_SURFACES
            hideOrientationMarker();
*/
        //Render delle finestre
        RenderAll();
    }
}


/*
void MainWidget::loadSurface()
{
    vtkGenericMedicalImageReader* reader = vtkGenericMedicalImageReader::New();

    if(readImageDialog(reader, tr("Load Surface")))
    {
        if (isSurfaceLoaded)
            CloseSurface();
        reader->Update();

        m_Surface3DActor_surface = Surface3DActor::New();
        m_Surface3DActor_surface->SetInput ( reader->GetOutput() );
        m_Rend_volume->AddActor ( m_Surface3DActor_surface );


        DisableControlsSurface(false);
        ConnectControlsSurface();

#ifdef SV_3_SURFACES
        if (!isVolumeLoaded && !isSurface2Loaded && !isSurface3Loaded)
#else // SV_3_SURFACES
        if (!isVolumeLoaded)
#endif // SV_3_SURFACES
            m_Rend_volume->ResetCamera();

        isSurfaceLoaded = true;
        statusBar()->showMessage("Surface Loaded...", 2000);

        showOrientationMarker();
    }
    reader->Delete();
}

void MainWidget::CloseSurface()
{
    if (isSurfaceLoaded)
    {
        statusBar()->showMessage("Closing Surface...");

        ResetControlsSurface();
        DisconnectControlsSurface();
        DisableControlsSurface(true);


        m_Rend_volume->RemoveActor(m_Surface3DActor_surface);
        m_Surface3DActor_surface->Delete();
        m_Surface3DActor_surface = NULL;

        statusBar()->showMessage("Surface Closed", 2000);
        isSurfaceLoaded = false;

#ifdef SV_3_SURFACES
        if (!isVolumeLoaded && !isSurface2Loaded && !isSurface3Loaded)
#else // SV_3_SURFACES
        if (!isVolumeLoaded)
#endif // SV_3_SURFACES
            hideOrientationMarker();
        RenderAll();
    }
}


#ifdef SV_3_SURFACES
void MainWidget::loadSurface2()
{
    vtkGenericMedicalImageReader* reader = vtkGenericMedicalImageReader::New();

    if(readImageDialog(reader, tr("Load Surface 2")))
    {
        if (isSurface2Loaded)
            CloseSurface2();
        reader->Update();

        m_Surface3DActor_surface2 = Surface3DActor::New();
        m_Surface3DActor_surface2->SetInput ( reader->GetOutput() );
        m_Rend_volume->AddActor ( m_Surface3DActor_surface2 );


        DisableControlsSurface2(false);
        ConnectControlsSurface2();


        if (!isVolumeLoaded && !isSurfaceLoaded && !isSurface3Loaded)
            m_Rend_volume->ResetCamera();

        m_ComboBox_surface2_color->setCurrentIndex(1);

        isSurface2Loaded = true;
        statusBar()->showMessage("Surface 2 Loaded...", 2000);

        showOrientationMarker();
    }
    reader->Delete();
}

void MainWidget::loadSurface3()
{
    vtkGenericMedicalImageReader* reader = vtkGenericMedicalImageReader::New();

    if(readImageDialog(reader, tr("Load Surface 3")))
    {
        if (isSurface3Loaded)
            CloseSurface3();
        reader->Update();

        m_Surface3DActor_surface3 = Surface3DActor::New();
        m_Surface3DActor_surface3->SetInput ( reader->GetOutput() );
        m_Rend_volume->AddActor ( m_Surface3DActor_surface3 );


        DisableControlsSurface3(false);
        ConnectControlsSurface3();


        if (!isVolumeLoaded && !isSurfaceLoaded && !isSurface2Loaded)
            m_Rend_volume->ResetCamera();

        m_ComboBox_surface3_color->setCurrentIndex(2);

        isSurface3Loaded = true;
        statusBar()->showMessage("Surface 3 Loaded...", 2000);

        showOrientationMarker();
    }
    reader->Delete();
}


void MainWidget::CloseSurface2()
{
    if (isSurface2Loaded)
    {
        statusBar()->showMessage("Closing Surface 2...");

        ResetControlsSurface2();
        DisconnectControlsSurface2();
        DisableControlsSurface2(true);


        m_Rend_volume->RemoveActor(m_Surface3DActor_surface2);
        m_Surface3DActor_surface2->Delete();
        m_Surface3DActor_surface2 = NULL;

        statusBar()->showMessage("Surface 2 Closed", 2000);
        isSurface2Loaded = false;

        if (!isVolumeLoaded && !isSurfaceLoaded && !isSurface3Loaded)
            hideOrientationMarker();
        RenderAll();
    }
}

void MainWidget::CloseSurface3()
{
    if (isSurface3Loaded)
    {
        statusBar()->showMessage("Closing Surface 3...");

        ResetControlsSurface3();
        DisconnectControlsSurface3();
        DisableControlsSurface3(true);


        m_Rend_volume->RemoveActor(m_Surface3DActor_surface3);
        m_Surface3DActor_surface3->Delete();
        m_Surface3DActor_surface3 = NULL;

        statusBar()->showMessage("Surface 3 Closed", 2000);
        isSurface3Loaded = false;

        if (!isVolumeLoaded && !isSurfaceLoaded && !isSurface2Loaded)
            hideOrientationMarker();
        RenderAll();
    }
}
#endif // SV_3_SURFACES
*/

void MainWidget::showOrientationMarker()
{
    if (!isOrientationMarkerLoaded)
    {
        vtkAxesActor* axesActor_volume = vtkAxesActor::New();
        axesActor_volume->SetShaftTypeToCylinder();
        axesActor_volume->SetXAxisLabelText("R");
        axesActor_volume->SetYAxisLabelText("A");
        axesActor_volume->SetZAxisLabelText("S");
        axesActor_volume->SetTotalLength(1.5, 1.5, 1.5);

        m_OrientationMarker_volume = vtkOrientationMarkerWidget::New();
        m_OrientationMarker_volume->SetOrientationMarker(axesActor_volume);
        m_OrientationMarker_volume->SetViewport(0.0, 0.0, 0.2, 0.2);
        m_OrientationMarker_volume->SetInteractor(m_Rend_volume->GetRenderWindow()->GetInteractor());
        m_OrientationMarker_volume->SetEnabled(1);
        m_OrientationMarker_volume->InteractiveOff();

        axesActor_volume->Delete();

        isOrientationMarkerLoaded = true;
    }
    else
        m_OrientationMarker_volume->SetEnabled(1);
}

void MainWidget::hideOrientationMarker()
{
        m_OrientationMarker_volume->SetEnabled(0);
}


void MainWidget::CloseAll()
{
    if (isVolumeLoaded)
        CloseVolume();

    for(int i=0; i<NUM_SURFACES; i++)
      m_SurfaceWidget[i]->closeSurface();
/*
    if (isOrientationMarkerLoaded)
    {
        m_OrientationMarker_volume->Delete();
        m_OrientationMarker_volume = NULL;
        isOrientationMarkerLoaded = false;
    }
*/
}



void MainWidget::ConnectControlsVolume()
{
// Sliders
    connect ( m_Slider_axial, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_axial ( int ) ) );
    connect ( m_Slider_sagittal, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_sagittal ( int ) ) );
    connect ( m_Slider_coronal, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_coronal ( int ) ) );
    connect ( m_Slider_slice_X, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_X ( int ) ) );
    connect ( m_Slider_slice_Y, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_Y ( int ) ) );
    connect ( m_Slider_slice_Z, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_Z ( int ) ) );
    connect ( m_Slider_slice_alpha, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_alpha ( int ) ) );
    connect ( m_Slider_slice_beta, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_beta ( int ) ) );
    connect ( m_Slider_slice_gamma, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_gamma ( int ) ) );

// Spinboxes
    connect ( m_SpinBox_axial, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_axial ( int ) ) );
    connect ( m_SpinBox_sagittal, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_sagittal ( int ) ) );
    connect ( m_SpinBox_coronal, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_coronal ( int ) ) );
    connect ( m_SpinBox_slice_X, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_X ( int ) ) );
    connect ( m_SpinBox_slice_Y, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_Y ( int ) ) );
    connect ( m_SpinBox_slice_Z, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_Z ( int ) ) );
    connect ( m_SpinBox_slice_alpha, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_alpha ( int ) ) );
    connect ( m_SpinBox_slice_beta, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_beta ( int ) ) );
    connect ( m_SpinBox_slice_gamma, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_gamma ( int ) ) );

// Comboboxes
    connect ( m_ComboBox_axial, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setCrop_axial ( int ) ) );
    connect ( m_ComboBox_sagittal, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setCrop_sagittal ( int ) ) );
    connect ( m_ComboBox_coronal, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setCrop_coronal ( int ) ) );
    connect ( m_ComboBox_slice, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setCrop_slice ( int ) ) );
}


void MainWidget::DisconnectControlsVolume()
{
// Sliders
    disconnect ( m_Slider_axial, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_axial ( int ) ) );
    disconnect ( m_Slider_sagittal, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_sagittal ( int ) ) );
    disconnect ( m_Slider_coronal, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_coronal ( int ) ) );
    disconnect ( m_Slider_slice_X, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_X ( int ) ) );
    disconnect ( m_Slider_slice_Y, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_Y ( int ) ) );
    disconnect ( m_Slider_slice_Z, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_Z ( int ) ) );
    disconnect ( m_Slider_slice_alpha, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_alpha ( int ) ) );
    disconnect ( m_Slider_slice_beta, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_beta ( int ) ) );
    disconnect ( m_Slider_slice_gamma, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_gamma ( int ) ) );

// Spinboxes
    disconnect ( m_SpinBox_axial, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_axial ( int ) ) );
    disconnect ( m_SpinBox_sagittal, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_sagittal ( int ) ) );
    disconnect ( m_SpinBox_coronal, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_coronal ( int ) ) );
    disconnect ( m_SpinBox_slice_X, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_X ( int ) ) );
    disconnect ( m_SpinBox_slice_Y, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_Y ( int ) ) );
    disconnect ( m_SpinBox_slice_Z, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_Z ( int ) ) );
    disconnect ( m_SpinBox_slice_alpha, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_alpha ( int ) ) );
    disconnect ( m_SpinBox_slice_beta, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_beta ( int ) ) );
    disconnect ( m_SpinBox_slice_gamma, SIGNAL ( valueChanged ( int ) ), this, SLOT ( setSlice_slice_gamma ( int ) ) );

// Comboboxes
    disconnect ( m_ComboBox_axial, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setCrop_axial ( int ) ) );
    disconnect ( m_ComboBox_sagittal, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setCrop_sagittal ( int ) ) );
    disconnect ( m_ComboBox_coronal, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setCrop_coronal ( int ) ) );
    disconnect ( m_ComboBox_slice, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setCrop_slice ( int ) ) );
}



void MainWidget::ResetControlsVolume()
{

// Sliders
    m_Slider_axial->setMinimum ( 0 );
    m_Slider_axial->setMaximum ( 0 );
    m_Slider_axial->setValue ( 0 );
    m_Slider_sagittal->setMinimum ( 0 );
    m_Slider_sagittal->setMaximum ( 0 );
    m_Slider_sagittal->setValue ( 0 );
    m_Slider_coronal->setMinimum ( 0 );
    m_Slider_coronal->setMaximum ( 0 );
    m_Slider_coronal->setValue ( 0 );
    m_Slider_slice_X->setMinimum ( 0 );
    m_Slider_slice_X->setMaximum ( 0 );
    m_Slider_slice_X->setValue ( 0 );
    m_Slider_slice_Y->setMinimum ( 0 );
    m_Slider_slice_Y->setMaximum ( 0 );
    m_Slider_slice_Y->setValue ( 0 );
    m_Slider_slice_Z->setMinimum ( 0 );
    m_Slider_slice_Z->setMaximum ( 0 );
    m_Slider_slice_Z->setValue ( 0 );
    m_Slider_slice_alpha->setMaximum ( 0 );
    m_Slider_slice_alpha->setValue ( 0 );
    m_Slider_slice_beta->setMaximum ( 0 );
    m_Slider_slice_beta->setValue ( 0 );
    m_Slider_slice_gamma->setMaximum ( 0 );
    m_Slider_slice_gamma->setValue ( 0 );


//SpinBoxes
    m_SpinBox_axial->setMinimum ( 0 );
    m_SpinBox_axial->setMaximum ( 0 );
    m_SpinBox_axial->setValue ( 0 );
    m_SpinBox_sagittal->setMinimum ( 0 );
    m_SpinBox_sagittal->setMaximum ( 0 );
    m_SpinBox_sagittal->setValue ( 0 );
    m_SpinBox_coronal->setMinimum ( 0 );
    m_SpinBox_coronal->setMaximum ( 0 );
    m_SpinBox_coronal->setValue ( 0 );
    m_SpinBox_slice_X->setMinimum ( 0 );
    m_SpinBox_slice_X->setMaximum ( 0 );
    m_SpinBox_slice_X->setValue ( 0 );
    m_SpinBox_slice_Y->setMinimum ( 0 );
    m_SpinBox_slice_Y->setMaximum ( 0 );
    m_SpinBox_slice_Y->setValue ( 0 );
    m_SpinBox_slice_Z->setMinimum ( 0 );
    m_SpinBox_slice_Z->setMaximum ( 0 );
    m_SpinBox_slice_Z->setValue ( 0 );
    m_SpinBox_slice_alpha->setMaximum ( 0 );
    m_SpinBox_slice_alpha->setValue ( 0 );
    m_SpinBox_slice_beta->setMaximum ( 0 );
    m_SpinBox_slice_beta->setValue ( 0 );
    m_SpinBox_slice_gamma->setMaximum ( 0 );
    m_SpinBox_slice_gamma->setValue ( 0 );

// ComboBoxes
    m_ComboBox_axial->setCurrentIndex(1);
    m_ComboBox_sagittal->setCurrentIndex(1);
    m_ComboBox_coronal->setCurrentIndex(1);
    m_ComboBox_slice->setCurrentIndex(1);
}


void MainWidget::DisableControlsVolume(bool disabled)
{
    m_Slider_axial->setDisabled(disabled);
    m_Slider_sagittal->setDisabled(disabled);
    m_Slider_coronal->setDisabled(disabled);
    m_Slider_slice_X->setDisabled(disabled);
    m_Slider_slice_Y->setDisabled(disabled);
    m_Slider_slice_Z->setDisabled(disabled);
    m_Slider_slice_alpha->setDisabled(disabled);
    m_Slider_slice_beta->setDisabled(disabled);
    m_Slider_slice_gamma->setDisabled(disabled);

//SpinBoxes
    m_SpinBox_axial->setDisabled(disabled);
    m_SpinBox_sagittal->setDisabled(disabled);
    m_SpinBox_coronal->setDisabled(disabled);
    m_SpinBox_slice_X->setDisabled(disabled);
    m_SpinBox_slice_Y->setDisabled(disabled);
    m_SpinBox_slice_Z->setDisabled(disabled);
    m_SpinBox_slice_alpha->setDisabled(disabled);
    m_SpinBox_slice_beta->setDisabled(disabled);
    m_SpinBox_slice_gamma->setDisabled(disabled);

// ComboBoxes
    m_ComboBox_axial->setDisabled(disabled);
    m_ComboBox_sagittal->setDisabled(disabled);
    m_ComboBox_coronal->setDisabled(disabled);
    m_ComboBox_slice->setDisabled(disabled);

// Labels
    m_Label_slice_X->setDisabled(disabled);
    m_Label_slice_Y->setDisabled(disabled);
    m_Label_slice_Z->setDisabled(disabled);
    m_Label_slice_alpha->setDisabled(disabled);
    m_Label_slice_beta->setDisabled(disabled);
    m_Label_slice_gamma->setDisabled(disabled);

// Menu
    m_Action_File_CloseVolume->setDisabled(disabled);
/*
#ifdef SV_3_SURFACES
    m_Action_File_CloseAll->setDisabled(!isSurfaceLoaded && !isSurface2Loaded && !isSurface3Loaded && disabled);
#else // SV_3_SURFACES
    m_Action_File_CloseAll->setDisabled(!isSurfaceLoaded && disabled);
#endif
*/
}

/*
#ifdef SV_3_SURFACES
    m_Action_File_CloseAll->setDisabled(!isVolumeLoaded && !isSurface2Loaded && !isSurface3Loaded && disabled);
#else // SV_3_SURFACES
    m_Action_File_CloseAll->setDisabled(!isVolumeLoaded && disabled);
#endif
*/

// TODO Creare una classe a parte
// -- create a box around the function to indicate the sampling volume --
vtkActor* MainWidget::boxActor ( vtkImageData* input )
{
    vtkOutlineFilter *outline = vtkOutlineFilter::New();
    outline->SetInput ( input );
    vtkPolyDataMapper *outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetResolveCoincidentTopologyToShiftZBuffer() ;
    outlineMapper->SetResolveCoincidentTopologyZShift(0.02) ;
    outlineMapper->SetInput ( outline->GetOutput() );
    vtkActor *outlineActor = vtkActor::New();
    outlineActor->SetMapper ( outlineMapper );
    outlineActor->GetProperty()->SetColor ( 1,1,1 );
    outline->Delete();
    outlineMapper->Delete();
    return outlineActor;
}


void MainWidget::setSlice_axial ( int slice )
{
    m_SliceActor_axial->setZ( slice );
    m_ImageViewer_axial->GetImageActor()->SetDisplayExtent ( -1,0,0,0,0,0 );
    m_OutlineActor_volume_axial->PokeMatrix(m_SliceActor_axial->getResliceAxes());
    m_RenderingWidget_axial->GetRenderWindow()->Render();
    m_RenderingWidget_volume->GetRenderWindow()->Render();

    m_Slider_axial->setValue(slice);
    m_SpinBox_axial->setValue(slice);
    CropVolume();
}

void MainWidget::setSlice_sagittal ( int slice )
{
    m_SliceActor_sagittal->setX( slice );
    m_ImageViewer_sagittal->GetImageActor()->SetDisplayExtent ( -1,0,0,0,0,0 );
    m_OutlineActor_volume_sagittal->PokeMatrix(m_SliceActor_sagittal->getResliceAxes());
    m_RenderingWidget_sagittal->GetRenderWindow()->Render();
    m_RenderingWidget_volume->GetRenderWindow()->Render();

    m_Slider_sagittal->setValue(slice);
    m_SpinBox_sagittal->setValue(slice);
    CropVolume();
}

void MainWidget::setSlice_coronal ( int slice )
{
    m_SliceActor_coronal->setY( m_SliceActor_coronal->getMaxY()-slice ); // getMaxY()-slice per far apparire il naso davanti
    m_ImageViewer_coronal->GetImageActor()->SetDisplayExtent ( -1,0,0,0,0,0 );
    m_OutlineActor_volume_coronal->PokeMatrix(m_SliceActor_coronal->getResliceAxes());
    m_RenderingWidget_coronal->GetRenderWindow()->Render();
    m_RenderingWidget_volume->GetRenderWindow()->Render();

    m_Slider_coronal->setValue(slice);
    m_SpinBox_coronal->setValue(slice);
    CropVolume();
}

void MainWidget::setSlice_slice_X ( int slice )
{
    m_SliceActor_slice->setX( slice );
    m_ImageViewer_slice->GetImageActor()->SetDisplayExtent ( -1,0,0,0,0,0 );
    m_OutlineActor_volume_slice->PokeMatrix(m_SliceActor_slice->getResliceAxes());
    m_RenderingWidget_slice->GetRenderWindow()->Render();
    m_RenderingWidget_volume->GetRenderWindow()->Render();

    m_Slider_slice_X->setValue(slice);
    m_SpinBox_slice_X->setValue(slice);

    m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::setSlice_slice_Y ( int slice )
{
    m_SliceActor_slice->setY( slice );
    m_ImageViewer_slice->GetImageActor()->SetDisplayExtent ( -1,0,0,0,0,0 );
    m_OutlineActor_volume_slice->PokeMatrix(m_SliceActor_slice->getResliceAxes());
    m_RenderingWidget_slice->GetRenderWindow()->Render();
    m_RenderingWidget_volume->GetRenderWindow()->Render();

    m_Slider_slice_Y->setValue(slice);
    m_SpinBox_slice_Y->setValue(slice);

    m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::setSlice_slice_Z ( int slice )
{
    m_SliceActor_slice->setZ( slice );
    m_ImageViewer_slice->GetImageActor()->SetDisplayExtent ( -1,0,0,0,0,0 );
    m_OutlineActor_volume_slice->PokeMatrix(m_SliceActor_slice->getResliceAxes());
    m_RenderingWidget_slice->GetRenderWindow()->Render();
    m_RenderingWidget_volume->GetRenderWindow()->Render();

    m_Slider_slice_Z->setValue(slice);
    m_SpinBox_slice_Z->setValue(slice);

    m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::setSlice_slice_alpha ( int angle )
{
    m_SliceActor_slice->setAngleAlpha( angle );
    m_ImageViewer_slice->GetImageActor()->SetDisplayExtent ( -1,0,0,0,0,0 );
    m_OutlineActor_volume_slice->PokeMatrix(m_SliceActor_slice->getResliceAxes());
    m_RenderingWidget_slice->GetRenderWindow()->Render();
    m_RenderingWidget_volume->GetRenderWindow()->Render();

    m_Slider_slice_alpha->setValue(angle);
    m_SpinBox_slice_alpha->setValue(angle);

    m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::setSlice_slice_beta ( int angle )
{
    m_SliceActor_slice->setAngleBeta( angle );
    m_ImageViewer_slice->GetImageActor()->SetDisplayExtent ( -1,0,0,0,0,0 );
    m_OutlineActor_volume_slice->PokeMatrix(m_SliceActor_slice->getResliceAxes());
    m_RenderingWidget_slice->GetRenderWindow()->Render();
    m_RenderingWidget_volume->GetRenderWindow()->Render();

    m_Slider_slice_beta->setValue(angle);
    m_SpinBox_slice_beta->setValue(angle);

    m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::setSlice_slice_gamma ( int angle )
{
    m_SliceActor_slice->setAngleGamma( angle );
    m_ImageViewer_slice->GetImageActor()->SetDisplayExtent ( -1,0,0,0,0,0 );
    m_OutlineActor_volume_slice->PokeMatrix(m_SliceActor_slice->getResliceAxes());
    m_RenderingWidget_slice->GetRenderWindow()->Render();
    m_RenderingWidget_volume->GetRenderWindow()->Render();

    m_Slider_slice_gamma->setValue(angle);
    m_SpinBox_slice_gamma->setValue(angle);

    m_RenderingWidget_volume->GetRenderWindow()->Render();
}


void MainWidget::setCrop_axial( int index )
{
    switch (index)
    {
        case 0:
            m_OutlineActor_volume_axial->SetVisibility ( false );
            m_SliceActor_axial->SetVisibility ( false );
            m_CropType_axial = 0;
            break;
        case 1:
            m_OutlineActor_volume_axial->SetVisibility ( true );
            m_SliceActor_axial->SetVisibility ( false );
            m_CropType_axial = 0;
            break;
        case 2:
            m_OutlineActor_volume_axial->SetVisibility ( true );
            m_SliceActor_axial->SetVisibility ( true );
            m_CropType_axial = 0;
            break;
        case 3:
            m_OutlineActor_volume_axial->SetVisibility ( true );
            m_SliceActor_axial->SetVisibility ( true );
            m_CropType_axial = 1;
            break;
        case 4:
            m_OutlineActor_volume_axial->SetVisibility ( true );
            m_SliceActor_axial->SetVisibility ( true );
            m_CropType_axial = 2;
            break;
        default:
            break;
    }
    CropVolume();
}

void MainWidget::setCrop_sagittal( int index )
{
    switch (index)
    {
        case 0:
            m_OutlineActor_volume_sagittal->SetVisibility ( false );
            m_SliceActor_sagittal->SetVisibility ( false );
            m_CropType_sagittal = 0;
            break;
        case 1:
            m_OutlineActor_volume_sagittal->SetVisibility ( true );
            m_SliceActor_sagittal->SetVisibility ( false );
            m_CropType_sagittal = 0;
            break;
        case 2:
            m_OutlineActor_volume_sagittal->SetVisibility ( true );
            m_SliceActor_sagittal->SetVisibility ( true );
            m_CropType_sagittal = 0;
            break;
        case 3:
            m_OutlineActor_volume_sagittal->SetVisibility ( true );
            m_SliceActor_sagittal->SetVisibility ( true );
            m_CropType_sagittal = 1;
            break;
        case 4:
            m_OutlineActor_volume_sagittal->SetVisibility ( true );
            m_SliceActor_sagittal->SetVisibility ( true );
            m_CropType_sagittal = 2;
            break;
        default:
            break;
    }
    CropVolume();

}

void MainWidget::setCrop_coronal( int index )
{
    switch (index)
    {
        case 0:
            m_OutlineActor_volume_coronal->SetVisibility ( false );
            m_SliceActor_coronal->SetVisibility ( false );
            m_CropType_coronal = 0;
            break;
        case 1:
            m_OutlineActor_volume_coronal->SetVisibility ( true );
            m_SliceActor_coronal->SetVisibility ( false );
            m_CropType_coronal = 0;
            break;
        case 2:
            m_OutlineActor_volume_coronal->SetVisibility ( true );
            m_SliceActor_coronal->SetVisibility ( true );
            m_CropType_coronal = 0;
            break;
        case 3:
            m_OutlineActor_volume_coronal->SetVisibility ( true );
            m_SliceActor_coronal->SetVisibility ( true );
            m_CropType_coronal = 1;
            break;
        case 4:
            m_OutlineActor_volume_coronal->SetVisibility ( true );
            m_SliceActor_coronal->SetVisibility ( true );
            m_CropType_coronal = 2;
            break;
        default:
            break;
    }
    CropVolume();
}

void MainWidget::setCrop_slice( int index )
{
    switch (index)
    {
        case 0:
            m_OutlineActor_volume_slice->SetVisibility ( false );
            m_SliceActor_slice->SetVisibility ( false );
            break;
        case 1:
            m_OutlineActor_volume_slice->SetVisibility ( true );
            m_SliceActor_slice->SetVisibility ( false );
            break;
        case 2:
            m_OutlineActor_volume_slice->SetVisibility ( true );
            m_SliceActor_slice->SetVisibility ( true );
            break;
        default:
            break;
    }
    m_RenderingWidget_volume->GetRenderWindow()->Render();
}

void MainWidget::CropVolume()
{
    double bounds[6];
    m_VolumeMapper_volume->GetBounds ( bounds );

    switch (m_CropType_axial)
    {
        case 0:
            m_VolumeBounds[4] = bounds[4];
            m_VolumeBounds[5] = bounds[5];
            break;
        case 1:
            m_VolumeBounds[4] = bounds[4];
            m_VolumeBounds[5] = m_SliceActor_axial->getZ();
            break;
        case 2:
            m_VolumeBounds[4] = m_SliceActor_axial->getZ();
            m_VolumeBounds[5] = bounds[5];
            break;
        default:
            break;
    }
    switch (m_CropType_sagittal)
    {
        case 0:
            m_VolumeBounds[0] = bounds[0];
            m_VolumeBounds[1] = bounds[1];
            break;
        case 1:
            m_VolumeBounds[0] = bounds[0];
            m_VolumeBounds[1] = m_SliceActor_sagittal->getX();
            break;
        case 2:
            m_VolumeBounds[0] = m_SliceActor_sagittal->getX();
            m_VolumeBounds[1] = bounds[1];
            break;
        default:
            break;
    }
    switch (m_CropType_coronal)
    {
        case 0:
            m_VolumeBounds[2] = bounds[2];
            m_VolumeBounds[3] = bounds[3];
            break;
        case 1:
            m_VolumeBounds[2] = bounds[2];
            m_VolumeBounds[3] = m_SliceActor_coronal->getY();
            break;
        case 2:
            m_VolumeBounds[2] = m_SliceActor_coronal->getY();
            m_VolumeBounds[3] = bounds[3];
            break;
        default:
            break;
    }

    m_VolumeMapper_volume->SetCroppingRegionPlanes ( m_VolumeBounds );

    m_RenderingWidget_volume->GetRenderWindow()->Render();
}

/**
 *  \brief Quit
 */
void MainWidget::fileQuit()
{
    close();
}


/**
 * \brief About Dialog
 * \todo About Dialog
 */
void MainWidget::About()
{
    std::cout << "About Dialog" << std::endl;
//    QDialog about(this);
//    about.exec();
}

/**
 * \brief Handbook
 * \todo Handbook
 */
void MainWidget::Handbook()
{
    std::cout << "Handbook" << std::endl;
}

#ifndef SV_DICEVIEWER

#include "libfilter/filterDialog.h"
#include "libfilter/convertFilter.h"
#include "libfilter/convertSeriesFilter.h"
#include "libfilter/padFilter.h"
#include "libfilter/testFilter.h"
#include "libfilter/sigmoidFilter.h"
#include "libfilter/orientFilter.h"
#include "libfilter/rescaleFilter.h"
#include "libfilter/rescaleInt2UCharFilter.h"
#include "libfilter/rescaleUShort2UCharFilter.h"
#include "libfilter/derivativeFilter.h"
#include "libfilter/gradientMagnitudeFilter.h"
#include "libfilter/hessianFilter.h"
#include "libfilter/binaryThresholdFilter.h"
#include "libfilter/maximumFilter.h"
#include "libfilter/minimumFilter.h"
#include "libfilter/connectedThresholdFilter.h"
#include "libfilter/fastMarchingFilter.h"
#include "libfilter/geodesicActiveContourFilter.h"
#include "libfilter/geodesicActiveContourFullFilter.h"
#include "libfilter/betFilter.h"
#include "libfilter/binaryThinningFilter.h"
#include "libfilter/binaryPruningFilter.h"
#include "libfilter/binaryErodeFilter.h"
#include "libfilter/binaryDilateFilter.h"
#include "libfilter/DanielssonDistanceMapFilter.h"
#include "libfilter/electrodeAxisExtractionFilter.h"
#include "libfilter/electrodeAxisExtractionFilter2.h"
#include "libfilter/electrodeAxisExtractionFilter3.h"
#include "libfilter/skelInsertFilter.h"
#include "libfilter/applyRigidTransformFilter.h"



void MainWidget::filterConvert()
{
    filter::convertFilter* convert = new filter::convertFilter;
    filter::filterDialog dialog(convert, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete convert;
}

void MainWidget::filterConvertSeries()
{
    filter::convertSeriesFilter* convertSeries = new filter::convertSeriesFilter;
    filter::filterDialog dialog(convertSeries, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete convertSeries;
}

void MainWidget::filterPad()
{
    filter::padFilter* pad = new filter::padFilter;
    filter::filterDialog dialog(pad, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete pad;
}

void MainWidget::filterTest()
{
    filter::testFilter* test = new filter::testFilter;
    filter::filterDialog dialog(test, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete test;
}

void MainWidget::filterRescale()
{
    filter::rescaleFilter* rescale = new filter::rescaleFilter;
    filter::filterDialog dialog(rescale, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete rescale;
}

void MainWidget::filterRescaleInt2UChar()
{
    filter::rescaleInt2UCharFilter* rescaleInt2UChar = new filter::rescaleInt2UCharFilter;
    filter::filterDialog dialog(rescaleInt2UChar, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete rescaleInt2UChar;
}

void MainWidget::filterRescaleUShort2UChar()
{
    filter::rescaleUShort2UCharFilter* rescaleUShort2UChar = new filter::rescaleUShort2UCharFilter;
    filter::filterDialog dialog(rescaleUShort2UChar, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete rescaleUShort2UChar;
}

void MainWidget::filterOrient()
{
    filter::orientFilter* orient = new filter::orientFilter;
    filter::filterDialog dialog(orient, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete orient;
}

void MainWidget::filterSigmoid()
{
    filter::sigmoidFilter* sigmoid = new filter::sigmoidFilter;
    filter::filterDialog dialog(sigmoid, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete sigmoid;
}

void MainWidget::filterDerivative()
{
    filter::derivativeFilter* derivative = new filter::derivativeFilter;
    filter::filterDialog dialog(derivative, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete derivative;
}

void MainWidget::filterGradientMagnitude()
{
    filter::gradientMagnitudeFilter* gradientMagnitude = new filter::gradientMagnitudeFilter;
    filter::filterDialog dialog(gradientMagnitude, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete gradientMagnitude;
}

void MainWidget::filterHessian()
{
    filter::hessianFilter* hessian = new filter::hessianFilter;
    filter::filterDialog dialog(hessian, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete hessian;
}

void MainWidget::filterBinaryThreshold()
{
    filter::binaryThresholdFilter* binaryThreshold = new filter::binaryThresholdFilter;
    filter::filterDialog dialog(binaryThreshold, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete binaryThreshold;
}

void MainWidget::filterMaximum()
{
    filter::maximumFilter* maximum = new filter::maximumFilter;
    filter::filterDialog dialog(maximum, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete maximum;
}

void MainWidget::filterMinimum()
{
    filter::minimumFilter* minimum = new filter::minimumFilter;
    filter::filterDialog dialog(minimum, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete minimum;
}

void MainWidget::segmentationConnectedThreshold()
{
    filter::connectedThresholdFilter* connectedThreshold = new filter::connectedThresholdFilter;
    filter::filterDialog dialog(connectedThreshold, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete connectedThreshold;
}

void MainWidget::segmentationFastMarching()
{
    filter::fastMarchingFilter* fastMarching = new filter::fastMarchingFilter;
    filter::filterDialog dialog(fastMarching, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete fastMarching;
}

void MainWidget::segmentationGeodesicActiveContour()
{
    filter::geodesicActiveContourFilter* geodesicActiveContour = new filter::geodesicActiveContourFilter;
    filter::filterDialog dialog(geodesicActiveContour, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete geodesicActiveContour;
}

void MainWidget::segmentationGeodesicActiveContourFull()
{
    filter::geodesicActiveContourFullFilter* geodesicActiveContourFull = new filter::geodesicActiveContourFullFilter;
    filter::filterDialog dialog(geodesicActiveContourFull, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete geodesicActiveContourFull;
}

void MainWidget::segmentationBet()
{
    filter::betFilter* bet = new filter::betFilter;
    filter::filterDialog dialog(bet, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete bet;
}


void MainWidget::skeletonizationBinaryThinning()
{
    filter::binaryThinningFilter* binaryThinning = new filter::binaryThinningFilter;
    filter::filterDialog dialog(binaryThinning, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete binaryThinning;
}


void MainWidget::skeletonizationBinaryPruning()
{
    filter::binaryPruningFilter* binaryPruning = new filter::binaryPruningFilter;
    filter::filterDialog dialog(binaryPruning, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete binaryPruning;
}

void MainWidget::skeletonizationBinaryErode()
{
    filter::binaryErodeFilter* binaryErode = new filter::binaryErodeFilter;
    filter::filterDialog dialog(binaryErode, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete binaryErode;
}


void MainWidget::skeletonizationBinaryDilate()
{
    filter::binaryDilateFilter* binaryDilate = new filter::binaryDilateFilter;
    filter::filterDialog dialog(binaryDilate, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete binaryDilate;
}

void MainWidget::skeletonizationDanielssonDistanceMap()
{
    filter::DanielssonDistanceMapFilter* DanielssonDistanceMap = new filter::DanielssonDistanceMapFilter;
    filter::filterDialog dialog(DanielssonDistanceMap, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete DanielssonDistanceMap;
}

void MainWidget::skeletonizationElectrodeAxisExtraction()
{
    filter::electrodeAxisExtractionFilter* electrodeAxisExtraction = new filter::electrodeAxisExtractionFilter;
    filter::filterDialog dialog(electrodeAxisExtraction, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete electrodeAxisExtraction;
}

void MainWidget::skeletonizationElectrodeAxisExtraction2()
{
    filter::electrodeAxisExtractionFilter2* electrodeAxisExtraction2 = new filter::electrodeAxisExtractionFilter2;
    filter::filterDialog dialog(electrodeAxisExtraction2, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete electrodeAxisExtraction2;
}

void MainWidget::skeletonizationElectrodeAxisExtraction3()
{
    filter::electrodeAxisExtractionFilter3* electrodeAxisExtraction3 = new filter::electrodeAxisExtractionFilter3;
    filter::filterDialog dialog(electrodeAxisExtraction3, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete electrodeAxisExtraction3;
}

void MainWidget::skelInsert()
{
    filter::skelInsertFilter* skelInsert = new filter::skelInsertFilter;
    filter::filterDialog dialog(skelInsert, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete skelInsert;
}

void MainWidget::registrationApplyRigidTransform()
{
    filter::applyRigidTransformFilter* applyRigidTransform = new filter::applyRigidTransformFilter;
    filter::filterDialog dialog(applyRigidTransform, this);
    int result = dialog.exec();
    std::cout << "Result = " << result << std::endl;
    delete applyRigidTransform;
}

#endif // SV_DICEVIEWER


/*    ---> BACKGROUND <--
	rend->SetBackground(0.7,0.7,0.5);
*/

/*    ---> DICOM <---
	// Legge l'immagine
	vtkDICOMImageReader* dread = vtkDICOMImageReader::New();
	dread->SetDirectoryName ( "/home/biolab/DADO/risonanze/pivan/s0004" );

---

	dread->Delete();

*/



/*   ---> PICKER <---


void MainWidget::PickerInteractionCallback( vtkObject* vtkNotUsed(object),
                                       unsigned long event,
                                       void* clientdata,
                                       void* vtkNotUsed(calldata) )
{
    vtkInteractorStyleTrackballCamera * style =
(vtkInteractorStyleTrackballCamera*)clientdata;
    switch( event )
    {
    case vtkCommand::LeftButtonPressEvent:
        MouseMotion = 0;
        style->OnLeftButtonDown();
        break;
    case vtkCommand::LeftButtonReleaseEvent:
        if (MouseMotion == 0)
        {
            int *pick = iren->GetEventPosition();
            picker->Pick((double)pick[0], (double)pick[1], 0.0, ren1);
        }
        style->OnLeftButtonUp();
        break;
    case vtkCommand::MouseMoveEvent:
        MouseMotion = 1;
        style->OnMouseMove();
        break;
    }
}


*/

