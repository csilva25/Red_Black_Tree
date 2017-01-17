/**
 * @file rbapp.cpp   hold main and calls the functions 
 *                   according to the input.
 *
 * @brief
 *    this function takes an input and calls the tree's
 *    function to process that input properly.
 *    also the heart of main.
 *
 * @author Cristian Silva
 * @date 11/09/16
 */

#include "rbapp.h"
#include "rbtree.h"
#include <iostream>
using namespace std;
/*
 in main, we intantiate a tree and call its mainloop
 */
int main()
{
   RBapp myRBT;
   myRBT.mainLoop();
}
/*
 this function processes an input as long as there is one
 */
void RBapp::mainLoop()
{
   string cmd;
   bool dont_exit = true ;
   while (dont_exit && cin >> cmd)
   {
     dont_exit = processCommand(cmd);
   }
}

/*
 @param string &cmd
         refence of the string cmd passed by the 
         mainloop. chooses which process to call.
         if quit is called or a input is incorrect
         it returns to the main.
 */


bool RBapp::processCommand(string & cmd)
{
   string key;
   if (cmd == "quit")
      return false;
   else
   {
      if (cmd == "find")
      {
         cin >> key;
         processFind(key);
         return true;
      }
      if (cmd == "delete")
      {
         cin >> key;
         processDelete(key);
         return true;
      }
      if (cmd == "insert")
      {
         cin >> key;
         processInsert(key);
         return true;
      }
      if (cmd == "print")
      {
         processPrint();
         return true;
      }
   }
   return true;
}

/*
 @param string &key
         refence of the string key passed by the
         mainloop. used to find the appopriet value that
         goes along with the key in order to pair it and
         print it as follows.
 */
void RBapp::processFind(string & key)
{
   vector<const string*> myvector;
 myvector =  myRBT.rbFind(key);
   for (unsigned int i =0; i < myvector.size(); i++)
      cout << key << " " << *myvector[i]<<endl;
   
   
}
/*
 @param string &key
         refence of the string key passed by the
         mainloop. used to pair with the value that
         was cin'd in order to insert it into the 
         r-b tree approprietely.
 */
void RBapp::processInsert(string & key)
{
   string value;
   cin >> value;
   myRBT.rbInsert(key, value);
}
void RBapp::processPrint()
{
   myRBT.rbPrintTree();
}
/*
 @param string &key
         refence of the string key passed by the
         mainloop. used to find the node that matches
         the value that was cin'd. once paired with the 
         value. if the values cin'd match with the paired
         value, it is deleted.
 */
void RBapp::processDelete(string & key)
{
   string value;
   cin >> value;
   vector<const string*> myvector;
  //  cout << 1 << endl;
   myvector =  myRBT.rbFind(key);
    //cout << 2 << endl;
    //for (unsigned int i =0; i < myvector.size(); i++)
    //   if (value == *myvector[i])
          myRBT.rbDelete(key, value);
}
