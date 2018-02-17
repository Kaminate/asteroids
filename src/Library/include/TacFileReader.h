/*!
\author Nathan Park
*/
#pragma once

#include <memory>
#include <string>

namespace Tac
{
  class FileReader
  {
  public:
    // Reads the file into a null terminated string
    FileReader();
    bool ReadFile(const std::string & filename);
    const char * GetSource() const;
  private:
    std::unique_ptr<char> mSource;
    unsigned mSize; // not including the null char
  };

}

