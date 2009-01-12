#include "PACSQueryDialog.h"

#include <QFile>
#include <QTextStream>
#include <sstream>

PACSQueryDialog::PACSQueryDialog(QWidget *parent)
{
  setupUi(this);
  connect( queryButton, SIGNAL(clicked()), this, SLOT(RunQuery()) );
}

PACSQueryDialog::~PACSQueryDialog()
{
}

void PACSQueryDialog::RunQuery()
{
  std::ostringstream cmd;

  const char* tmpfile = "/tmp/querypacs.out";

  cmd << "dcmqr ";
  cmd << qPrintable(ServerLineEdit->text());
  cmd << " -qPatientName=";
  cmd << qPrintable(PatientNameLineEdit->text());
  cmd << " > " << tmpfile;

  system(cmd.str().c_str());

  QFile file(tmpfile);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTextStream in(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  outputTextEdit->setPlainText(in.readAll());
  QApplication::restoreOverrideCursor();

}

void PACSQueryDialog::setupUi(QDialog *Dialog)
{
  if (Dialog->objectName().isEmpty())
    Dialog->setObjectName(QString::fromUtf8("Query Dialog"));
  Dialog->resize(600, 400);
/*  QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
  Dialog->setSizePolicy(sizePolicy);
  Dialog->setMaximumSize(QSize(600, 400));
*/
  vLayout = new QVBoxLayout(Dialog);
  vLayout->setObjectName(QString::fromUtf8("vLayout"));

  formLayout = new QFormLayout();
  formLayout->setObjectName(QString::fromUtf8("formLayout"));

  ServerLabel = new QLabel(Dialog);
  ServerLabel->setObjectName(QString::fromUtf8("ServerLabel"));

  formLayout->setWidget(0, QFormLayout::LabelRole, ServerLabel);

  PatientNameLabel = new QLabel(Dialog);
  PatientNameLabel->setObjectName(QString::fromUtf8("PatientNameLabel"));

  formLayout->setWidget(1, QFormLayout::LabelRole, PatientNameLabel);

  ServerLineEdit = new QLineEdit(Dialog);
  ServerLineEdit->setObjectName(QString::fromUtf8("ServerLineEdit"));

  formLayout->setWidget(0, QFormLayout::FieldRole, ServerLineEdit);

  PatientNameLineEdit = new QLineEdit(Dialog);
  PatientNameLineEdit->setObjectName(QString::fromUtf8("PatientNameLineEdit"));

  formLayout->setWidget(1, QFormLayout::FieldRole, PatientNameLineEdit);


  vLayout->addLayout(formLayout);

  queryButton = new QPushButton(Dialog);
  queryButton->setObjectName(QString::fromUtf8("queryButton"));
  formLayout->setWidget(2, QFormLayout::FieldRole, queryButton);


  outputTextEdit = new QPlainTextEdit(Dialog);
  outputTextEdit->setObjectName(QString::fromUtf8("outputTextEdit"));
  outputTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  outputTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  outputTextEdit->setReadOnly(true);

  vLayout->addWidget(outputTextEdit, 1);


  retranslateUi(Dialog);

  QMetaObject::connectSlotsByName(Dialog);
} // setupUi

void PACSQueryDialog::retranslateUi(QDialog *Dialog)
{
  Dialog->setWindowTitle(QApplication::translate("Dialog", "PACS Query", 0, QApplication::UnicodeUTF8));
  ServerLabel->setText(QApplication::translate("Dialog", "Server", 0, QApplication::UnicodeUTF8));
  PatientNameLabel->setText(QApplication::translate("Dialog", "Patient's Name", 0, QApplication::UnicodeUTF8));
  ServerLineEdit->setText(QApplication::translate("Dialog", "BIOPACS@pacs.bio.dist.unige.it:11112", 0, QApplication::UnicodeUTF8));
  queryButton->setText(QApplication::translate("Dialog", "Query Server", 0, QApplication::UnicodeUTF8));
  Q_UNUSED(Dialog);
} // retranslateUi
