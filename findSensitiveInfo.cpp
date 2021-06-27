#include<bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>

// colors
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */

using namespace std;

/* Method to get bash script's output into a string */
string getString(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}
/************************************************/

/* Checking for substrings */
bool checkSubStr(string str, int flag){
    if(flag == 0){
        string temp = str.substr(0, 6);
        if(temp == "Author")
            return true;
        return false;
    }
    else{
        string temp = str.substr(0, 11);
        if(temp == "commit hash")
            return true;
        return false;
    }
    return false;
}
/************************************************/

/* printing all the lines with the keyword found in commits */
void printInNewLines(string res){
    vector<string> result;
    vector<string> commitFind;
    vector<int> commitPos;

    auto ss = stringstream{res};

    int c=0;

    // Storing input line by line in result
    // Storing commit indices(indices in result vector) in commitPos
    for (string line; getline(ss, line, '\n');){
        result.push_back(line);
        if(checkSubStr(line, 0)){
            commitPos.push_back(c-4);
        }
        c++;
    }
    
    // Getting all info of commit having the keyword and storing in commitFind vector of strings
    for(auto i : commitPos){
        string tempStr;
        tempStr += result[i];
        for(int j=i+1; j<result.size(); j++){
            if(checkSubStr(result[j], 1))
                break;
            tempStr += result[j];
            tempStr += "\n";
        }
        if(tempStr.size()>1)
            commitFind.push_back(tempStr);
    }

    // If no such matche are found return 
    if(commitFind.size() == 0){
        cout << GREEN << "No commits found with provided keyWord." << RESET << endl;
        return;
    }

    // Print all found matches
    cout << GREEN << "Following commits match the given input: " << RESET << endl;
    for(auto j : commitFind)
        cout << j << endl;

}
/************************************************/

// checking if commitHashes.txt is empty or not
bool isCommitHashesEmpty(string directoryName){
    // cout << "in isCommitHashesEmpty with directory: " << directoryName << endl;
    string fileName = "commitHashes.txt";
    string file = directoryName + "/" + fileName;

    // cout << "file name: " << file << endl;
    const char *fileArg = &file[0];
    ifstream in(fileArg);

    if(!in || in.peek()==EOF)
        return true;
    return false;
}
/************************************************/

// checking if repo exists
bool repoExists(const char* const path){
    struct stat info;
    int statRC = stat(path, &info);
    if(statRC != 0){
        if(errno = ENOENT)
            return false;
        if(errno == ENOTDIR)
            return false;
        return false;
    }
    return (info.st_mode & S_IFDIR) ? true : false; 
}
/************************************************/

// cloning github repo 
void cloneRepo(string gitRepo, string directoryName){
    // cout << "[*] got to clone repo\n";
    string gitLink = "echo 9898 | sudo -S git clone " + gitRepo + " ./" + directoryName;
    char* gitLinkArg = &gitLink[0];
    system(gitLinkArg);
}
/************************************************/

// getting directory name Ex: BSardana01/testRepo.git
string getDirectoryName(string gitRepo){
    vector<string> tokens;
    istringstream ss(gitRepo);
    string token;

    // cout << "tokens: " << endl;
    while(getline(ss, token, '/')){
        // cout << token << " ";
        tokens.push_back(token);
    }
    // cout << endl;
    string directoryName = tokens[3] + "/" + tokens[4];

    // cout << "[*] Printing directory name: " << directoryName << endl;
    return directoryName;   
}
/************************************************/

// counting number of words in string
int numberOfWords(string str){
    istringstream ss(str);
    string token;
    int words=0;

    while(ss >> token)
        words++;
    return words;
}
/************************************************/

int main(int argc, char** argv){
    string gitRepo;
    cout << "Enter .git link for your github repository: " << endl;
    cin >> gitRepo;

    // cout << "got github link: " << gitRepo << endl;
    // getting directory name extracted from git link
    string directoryName = getDirectoryName(gitRepo);

    // cout << "[*] got directory name: " << directoryName << endl;
    char *directoryNameArg = &directoryName[0];
    if(repoExists(directoryNameArg)){
        // cout << "[*] repo exists!!!";
        // github repo is already cloned call ghdump/firstghDump on the directory
    }
    else{
        // github repo is not cloned, clone it and call ghdump/firstghDump on the directory
        // cout << "[*] repo does not exist!!!";
        cloneRepo(gitRepo, directoryName);
    }

    cout << "\nType your keyword and hit Enter (Press ctrl+D to exit the program)" << endl;
    string keyWord;
    while(getline(cin, keyWord)){
        string cmd;

        // check if commitHashes is empty
        if(isCommitHashesEmpty(directoryName)){
            // if true call firstghDump
            cmd = "echo 9898 | sudo -S ./firstghDump.sh " + keyWord + " " + directoryName + " " + "commitHashes.txt";
            if(numberOfWords(cmd) < 9){
                continue;
            }
            // cout << "cmd string for empty commithash: " << cmd << endl;
        }
        else{
            // call ghDump
            cmd = "echo 9898 | sudo -S ./ghdump.sh " + keyWord + " " + directoryName + " " + "commitHashes.txt";
            if(numberOfWords(cmd) < 9){
                continue;
            }
            // cout << "cmd string for commithash: " << cmd << endl;
        }

        char* arg = &cmd[0];
        string res = getString(arg);
        printInNewLines(res);
        cout << "\nType your keyword and hit Enter (Press ctrl+D to exit the program)" << endl;
    }
    cout << "Exiting..." << endl;
}