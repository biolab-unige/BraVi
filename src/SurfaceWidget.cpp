#include "SurfaceWidget.h"
#include "vtkGenericMedicalImageReader.h"
#include "ReadImageDialog.h"

#include <vtkRenderWindow.h>
#include <vtkProperty.h>



/*!
 * \brief Costruttore
 */
SurfaceWidget::SurfaceWidget( QWidget *parent, int number ) :
  QGroupBox (parent),
  m_Surface3DActor(NULL),
  m_Loaded(false)
{
  if (number < 0 )
    m_Name = tr ( "Surface" );
  else
    m_Name = QString(tr("Surface %1")).arg(number);

  this->setTitle( m_Name );

  this->createMenu();
  this->createLayout();
  this->resetWidgets();
  this->setDisabled(true);

  this->connectMenu();
}

//------------------------------------------------------------------------------

/*!
 * \brief Distruttore
 */
SurfaceWidget::~SurfaceWidget()
{
  if (m_Loaded)
    closeSurface();
}

//------------------------------------------------------------------------------

/*!
 * \brief Disables all widgets
 */
void SurfaceWidget::setDisabled(bool disabled)
{
  setMenuDisabled(disabled);
  setValueWidgetsDisabled(disabled);
  setRepresentationWidgetsDisabled(disabled);
  setColorWidgetsDisabled(disabled);
}

/*!
 * \brief Connects all widgets
 */
void SurfaceWidget::connectWidgets()
{
  connectValueWidgets();
  connectRepresentationWidgets();
  connectColorWidgets();
}

/*!
 * \brief Disconnects all widgets
 */
void SurfaceWidget::disconnectWidgets()
{
  disconnectValueWidgets();
  disconnectRepresentationWidgets();
  disconnectColorWidgets();
}

/*!
 * \brief Resets all widgets
 */
void SurfaceWidget::resetWidgets()
{
  resetValueWidgets();
  resetRepresentationWidgets();
  resetColorWidgets();
}

//------------------------------------------------------------------------------

/*!
 * \brief Crea i pulsanti da aggiungere al menu
 */
void SurfaceWidget::createMenu()
{
  m_Action_Load = new QAction ( tr ( "Load" ), this );
  m_Action_Load->setStatusTip ( tr ( "Load" ) );

  m_Action_Close = new QAction ( tr ( "Close" ), this );
  m_Action_Close->setStatusTip ( tr ( "Close" ) );

  m_Menu_Surface = new QMenu ( m_Name );

  m_Menu_Surface->addAction ( m_Action_Load );
  m_Menu_Surface->addAction ( m_Action_Close );
}

/*!
 * \brief Connette i pulsanti del menu
 */
void SurfaceWidget::connectMenu()
{
  connect ( m_Action_Load,  SIGNAL ( triggered() ), this, SLOT ( loadSurface()  ) );
  connect ( m_Action_Close, SIGNAL ( triggered() ), this, SLOT ( closeSurface() ) );
}

/*!
 * \brief Disconnette i pulsanti del menu
 */
void SurfaceWidget::disconnectMenu()
{
  disconnect ( m_Action_Load,  SIGNAL ( triggered() ), this, SLOT ( loadSurface() ) );
  disconnect ( m_Action_Close, SIGNAL ( triggered() ), this, SLOT ( closeSurface() ) );
}

/*!
 * \brief Disabilita i pulsanti del menu
 */
void SurfaceWidget::setMenuDisabled(bool disabled)
{
  m_Action_Close->setDisabled(disabled);
}

//------------------------------------------------------------------------------

/*!
 * \brief Crea i widget per modificare "Value"
 */
void SurfaceWidget::createValueWidgets()
{
  //Label
  m_Label_value = new QLabel( tr("Value") );

  //Slider
  m_Slider_value = new QSlider ( Qt::Horizontal );
  m_Slider_value->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );

  //SpinBox
  m_SpinBox_value = new QSpinBox ();

  m_SpinBox_value->setPalette ( QPalette ( QColor ( Qt::lightGray ) ) );

  //Button
  m_PushButton_value = new QPushButton ("Apply");

  //Layout
  m_HBoxLayout_value = new QHBoxLayout;
  m_HBoxLayout_value->setContentsMargins(0,0,0,0);
  m_HBoxLayout_value->setSpacing(1);

  m_HBoxLayout_value->addWidget(m_Label_value);
  m_HBoxLayout_value->addWidget(m_Slider_value);
  m_HBoxLayout_value->addWidget(m_SpinBox_value);
  m_HBoxLayout_value->addWidget(m_PushButton_value);

  resetValueWidgets();
}


/*!
 * \brief Resetta i valori widget per modificare "Value"
 */
void SurfaceWidget::resetValueWidgets()
{
  setValueWidgets(1,1,1,1);
}


/*!
 * \brief Setta i valori widget per modificare "Value"
 */
