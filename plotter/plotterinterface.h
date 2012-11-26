#ifndef PLOTTERINTERFACE_H
#define PLOTTERINTERFACE_H

#include <QtScript>

class QImage;
class QString;
class QScriptEngine;

class PlotterInterface
{
public:
    virtual ~PlotterInterface() {}
    virtual QImage makeImage() = 0;

    virtual void setScriptEngine(QScriptEngine* eng) = 0;

    virtual void setMode(const QString mode) = 0;
    virtual void setW(const int w) = 0;
    virtual void setH(const int h) = 0;
    virtual void setFontSize(const int fs) = 0;
    virtual void setTextX(const QString text_x) = 0;
    virtual void setTextY(const QString text_y) = 0;
    virtual void setMaximum(const int maximum) = 0;
    virtual void setDifferentColors(const bool difcol) = 0;

    virtual QString Mode()  = 0;
    virtual int W()  = 0;
    virtual int H()  = 0;
    virtual int FontSize()  = 0;
    virtual QString TextX()  = 0;
    virtual QString TextY()  = 0;
    virtual int Maximum()  = 0;
    virtual bool DifferentColors() = 0;
};

Q_DECLARE_INTERFACE(PlotterInterface, "asg.PlotterInterface/1.0")


#endif // PLOTTERINTERFACE_H
