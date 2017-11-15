#include "FileSystem.h"

FileSystem::FileSystem(): rootDirectory(new Directory("", nullptr)), workingDirectory(rootDirectory) {}

Directory& FileSystem::getRootDirectory() const {
    return *rootDirectory;
}

// TODO: ADD THIS
Directory& FileSystem::getWorkingDirectory() const {
    return *workingDirectory;
}
void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    workingDirectory = newWorkingDirectory;
}