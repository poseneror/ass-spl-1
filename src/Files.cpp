#include "Commands.h"
#include <algorithm>

BaseFile::BaseFile(string name): name(name) {}

string BaseFile::getName() const {
    return name;
}

void BaseFile::setName(string newName){
    name = newName;
}

File::File(string name, int size): BaseFile(name), size(size) {}

int File::getSize(){
    return size;
}

bool File::isFile() { return true; }

Directory::Directory(string name, Directory *parent): BaseFile(name), parent(parent), children(){}

Directory *Directory::getParent() const{
    return parent;
}

void Directory::setParent(Directory *newParent){
    parent = newParent;
}

void Directory::addFile(BaseFile* file){
    children.push_back(file);
}

void Directory::removeFile(string name){
    for(vector<BaseFile*>::iterator it = children.begin(); it != children.end(); ++it){
        if((*it) -> getName() == name){
            children.erase(it);
        }
    }
}

void Directory::removeFile(BaseFile* file){
    for(vector<BaseFile*>::iterator it = children.begin(); it != children.end(); ++it){
        if(*it == file){
            children.erase(it);
        }
    }
}

//TODO: sort be size also
bool nameComp(BaseFile *f1, BaseFile *f2){
    return (f1 -> getName() < f2 -> getName());
}

bool sizeComp(BaseFile *f1, BaseFile *f2){
    return (f1 -> getSize() < f2 -> getSize());
}

bool sizeNameComp(BaseFile *f1, BaseFile *f2){
    if(f1 -> getSize() == f2 ->getSize()){
        return nameComp(f1, f2);
    } else {
        return sizeComp(f1, f2);
    }
}

void Directory::sortByName(){
    std::sort(children.begin(), children.end(), nameComp);
}

void Directory::sortBySize(){
    std::sort(children.begin(), children.end(), sizeComp);
}

void Directory::sortBySizeName() {
    std::sort(children.begin(), children.end(), sizeNameComp);
}

vector<BaseFile*> Directory::getChildren(){
    return children;
}

int Directory::getSize(){
    int sum = 0;
    for(vector<BaseFile*>::iterator it = children.begin(); it != children.end(); ++it){
        sum += (*it) -> getSize();
    }
    return sum;
}

string Directory::getAbsolutePath(){
    if( getParent() == nullptr ){
        return "/";
    }
    if(getParent()->getParent() == nullptr){
        return (getParent()->getAbsolutePath()).append(getName());
    }
    return (getParent()->getAbsolutePath()).append("/").append(getName());
}

bool Directory::isFile(){
    return false;
}

// this method looks for a child with the specified name,
// returns a pointer to the child or a null pointer if it is not found
Directory* Directory::findChild(string childName) {
    for(vector<BaseFile*>::iterator it = children.begin(); it != children.end(); ++it){
        if(((*it) -> getName()) == childName && !((*it) -> isFile()))
            return (Directory*) (*it);
    }
    return nullptr;
}