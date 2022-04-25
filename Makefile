all: trans.exe

basicFlags = -I include -Wall -Wextra -D IS_LINUX

transSrc = BinTrans.cpp src/Logs.cpp

trans.exe: $(transSrc)
		   g++ $(transSrc) -o trans.exe $(basicFlags)
