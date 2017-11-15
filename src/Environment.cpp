#include "Environment.h"
#include "Commands.h"
#include "FileSystem.h"
#include "Files.h"

Environment::Environment(): commandsHistory(), fs() {}
void Environment::start() {
    string command = "";
    while(command != "exit"){
        if(command != ""){
            string cType = command.substr(0, command.find(" "));
            if(cType == "pwd") {
                PwdCommand(command).execute(fs);
            } else if(cType == "cd") {
                CdCommand(command).execute(fs);
            }
        }
        cout << fs.getWorkingDirectory().getAbsolutePath() << ">";
        cin >> command;
    }
}

FileSystem& Environment::getFileSystem() {
    return fs;
}

void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

const vector<BaseCommand *> & Environment::getHistory() const {
    return commandsHistory;
}