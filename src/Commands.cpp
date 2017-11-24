#include "../include/Commands.h"
#include "../include/GlobalVariables.h"
#include <sstream>
#include <iostream>

using namespace std;

BaseCommand::BaseCommand(string args): args(args){}
string BaseCommand::getArgs() {
    return args;
}

BaseCommand::~BaseCommand() {}

vector <string> Split(string src, char c){
    istringstream ss(src);
    vector <string> sections;
    string sec;
    while (std::getline(ss, sec, c)) {
        sections.insert(sections.begin(), sec);
    }
    return sections;
}

string TrimArgs(string args, int length){
    string trimmed = args.substr(length);
    if(trimmed.length() != 0 && trimmed.at(0) == ' '){
        trimmed = trimmed.substr(1);
    }
    return trimmed;
}

// prints the path to the current directory
PwdCommand::PwdCommand(string args): BaseCommand(args) {}

void PwdCommand::execute(FileSystem &fs) {
    cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
}

string PwdCommand::toString() {
    return "pwd";
}

// the function finds the directory or file specified and returns false if it dosn't exist
BaseFile *findFile(FileSystem &fs, string path){
    BaseFile *targetFile;
    // deter if should use relative or absolute path
    if(path.length() == 0 || path.at(0) == '/') {
        targetFile = &fs.getRootDirectory();
    } else {
        targetFile = &fs.getWorkingDirectory();
    }
    // insert a queue of files (dirs / files) into the vector, using a string stream
    vector<string> dirs = Split(path, '/');
    // pop the dirs from the vector and change the path
    bool flag = true;
    while (!dirs.empty() && flag) {
        string currentName = dirs.back();
        dirs.pop_back();
        if(currentName != ""){
            if(currentName == ".."){
                if(((Directory *) targetFile) -> getParent() != nullptr) {
                    targetFile = ((Directory *) targetFile) -> getParent();
                } else {
                    flag = false;
                }
            } else {
                BaseFile *foundChild = ((Directory *) targetFile)->findChild(currentName);
                if (dirs.empty()) {
                    if (foundChild) { return foundChild; }
                    else { flag = false; }
                } else {
                    if (foundChild) {
                        if (foundChild->isFile()) {
                            flag = false;
                        } else {
                            targetFile = foundChild;
                        }
                    } else {
                        flag = false;
                    }
                }
            }
        }
    }
    if (flag) {
        return targetFile;
    } else {
        return nullptr;
    }
}

//changes the current working directory
CdCommand::CdCommand(string args): BaseCommand(args) {}

void CdCommand::execute(FileSystem &fs) {
    string path = TrimArgs(getArgs(), 2);
    BaseFile *targetFile = findFile(fs, path);
    if(targetFile && !targetFile ->isFile()){
        fs.setWorkingDirectory((Directory *) targetFile);
    } else {
        cout << "The system cannot find the path specified" << endl;
    }
}

string CdCommand::toString() {
    return "cd";
}

LsCommand::LsCommand(string args): BaseCommand(args){};

void LsCommand::execute(FileSystem &fs) {
    string command = TrimArgs(getArgs(), 2);
    // insert a queue of dirs into the vector, using a string stream
    vector <string> sections = Split(command, ' ');
    bool sort = false;
    string path = "";
    if(!sections.empty()) {
        if (sections.back() == "-s") {
            sort = true;
            sections.pop_back();
        }
        if(!sections.empty()){
            path = sections.back();
        }
    }
    BaseFile *targetDir;
    if(path == ""){
        targetDir = &fs.getWorkingDirectory();
    } else {
        targetDir = findFile(fs, path);
    }
    if(targetDir && !targetDir->isFile()) {
        if(sort){
            ((Directory *)targetDir)->sortBySizeName();
        } else {
            ((Directory *)targetDir)->sortByName();
        }
        vector<BaseFile *> children = ((Directory*)targetDir) -> getChildren();
        for (vector<BaseFile *>::iterator it = children.begin(); it != children.end(); ++it) {
            if ((*it)->isFile()) {
                cout << "FILE" << "\t";
            } else {
                //TODO: check
                cout << "DIR" << "\t";
            }
            cout << (*it)->getName() << "\t" << (*it)->getSize() << endl;
        }
    } else {
        cout << "The system cannot find the path specified" << endl;
    }
}
string LsCommand::toString() {
    return "ls";
}

MkdirCommand::MkdirCommand(string args): BaseCommand(args){}

