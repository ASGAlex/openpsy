#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    fs_model(this),
    settings("Openpsy", "Launcher")
{
    setupUi(this);
    fs_model.setFilter(QDir::Files);
    fs_model.setNameFilterDisables(false);
    QStringList filters;
    filters.append("*.zip");
    fs_model.setNameFilters(filters);

    fs_model.setReadOnly(true);

    QString path = settings.value("testdir",QApplication::applicationDirPath()+"/tests").toString();

    QDir dir;
    bool applyPathSettingsToConfig = false;
    if(!dir.exists(path))
    {
      if(!dir.mkpath(path))
        qDebug()<<"mkdir error";
      else
        applyPathSettingsToConfig = true;
    }
    else
      applyPathSettingsToConfig = true;

    if(applyPathSettingsToConfig)
      config.setPath(path);

    fs_model.setRootPath(path);

    listView->setModel(&fs_model);
    listView->setRootIndex(fs_model.index(path));
    actIconMode->setChecked(true);
    connect(act_exit,SIGNAL(triggered()),qApp,SLOT(quit()));

    connect(actIconMode,SIGNAL(triggered()),SLOT(onIconMode()));
    connect(actListMode,SIGNAL(triggered()),SLOT(onListMode()));

    connect(listView,SIGNAL(doubleClicked(QModelIndex)),SLOT(startProgram()));
    connect(bt_ok,SIGNAL(clicked()),SLOT(startProgram()));

    connect(actConfig,SIGNAL(triggered()),&config,SLOT(exec()));
    connect(&config,SIGNAL(accepted()),SLOT(saveConfig()));

}

void MainWindow::onIconMode()
{
  listView->setViewMode(QListView::IconMode);
  actIconMode->setChecked(true);
  actListMode->setChecked(false);
}

void MainWindow::onListMode()
{
  listView->setViewMode(QListView::ListMode);
  actIconMode->setChecked(false);
  actListMode->setChecked(true);
}

void MainWindow::startProgram()
{
  QString program = "\""+QApplication::applicationDirPath()+"/TestWindow\"";
#ifdef Q_OS_WIN32
  program+=".exe";
#endif
  QString debug = (this->actDebug->isChecked() ? " debug" : "");
  QString argument=" "+fs_model.fileName(listView->currentIndex());
  argument.resize(argument.size()-4);
  QString cmd=program+argument+debug;
  qDebug()<<cmd;
  proc.start(cmd);
}

void MainWindow::closeEvent()
{
  qApp->exit();
}

void MainWindow::saveConfig()
{
  settings.setValue("testdir",config.getPath());
}
