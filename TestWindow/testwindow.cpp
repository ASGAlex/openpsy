#include "testwindow.h"


TestWindow::TestWindow(QWidget *parent)
{
  //buttons
  setupUi(this);

  debug = false;
  nozip = false;

  main_text = new QWebViewScriptable();
  verticalLayout->insertWidget(0,main_text);
  created = false;
  bt_next->hide();
  bt_prev->hide();
  bt_multi->hide();
  progressBar->hide();

  connect(act_new_test,SIGNAL(triggered(bool)),SLOT(onNewTest()));

  connect(act_about,SIGNAL(triggered(bool)),SLOT(onAbout()));
  connect(act_about_qt,SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));

  QuestionsCount=0;
  CurrentQuestion=0;
  State=NO_TEST;
  zoom=1.0;

  connect(bt_multi,SIGNAL(clicked()),SLOT(onMulti()));
  connect(bt_prev,SIGNAL(clicked()),SLOT(onPrev()));
  connect(bt_next,SIGNAL(clicked()),SLOT(onNext()));

  connect(main_text,SIGNAL(formSubmitted(QHash<QString,QString>)), SLOT(getScriptVariables(QHash<QString,QString>)));
  processCmdArguments();
}


QStringList TestWindow::parseToList(QString &str)
{
  QStringList list = str.split(QRegExp("]\\S["), QString::SkipEmptyParts);
  int i=list.first().indexOf("[");
  if(i!=-1)
    list.first().remove(i,1);

  i=list.last().indexOf("]");
  if(i!=-1)
    list.last().remove(i,1);

  return list;
}

void TestWindow::loadData(QString testFileName)
{
  QStringList single_line_files;
  single_line_files<<"TestName.txt"<<"Intro.txt"<<"Instruction.txt"<<"Help.txt"
                   <<"Authors.txt"<<"License.txt"<<"Calculate.qs"<<"ShowResult.qs"
                   <<"DrawSettings.qs"<<"beforeStepAction.qs"<<"afterStepAction.qs"
                   <<"onMultiAction.qs"<<"ScriptInit.qs";

  QStringList multiline_files;
  multiline_files<<"Questions.txt"<<"Answers.txt"<<"Answer_role.txt"<<"Shortcuts.txt";

  QList<QString*> sl_containers;
  sl_containers<<&testName<<&hello_text<<&instruction_text<<&help_text<<&authors_text
               <<&license_text<<&scrCalculate<<&scrShowResult<<&scrDrawSettings
               <<&beforeStepAction<<&afterStepAction<<&onMultiAction<<&onScriptInit;


  if(nozip)
  {
      if(!QFile::exists(testFileName))
      {
          qDebug()<<"no such directory!";
          return;
      }

      for(int i=0;i<single_line_files.size();i++)
      {
        QFile f(testFileName+"/"+single_line_files[i]);
        readSinglelineFiles(f,single_line_files,sl_containers,i);
      }

      for(int i=0;i<multiline_files.size();i++)
      {
        QFile f(testFileName+"/"+multiline_files[i]);
        readMultilineFiles(f,multiline_files,i);
      }

  }
  else
  {
      testFileName+=".zip";
      QFile zipf(testFileName);
      QuaZip zip(&zipf);
      if(!zip.open(QuaZip::mdUnzip))
      {
        qDebug()<<"File opening error: "<<zip.getZipError();
        return;
      }

      zip.setFileNameCodec("IBM866");


      for(int i=0;i<single_line_files.size();i++)
      {
        if(!zip.setCurrentFile(single_line_files[i]))
          continue;

        QuaZipFile f(&zip);
        readSinglelineFiles(f,single_line_files,sl_containers,i);
      }

      for(int i=0;i<multiline_files.size();i++)
      {
        if(!zip.setCurrentFile(multiline_files[i]))
          continue;

        QuaZipFile f(&zip);

        readMultilineFiles(f,multiline_files,i);
      }
      zip.close();
  }
  QuestionsCount=questions.size();
}

void TestWindow::readSinglelineFiles(QIODevice &f,QStringList single_line_files, QList<QString *> sl_containers,int index)
{
    if(f.open(QIODevice::ReadOnly))
    {
      QTextStream stream(&f);
      stream.setCodec("Utf-8");

      sl_containers[index]->clear();
      sl_containers[index]->append(stream.readAll());
    }
    else
    {
      qDebug()<<single_line_files[index]+": "+f.errorString();
    }
}

