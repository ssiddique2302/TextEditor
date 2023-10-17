// Test code for editor
#include "ECTextViewImp.h"
#include "observers.cpp"
#include "TextDocument.h"
#include <iostream>
#include <string>

using namespace  std;

int myCounter = 0;


int main(int argc, char *argv[])
{
    //
  ECTextViewImp wndTest;
  TextDocView docView(wndTest);
  string textfile = argv[1];
  TextDoc doc(docView, textfile);
  TextDocCtrl &docCtrl = doc.GetCtrl();
  printKeyOb pko(docCtrl, wndTest);
  leftKeyOb lko(docCtrl, wndTest);
  rightKeyOb rko(docCtrl, wndTest);
  enterKeyOb eko(docCtrl, wndTest);
  backspaceKeyOb bko(docCtrl, wndTest);
  upKeyOb uko(docCtrl, wndTest);
  downKeyOb dko(docCtrl, wndTest);
  ESCOb esko(docCtrl, wndTest);
  undoKeyOb udko(docCtrl, wndTest);
  redoKeyOb rdko(docCtrl, wndTest);
  wndTest.Attach(&rdko);
  wndTest.Attach(&udko);
  wndTest.Attach(&esko);
  wndTest.Attach(&dko);
  wndTest.Attach(&uko);
  wndTest.Attach(&bko);
  wndTest.Attach(&eko);
  wndTest.Attach(&rko);
  wndTest.Attach(&lko);
  wndTest.Attach(&pko);
  doc.setRowCol();
  doc.filemanage();
  // will need to check if there is a argument for a file
  // create a document function which will add a text document, then set all elements of txt file to its vector, then call updateview 
  // then whatever changes you make, you should also write those changes to the actual document .
  
  // send file name to document, it should have func to set file name to set it as one of its attributes. if it can open file, then copy everything to its vector and call update view. 
  //if it cannot open, make new txt file. 
  
  // from this point forward, view should clear the document and add all row of document to file aswell 
  wndTest.AddStatusRow("Editor", "Mode: Command", true);
  wndTest.Show();
    // document model should have reference to ectextview imp, and it should also have a control attribute which it gives *this to 
    // document control, shoudl get as reference the document, which has the ectextview imp methods also. purpose is that this is what actually calls those methods through commands. it will give to command the document, so it can call textview imp functions, and whatever params needed. 
    // the commands 
    return 0;
}
