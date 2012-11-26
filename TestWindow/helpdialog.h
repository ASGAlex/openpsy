#ifndef HELPDDIALOG_H
#define HELPDDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QWebHistory>
#include "ui_help.h"

class HelpDialog : public QDialog, private Ui::Dialog
{
    Q_OBJECT
public:
  explicit HelpDialog(QWidget *parent = 0);
  void setHelpText(QString text);
  void setAuthorsText(QString text);
  void setLicenseText(QString text);

};

#endif // HELPDDIALOG_H
