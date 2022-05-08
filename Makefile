all: k++

bin/all: k++
		 mv k++ /bin/k++

basicFlags = -I include -Wall -Wextra -O2 -g

langDepends = main.cpp src/DiffGraph.cpp src/files.cpp src/Ids.cpp src/Lang.cpp           \
			  src/Lexic.cpp src/Logs.cpp src/SimpleHash.cpp src/Tree.cpp src/ToNASM.cpp   \
              src/TreeRead.cpp src/TreeSave.cpp src/TreeToAsm.cpp src/Reverse.cpp

langFrontDepends =  src/DiffGraph.cpp src/files.cpp src/Ids.cpp src/Lang.cpp     		  \
			  		src/Lexic.cpp src/Logs.cpp src/SimpleHash.cpp src/Tree.cpp            \
              		src/TreeRead.cpp src/TreeSave.cpp src/Reverse.cpp

langBackDepends = main.cpp src/ToNASM.cpp src/TreeToAsm.cpp

OBJECTS = $(patsubst %.cpp, %.o, $(langFrontDepends))

$(OBJECTS): $(langFrontDepends)
	   		g++ $(basicFlags) $(langFrontDepends) -c
	   		mv *.o obj/
	 
lang: obj/*.o $(langBackDepends)
	  g++ $(basicFlags) $(langBackDepends) obj/*.o -o lang.exe
