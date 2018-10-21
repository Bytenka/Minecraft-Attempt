programName = program

buildDir = ../linux-build

libs = -lstdc++ -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor -lpng
flags = -I $(HOME)/Dropbox/OpenGL/Libraries/Includes/ -std=c++14 -Wall -Wextra -O0 -g -c
cflags = -I $(HOME)/Dropbox/OpenGL/Libraries/Includes/ -Wall -Wextra -O0 -g -c

cppsrc = $(wildcard *.cpp) \
         $(wildcard application/*.cpp) \
         $(wildcard graphics/*.cpp) \
         $(wildcard utils/*.cpp) \
         $(wildcard world/*.cpp)

csrc = utils/glad.c

obj = $(cppsrc:.cpp=.o) $(csrc:.c=.o)
obj := $(addprefix $(buildDir)/,$(obj))


all: $(programName)


$(programName): $(obj)
	g++ $(libs) -o $(buildDir)/$@ $^
	@echo -e "\nBuild done successfully."


$(buildDir)/%.o : %.cpp %.h
	@mkdir -p $(shell dirname -z $@)
	g++ $(flags) -o $@ $<


$(buildDir)/%.o: %.cpp
	@mkdir -p $(shell dirname -z $@)
	g++ $(flags) -o $@ $<


$(buildDir)/%.o: %.c
	@mkdir -p $(shell dirname -z $@)
	gcc $(cflags) -o $@ $<


.PHONY: rebuild

rebuild:
	@echo "Rebuilding project..."
	@rm -rf $(buildDir)/* && make

run:
	@make && $(buildDir)/./$(programName)