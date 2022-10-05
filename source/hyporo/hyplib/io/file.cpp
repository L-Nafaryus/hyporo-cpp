
#include "file.hpp"


namespace hpr
{

File::File() :
    p_name {"\0"},
    p_mode {File::Read}
{}

File::File(const std::string &filename) :
    p_name {filename},
    p_mode {File::Read}
{}

File::File(const char *filename) :
    p_name {filename},
    p_mode {File::Read}
{}

File::~File()
{
    if (p_file.is_open())
        p_file.close();
}

void File::open(const std::string &filename, unsigned int filemode)
{
    std::ios::openmode mode = filemode & File::Read ? std::ios::in : std::ios::out;

    if (filemode & File::Binary)
        mode |= std::ios::binary;

    p_file.open(filename, mode);

    if (!p_file.is_open())
        throw std::ios::failure("Could not open file");
}

void File::close()
{
    if (p_file.is_open())
        p_file.close();
}

size_t File::length()
{
    std::streampos size = 0;
    std::streampos oldPos = p_file.tellg();

    p_file.seekg(0, std::ios::beg);
    size = p_file.tellg();
    p_file.seekg(0, std::ios::end);
    size = p_file.tellg() - size;
    p_file.seekg(oldPos, std::ios::beg);

    return (size_t)size;
}

std::stringstream File::read()
{
    std::stringstream content;
    content << p_file.rdbuf();

    return content;
}

}