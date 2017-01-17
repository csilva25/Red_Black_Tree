/**
 * @file rbtree.cpp   functions that insert,delete,find,
 *                     and print the tree.
 *
 * @brief
 *    in this file, we will be setting up the tree
 *    with the given commands such as insert and delete.
 *    this file holds the functions to find a node as well
 *    as the public functions that call the private
 *
 * @author Cristian Silva
 * @date 11/09/16
 */

#include <iostream>
#include <iomanip>
#include "rbtree.h"

using namespace std;
using std::cout;
using std::setw;
using std::endl;

/*
 this function prints the given tree as it would look if 
demestrated on a visual level.
 
 @param node* x
   this is used as a base case, as long as the node doesnt 
   reach the root, keep printing
 @param int depth
   this is the int value which presses the output further out
   or in to display as the tree would look.
 */
void RBTree::reverseInOrderPrint(Node *x, int depth) {
   if ( x != nil ) {
      reverseInOrderPrint(x->right, depth+1);
      cout << setw(depth*4+4) << x->color << " ";
      cout << *(x->key) << " " << *(x->value) << endl;
      reverseInOrderPrint(x->left, depth+1);
   }
}
/*
 the constructor for the RBTree which makes a new node for 
 the tree and sets the root to that new node.
 */
RBTree::RBTree()
{
   nil = new Node();
   root = nil;
}
/*
 the destructor for the tree. takes apart the tree and sets it 
 up for the nodes to delete.
 */
RBTree::~RBTree()
{
   destructor(root);
   delete nil;
}
/*
 the destructor for the rbtree which is a recursive function that
traverses all the way left, moving right every step if avalible 
deleting the node and setting it to nil for later deletion.
 
 @param Node* &z
      a refenced node pointer that is set to the node that is 
      meant to be taken apart, it does this recursively moving 
      left and right in order to hit every node.
 */
void RBTree::destructor(Node* &z)
{
   if (z== nil)
      return;
   destructor(z->left);
   destructor(z->right);
   delete z;
   z = nil;
}
/*
 node constructor that instantiates the member variables
 to nullptr and sets the color to black in order to make 
 changes to appropriete values later.
 */
RBTree::Node::Node()
{
   left = nullptr;
   right = nullptr;
   parent = nullptr;
   key = nullptr;
   value = nullptr;
   color = 'B';
}
/*
 the constructor for the node which sets its member variables
 to default values in order to set up for later definitions or
 instantiation.
 @param const string& k
      constant string (does not change) refence k that 
      is set to the key by allocating memory and setting
      for it.
 @param const string& v
      constant string (does not change) refence v that
      is set to the value by allocating memory and setting
      for that member veriable.
 */
RBTree::Node::Node(const string& k, const string& v)
{
   color = 'R';
   parent = nullptr;
   left = nullptr;
   right = nullptr;
   key = new string(k);
   value = new string (v);
}
/*
 the destructor of the node that deallocates all key 
 and values.
 */
RBTree::Node::~Node()
{
   delete key;
   delete value;
}
/*
 fuction which traverses through the tree looking for a node
 which matches the given key in order add that nodes value
 to the vecter which will be returned.
 
 @return vector <const string*>
   returns a vector with strings of the values that were
   matched with the key refenced in.
 @param const string &key
      the paramater is a constant string refenced that is 
      used to find the paired value that will be inserted
      to the vector.
 */