void TestWindow::readMultilineFiles(QIODevice &f, QStringList multiline_files,int index)
{
    if(f.open(QIODevice::ReadOnly))
    {
      QTextStream stream(&f);
      stream.setCodec("Utf-8");

      if(multiline_files[index]=="Questions.txt")
      {
        questions.clear();
        while(!stream.atEnd())
          questions.append(stream.readLine());
      }
      else if(multiline_files[index]=="Answers.txt")
      {
        answer_text.clear();
        while(!stream.atEnd())
        {
          QString line = stream.readLine();
          line=line.mid(1,line.size()-2);
          QStringList sl=line.split("][", QString::SkipEmptyParts);
          answer_text.append(sl);
        }
      }
      else if(multiline_files[index]=="Answer_role.txt")
      {
        answer_role.clear();
        while(!stream.atEnd())
        {
          QString line = stream.readLine();
          line=line.mid(1,line.size()-2);
          QStringList sl=line.split("][", QString::SkipEmptyParts);

          QList<int> int_list;
          foreach (QString s, sl)
            int_list.append(s.toInt());

          answer_role.append(int_list);
        }
      }
      else if(multiline_files[index]=="Shortcuts.txt")
      {
        shortcuts.clear();
        QString line=stream.readAll();
        line=line.mid(1,line.size()-2);
        shortcuts=line.split(QRegExp("\\]\\s?\\["), QString::SkipEmptyParts);
      }
    }
}

void TestWindow::prepareButtons()
{

  //создаём кнопки: добавляем в окно, соединяем со слотом, настраиваем внешний вид, присваиваем имя объекта.
  buttons.clear();
  QStringList::iterator answ;

  for(answ=answer_text.first().begin();answ!=answer_text.first().end();++answ)
  {
    QPushButton* new_bt= new QPushButton(*answ,this);
    buttons.append(new_bt);
    connect(buttons.last(),SIGNAL(clicked()),SLOT(hasAnswer()));
    button_layout->addWidget(buttons.last());
    buttons.last()->setDisabled(true);
    buttons.last()->hide();
    //номер объекта - его имя
    QString bt_name;
    bt_name.setNum(buttons.count()-1);
    buttons.last()->setObjectName(bt_name);
  }

  base_point_size=buttons.last()->font().pointSizeF();

}

void TestWindow::prepareScript()
{
    for(int i = 0; i<buttons.size();i++)
    {
        QScriptValue scriptButton = scriptEngine.newQObject(buttons[i]);
        QString btNum;btNum.setNum(i+1);
        QString btName = "btAnswer"+btNum;
        scriptEngine.globalObject().setProperty(btName,scriptButton);
    }
    QScriptValue scriptMultiBt = scriptEngine.newQObject(this->bt_multi);
    scriptEngine.globalObject().setProperty("btMulti",scriptMultiBt);

    QScriptValue scriptNextBt = scriptEngine.newQObject(this->bt_next);
    scriptEngine.globalObject().setProperty("btNext",scriptNextBt);

    QScriptValue scriptPrevBt = scriptEngine.newQObject(this->bt_prev);
    scriptEngine.globalObject().setProperty("btPrev",scriptPrevBt);

    QScriptValue scriptMainText = scriptEngine.newQObject(this->main_text);
    scriptEngine.globalObject().setProperty("view",scriptMainText);

    QScriptValue scriptProgress = scriptEngine.newQObject(this->progressBar);
    scriptEngine.globalObject().setProperty("progressBar",scriptProgress);

    QScriptValue scriptAllWindow = scriptEngine.newQObject(this);
    scriptEngine.globalObject().setProperty("window",scriptAllWindow);
}

void TestWindow::createTest(QString name)
{
  //Creating test using config files from directory
  if(created==true)
    return;

  QDir dir(QApplication::applicationDirPath());
  dir.cd("tests");
  QString directory=dir.absolutePath()+"/";
  loadData(directory+name);
  main_text->setHtml(hello_text);
  prepareButtons();
  prepareScript();
  created=true;
}


//when testing is in progress:
void TestWindow::hasAnswer()
{
  //We has a new answer - let's save it!

  if(CurrentQuestion>=answers.count())
    answers.append(0);

  int button=sender()->objectName().toInt();
  int role=0;
  if(answer_role.size()<QuestionsCount)
    role=answer_role[0][button];
  else
    role=answer_role[CurrentQuestion][button];

  answers[CurrentQuestion]=role;

  //Проверка конца тестирования
  if(CurrentQuestion<QuestionsCount-1)
    CurrentQuestion+=1;
  else
  {
    //checking button instead pressing
    for(int i=0;i<buttons.size();i++)
    {
      buttons[i]->setChecked(false);
      buttons[i]->setCheckable(false);
    }
    buttons[button]->setCheckable(true);
    buttons[button]->setChecked(true);
    //multi-button apperance
    bt_multi->setText(QString::fromUtf8("Завершить тестирование"));
    bt_multi->show();
    bt_multi->setEnabled(true);
    //let's finish the test
    State=TEST_FINISHED;
  }

  //Настройка кнопок "вперед-назад"
  if(bt_prev->isEnabled()==false)
    bt_next->setDisabled(true);
  if(CurrentQuestion>0)
    bt_prev->setEnabled(true);

  displayQuestion();
}

