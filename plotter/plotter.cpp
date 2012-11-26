#include "plotter.h"


Plotter::Plotter()
{
    qDebug()<<"init";
}

Plotter::~Plotter()
{
  scriptEngine=0;
}

QImage Plotter::makeImage()
{
  QImage img;
  initSettings();

  if(_mode.toLower() == "plot")
    img = makePlot();
  else if(_mode.toLower() == "histro")
    img = makeHistro();

  return img;
}

void Plotter::setMode(const QString mode)
{
  _mode=mode;
}


void Plotter::setScriptEngine(QScriptEngine *eng)
{
  scriptEngine=eng;
}


void Plotter::setW(const int w)
{
  width=w;
}

void Plotter::setH(const int h)
{
  height=h;
}

void Plotter::setFontSize(const int fs)
{
  _fs=fs;
}

void Plotter::setTextX(const QString text_x)
{
  _text_x=text_x;
}

void Plotter::setTextY(const QString text_y)
{
  _text_y = text_y;
}

void Plotter::setMaximum(const int maximum)
{
  _maximum = maximum;
}

void Plotter::setDifferentColors(const bool difcol)
{
  _difcol = difcol;
}




QString Plotter::Mode()
{
  return _mode;
}


int Plotter::W()
{
  return width;
}

int Plotter::H()
{
 return  height;
}

int Plotter::FontSize()
{
  return _fs;
}

QString Plotter::TextX()
{
  return _text_x;
}

QString Plotter::TextY()
{
  return _text_y;
}

int Plotter::Maximum()
{
  return _maximum;
}

bool Plotter::DifferentColors()
{
  return _difcol;
}


void Plotter::initSettings()
{
  /* Цель - проверить, все ли настройки корректно заполнены.
    Если что-то не заполнено - инициализировать значениями по-умолчанию*/
  if(_fs == 0)
    _fs=10;
  if(width==0)
    width = 640;
  if(height==0)
    height=480;
  if(_mode.isEmpty())
    _mode="plot";
}

QImage Plotter::makeHistro()
{
  QImage img= QImage(width,height,QImage::Format_RGB32);
  img.fill(0xffffff);
  QPainter painter(&img);
  painter.setFont(QFont(painter.font().family(),_fs));

  float step_x=float(width/(_text_x.length()+1));
  float max = float(scriptEngine->evaluate("Math.max.apply( Math, results );").toNumber());
  float step_y=float(height)/max;

  float ident_right=step_x/2;
  float ident_left=ident_right;
  float ident_top=step_y+_fs*2;
  float ident_bottom=step_y+_fs*2;

  int dataCount = scriptEngine->evaluate("results.length").toInt32();
  step_x=(width-ident_left-ident_right)/float(dataCount);
  step_y=(height-ident_bottom-ident_top)/max;

  QVector<QRectF> rects;
  QStringList text_up;
  QList<QPointF> text_up_pos;
  QList<QPointF> text_down_pos;

  QFontMetrics metrics=QFontMetrics(painter.font());
  QString num;
  for(int i=0; i<dataCount; i++)
  {

    QScriptValue index = QScriptValue(i);
    scriptEngine->globalObject().setProperty("index",index);
    float curResult = (float)scriptEngine->evaluate("results[index]").toNumber();

    rects.append(QRectF(ident_left+i*step_x,height-(ident_bottom+curResult*step_y),step_x,curResult*step_y));
    num.setNum(curResult);
    text_up_pos.append(QPointF(ident_left+i*step_x+step_x/2-metrics.width(num)/2,height-(ident_bottom+curResult*step_y-_fs-_fs/2)));
    text_down_pos.append(QPointF(ident_left+i*step_x+step_x/2-metrics.width(_text_x[i])/2,height-ident_bottom+1.5*_fs));

    num.setNum(curResult);
    text_up.append(num);
  }

  QLine max_line;
  QString max_text=QString::fromUtf8("Максимум: ");
  QPointF max_text_pos;
  if(_maximum!=0)
  {
    if(_maximum<max)
      _maximum=max;
    max_line=QLine(ident_left,height-_maximum*step_y-ident_bottom,width-ident_right,height-_maximum*step_y-ident_bottom);
    num.setNum(_maximum);
    max_text.append(num);
    max_text_pos=QPointF(width/2-metrics.width(max_text)/2,height-_maximum*step_y-_fs/2-ident_bottom);
  }

  QPen pen=QPen(Qt::black,2);
  QBrush brush1=QBrush(Qt::lightGray);
  QBrush brush2=QBrush(Qt::lightGray);
  if(_difcol==true)
    brush2=QBrush(Qt::gray);

  painter.setPen(pen);
  painter.setBrush(brush1);

  QStringList sl = _text_x.split(";",QString::SkipEmptyParts,Qt::CaseSensitive);

  for(int i=0; i<rects.size(); i++)
  {
    if(_difcol==true)
    {
      if(i%2==0)//если чётное
        painter.setBrush(brush1);
      else if(i%2!=0)
        painter.setBrush(brush2);

      painter.drawRect(rects[i]);
    }
    if(i<sl.size())
      painter.drawText(text_down_pos[i],QString(sl[i]));

    painter.drawText(text_up_pos[i],QString(text_up[i]));
  }

  if(_difcol==false)
    painter.drawRects(rects);

  if(_maximum!=0)
  {
    painter.drawLine(max_line);
    painter.drawText(max_text_pos,max_text);
  }

  return img;
}