void SurfaceWidget::setValueWidgets(double minimum, double maximum, double value, double singlestep)
{
  m_Slider_value->setMinimum ( minimum );
  m_Slider_value->setMaximum ( maximum );
  m_Slider_value->setValue ( value );
  m_Slider_value->setSingleStep ( singlestep );

  m_SpinBox_value->setMinimum ( minimum );
  m_SpinBox_value->setMaximum ( maximum );
  m_SpinBox_value->setValue ( value );
  m_SpinBox_value->setSingleStep ( singlestep );
}


/*!
 * \brief Disabilita i widget per modificare "Value"
 */
void SurfaceWidget::setValueWidgetsDisabled(bool disabled)
{
  m_Label_value->setDisabled(disabled);
  m_Slider_value->setDisabled(disabled);
  m_SpinBox_value->setDisabled(disabled);
  m_PushButton_value->setDisabled(disabled);
}


/*!
 * \brief Connette i widget per modificare "Value"
 */
void SurfaceWidget::connectValueWidgets()
{
  connect (m_Slider_value,     SIGNAL ( valueChanged( int ) ), this, SLOT ( setSurfaceValue( int ) ));
  connect (m_SpinBox_value,    SIGNAL ( valueChanged( int ) ), this, SLOT ( setSurfaceValue( int ) ));
  connect (m_PushButton_value, SIGNAL ( clicked() ),           this, SLOT ( applySurfaceValue()    ));
}


/*!
 * \brief Disconnette i widget per modificare "Value"
 */
void SurfaceWidget::disconnectValueWidgets()
{
  disconnect (m_Slider_value,     SIGNAL ( valueChanged( int ) ), this, SLOT ( setSurfaceValue( int ) ));
  disconnect (m_SpinBox_value,    SIGNAL ( valueChanged( int ) ), this, SLOT ( setSurfaceValue( int ) ));
  disconnect (m_PushButton_value, SIGNAL ( clicked() ),           this, SLOT ( applySurfaceValue()    ));
}


/*!
 * \brief Modifica "Value" (ma non lo applica)
 */
void SurfaceWidget::setSurfaceValue(int value)
{
  m_Slider_value->setValue(value);
  m_SpinBox_value->setValue(value);

  m_PushButton_value->setDisabled(false);
}


/*!
 * \brief Applica "Value"
 */
void SurfaceWidget::applySurfaceValue()
{
  m_Surface3DActor->setSurface(m_SpinBox_value->value());
  m_PushButton_value->setDisabled(true);
  render();
}



//------------------------------------------------------------------------------


/*!
 * \brief Crea i widget per modificare "Representation"
 */
void SurfaceWidget::createRepresentationWidgets()
{
  m_ComboBox_representation = new QComboBox( this );
  m_ComboBox_representation->addItem( tr("Surface") );
  m_ComboBox_representation->addItem( tr("Wireframe") );
  m_ComboBox_representation->addItem( tr("Points") );

  resetRepresentationWidgets();
}


/*!
 * \brief Connette i widget per modificare "Representation"
 */
void SurfaceWidget::connectRepresentationWidgets()
{
  connect ( m_ComboBox_representation, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setSurfaceRepresentation ( int ) ) );
}


/*!
 * \brief Disconnette i widget per modificare "Representation"
 */
void SurfaceWidget::disconnectRepresentationWidgets()
{
  disconnect ( m_ComboBox_representation, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setSurfaceRepresentation ( int ) ) );
}


/*!
 * \brief Resetta i valori widget per modificare "Representation"
 */
void SurfaceWidget::resetRepresentationWidgets()
{
  m_ComboBox_representation->setCurrentIndex(0);
}


/*!
 * \brief Disabilita i widget per modificare "Representation"
 */
void SurfaceWidget::setRepresentationWidgetsDisabled(bool disabled)
{
    m_ComboBox_representation->setDisabled(disabled);
}


/*!
 * \brief Modifica "Representation"
 */
void SurfaceWidget::setSurfaceRepresentation(int index)
{
  switch (index)
  {
    case 0:
      m_Surface3DActor->GetProperty()->SetRepresentationToSurface();
      break;
    case 1:
      m_Surface3DActor->GetProperty()->SetRepresentationToWireframe();
      break;
    case 2:
      m_Surface3DActor->GetProperty()->SetRepresentationToPoints();
      break;
    default:
      break;
  }
  render();
}



//------------------------------------------------------------------------------


/*!
 * \brief Crea i widget per modificare "Color"
 */
void SurfaceWidget::createColorWidgets()
{
  m_ComboBox_color = new QComboBox( this );
  m_ComboBox_color->addItem( tr("Red") );
  m_ComboBox_color->addItem( tr("Green") );
  m_ComboBox_color->addItem( tr("Blue") );
  m_ComboBox_color->addItem( tr("Metal") );
  m_ComboBox_color->addItem( tr("Skin") );
  m_ComboBox_color->setCurrentIndex(0);

  resetColorWidgets();
}


