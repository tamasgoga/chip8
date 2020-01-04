NAME = gnes

WARNINGS = -Wall -Wextra -Werror
COMPILER = clang++-6.0 -std=c++17

SOURCES = main.cpp

SDL = -lSDL2
# SDL = -lSDL2 -lSDL2_image -lSDL2_ttf

rel: clang
	date +"%nCompiled on: %A, %T (%Y %b %d)"

dbg: debug
	date +"%nCompiled DEBUG on: %A, %T (%Y %b %d)"

clang: $(SOURCES)
	$(COMPILER) -O2 $(WARNINGS) -o $(NAME) $(SOURCES) $(SDL)

debug: $(SOURCES)
	$(COMPILER) -g $(WARNINGS) -o $(NAME) $(SOURCES) $(SDL)

clean:
	rm ./$(NAME)

