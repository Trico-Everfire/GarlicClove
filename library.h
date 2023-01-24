#pragma once


#include <fstream>
#include <map>
#include <any>

struct GCFile
{
    unsigned int fileSize; //size of the file.
    char fileName[256]; //the file's name.
    unsigned int iterationVersion; //the file's iteration version, 0 is the directory itself.
    unsigned int filePos; //the amount of bytes from the start of the file where this is stored.
    bool compressed; //if the file is compressed or not.
    unsigned int compressionType; //the type of compression used. 0 is no compression.
};

class GCPacker
{
private:
    std::fstream m_GarlicFile;
    std::string m_FileDirectory;
    std::string m_FileName;
    std::map<std::string, std::pair<int, GCFile>> m_GCFileCache;
    bool m_Initialised = false;
    static constexpr int GCFILE_STRUCT_SIZE = sizeof(GCFile);
public:
    enum class ErrorResult {
        NO_ERROR = 0,
        READ_ERROR,
        FILE_NOT_FOUND_ERROR,
        WRITE_ERROR,
        INITIALISATION_ERROR
    };

    GCPacker(const char* path, const char* fileName);
    ~GCPacker() {
        if(m_Initialised)
            m_GarlicFile.close();
    }
    void addFile(const std::string& name, const std::byte* file, size_t size, ErrorResult &error);
    void readFile(const std::string name, std::byte* file, size_t &size, ErrorResult &error);
};