/*!
 * \brief Connette i widget per modificare "Color"
 */
void SurfaceWidget::connectColorWidgets()
{
  connect ( m_ComboBox_color, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setSurfaceColor ( int ) ) );
}


/*!
 * \brief Disconnette i widget per modificare "Color"
 */
void SurfaceWidget::disconnectColorWidgets()
{
  disconnect ( m_ComboBox_color, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setSurfaceColor ( int ) ) );
}


/*!
 * \brief Resetta i valori widget per modificare "Color"
 */
void SurfaceWidget::resetColorWidgets()
{
  m_ComboBox_color->setCurrentIndex(0);
}


/*!
 * \brief Disabilita i widget per modificare "Color"
 */
void SurfaceWidget::setColorWidgetsDisabled(bool disabled)
{
  m_ComboBox_color->setDisabled(disabled);
}


/*!
 * \brief Modifica "Color"
 */
void SurfaceWidget::setSurfaceColor(int index)
{
  switch (index)
  {
    case 0:
      m_Surface3DActor->setColor(Surface3DActor::COLOR_RED);
      break;
    case 1:
      m_Surface3DActor->setColor(Surface3DActor::COLOR_GREEN);
      break;
    case 2:
      m_Surface3DActor->setColor(Surface3DActor::COLOR_BLUE);
      break;
    case 3:
      m_Surface3DActor->setColor(Surface3DActor::COLOR_METAL);
      break;
    case 4:
      m_Surface3DActor->setColor(Surface3DActor::COLOR_SKIN);
      break;
    default:
      break;
  }
  render();
}


//------------------------------------------------------------------------------


/*!
 * \brief Crea il layout
 */
void SurfaceWidget::createLayout()
{
  m_Layout = new QVBoxLayout();
  m_Layout->setContentsMargins(0,0,0,0);
  m_Layout->setSpacing(1);

  createValueWidgets();
  m_Layout->addLayout ( m_HBoxLayout_value );

  createRepresentationWidgets();
  m_Layout->addWidget ( m_ComboBox_representation );

  createColorWidgets();
  m_Layout->addWidget ( m_ComboBox_color );

  this->setLayout(m_Layout);
}



//------------------------------------------------------------------------------



/*!
 * \brief Legge l'immagine
 */
void SurfaceWidget::loadSurface()
{
  ReadImageDialog dialog(this, "Load " + m_Name);
  if ( dialog.exec() )
  {
    // Se c'è una superficie già aperta la chiude
    if (m_Loaded)
      closeSurface();


    std::cout << "filename: " << dialog.getFilename().c_str() << std::endl;
    // Legge l'immagine
    vtkGenericMedicalImageReader* reader = vtkGenericMedicalImageReader::New();
    if (dialog.useDirectory())
      reader->SetDirectoryName(dialog.getDirectory().c_str());
    else
      reader->SetFileName (dialog.getFilename().c_str());
    reader->SetImageType(dialog.getImageType());
    reader->Update();

    // Crea l'actor
    m_Surface3DActor = Surface3DActor::New();
    m_Surface3DActor->SetInput ( reader->GetOutput() );

    // Setta il range
    double range[2];
    reader->GetOutput()->GetScalarRange(range);
    setValueWidgets(range[0]+1, range[1], range[0]+1, 1);


    // Connette i widgets, eccetera
    setDisabled(false);
    connectWidgets();

    m_Loaded = true;
    reader->Delete();

    applySurfaceValue();

    for(RendererListType::iterator iter = m_RendererList.begin(); iter != m_RendererList.end(); iter++)
    {
      (*iter)->AddActor(m_Surface3DActor);
      (*iter)->ResetCamera();
    }

    render();
  }
}


void SurfaceWidget::closeSurface()
{
  if (m_Loaded)
  {
    disconnectWidgets();
    resetWidgets();
    setDisabled(true);

    for(RendererListType::iterator iter = m_RendererList.begin(); iter != m_RendererList.end(); iter++)
      (*iter)->RemoveActor(m_Surface3DActor);

    render();

    m_Surface3DActor->Delete();
    m_Surface3DActor = NULL;

    m_Loaded = false;
  }
}


void SurfaceWidget::render()
{
  for(RendererListType::iterator iter = m_RendererList.begin(); iter != m_RendererList.end(); iter++)
    (*iter)->GetRenderWindow()->Render();
}


void SurfaceWidget::addRenderer (vtkRenderer* rend)
{
  m_RendererList.push_back(rend);
  if (m_Loaded)
  {
    rend->AddActor(m_Surface3DActor);
    rend->GetRenderWindow()->Render();
  }
}



/*

#ifdef SV_3_SURFACES
        if (!isVolumeLoaded && !isSurface2Loaded && !isSurface3Loaded)
#else // SV_3_SURFACES
        if (!isVolumeLoaded)
#endif // SV_3_SURFACES
            m_Rend_volume->ResetCamera();
        showOrientationMarker();
            hideOrientationMarker();
*/
