#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    connect(btSelectPath,SIGNAL(clicked()),SLOT(selectPath()));
}

void ConfigDialog::selectPath()
{
  QString dir = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Выберите папку с тестами"),
                                                   QDir::homePath(),
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
  pathToTests->setText(dir);
}

QString ConfigDialog::getPath()
{
  return pathToTests->text();
}

void ConfigDialog::setPath(QString& path)
{
  pathToTests->setText(path);
}
