CC = gcc
CFLAGS = -Wall -Wextra -g -std=gnu17  # -O3 -ftrapv  -Warray-bounds -Wformat-overflow -Wstringop-overflow -D_FORTIFY_SOURCE=2 #-fsanitize=address
CFLAGS = -Wall -Wextra -g -std=gnu17 -ftrapv  -Warray-bounds -Wformat-overflow -Wstringop-overflow -D_FORTIFY_SOURCE=2 # -fsanitize=address

LDLIBS = -lm `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf

INCLUDE_PATH = ./include

TARGET = main

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCLUDE_PATH)/*.h)
# OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET):
	mkdir -p $(BINDIR)
	$(CC) -o $@ $(SOURCES) $(CFLAGS) $(LDLIBS) -I $(INCLUDE_PATH)
	rm -f *.o
	@echo "Building complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(LDLIBS) -I$(INCLUDE_PATH)

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(BINDIR)/$(TARGET)

all: $(BINDIR)/$(TARGET)
