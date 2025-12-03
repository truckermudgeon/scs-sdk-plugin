#ifndef SHAREDMEMORY_HPP
#define SHAREDMEMORY_HPP

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "scs-telemetry-common.hpp"

#undef SHAREDMEM_LOGGING
#if ETS2_PLUGIN_LOGGING_ON == 1
#if ETS2_PLUGIN_LOGGING_SHAREDMEMORY == 1
#define SHAREDMEM_FILENAME ETS2_PLUGIN_FILENAME_PREFIX "sharedmem.txt"
#define SHAREDMEM_LOGGING 1
#endif
#endif

class SharedMemory
{
protected:
#ifdef _WIN32
    LPCWSTR namePtr;
#else
    const char *namePtr;
#endif
    unsigned int mapsize;

    // MMF specifics
#ifdef _WIN32
    HANDLE hMapFile;
#else
    int hMapFile;
#endif
    void *pBufferPtr;

    // Status about hook
    bool isSharedMemoryHooked;

    // Logging
#ifdef SHAREDMEM_LOGGING
    FILE *logFilePtr;
#endif

    void LogError(const char *logPtr);

public:
    bool Hooked() { return isSharedMemoryHooked; }
    void *GetBuffer() { return pBufferPtr; }

    SharedMemory(
#ifdef _WIN32
        LPCWSTR newNamePtr,
#else
        const char *newNamePtr,
#endif
        unsigned int size);
    void Close();

    void *getPtrAt(int offset) { return (void *)&(((unsigned char *)pBufferPtr)[offset]); }
};

#endif