//TODO: check empty name dir creation error
void MkdirCommand::execute(FileSystem &fs) {
    string path = TrimArgs(getArgs(), 5);
    Directory* targetDir;
    if(path.length() != 0 && path.at(0) == '/') {
        targetDir = &fs.getRootDirectory();
    } else {
        targetDir = &fs.getWorkingDirectory();
    }
    vector <string> dirs = Split(path, '/');
    bool flag = true;
    while (!dirs.empty() && flag) {
        string currentName = dirs.back();
        dirs.pop_back();
        if(currentName != "") {
            if (currentName == "..") {
                if (targetDir -> getParent() != nullptr) {
                    targetDir = targetDir -> getParent();
                }
            } else {
                BaseFile *foundChild = targetDir -> findChild(currentName);
                if (foundChild){
                    if(!(foundChild->isFile())) {
                        if (dirs.empty()) {
                            flag = false;
                        } else {
                            targetDir = (Directory *) foundChild;
                        }
                    } else {
                        flag = false;
                    }
                } else {
                    targetDir -> addFile(new Directory(currentName, targetDir));
                    targetDir = (Directory *) targetDir -> findChild(currentName);
                }
            }
        }
    }
    if(!flag){
        cout << "The directory already exists" << endl;
    }
}
string MkdirCommand::toString() {
    return "mkdir";
}

MkfileCommand::MkfileCommand(string args): BaseCommand(args) {}
void MkfileCommand::execute(FileSystem &fs) {
    string command = TrimArgs(getArgs(), 6);
    vector <string> sections = Split(command, ' ');
    string path = "";
    int size = 0;
    if(!sections.empty()){
        path = sections.back();
        sections.pop_back();
    }
    if(!sections.empty()){
        size = stoi(sections.back());
    }
    Directory* targetDir;
    if(path.length() != 0 && path.at(0) == '/') {
        targetDir = &fs.getRootDirectory();
    } else {
        targetDir = &fs.getWorkingDirectory();
    }
    vector <string> files = Split(path, '/');
    bool flag = true;
    while (!files.empty() && flag) {
        string currentName = files.back();
        files.pop_back();
        if(currentName != "") {
            if (currentName == "..") {
                if (targetDir -> getParent() != nullptr) {
                    targetDir = targetDir -> getParent();
                }
            } else {
                BaseFile *foundChild = targetDir -> findChild(currentName);
                if (foundChild) {
                    if(files.empty()){
                        cout << "File already exists" << endl;
                    } else {
                        if(!foundChild->isFile()) {
                            targetDir = (Directory *) foundChild;
                        } else {
                            cout << "The system cannot find the path specified" << endl;
                            flag = false;
                        }
                    }
                } else {
                    if(files.empty()){
                        targetDir -> addFile(new File(currentName, size));
                    } else {
                        cout << "The system cannot find the path specified" << endl;
                        flag = false;
                    }
                }
            }
        }
    }
}
string MkfileCommand::toString() {
    return "mkfile";
}

CpCommand::CpCommand(string args): BaseCommand(args) {}

// check if dir is present in the working directory predessesors
bool isPre(FileSystem &fs, Directory * dir, Directory * current){
    while(current != nullptr){
        if(current == dir){
            return true;
        } else {
            current = current -> getParent();
        }
    }
    return false;
}
void CpCommand::execute(FileSystem &fs) {
    string command = TrimArgs(getArgs(), 2);
    vector <string> sections = Split(command, ' ');
    string src = "";
    string des = "";
    if(!sections.empty()){
        src = sections.back();
        sections.pop_back();
    }
    if(!sections.empty()){
        des = sections.back();
    }
    BaseFile* srcFile = findFile(fs, src);
    Directory* desDir = dynamic_cast<Directory*>(findFile(fs, des));
    if(desDir && srcFile ){
        //TODO: REMOVE MESSAGE
        if(!desDir->findChild(srcFile->getName())) {
            if (srcFile->isFile()) {
                desDir -> addFile(new File(*((File *) srcFile)));
            } else {
                if (isPre(fs, (Directory *) srcFile, desDir)) {
                    cout << "Can't copy directory" << endl;
                } else {
                    Directory *copy = new Directory(*(Directory *) srcFile);
                    copy->setParent(desDir);
                    desDir->addFile(copy);
                }
            }
        }
    } else {
        cout << "No such file or directory" << endl;
    }
}
string CpCommand::toString() {
    return "cp";
}

