#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "config.h"

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDockWidget>
#include <QBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>

#include <vtkVolumeTextureMapper3D.h>
#include <vtkImageViewer2.h>
#include <vtkOrientationMarkerWidget.h>


#include "SliceActor.h"
#include "Volume3DActor.h"
#include "Surface3DActor.h"
#include "QVTKSquareWidget.h"

#include "SurfaceWidget.h"


#ifdef SV_3_SURFACES
  #define NUM_SURFACES 3
#else
  #define NUM_SURFACES 1
#endif

class vtkGenericMedicalImageReader;

class MainWidget : public QMainWindow
{
  Q_OBJECT

public:
  MainWidget(QWidget *parent = 0);
  ~MainWidget();

private:

//QVTK Widgets
  QVTKSquareWidget* m_RenderingWidget_volume;
  QVTKSquareWidget* m_RenderingWidget_axial;
  QVTKSquareWidget* m_RenderingWidget_sagittal;
  QVTKSquareWidget* m_RenderingWidget_coronal;
  QVTKSquareWidget* m_RenderingWidget_slice;

//VTK
  vtkRenderer* m_Rend_volume;
  vtkImageViewer2* m_ImageViewer_axial;
  vtkImageViewer2* m_ImageViewer_coronal;
  vtkImageViewer2* m_ImageViewer_sagittal;
  vtkImageViewer2* m_ImageViewer_slice;

  Volume3DActor* m_Volume3DActor_volume;
  vtkOrientationMarkerWidget* m_OrientationMarker_volume;
  vtkVolumeMapper* m_VolumeMapper_volume;

  SliceActor* m_SliceActor_axial;
  SliceActor* m_SliceActor_sagittal;
  SliceActor* m_SliceActor_coronal;
  SliceActor* m_SliceActor_slice;

  vtkActor* m_OutlineActor_volume;
  vtkActor* m_OutlineActor_axial;
  vtkActor* m_OutlineActor_volume_axial;
  vtkActor* m_OutlineActor_sagittal;
  vtkActor* m_OutlineActor_volume_sagittal;
  vtkActor* m_OutlineActor_coronal;
  vtkActor* m_OutlineActor_volume_coronal;
  vtkActor* m_OutlineActor_slice;
  vtkActor* m_OutlineActor_volume_slice;

  SurfaceWidget* m_SurfaceWidget[NUM_SURFACES];

//QT Widgets

#ifdef SV_SIMPLEINTERFACE
  QGridLayout* m_Layout_main;
  QWidget* m_Widget_main;
#else // SV_SIMPLEINTERFACE
// Multiple Document Interface
  QMdiArea* m_MdiArea;
  QMdiSubWindow* m_VolumeMdiSubWindow;
  QMdiSubWindow* m_SlicesMdiSubWindow;

// Dock Widgets
  QDockWidget* m_orthoOptionsDockWidget;
  QDockWidget* m_sliceOptionsDockWidget;
  QDockWidget* m_surfaceOptionsDockWidget[NUM_SURFACES];

#endif // SV_SIMPLEINTERFACE

  //Axial Controls
  QGroupBox*     m_GroupBox_controls_axial;
  QVBoxLayout*   m_Layout_controls_axial;
   QHBoxLayout*   m_HBox_axial;
   QSlider*       m_Slider_axial;
   QSpinBox*      m_SpinBox_axial;
   QComboBox*     m_ComboBox_axial;

  //Sagittal Controls
  QGroupBox*     m_GroupBox_controls_sagittal;
  QVBoxLayout*   m_Layout_controls_sagittal;
   QHBoxLayout*   m_HBox_sagittal;
   QSlider*       m_Slider_sagittal;
   QSpinBox*      m_SpinBox_sagittal;
   QComboBox*     m_ComboBox_sagittal;

  //Coronal Controls
  QGroupBox*     m_GroupBox_controls_coronal;
  QVBoxLayout*   m_Layout_controls_coronal;
   QHBoxLayout*   m_HBox_coronal;
   QSlider*       m_Slider_coronal;
   QSpinBox*      m_SpinBox_coronal;
   QComboBox*     m_ComboBox_coronal;

