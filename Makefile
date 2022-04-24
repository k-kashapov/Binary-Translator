all: trans.exe

basicFlags = -I include -Wall -Wextra

trans.exe: BinTrans.cpp
		   g++ BinTrans.cpp -o trans.exe $(basicFlags)
