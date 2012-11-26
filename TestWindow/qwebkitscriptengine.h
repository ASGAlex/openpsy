#ifndef QWEBKITSCRIPTENGINE_H
#define QWEBKITSCRIPTENGINE_H

#include <QScriptEngine>

class QWebKitScriptEngine : public QScriptEngine
{
  Q_OBJECT
public:
    explicit QWebKitScriptEngine(QObject *parent = 0);

signals:

public slots:
  void addScriptObject(QString name, QVariant value);

};

#endif // QWEBKITSCRIPTENGINE_H