  //Slice Controls
  QGroupBox*     m_GroupBox_controls_slice;
  QGridLayout*   m_Layout_controls_slice;
   //Slice Controls X
   QLabel*        m_Label_slice_X;
   QSlider*       m_Slider_slice_X;
   QSpinBox*      m_SpinBox_slice_X;
   //Slice Controls Y
   QLabel*        m_Label_slice_Y;
   QSlider*       m_Slider_slice_Y;
   QSpinBox*      m_SpinBox_slice_Y;
   //Slice Controls Z
   QLabel*        m_Label_slice_Z;
   QSlider*       m_Slider_slice_Z;
   QSpinBox*      m_SpinBox_slice_Z;
   //Slice Controls alpha
   QLabel*        m_Label_slice_alpha;
   QSlider*       m_Slider_slice_alpha;
   QSpinBox*      m_SpinBox_slice_alpha;
   //Slice Controls beta
   QLabel*        m_Label_slice_beta;
   QSlider*       m_Slider_slice_beta;
   QSpinBox*      m_SpinBox_slice_beta;
   //Slice Controls gamma
   QLabel*        m_Label_slice_gamma;
   QSlider*       m_Slider_slice_gamma;
   QSpinBox*      m_SpinBox_slice_gamma;
   //Other
   QComboBox*     m_ComboBox_slice;


  QGroupBox*   m_GroupBox_view_volume;
  QVBoxLayout* m_Layout_view_volume;
  QGroupBox*   m_GroupBox_view_axial;
  QVBoxLayout* m_Layout_view_axial;
  QGroupBox*   m_GroupBox_view_sagittal;
  QVBoxLayout* m_Layout_view_sagittal;
  QGroupBox*   m_GroupBox_view_coronal;
  QVBoxLayout* m_Layout_view_coronal;
  QGroupBox*   m_GroupBox_view_slice;
  QVBoxLayout* m_Layout_view_slice;

  QWidget* m_Widget_allslices;
  QGridLayout* m_Layout_view_allslices;

  QLayout* m_Layout_controls_ortho;
  QWidget* m_Widget_controls_ortho;

//Menu
  // File Menu
  QMenu*   m_Menu_File;
  QAction* m_Action_File_LoadVolume;
/*
  QAction* m_Action_File_LoadSurface;
#ifdef SV_3_SURFACES
  QAction* m_Action_File_LoadSurface2;
  QAction* m_Action_File_LoadSurface3;
#endif // SV_3_SURFACES
*/
  QAction* m_Action_File_CloseVolume;
/*
  QAction* m_Action_File_CloseSurface;
#ifdef SV_3_SURFACES
  QAction* m_Action_File_CloseSurface2;
  QAction* m_Action_File_CloseSurface3;
#endif // SV_3_SURFACES
*/
  QAction* m_Action_File_CloseAll;
  QAction* m_Action_File_Quit;

#ifndef SV_DICEVIEWER
  // Operations Menu
  QMenu*   m_Menu_Operations;

  QMenu* m_Menu_Operations_Filter;
  QAction* m_Action_Operations_Filter_Convert;
  QAction* m_Action_Operations_Filter_ConvertSeries;
  QAction* m_Action_Operations_Filter_Pad;
  QAction* m_Action_Operations_Filter_Rescale;
  QAction* m_Action_Operations_Filter_RescaleInt2UChar;
  QAction* m_Action_Operations_Filter_RescaleUShort2UChar;
  QAction* m_Action_Operations_Filter_Orient;
  QAction* m_Action_Operations_Filter_Sigmoid;
  QAction* m_Action_Operations_Filter_Derivative;
  QAction* m_Action_Operations_Filter_GradientMagnitude;
  QAction* m_Action_Operations_Filter_Hessian;
  QAction* m_Action_Operations_Filter_BinaryThreshold;
  QAction* m_Action_Operations_Filter_Maximum;
  QAction* m_Action_Operations_Filter_Minimum;
  QAction* m_Action_Operations_Filter_Test;

  QMenu* m_Menu_Operations_Segmentation;
  QAction* m_Action_Operations_Segmentation_connectedThreshold;
  QAction* m_Action_Operations_Segmentation_fastMarching;
  QAction* m_Action_Operations_Segmentation_geodesicActiveContour;
  QAction* m_Action_Operations_Segmentation_geodesicActiveContourFull;
  QAction* m_Action_Operations_Segmentation_Bet;

