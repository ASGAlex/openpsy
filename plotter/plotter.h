#ifndef PLOTTER_H
#define PLOTTER_H

#include "plotterinterface.h"
#include <QObject>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QVector>
#include <algorithm>
#include <iostream>
#include <vector>
#include <QDebug>

class Plotter : public QObject, public PlotterInterface
{
Q_OBJECT
Q_INTERFACES(PlotterInterface)

private:
  QString _mode;
  QString _text_x;
  QString _text_y;
  int width;
  int height;
  int _fs;
  float _maximum;
  bool _difcol;

  void initSettings();
  QImage makeHistro();
  QImage makePlot();

  QScriptEngine *scriptEngine;

public:
    Plotter();
    virtual ~Plotter();
    virtual QImage makeImage();

    virtual void setScriptEngine(QScriptEngine *eng);

    virtual void setMode(const QString mode);
    virtual void setW(const int w);
    virtual void setH(const int h);
    virtual void setFontSize(const int fs);
    virtual void setTextX(const QString text_x);
    virtual void setTextY(const QString text_y);
    virtual void setMaximum(const int maximum);
    virtual void setDifferentColors(const bool difcol);

    virtual QString Mode();
    virtual int W();
    virtual int H();
    virtual int FontSize();
    virtual QString TextX();
    virtual QString TextY();
    virtual int Maximum();
    virtual bool DifferentColors();
};


#endif // PLOTTER_H
