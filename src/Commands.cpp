#include "Commands.h"

#include <sstream>
#include <iostream>

using namespace std;

BaseCommand::BaseCommand(string args): args(args){}
string BaseCommand::getArgs() {
    return args;
}

// prints the path to the current directory
PwdCommand::PwdCommand(string args): BaseCommand(args) {}

void PwdCommand::execute(FileSystem &fs) {
    cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
}

string PwdCommand::toString() {
    return "pwd";
}

// the function finds the directory specified and returns false if it dosn't exist
Directory *findDirectory(FileSystem &fs, string path){
    Directory *targetDir;
    // deter if should use relative or absolute path
    if(path.length() == 0 || path.at(0) == '/') {
        targetDir = &fs.getRootDirectory();
    } else {
        targetDir = &fs.getWorkingDirectory();
    }
    // insert a queue of dirs into the vector, using a string stream
    istringstream ss(path);
    vector <string> dirs;
    string dir;
    while (std::getline(ss, dir, '/')) {
        dirs.insert(dirs.begin(), dir);
    }
    // pop the dirs from the vector and change the path
    bool flag = true;
    while (!dirs.empty() && flag) {
        string currentName = dirs.back();
        dirs.pop_back();
        if(currentName != "") {
            if (currentName == "..") {
                if (fs.getWorkingDirectory().getParent() != nullptr) {
                    targetDir = fs.getWorkingDirectory().getParent();
                } else {
                    flag = false;
                }
            } else {
                Directory *foundChild = targetDir->findChild(currentName);
                if (foundChild) {
                    targetDir = foundChild;
                } else {
                    flag = false;
                }
            }
        }
    }
    if (flag) {
        return targetDir;
    } else {
        return nullptr;
    }
}

//changes the current working directory
CdCommand::CdCommand(string args): BaseCommand(args) {}

void CdCommand::execute(FileSystem &fs) {
    string path = getArgs().substr(2);
    if(path.length() != 0 && path.at(0) == ' '){
        path = path.substr(1);
    }
    Directory *targetDir = findDirectory(fs, path);
    if(targetDir){
        fs.setWorkingDirectory(targetDir);
    } else {
        cout << "The system cannot find the path specified" << endl;
    }
}

string CdCommand::toString() {
    return "cd";
}

LsCommand::LsCommand(string args): BaseCommand(args){};

void LsCommand::execute(FileSystem &fs) {
    string command = getArgs().substr(2);
    if(command.length() != 0 && command.at(0) == ' '){
        command = command.substr(1);
    }
    // insert a queue of dirs into the vector, using a string stream
    istringstream ss(command);
    vector <string> sections;
    string sec;
    while (std::getline(ss, sec, ' ')) {
        sections.insert(sections.begin(), sec);
    }
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
    Directory *targetDir;
    if(path == ""){
        targetDir = &fs.getWorkingDirectory();
    } else {
        targetDir = findDirectory(fs, path);
    }
    if(targetDir) {
        if(sort){
            targetDir->sortBySizeName();
        }
        vector<BaseFile *> children = targetDir -> getChildren();
        for (vector<BaseFile *>::iterator it = children.begin(); it != children.end(); ++it) {
            if ((*it)->isFile()) {
                cout << "FILE" << "\t";
            } else {
                //TODO: check
                cout << "DIR " << "\t";
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