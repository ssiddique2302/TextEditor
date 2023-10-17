#include "TextDocument.h"



using namespace std;

// Commands
InsertCmd::InsertCmd(TextDoc &docIn, char cIn, int xIn, int yIn) : doc(docIn), c(cIn), x(xIn), y(yIn) {}
InsertCmd::~InsertCmd() {}

void InsertCmd::Execute()
{
  vector<int> pos = doc.InsertDoc(this->c, x, y);
  x = pos[0];
  y = pos[1];
}
void InsertCmd::UnExecute()
{
  vector<int> location = doc.RemoveChar(x, y);
  x = location[0];
  y = location[1];
}

EnterCmd::EnterCmd(TextDoc &docIn,int xIn,int yIn) : doc(docIn), x(xIn), y(yIn) {}
EnterCmd::~EnterCmd() {}
void EnterCmd::Execute()
{
  vector<int> coords = doc.EnterVec(x, y);
  x = coords[0];
  y = coords[1];
}
void EnterCmd::UnExecute()
{
  vector<int> location = doc.RemoveChar(x,y);
  x = location[0];
  y = location[1];
}

BackspaceCmd::BackspaceCmd(TextDoc &docIn,int xIn,int yIn) : doc(docIn), x(xIn), y(yIn) {}
BackspaceCmd::~BackspaceCmd() {}
void BackspaceCmd::Execute()
{
   vector<int> location = doc.RemoveChar(x, y);
   x = location[0];
   y = location[1];
}
void BackspaceCmd::UnExecute()
{
  char place = doc.pop_remove_last();
  vector<int> pos = doc.InsertDoc(place, x, y);
  x = pos[0];
  y = pos[1];
}

// TextDocView
TextDocView::TextDocView(ECTextViewImp &TextViewIn) : TextView(TextViewIn) {}
TextDocView::~TextDocView() {}
void TextDocView::UpdateViewRows(vector<string> document, string docname, int y)
{
  TextView.InitRows();
  TextView.ClearColor();
  
  fstream ifile;
  ifile.open(docname,fstream::out | fstream::trunc);
  for ( auto s: document)
  {
    ifile << s;
    if (s == "")
    {
      ifile << '\n';
      ifile << '\n';
    }
  }
  ifile.close();
  
  set<string> kw;
  fstream kfile;
  kfile.open("keywords.txt");
  if(kfile)
  {
    string mytext;
    while(getline(kfile, mytext))
    {
      kw.insert(mytext);
    }
  }
  
  int c = 0;
  while (c <= y)
  {
    c += TextView.GetRowNumInView();
  }
  int start = 0;
  int end = document.size();
  if(c - TextView.GetRowNumInView() >= 0)
  {
    start = c - TextView.GetRowNumInView();
  }
  if(c < document.size())
  {
    end = c;
  }
  for(int i = start; i < end; ++i)
  {
    TextView.AddRow(document[i]);
    if (document[i] != "")
    {
      string word = "";
      string currow = document[i];
      bool start = true;
      int s = 0;
      for(int j = 0; j < currow.size(); ++j)
      {
        if (currow[j] != ' ' )
        {
          word += currow[j];
          if (start)
          {
            s = j;
            start = false;
          }
        }
        else
        {
          bool is_in = kw.find(word) != kw.end();
          if (is_in)
          {
            TextView.SetColor(i, s, j-1, TEXT_COLOR_BLUE);
          }
          start = true;
          word = "";
        }
      }
    }
  }
}
void TextDocView::UpdateViewCursor(int x, int y)
{
  TextView.SetCursorX(x);
  TextView.SetCursorY(y);
}
vector<int> TextDocView::retRowCol()
{
  vector<int> vals;
  vals.push_back(TextView.GetColNumInView()); // swapped rows and columns because I used rows wrong
  vals.push_back(TextView.GetRowNumInView()); // swap back and change every occurance of row to col 
  return vals;
}
void TextDocView::changeStatRow(bool commState)
{
  TextView.ClearStatusRows();
  if (commState)
  {
    TextView.AddStatusRow("Editor", "Mode: Command", true);
  }
  else
  {
    TextView.AddStatusRow("Editor", "Mode: Insert", true);
  }  
}
/*
void TextDocView::Color(string keywords)
{
  if (keywords == "")
  {
    return;
  }
  set<string> kw;
  fstream kfile;
  kfile.open(keywords);
  if(kfile)
  {
    string mytext;
    while(getline(kfile, mytext))
    {
      kw.insert(mytext);
    }
  }
  
}
*/

