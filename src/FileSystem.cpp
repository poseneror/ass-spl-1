#include "FileSystem.h"
#include "Commands.h"
#include "Files.h"

FileSystem::FileSystem(): rootDirectory(new Directory("", nullptr)), workingDirectory(rootDirectory) {}

Directory& FileSystem::getRootDirectory() const {
    return *rootDirectory;
}

void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    workingDirectory = newWorkingDirectory;
}