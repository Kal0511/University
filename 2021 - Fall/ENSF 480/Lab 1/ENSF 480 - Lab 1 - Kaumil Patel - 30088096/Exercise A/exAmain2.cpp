// exAmain.cpp
// ENSF 480 Lab 1 - Exercise A
// Author - M. Moussavi

#include <iostream>
using namespace std;
#include "mystring.h"

int main(void)
{
  cout << "Mystring *ar[3];" << endl;
  Mystring *ar[3];
  cout << "Mystring c = 3;" << endl;
  Mystring c = 3;
  cout << "ar[2] = &c;" << endl;
  ar[2] = &c;
  cout << "ar[1] = ar[0] = nullptr;" << endl;
  ar[1] = ar[0] = nullptr;
  
  cout << "A BLOCK OF CODE BEGINS HERE" << endl;  
  // A BLOCK OF CODE BEGINS HERE
  {
	cout << "Mystring x[2];" << endl;
    Mystring x[2];

cout << "x[0].set_str(ENEL);" << endl;
    x[0].set_str("ENEL");
	cout << "x[1].set_str(0);" << endl;
    x[1].set_str("0");
	cout << "Mystring* z = new Mystring(4);" << endl;
    Mystring* z = new Mystring("4");
      
	  cout << "x[0].append(*z).append(x[1]);" << endl;
    x[0].append(*z).append(x[1]);
  
  cout << "Mystring mars = x[0];" << endl;
    Mystring mars = x[0];

cout << "x[1] = x[0];" << endl;
    x[1] = x[0];
    
	cout << "Mystring jupiter(White);" << endl;
    Mystring jupiter("White");

cout << "ar[0] = new Mystring (Yellow);" << endl;
      ar[0] = new Mystring ("Yellow");

cout << "Before BLOCK ENDS HERE" << endl;
  } // BLOCK ENDS HERE
cout << "After BLOCK ENDS HERE" << endl;

cout << "c.set_str(A);" << endl;
  c.set_str("A");
  cout << "delete  ar [0];" << endl;
  delete  ar [0];
 
 cout << "Mystring d = Green;" << endl;
  Mystring d = "Green";
  
  cout << "\nProgram terminated successfully." <<endl;
  cout << "Before return" << endl;
  return 0;
  cout << "After return" << endl;
}

