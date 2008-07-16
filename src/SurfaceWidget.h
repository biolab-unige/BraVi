#ifndef SURFACEWIDGET_H
#define SURFACEWIDGET_H

#include "Surface3DActor.h"

#include <list>

#include <QGroupBox>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QMenu>
#include <QAction>

#include <vtkRenderer.h>

class SurfaceWidget : public QGroupBox
{
  Q_OBJECT

public:
  SurfaceWidget(QWidget *parent = 0, int number = -1);
  ~SurfaceWidget();

private:
  Surface3DActor* m_Surface3DActor;

  QVBoxLayout*   m_Layout;
  QGroupBox*     m_GroupBox;

  bool m_Loaded;
  QString m_Name;

  typedef std::list<vtkRenderer*> RendererListType;
  RendererListType m_RendererList;

public:
  inline bool isLoaded() const {return m_Loaded;}
  inline QMenu* getMenu() const {return m_Menu_Surface;}
  inline vtkActor* getActor() const {return m_Surface3DActor;}
//  void showActor(bool show = true, vtkRenderer* rend);
//  void hideActor(bool hide = true);

  void addRenderer (vtkRenderer* rend);
  void render();

  void setDisabled(bool disabled);
  void connectWidgets();
  void disconnectWidgets();
  void resetWidgets();

//------------------------------------------------------------------------------

// Menu
private:
  QMenu* m_Menu_Surface;
  QAction* m_Action_Load;
  QAction* m_Action_Close;

  void createMenu();
  void connectMenu();
  void disconnectMenu();
  void setMenuDisabled(bool disabled);

  void createLayout();

private slots:
  void loadSurface();
public slots:
  void closeSurface();

//------------------------------------------------------------------------------

// Value
private:
  QHBoxLayout*   m_HBoxLayout_value;
  QLabel*        m_Label_value;
  QSlider*       m_Slider_value;
  QSpinBox*      m_SpinBox_value;
  QPushButton*   m_PushButton_value;

  void createValueWidgets();
  void resetValueWidgets();
  void setValueWidgets(double minimum, double maximum, double value = 255, double singlestep = 1);
  void connectValueWidgets();
  void disconnectValueWidgets();
  void setValueWidgetsDisabled(bool disabled = true);

private slots:
  void setSurfaceValue( int value );
  void applySurfaceValue();

//------------------------------------------------------------------------------

// Representation
private:
  QComboBox*     m_ComboBox_representation;

  void createRepresentationWidgets();
  void connectRepresentationWidgets();
  void disconnectRepresentationWidgets();
  void resetRepresentationWidgets();
  void setRepresentationWidgetsDisabled(bool disabled = true);

private slots:
  void setSurfaceRepresentation( int index );

//------------------------------------------------------------------------------


// Color
private:
  QComboBox*     m_ComboBox_color;

  void createColorWidgets();
  void connectColorWidgets();
  void disconnectColorWidgets();
  void resetColorWidgets();
  void setColorWidgetsDisabled(bool disabled = true);

private slots:
  void setSurfaceColor( int index );







#endif // SURFACEWIDGET_H


// Funzione per restituire il menu
// Funzione per restituire l'actor
// Funzione per visualizzare o nascondere l'actor

};
