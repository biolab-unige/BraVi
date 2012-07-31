#include "config.h"

#include <QApplication>
#include <QPlastiqueStyle>

#include <QPixmap>
#include <QBitmap>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QDir>

#include "MainWidget.h"

#ifdef WIN32
//Header per windows - contengono Sleep()
  #include <windows.h>
  #include <winbase.h>
#else
  #include <unistd.h>
#endif // #ifdef WIN32




int main (int argc, char **argv)
{
  Q_INIT_RESOURCE(resources);

  QApplication app(argc, argv);
  app.setStyle(new QPlastiqueStyle);

  QDir::setCurrent(QDir::homePath());

  QPixmap pixmap(":/res/BraViSplash.png");
  QSplashScreen* splash = new QSplashScreen(pixmap);
  splash->setMask(pixmap.mask());
  splash->show();

  app.processEvents();

#ifndef SV_FASTLOAD
  #ifdef WIN32
    Sleep(2000);
  #else // #ifdef WIN32
    sleep(2);
  #endif // #ifdef WIN32
#endif // SV_FASTLOAD

  MainWidget* mainWidget = new MainWidget();
  mainWidget->setWindowIcon(QIcon(":/res/BraVi.png"));

//TODO
//WARNING
//DANGER
  if (argc ==2)
    mainWidget->loadOnStart(argv[1]);



  mainWidget->show();

  splash->finish(mainWidget);

  int ret = app.exec();

  delete mainWidget;
  delete splash;
  return ret;
}


/*
void center(QWidget *window)
{
  int scrn = 0;
  QWidget *w = window->topLevelWidget();
  if(w)
    scrn = QApplication::desktop()->screenNumber(w);
  else if(QApplication::desktop()->isVirtualDesktop())
    scrn = QApplication::desktop()->screenNumber(QCursor::pos());
  else
    scrn = QApplication::desktop()->screenNumber(window);

  QRect desk(QApplication::desktop()->availableGeometry(scrn));
#define MOVE_DOWN
#ifdef MOVE_DOWN
  window->move( (desk.width()  -  window->frameGeometry().width()  ) / 2,
                (desk.height() -  window->frameGeometry().height() ) / 2 + 60);
#else // MOVE_DOWN
  window->move( (desk.width()  -  window->frameGeometry().width()  ) / 2,
                (desk.height() -  window->frameGeometry().height() ) / 2);
#endif // MOVE_DOWN
}
*/
