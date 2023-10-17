#ifndef Tcomm_h
#define Tcomm_h

#include <vector>
using namespace std;
// ******************************************************
// Implement command design pattern

class ECCommand
{
public:
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
};

// ******************************************************
// Implement command history

class ECCommandHistory
{
public:
    ECCommandHistory();
    virtual ~ECCommandHistory();
    void Undo();
    void Redo();
    void clear_lists();
    void ExecuteCmd( ECCommand *pCmd );
    
private:
    vector<ECCommand*> history;
    vector<ECCommand*> redo_undo;
};


#endif /* ECCommand_h */
