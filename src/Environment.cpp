#include "../include/Environment.h"
#include "../include/GlobalVariables.h"

Environment::Environment(): commandsHistory(), fs() {}
void Environment::start() {
    string command;
    while(command != "exit"){
        if(!command.empty()){
            if(verbose == 2 || verbose == 3){
                cout << command << endl;
            }
            string cType = command.substr(0, command.find(' '));
            if(cType == "pwd") {
                PwdCommand *cmd = new PwdCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "cd") {
                CdCommand *cmd = new CdCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "ls") {
                LsCommand *cmd = new LsCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "mkdir"){
                MkdirCommand *cmd = new MkdirCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "mkfile"){
                MkfileCommand *cmd = new MkfileCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "cp"){
                CpCommand *cmd = new CpCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "mv"){
                MvCommand *cmd = new MvCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "rename"){
                RenameCommand *cmd = new RenameCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "rm"){
                RmCommand *cmd = new RmCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "history") {
                HistoryCommand *cmd = new HistoryCommand(command, getHistory());
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "verbose") {
                VerboseCommand *cmd = new VerboseCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            } else if(cType == "exec") { //TODO: infinite loop
                ExecCommand *cmd = new ExecCommand(command, getHistory());
                cmd->execute(fs);
                addToHistory(cmd);
            } else {
                ErrorCommand *cmd = new ErrorCommand(command);
                cmd -> execute(fs);
                addToHistory(cmd);
            }
        }
        cout << fs.getWorkingDirectory().getAbsolutePath() << ">";
        getline(cin, command);
    }
}

const FileSystem& Environment::getFileSystem() const {
    return fs;
}

void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

const vector<BaseCommand *> & Environment::getHistory() const {
    return commandsHistory;
}

void Environment::clear() {
    vector<BaseCommand*>::iterator it = commandsHistory.begin();
    for(vector<BaseCommand*>::iterator it = commandsHistory.begin(); it != commandsHistory.end();++it){
        delete (*it);
    }
}
void Environment::copy(const Environment &other) {
    for(vector<BaseCommand*>::const_iterator it = other.commandsHistory.begin();
        it != other.commandsHistory.end(); ++it){
        commandsHistory.push_back((*it));
    }
    fs = *new FileSystem(other.getFileSystem());
}

Environment::~Environment() { clear(); }

Environment::Environment(const Environment &other): commandsHistory(), fs() { copy(other); }

//TODO: ????
Environment::Environment(Environment &&other): commandsHistory(), fs() {
    commandsHistory = other.commandsHistory;
    fs = other.getFileSystem();
    other.commandsHistory.clear();
}

Environment& Environment::operator=(const Environment &other) {
    if(this != &other){
        clear();
        copy(other);
    }
    return *this;
}

Environment& Environment::operator=(Environment &&other) {
    if(this != &other){
        clear();
        commandsHistory = other.commandsHistory;
        fs = other.getFileSystem();
        other.commandsHistory.clear();
    }
    return *this;
}