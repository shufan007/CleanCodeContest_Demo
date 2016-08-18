
#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include "../src/common.h"
#include "../gtest-1.7.0/include/gtest/gtest.h"


/*test for  FileNameFilter()*/
TEST(FileNameFilter_Test, return_1)
{
	dirent pDir;
	strcpy(pDir.d_name, "abc");
	int ret_value = FileNameFilter(&pDir);
	EXPECT_EQ(1, ret_value) ;
}

TEST(FileNameFilter_Test, return_0)
{
	dirent pDir;
	strcpy(pDir.d_name, "..");
	int ret_value = FileNameFilter(&pDir);
	EXPECT_EQ(0, ret_value) ;
}


/*test for  TraverseDir()*/
TEST(TraverseDir_Test, Normal_directory)
{
	std::vector<std::string> fileList = TraverseDir("../drx_log");
	EXPECT_LT(0, fileList.size()) ;
}

TEST(TraverseDir_Test, Empty_directory)
{
	std::vector<std::string> fileList = TraverseDir("../abc");
	EXPECT_EQ(0, fileList.size()) ;
}

/*test for  StrSplit()*/
TEST(StrSplit_Test, Normal_pattern)
{
	std::vector<std::string> subStrList = StrSplit(" aa bb_cc", " _", 3);
	EXPECT_EQ(3, subStrList.size()) ;
}

TEST(StrSplit_Test, Empty_pattern)
{
	std::vector<std::string> subStrList = StrSplit(" aa bb_cc", "", 3);
	EXPECT_EQ(0, subStrList.size()) ;
}

/*test for SaveStringListToFile */
TEST(JosnFormatCombine_Test, JosnFormatCombine_ok)
{
	D_PAIR_SS pairItem;
	pairItem.push_back(PAIR_SS("name", "ABCD"));
	pairItem.push_back(PAIR_SS("id", "0123"));
	std::string outJosnStr = JosnFormatCombine(pairItem);
	ASSERT_TRUE("{name:\"ABCD\", id:\"0123\"}" == outJosnStr);
}

/*test for SaveStringListToFile */
TEST(SaveStringListToFile_Test, SaveStringListToFile_ok)
{
	VS strList;
	std::string str1("abc"); 
	strList.push_back(str1);
	const char* outfile("test_files/saveStrList_Test.txt");	
	SaveStringListToFile(strList, outfile);
    std::ifstream infile(outfile);
    ASSERT_TRUE(infile);
}



