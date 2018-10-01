//! @file ifstreamcheck.cpp
//!
//!
//! @brief C and C++ file reading and writing styles.
//! 
//!
//! @Author Usman Ashraf 
//! @email  u.ashraf1988@gmail.com
//!
//! GNU Lesser General Public License v3.0, free to use, re-use, distriution and modification
//! provided this above header information. Author take no responsibility for any outcome of
//! this code. 
//!
//! If you need more information, please, contact me at my above email address.
//
//! Successfully executed under OS:
//!                                RedHat 7
//!                                RedHat 5
//!                                CentOS 7
//!                                Ubuntu 16
//!                                yocto-created minimal image
//!
//!
//! Compiled with GCC Version > 4.5


#include<iostream>
#include<fstream>
#include <sstream>
#include<string>
#include<exception>
#include<string.h>

using namespace std;

#define MAX_BUF 1024

void usageErr(const char * formatStyle, const char * msg)
{
  fprintf(stdout, formatStyle, msg);
  exit(1);
}

void info(const char * formatStyle, const char * msg)
{
  fprintf(stdout, formatStyle, msg);
}


int main(int argc, char * argv[] )
{
  int num1=0;
  int num2=0;
  string str="";

  if(argc < 2)
  {
    usageErr("\n%s\n", "Need a fileName");
  }

  string origFileName(argv[1]);
  info("\nFileName is: %s\n", fileName.c_str());

  string fileName = origFileName;

//##########  for c++ style reading and writing.

  ifstream fileReader;
  ofstream fileWriter;
  try
  {
    fileReader.open(fileName.c_str(), ifstream::in);

    fileName = "data2.txt";
    fileWriter.open(fileName.c_str(), fstream::out);

    if(fileReader.is_open() && fileWriter.is_open())
    {
      while( (fileReader.peek()!=EOF) && (!fileReader.eof()) )
      {
        // reading.
        //fileReader >> num1 >> num2 >> str;
        string str1;
        getline(fileReader, str1,'\n');

        // a little processing part too.
        stringstream ss1(str1);
        ss1 >> num1 >> num2 >> str;

        //writing 
        stringstream ss2;
        ss2 << "\nnum1: " << num1 << ", num2: " << num2 << ", str: " << str << endl;
        cout << ss2.str();
        fileWriter << ss2.str();
      }
    }
  }
  catch (exception & ex)
  {
    fileReader.close();
    fileWriter.close();
    usageErr("\nException: %s\n", ex.what());
  }

  fileReader.close();
  fileWriter.close();



//########## For C style reading and writing.


  FILE *fileStreamRead;
  FILE *fileStreamWrite;
  char buffer[MAX_BUF]={0};

  fileStreamRead  = fopen(fileName.c_str(), "r");

  fileName = "data3.txt";
  fileStreamWrite = fopen(fileName.c_str(), "w");

  
  if(fileStreamRead!=NULL && fileStreamWrite!=NULL)
  {
    // reading and checks.
    while( (!feof(fileStreamRead)) && (fgets(buffer, MAX_BUF, fileStreamRead)!=NULL)    )
    {
      info("\n%s\n", buffer);

      // writing and checks.
      if(fputs(buffer, fileStreamWrite)<0)
      {
        fclose(fileStreamRead);
        fclose(fileStreamWrite);
        usageErr("\nError: %s\n","C File writing error.");
      }
    }
  }

  fclose(fileStreamRead);
  fclose(fileStreamWrite);



  return 0;
}
