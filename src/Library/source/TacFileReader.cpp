/*!
\author Nathan Park
*/
#include "TacFileReader.h"
//#include <cstdio>
#include <fstream>
#include "TacErrorIf.h"

namespace Tac
{
  bool FileReader::ReadFile( const std::string & filename )
  {
    //*
    std::ifstream myfile(filename.c_str(), std::ifstream::binary);
    bool success = myfile.is_open();
    if (success)
    {
      myfile.seekg(0, myfile.end);
      unsigned length = (unsigned) myfile.tellg();
      mSize = length;
      myfile.seekg(0, myfile.beg);

      // memory allocation
      mSource = std::unique_ptr<char>(new char[length + 1]);
      char * data = mSource.get();
      
      // null termination
      data[length] = 0; 
      myfile.read (data,length);
      myfile.close();
    }
    return success;
  }

  const char * FileReader::GetSource() const
  {
    return mSource.get();
  }

  FileReader::FileReader() : mSize(0)
  {

  }

}