  QMenu* m_Menu_Operations_Skeletonization;
  QAction* m_Action_Operations_Skeletonization_BinaryThinning;
  QAction* m_Action_Operations_Skeletonization_BinaryPruning;
  QAction* m_Action_Operations_Skeletonization_BinaryErode;
  QAction* m_Action_Operations_Skeletonization_BinaryDilate;
  QAction* m_Action_Operations_Skeletonization_DanielssonDistanceMap;
  QAction* m_Action_Operations_Skeletonization_electrodeAxisExtraction;
  QAction* m_Action_Operations_Skeletonization_electrodeAxisExtraction2;
  QAction* m_Action_Operations_Skeletonization_electrodeAxisExtraction3;
  QAction* m_Action_Operations_Skeletonization_skelInsert;

  QMenu* m_Menu_Operations_Registration;
  QAction* m_Action_Operations_Registration_ApplyRigidTransform;
#endif // SV_DICEVIEWER


#ifndef SV_DICEVIEWER
  // Windows Menu
  QMenu* m_Menu_Window;

  // Actions for Mdi Area
  QAction* m_Menu_Window_Volume;
  QAction* m_Menu_Window_Slices;
  QAction* m_Menu_Window_Tile;
  QAction* m_Menu_Window_Cascade;
#endif // SV_DICEVIEWER


#ifndef SV_DICEVIEWER
  // Help Menu
  QMenu* m_Menu_Help;
  QAction* m_Menu_Help_Handbook;
  QAction* m_Menu_Help_AboutQt;
  QAction* m_Menu_Help_About;
#endif // SV_DICEVIEWER

  double m_VolumeBounds[6];

  int m_CropType_axial;
  int m_CropType_sagittal;
  int m_CropType_coronal;

private slots:

  void loadVolume();

  void setSlice_axial(int slice);
  void setSlice_sagittal(int slice);
  void setSlice_coronal(int slice);
  void setSlice_slice_X(int slice);
  void setSlice_slice_Y(int slice);
  void setSlice_slice_Z(int slice);
  void setSlice_slice_alpha(int angle);
  void setSlice_slice_beta(int angle);
  void setSlice_slice_gamma(int angle);

  void setCrop_axial(int index);
  void setCrop_sagittal(int index);
  void setCrop_coronal(int index);
  void setCrop_slice(int index);

  void showOrientationMarker();
  void hideOrientationMarker();

  void CloseVolume();

  void CloseAll();
  void fileQuit();

  void About();
  void Handbook();

#ifndef SV_DICEVIEWER
  void filterTest();
  void filterConvert();
  void filterConvertSeries();
  void filterPad();
  void filterRescale();
  void filterRescaleInt2UChar();
  void filterRescaleUShort2UChar();
  void filterOrient();
  void filterSigmoid();
  void filterDerivative();
  void filterGradientMagnitude();
  void filterHessian();
  void filterBinaryThreshold();
  void filterMaximum();
  void filterMinimum();
  void segmentationConnectedThreshold();
  void segmentationFastMarching();
  void segmentationGeodesicActiveContour();
  void segmentationGeodesicActiveContourFull();
  void segmentationBet();
  void skeletonizationBinaryThinning();
  void skeletonizationBinaryPruning();
  void skeletonizationBinaryErode();
  void skeletonizationBinaryDilate();
  void skeletonizationDanielssonDistanceMap();
  void skeletonizationElectrodeAxisExtraction();
  void skeletonizationElectrodeAxisExtraction2();
  void skeletonizationElectrodeAxisExtraction3();
  void skelInsert();
  void registrationApplyRigidTransform();
#endif // SV_DICEVIEWER

private:
  void CreateMenu();
  void CreateRenderingWidgets();
  void CreateLayout();
  void CreateAxialControls();
  void CreateSagittalControls();
  void CreateCoronalControls();
  void CreateSliceControls();

  void CreateImageViewer();
  void RenderAll();

  bool readImageDialog(vtkGenericMedicalImageReader* reader, QString title = "Load");

  void ResetControlsVolume();
  void ConnectControlsVolume();
  void DisconnectControlsVolume();
  void DisableControlsVolume(bool disabled = true);

  void CropVolume();

  void SetCameraVolumeFront();
  void SetCameraVolumeBack();
  void SetCameraVolumeLeft();
  void SetCameraVolumeRight();
  void SetCameraVolumeTop();
  void SetCameraVolumeBottom();


  vtkActor* boxActor(vtkImageData* input);


  void PickerInteractionCallback( vtkObject* vtkNotUsed(object),
                                  unsigned long event,
                                  void* clientdata,
                                  void* vtkNotUsed(calldata) );

  bool isVolumeLoaded;

  bool isOrientationMarkerLoaded;
};

#endif //MAINWIDGET_H
