#ifndef FileManager_h
#define FileManager_h

#include <FS.h>
#include <LittleFS.h>
#include <RemoteDebug.h>

class FileManager {
  private:
    RemoteDebug* _debug;
    bool isMount;

  public:
    FileManager(RemoteDebug* debug);
    void UnMount();
    bool Mount();
    void Format();
    void ListDir(const char *dirname);
    void ReadFile(const char *path);
    void WriteFile(const char *path, const char *message);
    void AppendFile(const char *path, const char *message);
    void RenameFile(const char *path1, const char *path2);
    void DeleteFile(const char *path);

};
#endif