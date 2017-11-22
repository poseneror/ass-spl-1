#include "../include/FileSystem.h"

FileSystem::FileSystem(): rootDirectory(new Directory("", nullptr)), workingDirectory(rootDirectory) {}

Directory& FileSystem::getRootDirectory() const {
    return *rootDirectory;
}

Directory& FileSystem::getWorkingDirectory() const {
    return *workingDirectory;
}
void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    workingDirectory = newWorkingDirectory;
}

void FileSystem::clear() {
    delete rootDirectory;
    rootDirectory = nullptr;
    workingDirectory = nullptr;
}
void FileSystem::copy(const FileSystem &other) {
    rootDirectory = new Directory(*other.rootDirectory);
    Directory *current = &other.getWorkingDirectory();
    vector<string> prev;
    while(current->getParent() != nullptr){
        prev.push_back(current->getName());
        current = current -> getParent();
    }
    workingDirectory = rootDirectory;
    while(!prev.empty()){
        workingDirectory = (Directory *)workingDirectory->findChild(prev.back());
        prev.pop_back();
    }
}

FileSystem::~FileSystem() { clear(); }

FileSystem::FileSystem(const FileSystem &other): rootDirectory(), workingDirectory() { copy(other); }

FileSystem::FileSystem(FileSystem &&other):
        rootDirectory(other.rootDirectory), workingDirectory(other.workingDirectory) {
    other.rootDirectory = nullptr;
    other.workingDirectory = nullptr;
}
FileSystem& FileSystem::operator=(const FileSystem &other) {
    if(this != &other) {
        clear();
        copy(other);
    }
    return *this;

}
FileSystem& FileSystem::operator=(FileSystem &&other) {
    if(this != &other){
        clear();
        rootDirectory = other.rootDirectory;
        workingDirectory = other.workingDirectory;
    }
    return *this;
}