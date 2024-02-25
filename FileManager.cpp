
#include "FileManager.h"

FileManager::FileManager(RemoteDebug* debug) {
  _debug = debug; 
}

bool FileManager::Mount() {
  _debug->println("Mount LittleFS");
  LittleFS.begin();
  return isMount;
}

void FileManager::UnMount() {
  
   if(!isMount) {
    _debug->println("File system is not mount, please mount before use...");
    return;
  }

  _debug->println("unmount filesystem");
  LittleFS.end();
  _debug->println("unmount filesystem completed");
}

void FileManager::Format() {

  if(!isMount) {
    _debug->println("File system is not mount, please mount before use...");
    return;
  }

  _debug->println("Formatting LittleFS filesystem");
  LittleFS.format();
  _debug->println("filesystem Formated");
}

void FileManager::ListDir(const char *dirname) {
    
  if(!isMount) {
    _debug->println("File system is not mount, please mount before use...");
    return;
  }

  _debug->printf("Listing directory: %s\n", dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) {
    File file = root.openFile("r");
    _debug->println("  FILE: ");
    _debug->println(root.fileName());
    _debug->println("  SIZE: ");
    _debug->println(file.size());
    time_t cr = file.getCreationTime();
    time_t lw = file.getLastWrite();
    file.close();
    struct tm *tmstruct = localtime(&cr);
    _debug->printf("    CREATION: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    tmstruct = localtime(&lw);
    _debug->printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
  }
}

void FileManager::PrintFile(const char *path) {
  
  if(!isMount) {
    _debug->println("File system is not mount, please mount before use...");
    return;
  }

  _debug->printf("Reading file: %s\n", path);

  File file = LittleFS.open(path, "r");
  if (!file) {
    _debug->println("Failed to open file for reading");
    return;
  }

  _debug->println("Read from file: ");
  while (file.available()) { _debug->print(file.read()); }
  file.close();
}

bool FileManager::OpenFileWrite(const char* path, File& file) {
  if(!isMount) {
    _debug->println("File system is not mount, please mount before use...");
    return false;
  }

  _debug->printf("Writing file: %s\n", path);
  file = LittleFS.open(path, "w");

  if (!file) {
    _debug->println("Failed to open file for writing");
    return false;
  }

  return true;
}

void FileManager::WriteFile(const char *path, const char *message) {
  
  File file;
  if(OpenFileWrite(path, file)) {
    if (file.print(message)) {
      _debug->println("File written");
    } else {
      _debug->println("Write failed");
    }  
  }

  delay(2000);  // Make sure the CREATE and LASTWRITE times are different
  file.close();
}

void FileManager::AppendFile(const char *path, const char *message) {
  
  if(!isMount) {
    _debug->println("File system is not mount, please mount before use...");
    return;
  }

  _debug->printf("Appending to file: %s\n", path);

  File file = LittleFS.open(path, "a");
  if (!file) {
    _debug->println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    _debug->println("Message appended");
  } else {
    _debug->println("Append failed");
  }
  file.close();
}

void FileManager::RenameFile(const char *path1, const char *path2) {
  
  if(!isMount) {
    _debug->println("File system is not mount, please mount before use...");
    return;
  }

  _debug->printf("Renaming file %s to %s\n", path1, path2);
  if (LittleFS.rename(path1, path2)) {
    _debug->println("File renamed");
  } else {
    _debug->println("Rename failed");
  }
}

bool FileManager::DeleteFile(const char *path) {
  
  if(!isMount) {
    _debug->println("File system is not mount, please mount before use...");
    return false;
  }

  _debug->printf("Deleting file: %s\n", path);
  
  if (LittleFS.remove(path)) {
    _debug->println("File deleted");
    return true;
  } else {
    _debug->println("Delete failed");
    return false;
  }

}

bool FileManager::ReadFile(const char *path, File& file) {

  if(!isMount) {
    _debug->println("File system is not mount, please mount before use...");
    return false;
  }

  _debug->printf("Reading file: %s\n", path);
  file = LittleFS.open(path, "r");
  
  if (!file) {
    _debug->println("Failed to open file for reading");
    return false;
  }

  return true;
}

bool FileManager::ReadJson(const char* configPath, JsonDocument& doc) {

  //open file
  File jsonFile = LittleFS.open(configPath, "r");
  
  //Guard can't open file
  if(!jsonFile) {
    _debug->printf("cannot open the file: %s", configPath);
    jsonFile.close(); // close the file
    return false;
  }

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, jsonFile);

 // Guard error during deserialize to json
 if (error) {
   _debug->println("Error during json deserialize");
   jsonFile.close(); // close the file
   return false;
 }
 
 jsonFile.close(); // close the file
 return true;
}

bool FileManager::WriteJson(const char* configPath, JsonDocument& doc) {
  
  // Delete existing file, otherwise the configuration is appended to the file
  if(LittleFS.exists(configPath) && !DeleteFile(configPath)){
    return false;
  }

  File file;

  // Serialize JSON to file
  if(!OpenFileWrite(configPath, file) && serializeJson(doc, file) == 0)
  {
    _debug->println("Failed to write to file");
    file.close();
    return false;
  }
  
  delay(2000);  // Make sure the CREATE and LASTWRITE times are different
  file.close(); // Close the file
  return true;
}