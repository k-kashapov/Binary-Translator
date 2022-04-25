all: k++

bin/all: k++
		 mv k++ /bin/k++

basicFlags = -I include -Wall -Wextra -O2

langDepends = main.cpp src/DiffGraph.cpp src/files.cpp src/Ids.cpp src/Lang.cpp \
			  src/Lexic.cpp src/Logs.cpp src/SimpleHash.cpp src/Tree.cpp  		\
              src/TreeRead.cpp src/TreeSave.cpp src/TreeToAsm.cpp src/Reverse.cpp

lang: $(langDepends)
	g++ $(basicFlags) $(langDepends) -o lang.exe
