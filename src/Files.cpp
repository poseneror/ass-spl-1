#include "Commands.h"
#include <algorithm>

BaseFile::BaseFile(string name): name(name) {}

string BaseFile::getName() const {
    return name;
}

void BaseFile::setName(string newName){
    name = newName;
}

BaseFile::~BaseFile() {};

File::File(string name, int size): BaseFile(name), size(size) {}

int File::getSize(){
    return size;
}

bool File::isFile() { return true; }

File::~File() {}

Directory::Directory(string name, Directory *parent): BaseFile(name), parent(parent), children(){}

Directory *Directory::getParent() const{
    return parent;
}

void Directory::setParent(Directory *newParent){
    parent -> removeFile(this);
    parent = newParent;
    parent -> addFile(this);
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

//TODO: why like this
void Directory::removeFile(BaseFile* file){
    for(vector<BaseFile*>::iterator it = children.begin(); *it != nullptr && it != children.end(); ++it){
        if(*it == file){
            children.erase(it);
        }
    }
}

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
BaseFile* Directory::findChild(string childName) {
    for(vector<BaseFile*>::iterator it = children.begin(); it != children.end(); ++it){
        if(((*it) -> getName()) == childName)
            return *it;
    }
    return nullptr;
}

void Directory::clear(){
    // clear children
    for(vector<BaseFile*>::iterator it = children.begin(); it != children.end(); ++it){
        delete (*it);
    }
    // remove from parent
    if(this->parent != nullptr){
        parent->removeFile(this);
    }
}

void Directory::copy(const Directory &other) {
    setName(other.getName());
    parent = other.parent;
    // copy children
    for(vector<BaseFile*>::const_iterator it = other.children.begin(); it != other.children.end(); ++it){
        if((*it)->isFile()){
            children.push_back(new File(((File*)(*it)) -> getName(), ((File*)(*it)) -> getSize()));
        } else {
            children.push_back(new Directory(*((Directory *)(*it))));
            ((Directory *)children.back()) -> setParent(this);
        }
    }


}

Directory::~Directory() {
    clear();
}

Directory::Directory(const Directory &other): BaseFile(other.getName()){ copy(other); }

Directory& Directory::operator=(const Directory &other) {
    if(this != &other){
        clear();
        copy(other);
    }
}

Directory::Directory(Directory &&other):
        BaseFile(other.getName()), parent(other.parent), children(move(other.children)) {
    other.parent = nullptr;
}

Directory& Directory::operator=(Directory &&other) {
    if(this != &other){
        clear();
        setName(other.getName());
        parent = other.parent;
        children = move(move(other.children));
        other.parent = nullptr;
    }
}