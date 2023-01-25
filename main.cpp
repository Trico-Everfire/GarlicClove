
#include <cstring>
#include <bits/unique_ptr.h>
#include "library.h"
#include "logger.h"

int main( int argc, char **argv )
{
    LOG("TESTING CREATE!")
    auto packer = new GCPacker("/home/trico/Documents/GithubProjects/GarlicClove/cmake-build-debug/test/","textures");
    GCPacker::ErrorResult err;
    std::string text = "Father, it's me, Angelo";
    packer->addFile("henry.txt",(const std::byte *) text.c_str(), text.length(), err);
    LOG(text.length())

    std::string text2 = "Father, it's me, Stefan";
    packer->addFile("henry2.txt", (const std::byte *) text2.c_str(), text2.length(), err);
    LOG(text2.length())


    LOG(std::to_string(static_cast<int>(err)))

    std::byte data[2068];
    size_t size;
    packer->readFile("henry.txt", data, size, err);

    LOG(std::string(reinterpret_cast<char*>(data)).substr(0,size));
    LOG(size)

    LOG(std::to_string(static_cast<int>(err)))

    packer->readFile("henry2.txt", data, size, err);

    LOG(std::string(reinterpret_cast<char*>(data)).substr(0,size));
    LOG(size)

    LOG(std::to_string(static_cast<int>(err)))
    delete packer;

}