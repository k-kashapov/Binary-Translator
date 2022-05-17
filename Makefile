all: k++

bin/all: k++
		 mv k++ /bin/k++

basicFlags = -I include -Wall -Wextra -O2 -g -D LOGGING -no-pie

ldLibs = /lib/x86_64-linux-gnu/libc-2.31.so -lm --dynamic-linker /lib64/ld-linux-x86-64.so.2

langDepends = main.cpp src/DiffGraph.cpp src/files.cpp src/Ids.cpp src/Lang.cpp           \
			  src/Lexic.cpp src/Logs.cpp src/SimpleHash.cpp src/Tree.cpp src/ToNASM.cpp   \
              src/TreeRead.cpp src/TreeSave.cpp src/TreeToAsm.cpp src/Reverse.cpp

langFrontDepends =  src/DiffGraph.cpp src/files.cpp src/Ids.cpp src/Lang.cpp     		  \
			  		src/Lexic.cpp src/Logs.cpp src/SimpleHash.cpp src/Tree.cpp            \
              		src/TreeRead.cpp src/TreeSave.cpp src/Reverse.cpp

langBackDepends = main.cpp src/ToNASM.cpp src/TreeToAsm.cpp

OBJECTS = $(patsubst %.cpp, %.o, $(langFrontDepends))

obj/*.o: $(langFrontDepends)
	     g++ $(basicFlags) $(langFrontDepends) -c
	   	 mv *.o obj/
	 
lang: obj/*.o $(langBackDepends)
	  g++ $(basicFlags) $(langBackDepends) obj/*.o -o lang.exe -D CREATE_IMG -D SAVE_TO_BASE

lang_no_misc: obj/*.o $(langBackDepends)
	  		  g++ $(basicFlags) $(langBackDepends) obj/*.o -o lang_no_misc.exe

# ================= <NASM Tests> ===================
ANEK_DIR    = examples
SRC_DIR     = results/asm
RES_OBJ_DIR = results/obj
RES_EXE_DIR = results/exe

tests: make_tests
	   @echo "\nTesting..."
	   @$(foreach file, $(wildcard $(RES_EXE_DIR)/*), echo Test "$(file)";\
	   												  ./$(file);\
	   												  echo Result: $$?;\
	   												  echo ;)

make_tests:
		@$(foreach file, $(wildcard $(ANEK_DIR)/*.anek), make $(subst $(ANEK_DIR),$(RES_EXE_DIR),$(subst .anek,.exe, $(file)));)

clean_tests:
		@$(foreach file, $(wildcard $(RES_EXE_DIR)/*.exe), rm $(file);)
		@$(foreach file, $(wildcard $(RES_OBJ_DIR)/*.o),   rm $(file);)
		@$(foreach file, $(wildcard $(SRC_DIR)/*.s),       rm $(file);)

.PRECIOUS: results/asm/%.s results/obj/%.o

$(RES_EXE_DIR)/%.exe: $(RES_OBJ_DIR)/%.o
					  @echo
					  @echo Linking $@
					  @ld $^ -o $@
					  @echo

$(RES_OBJ_DIR)/%.o: $(SRC_DIR)/%.s
					@echo
					@echo Compiling $@
					@nasm -f elf64 -Wall $^ -o $@
					@echo

$(SRC_DIR)/%.s: $(ANEK_DIR)/%.anek
				@echo
				@echo To NASM $@
				@./lang_no_misc.exe -i $^ -o $@ -S
				@echo
