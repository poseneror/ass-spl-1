#include "Commands.h"
#include "FileSystem.h"
#include "Files.h"

#include <sstream>
#include <iostream>
#include <vector>

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

//changes the current working directory
CdCommand::CdCommand(string args): BaseCommand(args) {}

void CdCommand::execute(FileSystem &fs) {
    string path = getArgs().substr(3);
    if(path == ".."){
        if(fs.getWorkingDirectory().getParent() != nullptr) {
            fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
        } else {
            cout << "The system cannot find the path specified" << endl;
        }
    } else {
        Directory *targetDir;
        // deter if should use relative or absolute path
        if(path.length() != 0 && path.at(0) == '/') {
            targetDir  = &fs.getRootDirectory();
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
            Directory *foundChild = targetDir->findChild(currentName);
            if (foundChild) {
                targetDir = foundChild;
            } else {
                flag = false;
            }
        }
        if (flag) {
            fs.setWorkingDirectory(targetDir);
        } else {
            cout << "The system cannot find the path specified" << endl;
        }
    }
}