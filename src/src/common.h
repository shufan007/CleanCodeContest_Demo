
#ifndef _COMMON_H
#define _COMMON_H

#include <string>
#include <vector>
#include <deque>
#include <dirent.h>

typedef std::vector<std::string> 			VS;
typedef std::pair<std::string, std::string> PAIR_SS;
typedef std::deque<PAIR_SS> 				D_PAIR_SS;


 /**
****************************************************************************
*
* @brief    FileNameFilter
*
* File Name Filter, used by function "TraverseDir"
*
***************************************************************************/
int FileNameFilter(const struct dirent *pDir);


/* ******************************************************************
 Function:  
    
******************************************************************** */
 /**
****************************************************************************
*
* @brief    TraverseDir
*
* Traverse directory and return all the files
*
* @param    path    		the input path
*
* return    file list
*
***************************************************************************/  
std::vector<std::string> TraverseDir(const char *path);



 /**
****************************************************************************
*
* @brief    StrSplit
*
* Split a string by a given string pattern and the number of split times: n
*
* @param    str    		the original string
*			pattern		split pattern
*			n			split times
*
* return    the splited substr list.
*
***************************************************************************/            
VS StrSplit(const std::string &str, const std::string &pattern, const int n);


 /**
****************************************************************************
*
* @brief    JosnFormatCombine
*
* combine D_PAIR_SS type to josn format
* josn format:     
*	{time:"09:00", message:"PHY_PdcchSendReq", from:"3011110d", to:"1011127f"}
*	{time:"09:02", note:"######### Msg 4 start #########"}
*
* @param    josnLog    to save the josn format log
*
***************************************************************************/
std::string JosnFormatCombine(D_PAIR_SS &pairItem);



 /**
****************************************************************************
*
* @brief    SaveStringListToFile
*
*  Save the string list to file 
*
* @param    strList		input string list
*			fileName    the name of the out put file
*
***************************************************************************/
void SaveStringListToFile(const VS &strList, const char* fileName);


#endif /*  _COMMON_H */

