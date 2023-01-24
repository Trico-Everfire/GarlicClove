
#include <cstring>
#include "library.h"
#include "logger.h"

int main( int argc, char **argv )
{
    LOG("TESTING CREATE!")
    auto packer = new GCPacker("/home/trico/Documents/GithubProjects/GarlicClove/cmake-build-debug/test/","textures");
    GCPacker::ErrorResult err;
    std::string text = "Father, it's me, Angelo";
    packer->addFile("henry.txt",(const std::byte *) text.c_str(), text.length(), err);

    std::string text2 = "Father, it's me, Micheal";
    packer->addFile("henry2.txt", (const std::byte *) text2.c_str(), text2.length(), err);
    LOG(std::to_string(static_cast<int>(err)))
    delete packer;

}