// TextDocCtrl
TextDocCtrl::TextDocCtrl(TextDoc &docIn) : doc(docIn) {}
TextDocCtrl:: ~TextDocCtrl() {}

void TextDocCtrl::InsertChar(char c)
{
  if(!(this->doc.GetCommState()))
  {
    int x = doc.GetX();
    int y = doc.GetY();
    InsertCmd *inscmd = new InsertCmd(this->doc, c, x, y);
    hist.ExecuteCmd(inscmd);
  }
  else if(c == 'i')
  {
    this->doc.CommandButton(false);
    hist.clear_lists();
  }
}
void TextDocCtrl::Enter()
{
  if(!(this->doc.GetCommState()))
  {
  int x = doc.GetX();
  int y = doc.GetY();
  EnterCmd *entcmd = new EnterCmd(this->doc,x,y);
  hist.ExecuteCmd(entcmd);
  }
}
void TextDocCtrl::Backspace()
{
  if(!(this->doc.GetCommState()))
  {
  int x = doc.GetX();
  int y = doc.GetY();
  BackspaceCmd *bspcmd = new BackspaceCmd(this->doc,x,y);
  hist.ExecuteCmd(bspcmd);
  }
}
void TextDocCtrl::LeftArrow()
{
  doc.MoveCursLeft();
}
void TextDocCtrl::RightArrow()
{
  doc.MoveCursRight();
}
void TextDocCtrl::UpArrow()
{
  doc.MoveCursUp();
}
void TextDocCtrl::DownArrow()
{
  doc.MoveCursDown();
}
void TextDocCtrl::SetCommand()
{
  doc.CommandButton(true);
}
void TextDocCtrl::Undo()
{
  if(this->doc.GetCommState())
  {
    this->doc.CommandButton(false);
    hist.Undo();
    this->doc.CommandButton(true);
  }
}
void TextDocCtrl::Redo()
{
  if(this->doc.GetCommState())
  {
    this->doc.CommandButton(false);
    hist.Redo();
    this->doc.CommandButton(true);
  }
}

// TextDoc

TextDoc::TextDoc(TextDocView &docViewIn, string docIn) : docCtrl(*this), docView(docViewIn), x_pos(0), y_pos(0), txtfile(docIn), command_state(true), keywords(""){}
TextDoc::~TextDoc() {}
TextDocCtrl & TextDoc::GetCtrl()
{
  return docCtrl;
}
void TextDoc::setRowCol()
{
  vector<int> row_col_vals = docView.retRowCol();
  rowLen = row_col_vals[0];
  colLen = row_col_vals[1];
}
void TextDoc::CommandButton(bool t)
{
  this->command_state = t;
  docView.changeStatRow(this->command_state);
}
bool TextDoc::GetCommState()
{
  return command_state;
}
vector<int> TextDoc::InsertDoc(char c, int xIn, int yIn)
{
  if (xIn != -1)
  {
    x_pos = xIn;
    y_pos = yIn;
  }
  string toIns(1, c);
  if(document.size() == 0)
  {
    document.push_back("");
    //document[y_pos].append(toIns);
    //x_pos = document[y_pos].size();
  }
  
  if((x_pos == document[y_pos].size()) && (y_pos + 1 == document.size()))
  {
    string toAlter = document[y_pos];
    if (document[y_pos].size() != rowLen)
    {
      toAlter.append(toIns);
      document[y_pos] = toAlter;
    }
    else
    {
      document.push_back("");
      y_pos+=1;
      document[y_pos].append(toIns);
    }
    x_pos = document[y_pos].size();
  }
  else //if(x_pos != document[y_pos].size())
  {
    string toAlter = document[y_pos];
    if (document[y_pos].size() != rowLen)
    {
      toAlter.insert(x_pos, toIns);
      document[y_pos] = toAlter;
    }
    else
    {
      toAlter.insert(x_pos, toIns);
      char last_elem  = toAlter[toAlter.size() -1];
      string new_line(1, last_elem);
      toAlter.pop_back();
      document[y_pos] = toAlter;
      if (y_pos + 1 == document.size())
      {
        document.push_back("");
        document[y_pos+1].append(new_line);
      }
      else
      {
        new_line.append(document[y_pos+1]);
        document[y_pos+1] = new_line;
        for(int i = (y_pos+1); i < document.size(); ++i)
        {
          if (i == document.size()-1 && document[i].size() > rowLen)
          {
            document.push_back("");
            string last = document[i];
            char last_elem = last[last.size() - 1];
            string new_last(1, last_elem);
            last.pop_back();
            document[i] = last;
            document[i+1].append(new_last);
          }
          else if(document[i].size() > rowLen)
          {
            string current = document[i];
            string next = document[i+1];
            char current_last = current[current.size() - 1];
            string add_next(1, current_last);
            current.pop_back();
            add_next.append(next);
            document[i] = current;
            document[i+1] = add_next;
          }
        }
      }
    }
    if(x_pos >= rowLen)
    {
      x_pos = 0;
      y_pos += 1;
    }
    else
    {
      x_pos += 1;
    }
  }
  vector<int> ret = {x_pos, y_pos};
  docView.UpdateViewCursor(x_pos, y_pos);
  docView.UpdateViewRows(this->document, this->txtfile, y_pos);
  //docView.Color(keywords);
  return ret;
}

