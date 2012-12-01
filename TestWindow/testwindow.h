#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include "ui_testwindow.h"
#include "qwebviewscriptable.h"
#include <QImage>
#include "helpdialog.h"
#include "imagedialog.h"
#include <QTextStream>
#include <QDir>
#include <QList>
#include <QStringList>
#include <QKeyEvent>
#include <QFont>
#include <QtScript>
#include <QtScriptTools/QScriptEngineDebugger>
#include <QPluginLoader>
#include <QMessageBox>
#include <QDebug>
#include <QHash>
#include "../plotter/plotterinterface.h"
#include "buttonstate.h"

#include "quazip/quazip.h"
#include "quazip/quazipfile.h"

enum TestStates {NO_TEST,NEW_STARTED,TEST_FINISHED,SHOW_PIC};


class TestWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
    explicit TestWindow(QWidget *parent = 0);

private:

  QWebViewScriptable* main_text;
  void changeEvent(QEvent *e);
  int QuestionsCount;
  int CurrentQuestion;
  int State;
  float zoom;
  QImage image;
  ImageDialog image_dialog;


  void processCmdArguments();
  QString man_string;

  void loadData(QString testName);
  void readSinglelineFiles(QIODevice &f,QStringList single_line_files, QList<QString*> sl_containers,int index);
  void readMultilineFiles(QIODevice &f, QStringList multiline_files,int index);

  void prepareButtons();
  void prepareScript();
  void createTest(QString name);

  QStringList parseToList(QString & str);

  void keyReleaseEvent(QKeyEvent *);

  QStringList questions;
  QList<int> answers;

  QList<QStringList> answer_text;
  QList< QList<int> > answer_role;

  QStringList shortcuts;
  QString onScriptInit;
  QString beforeStepAction;
  QString afterStepAction;
  QString onMultiAction;
  QString hello_text;
  QString instruction_text;
  QString help_text;
  QString authors_text;
  QString license_text;
  QString testName;
  bool created;

  QList<QPushButton*> buttons;

  float base_point_size;

  QScriptEngine scriptEngine;
  QScriptEngineDebugger debugger;
  bool debug;
  bool nozip;
  QString scrCalculate;
  QString scrShowResult;
  QString scrDrawSettings;

  QList<ButtonState> buttons_state;

private slots:

  void saveButtonsState();
  void restoreButtonsState();
  void onNext();
  void onPrev();


  void onNewTest();
  void onMulti();
  void hasAnswer();

  void onAbout();

  void getScriptVariables(QHash<QString,QString>);

public slots:
  void show();

  //only for scripting!
  void displayQuestion();
  void setCurrentQuestionNum(int num);

};

#endif // TESTWINDOW_H
