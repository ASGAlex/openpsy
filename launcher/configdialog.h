#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "ui_configdialog.h"
#include <QFileDialog>
#include <QDir>

class ConfigDialog : public QDialog, private Ui::ConfigDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);

  QString getPath();
  void setPath(QString&);

private slots:
  void selectPath();
};

#endif // CONFIGDIALOG_H