QImage Plotter::makePlot()
{
  QImage img=QImage(width,height,QImage::Format_RGB32);
  img.fill(0xffffff);
  QPainter painter(&img);
  painter.setFont(QFont(painter.font().family(),_fs));
  //Отступы
  QFontMetrics metrics(painter.font());
  std::vector<int> lens;
  foreach(QString str,_text_y)
    lens.push_back(str.size());

  float ident_left;
  std::vector<int>::const_iterator it, it2;
  for(int i=0;i<lens.size(); i++)
  {
    it2 = std::max_element(lens.begin(), lens.end());
    if(lens.at(i)==*it2)
    {
      ident_left=metrics.width(_text_y[i])+metrics.width(_text_y[i])/5;
      break;
    }
  }

  float ident_right=metrics.width(_text_x[_text_x.size()-1])+metrics.width(_text_x[_text_x.size()-1])/5;

  //Шаги и отступы снизу и сверху
  float step_x=(width-ident_right-ident_left)/_text_x.size();
  float step_y=height/(_text_y.size()+1);
  float ident_bottom=step_y+_fs;
  float ident_top=step_y+_fs;
  step_y=(height-ident_top-ident_bottom)/(_text_y.size());
  //Оси
  QLineF axis_x(ident_left,height-ident_bottom,width-ident_right,height-ident_bottom);
  QLineF axis_y(ident_left,height-ident_bottom,ident_left,ident_top);

  //Черточки и подписи
  QVector<QLineF> x_hyphen;
  float hx_size=step_x/10;
  float hy_size=step_y/10;
  QList<QRectF> text_x_pos;

  QStringList sl_x = _text_x.split(";",QString::SkipEmptyParts);
  for(int i=0; i<sl_x.size();i++)
  {
    if(i<sl_x.size()-1)
      x_hyphen.append(QLineF(ident_left+(i+1)*step_x,height-ident_bottom-hx_size,ident_left+(i+1)*step_x,height-ident_bottom));

    QFontMetrics metrics(painter.font());

    if(i==sl_x.size()-1)
    {
      text_x_pos.append(QRectF(ident_left+(i+1)*step_x,height-ident_bottom+_fs+hy_size*2,step_x,ident_bottom-_fs-hy_size*2));
      //text_x_pos.append(QRectF(x,y,w,h))
    }
    else
    {
      //text_x_pos.append(QPointF(ident_left+(i+1)*step_x-metrics.width(text_x[i])/2,height-ident_bottom+font+hy_size*2))
      text_x_pos.append(QRectF(ident_left+(i+1)*step_x-metrics.width(sl_x[i])/2,height-ident_bottom+_fs+hy_size*2,step_x,ident_bottom-_fs-hy_size*2));
    }
  }

  QVector<QLineF> y_hyphen;
  QList<QPointF> text_y_pos;
  QStringList sl_y = _text_y.split(";",QString::SkipEmptyParts);
  for(int i=0; i<sl_y.size();i++)
  {
    if(i<sl_y.size()-1)
      y_hyphen.append(QLineF(ident_left,height-ident_bottom-(i+1)*step_y,ident_left+hy_size,height-ident_bottom-(i+1)*step_y));

    QFontMetrics metrics(painter.font());

    if(i==sl_y.size()-1)
      text_y_pos.append(QPointF(ident_left-metrics.width(sl_y[i])/2,ident_top/2+_fs/2));
    else
      text_y_pos.append(QPointF(ident_left-metrics.width(sl_y[i])-hy_size,height-ident_bottom-(i+1)*step_y+_fs/2));
  }

  //Точки графика
  QVector<QPoint> graph_points;
  QVector<QLineF> dot_line_x;
  QVector<QLineF> dot_line_y;

  int dataCount = scriptEngine->evaluate("results.length").toInteger();


  for(int i=0;i<dataCount; i++)
  {
    QScriptValue index = QScriptValue(i);
    scriptEngine->globalObject().setProperty("index",index);
    float curResult = (float)scriptEngine->evaluate("results[index]").toNumber();

    graph_points.append(QPoint(ident_left+step_x*(i+1),height-(ident_bottom+curResult*step_y)));
    dot_line_x.append(QLineF(QPointF(ident_left,graph_points[graph_points.size()-1].y()),graph_points[graph_points.size()-1]));
    dot_line_y.append(QLineF(QPointF(graph_points[graph_points.size()-1].x(),height-ident_bottom),graph_points[graph_points.size()-1]));
  }

  //Перья для рисования
  QPen axis_pen(Qt::black);
  axis_pen.setWidth(4);

  QPen dot_pen(Qt::black);
  dot_pen.setWidth(1);
  dot_pen.setStyle(Qt::DotLine);

  QPen main_pen(Qt::black);
  main_pen.setWidth(2);

  QPen point_pen(Qt::red);
  point_pen.setWidth(4);

  //Рисуем
  //Оси
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(axis_pen);
  painter.drawLine(axis_x);
  painter.drawLine(axis_y);
  //Черточки и подписи
  painter.setPen(main_pen);
  painter.drawLines(x_hyphen);
  painter.drawLines(y_hyphen);
  for(int i=0; i<text_x_pos.size(); i++)
  {
    if(i==text_x_pos.size()-1)
    {
      QFont nf(painter.font());
      nf.setWeight(QFont::Bold);
      painter.setFont(nf);
      painter.drawText(text_x_pos[i],QString(sl_x[i]));
      nf.setWeight(QFont::Normal);
      painter.setFont(nf);
    }
    else
      painter.drawText(text_x_pos[i],Qt::TextWordWrap,QString(sl_x[i]));
  }

  for(int i=0;i<text_y_pos.size();i++)
  {
    if(i==text_y_pos.size()-1)
    {
      QFont nf(painter.font());
      nf.setWeight(QFont::Bold);
      painter.setFont(nf);
      painter.drawText(text_y_pos[i],QString(sl_y[i]));
      nf.setWeight(QFont::Normal);
      painter.setFont(nf);
    }
    else
      painter.drawText(text_y_pos[i],QString(sl_y[i]));
  }
  //График
  QPolygon p(graph_points);
  painter.drawPolyline(p);
  //Пунктирные линии
  painter.setRenderHint(QPainter::Antialiasing,false);
  painter.setPen(dot_pen);
  painter.drawLines(dot_line_x);
  painter.drawLines(dot_line_y);
  //Точки графика
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(point_pen);
  painter.drawPoints(p);
  return img;
}


Q_EXPORT_PLUGIN2(plotter, Plotter);
