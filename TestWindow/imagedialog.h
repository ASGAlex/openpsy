#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QDialog>
#include "ui_image.h"
#include <QImage>
#include <QPixmap>
#include <QFileDialog>

class ImageDialog : public QDialog, private Ui::Form
{
  Q_OBJECT
public:
  explicit ImageDialog(QWidget *parent = 0);
  void Init(QImage & img);

public slots:
  void onSaveAs();

};

#endif // IMAGEDIALOG_H
