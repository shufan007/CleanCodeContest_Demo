
#ifndef _CMESSAGE_H
#define _CMESSAGE_H

#include <string>
#include <vector>
#include <map>


typedef std::vector<std::string> VS;
typedef std::pair<std::string, std::string> PAIR_SS;
typedef std::map<std::string, std::string>  MAP_SS;

/******************************************************************************
 * @brief CMessage
 *
 * The is main class for Message Process.
 *******************************************************************************/
class CMessage
{
public:

	CMessage();

	CMessage(char mode);

	~CMessage(void);


	 /**
	****************************************************************************
	*
	* @brief    GetFinalMsgDic
	*
	* Get the final Message dictionary 'm_msgDic' by 
	*   remove the filter list from the original messages 
	*
	* @param    -
	*
	***************************************************************************/	
	MAP_SS GetFinalMsgDic();


	 /**
	****************************************************************************
	*
	* @brief    MsgIdCheck
	*
	*  Check if the string is a valid message Id string
	*  the string just contain '0-9','a-f','A-F',' '
	*  the size must no more than 4 after removed the spaces.
	*
	* @param    MsgIdStr     string to be checked
	*
	* @return   check result
	*
	***************************************************************************/
	bool MsgIdCheck(const std::string &str);


	 /**
	****************************************************************************
	*
	* @brief    AdjustMsgId
	*
	*  Adjust the message Id string
	*  @ Example: " 97D", "97D ", "97D" ==> "097d"
	*
	* @param    msgId    message Id str to be adjust
	*
	* @return   message Id str being adjusted
	*
	***************************************************************************/	
	std::string AdjustMsgId(const std::string &msgId);


	
	 /**
	****************************************************************************
	*
	* @brief    AdjustMsgName
	*
	*  djust the message name string: remove the spaces
	*
	* @param    msgName 
	*
	* @return   message name str being adjusted
	*
	***************************************************************************/
	std::string AdjustMsgName(const std::string &msgName);	


private:
	char 		m_mode;
	MAP_SS 		m_msgDic;
	MAP_SS 		m_filterMsgList;


	 /**
	****************************************************************************
	*
	* @brief    MessagePropertyExtract
	*
	*  Extract message Id and message name from message property string, 
	*  save original messages to a map 'm_msgDic', in format:
	*    msgId -> msgName
	*    msgName -> msgId 
	*
	* @param    msgPropertyStr  message property string like:"0970=RTM_NodeId_CpidReq"
	*
	***************************************************************************/
	void MessagePropertyExtract(const std::string &msgPropertyStr);

	
	 /**
	****************************************************************************
	*
	* @brief    FilterMessageExtract
	*
	* Extract message Id or message name or both from filter messages file:
	*    "white_list.txt" or "black_list", save messages msgId or msgName
	*
	*  save filter messages to a map 'm_filterMsgList', in format:
	*    msgId -> msgName
	*
	* @param    filterMsgStr  message property string like:
	*               "0970", "RTM_NodeId_CpidReq" or "0970=RTM_NodeId_CpidReq"
	*                            
	***************************************************************************/
	void FilterMessageExtract(const std::string &filterMsgStr);

	
	 /**
	****************************************************************************
	*
	* @brief    LoadMessageProperty
	*
	*  Load Messages Properties from file given file, 
	*  extract and save messages by the given filter method
	*
	* @param    fileName	message property file	  
	*
	***************************************************************************/
	void LoadMessageProperty(const char* fileName);


	 /**
	****************************************************************************
	*
	* @brief    LoadFilterMessage
	*
	*  Load Filter Messages from file given file, 
	*  extract and save messages by the given filter method
	*
	* @param    fileName	message property file
	*
	***************************************************************************/
	void LoadFilterMessage(const char* fileName);

	
};

#endif /* _CMESSAGE_H */

