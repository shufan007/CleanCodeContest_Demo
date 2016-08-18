
#include <iostream>
#include <string.h>
#include <fstream>
#include "common.h"


int FileNameFilter(const struct dirent *pDir)
{
    if (strcmp(pDir->d_name, ".") 
        && strcmp(pDir->d_name, ".."))
    {
        return 1;
    }
    return 0;
}

std::vector<std::string> TraverseDir(const char *path) 
{
    std::vector<std::string> fileList;
    struct dirent **namelist;
    int n = scandir(path, &namelist, FileNameFilter, alphasort);

    if (n < 0) {
        std::ifstream infile(path);
        if (!infile){
            perror ("** Error opening file!");
            return fileList;
        }              
        fileList.push_back(path);
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            std::string _path(path);  
            std::string _dirName(namelist[i]->d_name);  
            std::string fullPath = _path + "/" + _dirName;
            fileList.push_back(fullPath);
            delete namelist[i];
        }
        delete namelist;
    } 
    std::cout<<"\n * file list:"<<std::endl;
    VS::const_iterator it;
    for(it = fileList.begin(); it != fileList.end(); it++)
    {
        std::cout<<"    "<<*it<<std::endl;       
    } 
    return fileList;  
}


VS StrSplit(const std::string &str, const std::string &pattern, const int n = 1024)
{
    VS subStrList;
    if(pattern.empty()) {
    	return subStrList;
    }
    int splitTimes = 0;
    size_t pos, start_pos = 0;
    pos = str.find_first_of(pattern, start_pos);
    while((pos != str.npos) && (splitTimes < n))
	{   	
        if(start_pos != pos){
            splitTimes ++;
            subStrList.push_back(str.substr(start_pos, pos - start_pos));
        }       
        start_pos = pos + 1;
        pos = str.find_first_of(pattern, start_pos);   
    }
    if(!str.substr(start_pos).empty()){
        subStrList.push_back(str.substr(start_pos));
    }
    return subStrList;
}


std::string JosnFormatCombine(D_PAIR_SS &pairItem)
{
    std::string outStr = "{";
    D_PAIR_SS::const_iterator it;
    for(it = pairItem.begin(); it != pairItem.end()-1; it ++)
    {
        outStr += (it->first + ":\"" + it->second + "\", "); 
    } 
    outStr += (it->first + ":\"" + it->second + "\"");
    outStr += "}";
    return outStr;
}


void SaveStringListToFile(const VS &strList, const char* fileName)
{   
    std::ofstream fcout(fileName);
    VS::const_iterator it;
    for(it = strList.begin(); it != strList.end(); it++)
    {
        fcout<< *it <<std::endl;
    }
}