vector<const string* > RBTree::rbFind(const string &key)
{
    Node* z = rbTreeSearch(root,key);
    Node* saved;
    vector <const string*> myvector;
    
    if (z != nil){
        
        Node* predecessor = rbTreePredecessor(z);
        Node* successor = rbTreeSuccessor(z);
        
        while (*(predecessor->key) == key)
        {
            
            while (predecessor->parent != nil &&
                   *(predecessor->parent->key) == key &&
                   predecessor== predecessor->parent->right)
                
                predecessor = predecessor->parent;
            
            if (predecessor->left != nil)
            {
                saved = predecessor;
                predecessor = rbTreePredecessor(predecessor);
                
                if (*(predecessor->key) != key)
                {
                    predecessor = saved;
                    break;
                }
            }
            else break;
        }
        
        if (*(predecessor->key) == key)
            successor = predecessor;
        
        else successor = z;
        
        while (*(successor->key) == key)
        {
            if (root == successor &&
                successor->left == nil &&
                successor->right == nil)
            {
                myvector.push_back(successor->value);
                break;
            }
            
            if (root == successor &&
                successor->left != nil &&
                successor->right == nil)
            {
                myvector.push_back(successor->value);
                break;
            }
            
            else if (successor->right == nil)
            {
                myvector.push_back(successor->value);
                
                if (successor == successor->parent->left)
                    successor = successor->parent;
                
                else
                {
                    if (successor->parent == root)
                        break;
                    
                    while (successor == successor->parent->right)
                    {
                        successor = successor->parent;
                        if (successor->parent == nil)
                            break;
                    }
                    
                    if (successor->parent != nil)
                        successor = successor->parent;
                    
                    else break;
                }
            }
            
            else if (successor->right != nil && successor->left != nil)
            {
                myvector.push_back(successor->value);
                successor = rbTreeSuccessor(successor);
            }
            
            else if (successor->right != nil && successor->left == nil)
            {
                myvector.push_back(successor->value);
                successor = rbTreeSuccessor(successor);
            }
        }
    }
    
    return myvector;
}

/*
 calls print from the root of the tree.
 */
void RBTree::rbPrintTree()
{
   reverseInOrderPrint(root, 0);
}
/*
 function which called the insert function after a node is 
 paired with its key and value then allocated to a node
 which it will pass to the insert and insert in the 
 proper place.
 @param const string& key
      this refence to constant string is simply paired 
      with the refenced value and is set to a allocated
      node and set to a node pointer named 'z'. then it 
      is passed to the insert function.
 @param const string& value
      this refence to constant string is simply paired
      with the refenced key and is set to z as mentioned
      above.
 */
void RBTree::rbInsert(const string &key, const string &value)
{
   Node* z = new Node (key,value);
   rbInsert(z);
}
/*
 the insert function which given the node that we want to insert
 will traverse down from the root, comparing if the node is 
 greater or lesser than every node it passes through.
 
 @param Node* z
   this is a node pointer named 'z' which is used to compare
   each node throughout the tree in order to find where to
   insert it while passing all properities.
 */
void RBTree::RBTree::rbInsert(Node *z)
{
   Node* y = nil;
   Node* x = root;
   while (x != nil)
   {
      y = x;
      if (*z->key < *x->key)
      {
         x = x->left;

      }
      else
      {
         x = x->right;
      }
   }
   z->parent = y;
   if (y == nil)
      root = z;
   else if (*z->key < *y->key)
      y->left = z;
   else y->right = z;
   z->left = nil;
   z->right = nil;
   z->color = 'R';
   rbInsertFixup(z);
}
/*
 function which decideds what color should the newly allocated
 node be by changing its color or surrounding node's colors.
 
 @param Node* z
      node passed in which is looked at its color, so 
      the new node can be placed in the right location
      based on its color.
 */

