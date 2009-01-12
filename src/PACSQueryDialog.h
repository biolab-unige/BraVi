#ifndef PACSQUERYDIALOG_H
#define PACSQUERYDIALOG_H

#include <QDialog>
#include <QApplication>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>


class PACSQueryDialog : public QDialog
{
  Q_OBJECT
public:
  PACSQueryDialog(QWidget *parent = 0);
  ~PACSQueryDialog();

private:
  QVBoxLayout *vLayout;
  QFormLayout *formLayout;

  QLabel *ServerLabel;
  QLabel *PatientNameLabel;
  QLineEdit *ServerLineEdit;
  QLineEdit *PatientNameLineEdit;

  QPushButton *queryButton;

  QPlainTextEdit *outputTextEdit;


  void setupUi(QDialog *Dialog);
  void retranslateUi(QDialog *Dialog);



private slots:
  void RunQuery();

};


#endif // DESIGNERIZZ756_H
