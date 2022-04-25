all: trans.exe

basicFlags = -I include -Wall -Wextra -D IS_LINUX

trans.exe: BinTrans.cpp
		   g++ BinTrans.cpp -o trans.exe $(basicFlags)
