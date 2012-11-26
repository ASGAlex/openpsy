#include "qwebkitscriptengine.h"

QWebKitScriptEngine::QWebKitScriptEngine(QObject *parent) :
    QScriptEngine(parent)
{
}

QWebKitScriptEngine::addScriptObject(QString name, QVariant value)
{
  this->newVariant()
}