void RBTree::rbInsertFixup(Node* z)
{
   Node* y;
   while (z->parent->color == 'R')
   {
      if (z->parent == z->parent->parent->left)
      {
         y = z->parent->parent->right;
         if (y->color == 'R')
         {
            z->parent->color = 'B';
            y->color = 'B';
            z->parent->parent->color = 'R';
            z = z->parent->parent;
         }
         else 
         {
            if (z == z->parent->right)
            {
               z = z->parent;
               leftRotate(z);
            }
            z->parent->color = 'B';
            z->parent->parent->color = 'R';
            rightRotate(z->parent->parent);
         }
      }
      else
      {
         y = z->parent->parent->left;
         if (y->color == 'R')
         {
            z->parent->color = 'B';
            y->color = 'B';
            z->parent->parent->color = 'R';
            z = z->parent->parent;
         }
         else 
         {
            if (z == z->parent->left)
            {
               z = z->parent;
               rightRotate(z);
            }
            z->parent->color = 'B';
            z->parent->parent->color = 'R';
            leftRotate(z->parent->parent);
         }
      }

   }
   root->color = 'B';
}
/*
 keeps the tree balanced by grabbing the nodes left child 
 putting it in the same position as the current and adding
 the current to the left child right.and add the previous 
 right and seeting it to the currents left
 
 
 @param Node* z
      from the node that is passed in, it finds the proper nodes
      which it will rotate left
 */
void RBTree::leftRotate(Node* x)
{
   Node* y = x->right;
   x->right = y->left;
   if (y->left != nil)
      y->left->parent = x;
   y->parent = x->parent;
   if (x->parent == nil)
      root = y;
   else if (x == x->parent->left)
      x->parent->left = y;
   else x->parent->right = y;
   y->left = x;
   x->parent = y;

}
/*
 sets the passed in node to y. turns ys subtree to the nodes 
 right subtree. linkes the nodes parent to y and puts the nodes
 on y's left.
 
 @param Node* z
 from the node that is passed in, it finds the proper nodes
 which it will rotate right
 */
void RBTree::rightRotate(Node* x)
{
   Node* y = x->left;
   x->left = y->right;
   if (y->right != nil)
      y->right->parent = x;
   y->parent = x->parent;
   if (x->parent == nil)
      root = y;
   else if (x == x->parent->right)
      x->parent->right = y;
   else x->parent->left = y;
   y->right = x;
   x->parent = y;

}
/*
 replaces two nodes by useing nil.
 
 @param Node* u
   this node is usually a static node where v where be refenced from
 @param Node* v
   this node is usually a left or right child of a node.
 */
void RBTree::rbTransplant(Node* u, Node* v)
{
   if (u->parent == nil)
      root = v;
   else if (u == u->parent->left)
      u->parent->left = v;
   else u->parent->right = v;
   v->parent = u->parent;
}

/*
 this is the fuction that is called from rbapp that deletes 
 the node that matches key and value only. 
 
 @param const string & key
      this is the key refence which is be ing used to find the 
      node with the proper value
 @param const string & value
 this is the value refence which is be ing used to search the
 node with the proper key
 */

void RBTree::rbDelete(const string &key, const string &value)
{
   vector<const string*> myvector;
   // cout << 1 << endl;
   Node* z = rbTreeSearch(root,value);
   // cout << 2 << endl;
   myvector = rbFind(key);
    //cout << 3 << endl;
   for (unsigned int i =0; i < myvector.size(); i++)
      if (z->value == myvector[i])
         rbDelete(z);
         //cout << z-> value << " " << myvector[i];
}
/*
 this function passes in a node that sets up the node thats 
 going to be taken out of the tree by placing the surrounding
 nodes in its right place to compinsate for the absence of the
 node being delete.
 
 @param Node* z
      the node which where we will start on the tree and travel 
      through the tree.
 
 */
void RBTree::rbDelete(Node* z)
{
   Node* y = z;
   Node* x = nullptr;
   char yColor = y->color;
   if (z->left == nil)
   {
      x = z->left;
      rbTransplant(z, z->left);
   }
   else
   {
      y = rbTreeMaximum(z->left);
      yColor = y->color;
      x = y->left;
      if (y->parent == z)
         x->parent = y;
      else
      {
         rbTransplant(y, y->left);
         y->left = z->left;
         y->left->parent = y;
      }
      rbTransplant(z, y);
      y->right = z->right;
      y->right->parent = y;
      y->color = z->color;
   }
   if (yColor == 'B')
      rbDeleteFixup(x);
}
/*
 this function fixes the tree after the node has been removed.
 
 @param Node* z
   the node that is being used to travel through the tree
   that will fix the tree after a node is deleted.
 
 */