vector<int> TextDoc::EnterVec(int xIn, int yIn)
{
  if (xIn != -1)
  {
    x_pos = xIn;
    y_pos = yIn;
  }
  string app_to_next;
  if (x_pos == document[y_pos].size() && y_pos + 1 == document.size())
  {
    document.push_back("");
    app_to_next = "";
  }
  else if (x_pos != document[y_pos].size() && y_pos + 1 == document.size())
  {
    app_to_next = document[y_pos].substr(x_pos, document[y_pos].size());
    document[y_pos].erase(x_pos, document[y_pos].size()-x_pos);
    document.push_back("");
    document[document.size()-1].append(app_to_next);
  }
  else if (x_pos == document[y_pos].size() && y_pos + 1 != document.size())
  {
    document.insert(document.begin() + y_pos+1, "");
    app_to_next = "";
  }
  else
  {
    app_to_next = document[y_pos].substr(x_pos, document[y_pos].size());
    document[y_pos].erase(x_pos, document[y_pos].size()-x_pos);
    app_to_next.append(document[y_pos+1]);
    document[y_pos+1] = app_to_next;
    
    
  }
  
  for(int i = y_pos+1; i < document.size(); ++i)
  {
    if (document[i].size() > rowLen && i+1 != document.size())
    {
      string add = document[i].substr(rowLen-1, document[i].size() - (rowLen-1));
      document[i].erase(rowLen-1, document[i].size() - (rowLen-1));
      add.append(document[i+1]);
      document[i+1] = add;
    }
    else if(document[i].size() > rowLen && i+1 == document.size())
    {
      string add = document[i].substr(rowLen-1, document[i].size() - (rowLen-1));
      document[i].erase(rowLen-1, document[i].size() - (rowLen-1));
      document.push_back("");
      document[i+1] = add;
    }
  }
  
  x_pos = 0;
  y_pos += 1;
  
  vector<int> ret = {x_pos, y_pos};
  docView.UpdateViewCursor(x_pos, y_pos);
  docView.UpdateViewRows(this->document, this->txtfile, y_pos);
  //docView.Color(keywords);
  return ret;
}

vector<int> TextDoc::RemoveChar(int xIn, int yIn)
{
  if (xIn != -1)
  {
    x_pos = xIn;
    y_pos = yIn;
  }
  string retS;
  char ret;
  //if (x_pos != 0 && y_pos != 0 && document[y_pos-1].size() == rowLen)
  if (x_pos == 0 && y_pos == 0)
  {
    removed.push_back(ret);
    vector<int> coords = {x_pos, y_pos};
    return coords;
  }
  else if (x_pos == 0 && y_pos != 0 && document[y_pos-1].size() == rowLen)
  {
    retS = document[y_pos-1].substr(document[y_pos-1].size()-1,1);
    ret = retS[0];
    document[y_pos-1].erase(document[y_pos-1].size()-1,1);
    x_pos = document[y_pos-1].size();
    y_pos -= 1;
    for(int i = y_pos; i < document.size()-1; ++i)
    {
      string first_from_next = document[i+1].substr(0,1);
      document[i+1].erase(0,1);
      document[i].append(first_from_next);
      if (document[i+1].size() < rowLen)
      { 
        break;
      }
    }
  }
  else if(x_pos == 0 && y_pos != 0 && document[y_pos-1].size() != rowLen)
  {
    string add_prev  = document[y_pos];
    int original_prev_size = document[y_pos-1].size();
    document[y_pos-1].append(add_prev);
    document.erase(document.begin()+y_pos);
    char ret = '\0';
    for(int i = y_pos-1; i < document.size(); ++i)
      {
        if (document[i].size() > rowLen && i+1 != document.size())
        {
          string add = document[i].substr(rowLen-1, document[i].size() - (rowLen-1));
          document[i].erase(rowLen-1, document[i].size() - (rowLen-1));
          add.append(document[i+1]);
          document[i+1] = add;
        }
        else if(document[i].size() > rowLen && i+1 == document.size())
        {
          string add = document[i].substr(rowLen-1, document[i].size() - (rowLen-1));
          document[i].erase(rowLen-1, document[i].size() - (rowLen-1));
          document.push_back("");
          document[i+1] = add;
        }
      }
    x_pos = original_prev_size;
    y_pos -= 1;
  }
  else
  {
    retS = document[y_pos].substr(x_pos-1,1);
    ret = retS[0];
    document[y_pos].erase(x_pos-1,1);
    x_pos -= 1;
    for(int i = y_pos; i < document.size()-1; ++i)
    {
      string first_from_next = document[i+1].substr(0,1);
      document[i+1].erase(0,1);
      document[i].append(first_from_next);
      if (document[i+1].size() < rowLen)
      { 
        break;
      }
    }
  }
  removed.push_back(ret);
  
  vector<int> coords = {x_pos, y_pos};
  docView.UpdateViewCursor(x_pos, y_pos);
  docView.UpdateViewRows(this->document, this->txtfile, y_pos);
  //docView.Color(keywords);
  return coords;
}

