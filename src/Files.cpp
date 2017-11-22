#include "../include/Commands.h"
#include "../include/GlobalVariables.h"
#include <algorithm>
#include <iostream>
using namespace std;

BaseFile::BaseFile(string name): name(name) {}

string BaseFile::getName() const {
    return name;
}

void BaseFile::setName(string newName){
    name = newName;
}

BaseFile::~BaseFile() {
    if(verbose == 1 || verbose == 3){
        cout << "BaseFile::~BaseFile()" << endl;
    }
}

File::File(string name, int size): BaseFile(name), size(size) {}

int File::getSize(){
    return size;
}

bool File::isFile() { return true; }

File::~File() {
    if(verbose == 1 || verbose == 3){
        cout << "File::~File()" << endl;
    }
}

Directory::Directory(string name, Directory *parent): BaseFile(name), children(), parent(parent) {}

Directory *Directory::getParent() const{
    return parent;
}

void Directory::setParent(Directory *newParent){
    parent -> removeFile(this);
    parent = newParent;
    parent -> addFile(this);
}
// TODO: add this thing here
void Directory::addFile(BaseFile* file){
    if(!findChild(file->getName())) {
        children.push_back(file);
    }
}

//TODO: iterator fix like before
void Directory::removeFile(string name){
    vector<BaseFile*>::iterator it = children.begin();
    while(it != children.end()){
        if((*it) -> getName() == name){
            children.erase(it);
            it = children.end();
        } else {
            ++it;
        }
    }
}

void Directory::removeFile(BaseFile* file){
    vector<BaseFile*>::iterator it = children.begin();
    while(it != children.end()){
        if(*it == file){
            children.erase(it);
        } else {
            ++it;
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
    vector<BaseFile*>::iterator it = children.begin();
    while(it != children.end()){
        if(!(*it) ->isFile()){
            ((Directory *)(*it)) -> parent = nullptr;
        }
        delete (*it);
        it = children.erase(it);
    }
    // remove from parent
    if(this -> parent != nullptr){
        parent -> removeFile(this);
    }
}

void Directory::copy(const Directory &other) {
    parent = other.parent;
    // copy children
    for(vector<BaseFile*>::const_iterator it = other.children.begin(); it != other.children.end(); ++it){
        if((*it)->isFile()){
            children.push_back(new File(((File*)(*it)) -> getName(), ((File*)(*it)) -> getSize()));
        } else {
            children.push_back(new Directory(*((Directory *)(*it))));
            ((Directory *)children.back())->parent = this;
        }
    }


}

Directory::~Directory() {
    if(verbose == 1 || verbose == 3){
        cout << getName() << "Directory::~Directory()" << endl;
    }
    clear();
}

Directory::Directory(const Directory &other): BaseFile(other.getName()), children(),  parent(){
    copy(other);
    if(verbose == 1 || verbose == 3){
        cout << "Directory::Directory(const Directory &other): BaseFile(other.getName())" << endl;
    }
}

Directory& Directory::operator=(const Directory &other) {
    if(this != &other){
        clear();
        copy(other);
    }
    if(verbose == 1 || verbose == 3){
        cout << "Directory& Directory::operator=(const Directory &other)" << endl;
    }
    return *this;
}

Directory::Directory(Directory &&other):
        BaseFile(other.getName()), children(move(other.children)), parent(other.parent) {
    other.parent = nullptr;
    if(verbose == 1 || verbose == 3){
        cout << "Directory::Directory(Directory &&other)" << endl;
    }
}

Directory& Directory::operator=(Directory &&other) {
    if(this != &other){
        clear();
        setName(other.getName());
        parent = other.parent;
        children = move(move(other.children));
        other.parent = nullptr;
    }
    if(verbose == 1 || verbose == 3){
        cout << "Directory& Directory::operator=(Directory &&other)" << endl;
    }
    return *this;
}