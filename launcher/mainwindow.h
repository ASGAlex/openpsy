#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QFileSystemModel>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include "configdialog.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
private:
  QFileSystemModel fs_model;
  QProcess proc;
  ConfigDialog config;

  QSettings settings;

private slots:
  void onIconMode();
  void onListMode();
  void startProgram();
  void saveConfig();

protected:
  virtual void closeEvent();
};

#endif // MAINWINDOW_H