void RBTree::rbDeleteFixup(Node* x)
{
   Node* w = nullptr;
   while (x != root && x->color == 'B')
   {
      if (x == x->parent->left)
      {
         w = x->parent->right;
         if (w->color == 'R')
         {
            w->color = 'B';
            x->parent->color = 'R';
            leftRotate(x->parent);
            w = x->parent->right;
         }
         if (w->left->color == 'B' and w->right->color == 'B')
         {
            w->color = 'R';
            x = x->parent;
         }
         else if (w->right->parent->color == 'B')
         {
            w->left->color = 'B';
            w->color = 'R';
            rightRotate(w);
            w = x->parent->right;
         }
         w->color = x->parent->color;
         x->parent->color = 'B';
         w->right->color = 'B';
         leftRotate(x->parent);
         x = root;
      }
      else
      {
         if (x == x->parent->right)
         {
            w = x->parent->left;
            if (w->color == 'R')
            {
               w->color = 'B';
               x->parent->color = 'R';
               rightRotate(x->parent);
               w = x->parent->left;
            }
            if (w->right->color == 'B' and w->left->color == 'B')
            {
               w->color = 'R';
               x = x->parent;
            }
            else if (w->left->parent->color == 'B')
            {
               w->right->color = 'B';
               w->color = 'R';
               leftRotate(w);
               w = x->parent->left;
            }
            w->color = x->parent->color;
            x->parent->color = 'B';
            w->left->color = 'B';
            rightRotate(x->parent);
            x = root;
         }
      }
   }
   x->color = 'B';
}
/*
 this function finds the predecessor of the node passed in.
 i looks for the largest smallest value.
 
 @param Node* z
 the node in which the tree will travel to find the correct node
 */
RBTree:: Node* RBTree::rbTreePredecessor(Node* z){
    if (z == nil) return nil;
   if(z->left!=nil)
   {
      z=z->left;
      while(z->right!=nil)
      {
         z=z->right;
      }
   }
   return z;
}
/*
 this function finds the successor of the node passed in.
 i looks for the smallest largest value.
 
 @param Node* z
   the node in which the tree will travel to find the correct node
 */
RBTree:: Node* RBTree::rbTreeSuccessor(Node* z){
    if (z == nil) return nil;
   if(z->right!=nil)
   {
      z=z->right;
      while(z->left!=nil)
         z=z->left;
   }
   return z;
}
/*
 search function which compares it given value to the current 
 nodes value, if less then, it traverses left and vice versa.
 this function calls the tree search which does something similiar
 except returns the spot where it found the node.
 
 @param Node* z
 @param const string & value
 */
RBTree:: Node* RBTree::rbTreeSearch(Node* z, const string & value)
{
   if (z == nil)
   {
      return z;
   }
   else if (value == *z -> value)
   {
      return z;
   }
   else if (value < *z->value)
   {
      return rbTreeSearch( z->left, value);
   }
   else
   {
      return rbTreeSearch( z -> right, value);
   }

}
/*
 this function travels as far left from the tree as possible, trying
 to find the minuim value of the tree
 @param Node* z
      node which the tree will begin its search for the least node
 */
RBTree:: Node* RBTree::rbTreeMinimum(Node* z){
   while (z->left != nil) {
      z = z->left;
   }
   return z;
}
/*
 this function travels as right as possible trying to fiund the greatest 
 node.
 
 @param Node* z
          node which the tree will begin its search for the greatest node
 */
RBTree:: Node* RBTree::rbTreeMaximum(Node* z){
   while (z->right !=nil) {
      z=z->right;
   }
   return z;
}

