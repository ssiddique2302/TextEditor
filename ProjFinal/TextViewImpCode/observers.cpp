#include "ECTextViewImp.h"
#include "ECObserver.h"
#include "TextDocument.h"
#include <stdio.h>
#include <ctype.h>

using namespace std;

class ESCOb : public ECObserver
{
public:  
  ESCOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event =  Subject.GetPressedKey();
    if (event == CTRL_A) //ctrl b
    {
      ctrl.SetCommand();
    }
  } 
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};

//observer for key, and when it detects key it calls the document controllers function fo that key, because controller wil add to history allso 
// 

class printKeyOb : public ECObserver
{
public:
  printKeyOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event = Subject.GetPressedKey();
    if(isprint(event))
    {
      ctrl.InsertChar(char(event));
     //Subject.AddRow("yes");
    }
  }
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};

class leftKeyOb : public ECObserver
{
public:
  leftKeyOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event = Subject.GetPressedKey();
    if(event == ARROW_LEFT)
    {
      ctrl.LeftArrow();
     
    }
  }
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};

class rightKeyOb : public ECObserver
{
public:
  rightKeyOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event = Subject.GetPressedKey();
    if(event == ARROW_RIGHT)
    {
      ctrl.RightArrow();
     
    }
  }
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};

class enterKeyOb : public ECObserver
{
public:
  enterKeyOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event = Subject.GetPressedKey();
    if(event == ENTER)
    {
      ctrl.Enter();
     
    }
  }
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};

class backspaceKeyOb : public ECObserver
{
public:
  backspaceKeyOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event = Subject.GetPressedKey();
    if(event == BACKSPACE)
    {
      ctrl.Backspace();
     
    }
  }
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};

class upKeyOb : public ECObserver
{
public:
  upKeyOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event = Subject.GetPressedKey();
    if(event == ARROW_UP)
    {
      ctrl.UpArrow();
     
    }
  }
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};

class downKeyOb : public ECObserver
{
public:
  downKeyOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event = Subject.GetPressedKey();
    if(event == ARROW_DOWN)
    {
      ctrl.DownArrow();
     
    }
  }
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};

class undoKeyOb : public ECObserver
{
public:
  undoKeyOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event = Subject.GetPressedKey();
    if(event == CTRL_Z)
    {
      ctrl.Undo();
     
    }
  }
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};

class redoKeyOb : public ECObserver
{
public:
  redoKeyOb(TextDocCtrl &ctrlIn, ECTextViewImp &SubjectIn) : ctrl(ctrlIn), Subject(SubjectIn) {}
  void Update()
  {
    auto event = Subject.GetPressedKey();
    if(event == CTRL_Y)
    {
      ctrl.Redo();
     
    }
  }
  
private:
  TextDocCtrl &ctrl;
  ECTextViewImp &Subject;
};



