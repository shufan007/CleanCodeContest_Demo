
#ifndef _CRTMLOG_H
#define _CRTMLOG_H

#include <string>
#include <vector>
#include <deque>
#include <map>

typedef std::vector<std::string> VS;
typedef std::pair<std::string, std::string> PAIR_SS;
typedef std::map<std::string, std::string>  MAP_SS;
typedef std::deque<PAIR_SS> 				D_PAIR_SS;
typedef std::vector<D_PAIR_SS> 				V_D_PAIR_SS;
typedef std::pair<std::string, D_PAIR_SS> 	PAIR_S_D_PAIR_SS;
typedef std::multimap<std::string, D_PAIR_SS> 	MULTIMAP_S_D_PAIR_SS;

/******************************************************************************
 * @brief CRtmLog
 *
 * The is main class of Run Time Log Parser.
 *******************************************************************************/
class CRtmLog
{
public:
	CRtmLog();

	CRtmLog(const MAP_SS &msgDic, const char mode);

	~CRtmLog(void);

	 /**
	****************************************************************************
	*
	* @brief    LogParseOnFile
	*
	*  Log Parse on one file 
	*
	* @param    fileName    the name of the original file
	*
	***************************************************************************/	
	void LogParseOnFile(const char* logFile);


	 /**
	****************************************************************************
	*
	* @brief    LogParseOnFileList
	*
	*  Log Parse on file list 
	*
	* @param    logFileList    the log file list
	*
	***************************************************************************/	
	void LogParseOnFileList(const VS &logFileList);

private:
	char 		m_mode;
	MAP_SS 			m_msgDic;
	V_D_PAIR_SS 	m_extractedLog;


	 /**
	****************************************************************************
	*
	* @brief    SaveMessageItems
	*
	*   Search message Id from m_msgDic, if found, replace it by message name,
	*    then save it together with 'from Id', 'to Id', to 'm_extractedLog'
	*    
	* @param: 
	*   - msgId     message id
	*   - fromId    message from id
	*   - toId      message to id
	*
	***************************************************************************/	
	void SaveMessageItems(std::string &msgId, std::string &fromId, std::string &toId);


	 /**
	****************************************************************************
	*
	* @brief    ExtractPattern_messageType1
	*
	* Extract Pattern message type1:
	*    [0x1011110b->0x1011127f] msg_type:42e1, flag:0, payload length:40 
	*   extract log items and save them to 'm_extractedLog'
	*
	* @param    logStr    original log string
	*
	* @return   matched result
	*
	***************************************************************************/	
	bool ExtractPattern_messageType1(const std::string &logStr);


	 /**
	****************************************************************************
	*
	* @brief    ExtractPattern_messageType2
	*
	* Extract Pattern message type2:
	*    0x26bc: 1011127f --> 30111103
	*   extract log items and save them to 'm_extractedLog'
	*
	* @param    logStr    original log string
	*
	* @return   matched result
	*
	***************************************************************************/	
	bool ExtractPattern_messageType2(const std::string &logStr);

	
	 /**
	****************************************************************************
	*
	* @brief    ExtractPattern_timeType
	*
	* Extract Pattern time type:
	*    RTM   <24:02.00> ######### Prev_Subframe=1,
	*   extract log items and save them to 'm_extractedLog'
	*
	* @param    logStr    original log string
	*
	* @return   matched result
	*
	***************************************************************************/
	bool ExtractPattern_timeType(const std::string &logStr);


	 /**
	****************************************************************************
	*
	* @brief    ExtractPattern_noteType
	*
	* Extract Pattern note type:
	*    RTM   <24:05.00> User Delete: crnti (54135), ueId (0), ueIndex (0), TM(4) 
	*   extract log items and save them to 'm_extractedLog'
	*
	* @param    logStr    original log string
	*
	* @return   matched result
	*
	***************************************************************************/	
	bool ExtractPattern_noteType(const std::string &logStr);


	 /**
	****************************************************************************
	*
	* @brief    ExtractLogInfo
	*
	* Match original log string to each Extract Pattern,
	*   Extract Log Info from original log string
	*   extract log items and save them to 'm_extractedLog'
	*
	* @param    logStr    original log string
	*
	***************************************************************************/	
	void ExtractLogInfo(const std::string &str);

	
	 /**
	****************************************************************************
	*
	* @brief    ExtractLogInfoFromFile
	*
	* Extract log info from a given file
	*
	* @param    logFile    the name of the log file
	*
	***************************************************************************/
	void ExtractLogInfoFromFile(const char* logFile);


	 /**
	****************************************************************************
	*
	* @brief    GetExtractLogMap
	*			map the extract log list to MULTIMAP_S_D_PAIR_SS format
	*
	* @param    -
	*
	* return    extractLogMap    type MULTIMAP_S_D_PAIR_SS
	*
	***************************************************************************/
	MULTIMAP_S_D_PAIR_SS  GetExtractLogMap();

	
	 /**
	****************************************************************************
	*
	* @brief    JosnFormatConversion
	*
	* Convert extracted logs to josn format, save the logs to 'josnLog'
	*
	* @param    josnLog    to save the josn format log
	*
	***************************************************************************/
	void JosnFormatConversion(VS &josnLog);

	
};

#endif /* _CRTMLOG_H */

