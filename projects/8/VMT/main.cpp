#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include "VMTranslator.hpp"
std::vector<std::string> getVMFilesList(std::string dirPath) {
    DIR* dir = opendir(dirPath.c_str());
    if (dir == nullptr) {
        std::cerr << "OpenDir failed." << std::endl;
        throw;
    }
    std::vector<std::string> fileList;
    dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            continue;
        } else {
            std::string fileName = entry->d_name;
            if (fileName.size() < 3 ||
                fileName.substr(fileName.size() - 3) != ".vm") {
                continue;
            }
            std::string tarDir = dirPath + "/" + fileName;
            fileList.push_back(tarDir);
        }
    }
    closedir(dir);
    return fileList;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "invalid number of arguments" << std::endl;
        std::cerr << "Usage: VMTranslator <filename>.vm" << std::endl;
        return 1;
    }
    std::string inpath(argv[1]);
    std::vector<std::string> infileList;
    std::string resultPath;
    bool need_init;

    int slash_index = inpath.rfind('/');
    if (slash_index == -1) {
        inpath = "./" + inpath;
        slash_index = 1;
    }

    int vm_index = inpath.rfind(".vm");
    if (vm_index == -1 ||
        vm_index + 3 < (int)inpath.length()) {  // Dir or other extension
        infileList = getVMFilesList(inpath);
        if (infileList.size() == 0) {
            std::cerr << "VMTranslator: invalid directory." << std::endl;
            std::cerr << "Usage: ./VMTranslator <fileName.vm>" << std::endl;
            std::cerr << "   or: ./VMTranslator <dirName>" << std::endl;
            return 1;
        }
        resultPath = inpath + inpath.substr(slash_index) + ".asm";
        need_init = true;
    } else {
        infileList.push_back(inpath);
        resultPath = inpath.substr(0, vm_index) + ".asm";
        need_init = false;
    }

    VMTranslator translator(infileList, resultPath, need_init);
    translator.convert();
    return 0;
}