void TestWindow::displayQuestion()
{
    saveButtonsState();

    scriptEngine.evaluate(beforeStepAction);

    if(scriptEngine.evaluate("stepStandartAction").toBool())
    {

        //displaying current question
        main_text->setHtml(questions[CurrentQuestion]);

        //Если в каждом случае ответы разные - присваиваем кнопкам новые имена
        if(answer_role.size()>=QuestionsCount)
        {
          for(int i=0;i<buttons.size();i++) {
            buttons[i]->setText(answer_text[CurrentQuestion][buttons[i]->objectName().toInt()]);
            //Настройка горячих клавиш
            if(!shortcuts.isEmpty())
            {
                QKeySequence s = QKeySequence::fromString(shortcuts[i]);
                buttons[i]->setShortcut(s);
            }
          }
        }
        //индикатор прогресса
        progressBar->setValue(CurrentQuestion+1);
    }

    scriptEngine.evaluate(afterStepAction);
}

void TestWindow::saveButtonsState()
{
  buttons_state.clear();
  for(int i=0;i<buttons.count(); i++)
  {
    ButtonState bs(buttons[i]);
    buttons_state.append(bs);
  }

  //Порядок ВАЖЕН!!!
  ButtonState next(bt_next); buttons_state.append(next);
  ButtonState prev(bt_prev); buttons_state.append(prev);
  ButtonState multi(bt_multi); buttons_state.append(multi);

}

void TestWindow::restoreButtonsState()
{
  if(buttons_state.isEmpty())
    return;

  int i=0;
  while(i<buttons.count())
  {
    buttons_state[i].restoreButton(buttons[i]);
    i++;
  }

  //Порядок ВАЖЕН!!!
  buttons_state[i].restoreButton(bt_next); i++;
  buttons_state[i].restoreButton(bt_prev); i++;
  buttons_state[i].restoreButton(bt_multi); i++;

  buttons_state.clear();
}

void TestWindow::onNext()
{
  CurrentQuestion+=1;
  restoreButtonsState();
  bt_next->setDisabled(true);
  bt_prev->setEnabled(true);
  displayQuestion();
}

void TestWindow::onPrev()
{
  CurrentQuestion-=1;
  restoreButtonsState();
  bt_prev->setDisabled(true);
  bt_next->setEnabled(true);
  displayQuestion();
}

void TestWindow::onNewTest()
{
  CurrentQuestion=0;
  progressBar->setMinimum(0);
  progressBar->setMaximum(QuestionsCount);
  progressBar->setValue(CurrentQuestion+1);

  main_text->setHtml(instruction_text);
  State=NEW_STARTED;
  bt_multi->setText(QString::fromUtf8("Начать тестирование"));
  main_text->setHtml(instruction_text);
  bt_multi->show();
  for(int i=0;i<buttons.size();i++)
    buttons[i]->show();

  if(debug)
  {
    debugger.attachTo(&scriptEngine);
    debugger.standardWindow()->show();
  }

  scriptEngine.evaluate(onScriptInit);
}

