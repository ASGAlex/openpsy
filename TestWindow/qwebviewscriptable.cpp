#include "qwebviewscriptable.h"

QWebViewScriptable::QWebViewScriptable(QWidget *parent) :
    QWebView(parent)
{
  connect(this->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
              this, SLOT(populateJavaScriptWindowObject()));
}

QString QWebViewScriptable::text()
{
  return this->page()->mainFrame()->documentElement().toPlainText();
}

void QWebViewScriptable::setText(QString text)
{
  setHtml(text);
  emit textChanged(text);
}

void QWebViewScriptable::populateJavaScriptWindowObject()
{
  this->page()->mainFrame()->addToJavaScriptWindowObject("view", this);
}

void QWebViewScriptable::submit()
{
  QHash<QString,QString> variablesForScript;
  QHash<QString,int> checkboxCount;
  QWebElementCollection inputs = this->page()->mainFrame()->findAllElements("input");

  foreach(QWebElement input, inputs)
  {
    if(input.attribute("type") == "submit")
      continue;

    if(input.attribute("type") == "radio")
    {
      if(!input.evaluateJavaScript("this.checked").toBool())
        continue;

      variablesForScript[input.attribute("name")] = input.attribute("value"); //insert or rewrite
    }
    else if(input.attribute("type") == "checkbox")
    {
      if(!input.evaluateJavaScript("this.checked").toBool())
        continue;

      int last = checkboxCount[input.attribute("name")]; //0 if first
      checkboxCount[input.attribute("name")] = last++;

      QString lastNum; lastNum.setNum(last);
      variablesForScript[ input.attribute("name")+lastNum ] = input.attribute("value");
    }
    else
    {
      variablesForScript[input.attribute("name")] = input.evaluateJavaScript("this.value").toString();
    }
  }
  emit formSubmitted(variablesForScript);
}
