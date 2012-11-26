#include "helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent)
{
  setupUi(this);
}
void HelpDialog::setHelpText(QString text)
{
  text_about->setHtml(text);
}

void HelpDialog::setAuthorsText(QString text)
{
  text_authors->setHtml(text);
}

void HelpDialog::setLicenseText(QString text)
{
  text_license->setHtml(text);
}