void TestWindow::onMulti()
{

  scriptEngine.evaluate(onMultiAction).toString();
  this->main_text->submit();//На случай, если там была форма

  if(!scriptEngine.evaluate("multiStandartAction").toBool())
      return;

  if(State==NEW_STARTED)
  {

    //настроить вид мульти-кнопки
    bt_multi->hide();
    bt_multi->setText(QString::fromUtf8("Завершить тест"));

    //Настроить счётчик
    progressBar->show();
    //настроить кнопки"вперёд-назад"
    bt_prev->show();
    bt_prev->setDisabled(true);
    bt_next->show();
    bt_next->setDisabled(true);

    //displayind answer buttons
    for(int i=0;i<buttons.size();i++)
    {
      buttons[i]->show();
      buttons[i]->setEnabled(true);
    }

    //Настройка горячих клавиш
    if(!shortcuts.isEmpty())
    {
      for(int i=0;i<shortcuts.count();i++)
       buttons[i]->setShortcut(QKeySequence(shortcuts[i]));
    }

    displayQuestion();

  }
  else if(State==TEST_FINISHED)
  {
    for(int i=0;i<buttons.size();i++)
    {
      buttons[i]->setChecked(false);
      buttons[i]->setCheckable(false);
      buttons[i]->setDisabled(true);
    }
    QScriptValue scrAnswers = scriptEngine.newArray(answers.size());
    for(int i=0;i<answers.size();i++)
      scrAnswers.setProperty(i,answers[i]);

    scriptEngine.globalObject().setProperty("answers",scrAnswers);
    QScriptValue scrResult = scriptEngine.evaluate("function calc(){"+scrCalculate+"};calc();");
    if(scrResult.isUndefined())
        QMessageBox::warning(this,QString("Script error"),QString("scrCalculate script return undefined!"));

    scriptEngine.globalObject().setProperty("results",scrResult);
    QScriptValue scrText = scriptEngine.evaluate("function show(){"+scrShowResult+"}; show();");
    main_text->setHtml(scrText.toString());

    if(!scrDrawSettings.isEmpty())
    {
        QString pluginName;

    #ifdef Q_OS_WIN32
        pluginName = "plotter.dll";
    #endif
    #ifdef Q_OS_UNIX
        pluginName = "libplotter.so";
    #endif

        QDir dir(QApplication::applicationDirPath());
        if(dir.cd("plugins"))
        {
          QPluginLoader loader(dir.absoluteFilePath(pluginName));
          QObject* plugin = loader.instance();
          if(plugin)
          {
            PlotterInterface* plotter=qobject_cast<PlotterInterface*>(plugin);
            if(plotter)
            {
              scriptEngine.evaluate(scrDrawSettings);
              scriptEngine.evaluate("if(Maximum=='Auto') Maximum=Math.max.apply(Math, results);");
              QString Mode = scriptEngine.evaluate("Mode").toString();
              int Width = scriptEngine.evaluate("Width").toInteger();
              int Height = scriptEngine.evaluate("Height").toInteger();
              int FontSize = scriptEngine.evaluate("FontSize").toInteger();
              QString TextX = scriptEngine.evaluate("TextX").toString();
              QString TextY = scriptEngine.evaluate("TextY").toString();
              int Maximum= scriptEngine.evaluate("Maximum").toInteger();
              bool DifferentColors= scriptEngine.evaluate("DifferentColors").toBool();

              plotter->setMode(Mode);
              plotter->setW(Width);
              plotter->setH(Height);
              plotter->setFontSize(FontSize);
              plotter->setTextX(TextX);
              plotter->setTextY(TextY);
              plotter->setMaximum(Maximum);
              plotter->setDifferentColors(DifferentColors);
              plotter->setScriptEngine(&scriptEngine);

              image = plotter->makeImage();
            }
          }
          else
          {
            qDebug()<<loader.errorString();
          }

        }
    }


    //настроить вид мульти-кнопки
    if(image.isNull() || scrDrawSettings.isEmpty())
      bt_multi->hide();
    else
      State=SHOW_PIC;

    bt_multi->setText(QString::fromUtf8("Показать график"));
    bt_multi->setEnabled(true);

    //настроить кнопки"вперёд-назад"
    bt_prev->setDisabled(true);
    bt_next->setDisabled(true);

    //Настроить счётчик
    progressBar->hide();

  }
  else if(State==SHOW_PIC)
    image_dialog.Init(image);
  //done
}


void TestWindow::keyReleaseEvent(QKeyEvent* event)
{
  if(event->matches(QKeySequence::ZoomIn))
    zoom+=0.1;
  else if(event->matches(QKeySequence::ZoomOut))
    zoom-=0.1;

  main_text->setZoomFactor(zoom);

  QList<QPushButton*> all_buttons=buttons;
  all_buttons<<bt_prev<<bt_next<<bt_multi;

  for(int i=0;i<all_buttons.size();i++)
  {
    QFont font=all_buttons[i]->font();
    font.setPointSizeF(base_point_size*zoom);
    all_buttons[i]->setFont(font);
  }
}

void TestWindow::processCmdArguments()
{
  QStringList argv = qApp->arguments();

  if(argv.count()==1)
  {
    QFile f(":/manual.txt");
    if(f.open(QIODevice::ReadOnly))
    {
      QTextStream s(&f);
      man_string=s.readAll();
      f.close();
    }
  }
  else
  {
      int index = argv.indexOf("debug");
      if(index > 0)
          debug = true;

      index = argv.indexOf("nozip");
      if(index > 0)
          nozip = true;

      createTest(argv[1]);
  }
}

void TestWindow::show()
{
  QMainWindow::show();
  this->setWindowTitle(testName);
}


void TestWindow::onAbout()
{
  HelpDialog help;
  help.setHelpText(help_text);
  help.setAuthorsText(authors_text);
  help.setLicenseText(license_text);
  help.exec();

}


void TestWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void TestWindow::setCurrentQuestionNum(int num)
{
  CurrentQuestion = num;
}

void TestWindow::getScriptVariables(QHash<QString,QString> variables)
{
  QHash<QString, QString>::iterator v = variables.begin();
  QString script;
  while(v != variables.end())
  {
    script.append(v.key()+"='"+v.value()+"';");
    ++v;
  }
  scriptEngine.evaluate(script);
}