void TextDoc::MoveCursLeft()
{
  if(x_pos != 0)
  {
    x_pos -=1;
  }
  else if (x_pos == 0 && y_pos != 0)
  {
    y_pos -= 1;
    x_pos = document[y_pos].size();
  }
  int temp_y = y_pos;
  if (y_pos > colLen )
  {
    temp_y = y_pos - colLen;
  }
  docView.UpdateViewCursor(x_pos, y_pos);
  docView.UpdateViewRows(this->document, this->txtfile, temp_y);
  //docView.Color(keywords);
}
void TextDoc::MoveCursRight()
{
  if(x_pos != document[y_pos].size())
  {
    x_pos +=1;
  }
  else if (x_pos == document[y_pos].size() && y_pos + 1 != document.size())
  {
    y_pos += 1;
    x_pos = 0;
  }
  docView.UpdateViewCursor(x_pos, y_pos);
  docView.UpdateViewRows(this->document, this->txtfile, y_pos);
  //docView.Color(keywords);
}
void TextDoc::MoveCursUp()
{
  if(y_pos != 0)
  {
    if(document[y_pos-1].size() < document[y_pos].size())
    {
      x_pos = document[y_pos-1].size();
    }
    y_pos -=1;
  }
  docView.UpdateViewCursor(x_pos, y_pos);
  docView.UpdateViewRows(this->document, this->txtfile, y_pos);
  //docView.Color(keywords);
}
void TextDoc::MoveCursDown()
{
  if (y_pos + 1 != document.size())
  {
    if(document[y_pos+1].size() < document[y_pos].size())
    {
      x_pos = document[y_pos+1].size();
    }
    y_pos +=1;
  }
  docView.UpdateViewCursor(x_pos, y_pos);
  docView.UpdateViewRows(this->document, this->txtfile, y_pos);
  //docView.Color(keywords);
}

void TextDoc::normalizevec()
{
  for(int i = 0; i < document.size(); ++i)
  {
    if (document[i].size() > rowLen && i+1 != document.size())
    {
      string add = document[i].substr(rowLen-1, document[i].size() - (rowLen-1));
      document[i].erase(rowLen-1, document[i].size() - (rowLen-1));
      add.append(document[i+1]);
      document[i+1] = add;
    }
    else if(document[i].size() > rowLen && i+1 == document.size())
    {
      string add = document[i].substr(rowLen-1, document[i].size() - (rowLen-1));
      document[i].erase(rowLen-1, document[i].size() - (rowLen-1));
      document.push_back("");
      document[i+1] = add;
    }
  }
}
void TextDoc::filemanage()
{
  fstream ifile;
  ifile.open(txtfile);
  if(ifile)
  {
    string mytext;
    while(getline(ifile, mytext))
    {
      document.push_back(mytext);
      if (mytext[mytext.size()-1] == '\n')
      {
        document.push_back("");
      }
      normalizevec();
    }
  }
  else
  {
    ifile.open(txtfile,fstream::out);
  }
  fstream kfile;
  kfile.open("keywords.txt");
  if(kfile)
  {
    this->keywords = "keywords.txt";
  }
  ifile.close();
  docView.UpdateViewRows(this->document, this->txtfile);
  //docView.Color(keywords);
}
char TextDoc::pop_remove_last()
{
  char last = removed[removed.size() - 1];
  removed.pop_back();
  return last;
}