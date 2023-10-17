
#include "TextCommands.h"

using namespace std;

// ******************************************************
// Implement command history

ECCommandHistory :: ECCommandHistory() 
{
  // your code goes here
}

ECCommandHistory :: ~ECCommandHistory()
{
    for(unsigned int i=0; i<history.size(); ++i)
    {
        delete history[i];
    }
    history.clear();
    for(unsigned int i=0; i<redo_undo.size(); ++i)
    {
        delete redo_undo[i];
    }
    redo_undo.clear();
    
}


void ECCommandHistory :: Undo()
{
  if(history.size() == 0)
  {
    return;
  }
  int i = 0;
  while ( i < history.size())
  {
    auto last_cmd = history.back();
    history.pop_back();
    last_cmd->UnExecute();
    redo_undo.push_back(last_cmd);
    ++i;
  }
  
}

void ECCommandHistory :: Redo()
{
  if(redo_undo.size() == 0)
  {
    return ;
  }
  int i = 0;
  while ( i < redo_undo.size())
  {
    auto redo_last = redo_undo.back();
    redo_undo.pop_back();
    redo_last->Execute();
    history.push_back(redo_last);
    ++i;
  }
}

void ECCommandHistory :: clear_lists()
{
  for(unsigned int i=0; i<history.size(); ++i)
    {
        delete history[i];
    }
    history.clear();
    for(unsigned int i=0; i<redo_undo.size(); ++i)
    {
        delete redo_undo[i];
    }
    redo_undo.clear();
}

void ECCommandHistory :: ExecuteCmd( ECCommand *pCmd )
{
  pCmd->Execute();
  history.push_back(pCmd);
}
