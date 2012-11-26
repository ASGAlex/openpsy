#ifndef QWEBVIEWSCRIPTABLE_H
#define QWEBVIEWSCRIPTABLE_H

#include <QWebView>
#include <QWebFrame>
#include <QWebElement>
#include <QHash>
#include <QDebug>

class QWebViewScriptable : public QWebView
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
    explicit QWebViewScriptable(QWidget *parent = 0);

  void setText(QString text);
  QString text();

signals:
  void textChanged(QString);
  void formSubmitted(QHash<QString,QString> variables);

private slots:
  void populateJavaScriptWindowObject();

public slots:
  void submit();
};

#endif // QWEBVIEWSCRIPTABLE_H