MvCommand::MvCommand(string args): BaseCommand(args) {}
void MvCommand::execute(FileSystem &fs) {
    string command = TrimArgs(getArgs(), 2);
    vector <string> sections = Split(command, ' ');
    string src = "";
    string des = "";
    if(!sections.empty()){
        src = sections.back();
        sections.pop_back();
    }
    if(!sections.empty()){
        des = sections.back();
    }
    BaseFile* srcFile = findFile(fs, src);
    Directory* desDir = dynamic_cast<Directory*>(findFile(fs, des));
    if(desDir && srcFile){
        //TODO: REMOVE MESSAGE
        if(!desDir->findChild(srcFile->getName())){
            if (srcFile->isFile()) {
                Directory *parent;
                size_t s = src.find_last_of('/');
                if (s == string::npos) {
                    parent = &fs.getRootDirectory();
                } else {
                    parent = (Directory *) findFile(fs, src.substr(0, s));
                }
                // HERE
                parent -> removeFile((File *) srcFile);
                desDir -> addFile(srcFile);
            } else {
                if (isPre(fs, (Directory *) srcFile, desDir) || isPre(fs, (Directory *) srcFile, &fs.getWorkingDirectory())) {
                    cout << "Can't move directory" << endl;
                } else {
                    ((Directory *) srcFile) -> getParent() -> removeFile(srcFile);
                    desDir -> addFile(srcFile);
                    ((Directory *) srcFile) -> setParent(desDir);
                }
            }
        } else {
            cout << "Can't move directory" << endl;
        }
    } else {
        cout << "No such file or directory" << endl;
    }
}
string MvCommand::toString() {
    return "mv";
}

RenameCommand::RenameCommand(string args): BaseCommand(args) {}

void RenameCommand::execute(FileSystem &fs) {
    string command = TrimArgs(getArgs(), 6);
    vector <string> sections = Split(command, ' ');
    string target = "";
    string newName = "";
    if(!sections.empty()){
        target = sections.back();
        sections.pop_back();
    }
    if(!sections.empty()){
        newName = sections.back();
    }
    BaseFile* targetFile = findFile(fs, target);
    if(targetFile){
        if(!targetFile->isFile() && (Directory *) targetFile == &fs.getWorkingDirectory()){
            cout << "Can't rename the working directory" << endl;
        } else {
            //TODO: file name already exist check - print nothing but don't change the name
            Directory *parent;
            if (targetFile->isFile()) {
                size_t s = target.find_last_of('/');
                if (s == string::npos) {
                    parent = &fs.getRootDirectory();
                } else {
                    parent = (Directory *) findFile(fs, target.substr(0, s));
                }
            } else {
                parent = ((Directory *) targetFile) -> getParent();
            }
            if(!parent->findChild(newName)) {
                targetFile->setName(newName);
            }
        }
    } else {
        cout << "No such file or directory" << endl;
    }
}

string RenameCommand::toString() {
    return "rename";
}

RmCommand::RmCommand(string args): BaseCommand(args) {};

void RmCommand::execute(FileSystem &fs) {
    string target = TrimArgs(getArgs(), 2);
    BaseFile *targetFile = findFile(fs, target);
    if(targetFile){
        if(!targetFile->isFile() && isPre(fs, (Directory *)targetFile, &fs.getWorkingDirectory())){
            cout << "Can't remove directory" << endl;
        } else {
            if (targetFile->isFile()) {
                Directory *parent;
                size_t s = target.find_last_of('/');
                if (s == string::npos) {
                    parent = &fs.getRootDirectory();
                } else {
                    parent = (Directory *) findFile(fs, target.substr(0, s));
                }
                parent->removeFile((File *) targetFile);
            }
            delete targetFile;
        }
    } else {
        cout << "No such file or directory" << endl;
    }
}

string RmCommand::toString() {
    return "rm";
}

HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> &history): BaseCommand(args), history(history) {}

void HistoryCommand::execute(FileSystem &fs) {
    int i = 0;
    for(vector<BaseCommand*>::const_iterator it = history.begin(); it != history.end(); ++it, i++){
        cout << i << "\t" << (*it) -> getArgs() << endl;
    }
}

string HistoryCommand::toString() {
    return "history";
}

VerboseCommand::VerboseCommand(string args): BaseCommand(args) {}

void VerboseCommand::execute(FileSystem &fs) {
    string val = TrimArgs(getArgs(), 7);
    int v = stoi(val);
    if(v <= 3 && v >= 0){
        verbose = v;
    } else {
        cout << "Wrong verbose input" << endl;
    }
}

string VerboseCommand::toString() {
    return "vebose";
}

ExecCommand::ExecCommand(string args, const vector<BaseCommand *> &history): BaseCommand(args), history(history) {}

void ExecCommand::execute(FileSystem &fs) {
    string val = TrimArgs(getArgs(), 4);
    size_t v = stoi(val);
    if(v >= 0 && v < history.size()){
        history.at(v)->execute(fs);
    } else {
        cout << "Command not found" << endl;
    }
}

string ExecCommand::toString() {
    return "exec";
}

ErrorCommand::ErrorCommand(string args): BaseCommand(args) {}
void ErrorCommand::execute(FileSystem &fs) {
    string cType = getArgs().substr(0, getArgs().find(" "));
    cout << cType << ": Unknown command" << endl;
}

string ErrorCommand::toString() {
    return "error";
}