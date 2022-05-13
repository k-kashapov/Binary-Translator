all: k++

bin/all: k++
		 mv k++ /bin/k++

basicFlags = -I include -Wall -Wextra -O2 -g -D LOGGING

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
	  g++ $(basicFlags) $(langBackDepends) obj/*.o -o lang.exe

# ================= <NASM Tests> ===================
ANEK_DIR    = examples
SRC_DIR     = results/asm
RES_OBJ_DIR = results/obj
RES_EXE_DIR = results/exe

tests: make_tests
	   @echo "\nTesting..."
	   @$(foreach file, $(wildcard $(RES_EXE_DIR)/*), ./$(file); echo Test "$(file)": $$?;)

make_tests:
		@$(foreach file, $(wildcard $(ANEK_DIR)/*.anek), make $(subst $(ANEK_DIR),$(RES_EXE_DIR),$(subst .anek,.exe, $(file)));)

clean_tests:
		@$(foreach file, $(wildcard $(RES_EXE_DIR)/*.exe), rm $(file);)

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
				@./lang_no_img.exe -i $^ -o $@ -S
				@echo
