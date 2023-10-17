#ifndef Tdoc_h
#define Tdoc_h

#include "TextCommands.h"
#include "ECObserver.h"
#include "ECTextViewImp.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <set>

class TextDoc;
using namespace std;


class InsertCmd : public ECCommand
{
public:
  InsertCmd(TextDoc &docIn, char cIn, int xIn, int yIn);
  ~InsertCmd();
  virtual void Execute();
  virtual void UnExecute();
private:
  TextDoc &doc;
  char c;
  int x;
  int y;
};

class EnterCmd : public ECCommand
{
public:
  EnterCmd(TextDoc &docIn,int xIn,int yIn);
  ~EnterCmd();
  virtual void Execute();
  virtual void UnExecute();
private:
  TextDoc &doc;
  int x; 
  int y;
};

class BackspaceCmd : public ECCommand
{
public:
  BackspaceCmd(TextDoc &docIn,int xIn,int yIn);
  ~BackspaceCmd();
  virtual void Execute();
  virtual void UnExecute();
private:
  char char_removed;
  TextDoc &doc;
  int x; 
  int y;
};

class TextDocView
{
public:
  TextDocView(ECTextViewImp &TextViewIn);
  virtual ~TextDocView();
  void UpdateViewRows(vector<string> document, string docname, int y = 0);
  void UpdateViewCursor(int x, int y);
  vector<int> retRowCol();
  void changeStatRow(bool commState);
  //void Color(string keywords = "");
  
private:
  ECTextViewImp &TextView;
  
};

class TextDocCtrl
{
public:
  TextDocCtrl(TextDoc &docIn);
  virtual ~TextDocCtrl();
  
  // commands
  void InsertChar(char c);
  void Enter();
  void Backspace();
  
  // non commands
  void LeftArrow();
  void RightArrow();
  void UpArrow();
  void DownArrow();
  void SetCommand();
  void Undo();
  void Redo();
private:
  TextDoc &doc;
  ECCommandHistory hist;
  
};

class TextDoc
{
public:
  TextDoc(TextDocView &docViewIn, string docIn);
  virtual ~TextDoc();
  TextDocCtrl &GetCtrl();
  int GetX() {return x_pos;}
  int GetY() {return y_pos;}
  void setRowCol();
  void CommandButton(bool t);
  bool GetCommState();
  vector<int> InsertDoc(char c, int xIn = -1, int yIn = -1); // get a character, insert to vector
  vector<int> EnterVec(int xIn = -1, int yIn = -1);
  vector<int> RemoveChar(int xIn = -1, int yIn = -1);
  void MoveCursLeft();
  void MoveCursRight();
  void MoveCursUp();
  void MoveCursDown();
  void normalizevec();
  void filemanage();
  char pop_remove_last();

private:
  TextDocCtrl docCtrl;
  TextDocView &docView;
  vector<string> document;
  int x_pos;
  int y_pos;
  int rowLen;
  int colLen;
  string txtfile;
  bool command_state;
  vector<char> removed;
  string keywords;
  
  
};

#endif