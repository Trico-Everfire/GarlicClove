#include <filesystem>
#include <cstring>
#include "library.h"
#include "logger.h"

GCPacker::GCPacker(const char *path, const char *fileName) {
    m_FileDirectory = path;
    m_FileName = fileName;

    std::string fullPath = std::string(m_FileDirectory);
    fullPath.append("/");
    fullPath.append(fileName);
    fullPath.append(".bulb");
    if (!std::filesystem::exists(fullPath))
        std::ofstream(fullPath).close();

    m_GarlicFile = std::fstream(fullPath, std::ios::in | std::ios::out | std::ios::ate | std::ios::binary);

    if (m_GarlicFile.is_open() && ((m_GarlicFile.tellg() % sizeof(GCFile)) == 0 || m_GarlicFile.tellg() == 0))
        m_Initialised = true;

    LOG("Initialised:")
    LOG(m_Initialised)


    for (int i = 0; i < m_GarlicFile.tellg(); i += GCFILE_STRUCT_SIZE) {

        m_GarlicFile.seekg(i);

        char pRawFile[GCFILE_STRUCT_SIZE];
        m_GarlicFile.read(pRawFile, GCFILE_STRUCT_SIZE);
        auto pGarlicFile = reinterpret_cast<GCFile *>(pRawFile);
        m_GCFileCache[pGarlicFile->fileName] = std::make_pair(m_GCFileCache.size(), *pGarlicFile);
        m_GarlicFile.seekg(0, std::ios::end);
    }
}

void GCPacker::addFile(const std::string &name, const std::byte *file, size_t size,
                       ErrorResult &error) {
    if (!m_Initialised) {
        error = ErrorResult::INITIALISATION_ERROR;
        return;
    }
    GCFile gcFile;
    if (m_GCFileCache.contains(name)) {
        gcFile = m_GCFileCache[name].second;
        gcFile.iterationVersion++;
        gcFile.fileSize = size;
        LOG(m_GCFileCache[name].first)
        m_GarlicFile.seekg(m_GCFileCache[name].first * GCFILE_STRUCT_SIZE);
    } else {
        gcFile = GCFile();
        gcFile.iterationVersion = 0;
        strcpy(gcFile.fileName, name.c_str());
        gcFile.compressed = false;
        gcFile.compressionType = 0;
        gcFile.fileSize = size;
    }

    std::string path{m_FileDirectory};
    path.append("/");
    path.append(m_FileName);
    path.append("_");
    path.append(std::to_string(gcFile.iterationVersion));
    path.append(".clove");

    std::ofstream vFile = std::ofstream(path, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);

    if (!vFile.is_open()) {
        error = ErrorResult::WRITE_ERROR;
        return;
    }

    gcFile.filePos = vFile.tellp();

    char rawFile[GCFILE_STRUCT_SIZE];
    memcpy(rawFile, &gcFile, GCFILE_STRUCT_SIZE);

    m_GarlicFile.write(rawFile, GCFILE_STRUCT_SIZE);

    if (m_GarlicFile.bad()) {
        error = ErrorResult::WRITE_ERROR;
        return;
    }

    vFile.write(reinterpret_cast<const char *>(file), size);

    if (vFile.bad()) {
        vFile.close();
        error = ErrorResult::WRITE_ERROR;
        return;
    }

    if (m_GCFileCache.contains(name))
        m_GCFileCache[name].second = gcFile;
    else
        m_GCFileCache[name] = std::make_pair(m_GCFileCache.size(), gcFile);

    error = ErrorResult::NO_ERROR;

}

void GCPacker::readFile(const std::string name, std::byte *file, size_t &size, GCPacker::ErrorResult &error) {

    if (!m_GCFileCache.contains(name)) {
        error = ErrorResult::FILE_NOT_FOUND_ERROR;
        return;
    }
    auto gcFile = m_GCFileCache[name].second;

    std::string path{m_FileDirectory};
    path.append("/");
    path.append(m_FileName);
    path.append("_");
    path.append(std::to_string(gcFile.iterationVersion));
    path.append(".clove");

    std::ifstream vFile(path);
    if (!vFile.is_open()) {
        error = ErrorResult::READ_ERROR;
        return;
    }

    vFile.seekg(gcFile.filePos);

    if (!vFile.read(reinterpret_cast<char *>(file), gcFile.fileSize)) {
        error = ErrorResult::READ_ERROR;
        return;
    }

    size = gcFile.fileSize;
    error = ErrorResult::NO_ERROR;
}

