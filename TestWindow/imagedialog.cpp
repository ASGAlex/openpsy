#include "imagedialog.h"

ImageDialog::ImageDialog(QWidget *parent) :
    QDialog(parent)
{
  setupUi(this);
  connect(bt_save,SIGNAL(clicked()),SLOT(onSaveAs()));

}



void ImageDialog::Init(QImage & img)
{
  lbl_image->setPixmap(QPixmap::fromImage(img));
  show();
  resize(img.width(),img.height());
}

void ImageDialog::onSaveAs()
{
  QString filename=QFileDialog::getSaveFileName(0,QString::fromUtf8("Выберите путь для сохранения файла"),"","Images (*.png *.xpm *.jpg *bmp)");
  if(filename.isNull()!=0)
    lbl_image->pixmap()->save(filename);
}
