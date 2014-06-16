#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include "sha256.h"

struct File {
    std::string path;
    std::string hash;
    std::string size;
};

std::vector<File> getDirectory(std::string dir)
{
    using namespace boost::filesystem;

    std::vector<File> files;
    path p(dir);

    if(exists(p)) {
        if(is_directory(p)) {
            directory_iterator iter(p), end;
            while(iter != end) {
                if(is_directory(iter->path())) {
                    std::vector<File> folder = getDirectory(iter->path().string());
                    files.insert(files.end(), folder.begin(), folder.end());
                } else {
                    File file;
                    file.hash = "";
                    file.path = iter->path().string();
                    file.size = file_size(iter->path());
                    files.push_back(file);
                }
                ++iter;
            }
        } else {
            std::cout << "Not a directory!" << std::endl;
        }
    } else {
        std::cout << "Doesn't exist!" << std::endl;
    }
    return files;
}

std::string getFileHash(std::string path) {
    std::ifstream file(path);
    if(file.good()) {
        SHA256 sha;
        std::string line;
        while(getline(file, line)) {
            sha.add(line.c_str(), line.size());
        }
        file.close();
        return sha.getHash();
    } else {
        file.close();
        return "";
    }
}

std::vector<std::string> getMatches(std::vector<File> files) {
    std::vector<std::string> matches;
    for(File aFile : files) {
        if(std::find(matches.begin(), matches.end(), aFile.path) == matches.end()) {
            for(File bFile : files) {
                if(aFile.path != bFile.path) {
                    if(aFile.hash == bFile.hash) {
                        matches.push_back(bFile.path);
                    }
                }
            }
        }
    }
    return matches;
}

void writeLog(std::string path, std::vector<File> files, std::vector<std::string> matches) {
    std::ofstream file(path);

    file << "Duplicate file finder log" << std::endl
         << "Written by Jacob Hegna" << std::endl
         << std::endl
         << files.size() << " files processed, " << matches.size() << " files deleted as duplicates ("
            << ((float)matches.size())/((float)files.size()) << "%)" << std::endl
         << "Below are the deleted files" << std::endl
         << "---------------------------" << std::endl;

    for(std::string match : matches) {
        file << match << std::endl;
    }

    file.close();
}

int main(int argc, char *argv[]) {
    if(argc == 3) {
        std::cout << "Scanning folder..." << std::endl;
        SHA256 sha;
        std::vector<File> files = getDirectory(argv[1]);
        for(int i = 0; i < files.size(); ++i) {
            std::ifstream ifile(files.at(i).path);
            std::string line;
            while(getline(ifile, line)) {
                sha.add(line.c_str(), line.size());
            }
            files.at(i).hash = sha.getHash();
            ifile.close();
            sha.reset();
        }

        std::cout << "Finding matches..." << std::endl;
        std::vector<std::string> matches = getMatches(files);

        std::cout << "Deleting matches..." << std::endl;
        for(std::string match : matches) {
            boost::filesystem::remove(boost::filesystem::path(match));
        }

        std::cout << "Writing log..." << std::endl;
        writeLog(argv[2], files, matches);

        matches.clear();
        files.clear();
    } else {
        std::cout << "Usage: " << argv[0] << " [directory] [log name]" << std::endl;
    }
}