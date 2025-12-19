#include "sharedmemory.hpp"

void SharedMemory::LogError(const char *logPtr)
{
#ifdef SHAREDMEM_LOGGING
    if (this->logFilePtr == NULL)
    {
        this->logFilePtr = fopen(SHAREDMEM_FILENAME, "a");
    }
    if (this->logFilePtr != NULL)
    {
        fprintf(this->logFilePtr, "%s\r\n", logPtr);
        fprintf(this->logFilePtr, "Windows Error code: %d\r\n\r\n", GetLastError());
    }
#endif
}

SharedMemory::SharedMemory(
#ifdef _WIN32
    LPCWSTR newNamePtr,
#else
    const char *newNamePtr,
#endif
    unsigned int size)
{
    this->mapsize = size;
    this->namePtr = newNamePtr;
    this->isSharedMemoryHooked = false;
#ifdef SHAREDMEM_LOGGING
    this->logFilePtr = NULL;
#endif

#ifdef _WIN32
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE, // use paging file
        nullptr,              // default security
        PAGE_READWRITE,       // read/write access
        0,                    // maximum object size (high-order DWORD)
        size,                 // maximum object size (low-order DWORD)
        this->namePtr);       // name of mapping object
    LogError("Created file map");
    if (hMapFile == nullptr)
    {
        LogError("but it's NULL!");
        if (GetLastError() == static_cast<DWORD>(183)) // already exists
        {
            hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, this->namePtr);
            if (hMapFile == nullptr)
            {
                LogError("Could not open existing file mapping");
                return;
            }
        }
        else
        {
            LogError("Could not create file mapping object");
            return;
        }
    }
    else
    {
        LogError("and it's not NULL!");
    }

    this->pBufferPtr = static_cast<void *>(MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size));
    if (this->pBufferPtr == nullptr)
    {
        LogError("Could not reserve buffer for shared memory");
        CloseHandle(hMapFile);
    }
#else
    hMapFile = shm_open(newNamePtr, O_CREAT | O_RDWR, 0600);
    LogError("Created file map");
    if (hMapFile == -1)
    {
        LogError(("shm_open failed: " + std::to_string(errno)).c_str());
        return;
    }

    if (ftruncate(hMapFile, mapsize) == -1)
    {
        LogError(("ftruncate failed: " + std::to_string(errno)).c_str());
        return;
    }

    this->pBufferPtr = mmap(nullptr, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, hMapFile, 0);
    close(hMapFile);

    if (this->pBufferPtr == MAP_FAILED)
    {
        LogError(("mmap failed: " + std::to_string(errno)).c_str());
        close(hMapFile);
        return;
    }
#endif
    else
    {
        memset(this->pBufferPtr, 0, size);
        this->isSharedMemoryHooked = true;
        LogError("Opened MMF");
    }
}

void SharedMemory::Close()
{
#ifdef SHAREDMEM_LOGGING
    if (logFilePtr != NULL)
    {
        fclose(logFilePtr);
        // TODO: Is this closed properly?
    }
#endif
    if (isSharedMemoryHooked)
    {
#ifdef _WIN32
        if (pBufferPtr != nullptr)
            UnmapViewOfFile(pBufferPtr);
        if (hMapFile != nullptr)
            CloseHandle(hMapFile);
#else
        if (pBufferPtr != nullptr)
            munmap(pBufferPtr, mapsize);
        if (hMapFile >= 0)
            shm_unlink(namePtr);
#endif
    }

    isSharedMemoryHooked = false;
}
