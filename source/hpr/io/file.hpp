#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace hpr
{

class File
{

public:

    enum FileMode
    {
        Write = std::ios::out,
        Read = std::ios::in,
        Binary = std::ios::binary,
        Append = std::ios::app
    };

protected:

    std::string p_name;
    unsigned int p_mode;
    std::fstream p_file;

public:

    // Constructors

    File();

    File(const std::string& filename);

    File(const char* filename);

    ~File();

    // Member functions

    void open(const std::string& filename, unsigned int filemode = File::Read);

    void close();

    size_t length();

    std::stringstream read();
};

}
