
SRC_PATH = src
LOG_PATH = drx_log

CXX = g++

objects = common.o CMessage.o CRtmLog.o rtm_log_parser.o

rtm_log_parser: $(objects)
	$(CXX) -o rtm_log_parser $(objects)

common.o: $(SRC_PATH)/common.cpp $(SRC_PATH)/common.h
	$(CXX) -c $(SRC_PATH)/common.cpp

CMessage.o: $(SRC_PATH)/CMessage.cpp $(SRC_PATH)/CMessage.h
	$(CXX) -c $(SRC_PATH)/CMessage.cpp

CRtmLog.o: $(SRC_PATH)/CRtmLog.cpp $(SRC_PATH)/CRtmLog.h
	$(CXX) -c $(SRC_PATH)/CRtmLog.cpp

rtm_log_parser.o: $(SRC_PATH)/rtm_log_parser.cpp $(SRC_PATH)/CRtmLog.h $(SRC_PATH)/CMessage.h
	$(CXX) -c $(SRC_PATH)/rtm_log_parser.cpp

clean:
	rm -r *.o rtm_log_parser
	rm -r $(LOG_PATH)/*.txt
