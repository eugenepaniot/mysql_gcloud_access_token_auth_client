LIBS := $(shell mysql_config --cflags)
FLAGS := $(shell mysql_config --libs)
WARN_FLAGS := -Wunused-command-line-argument
INCLUDE := $(shell mysql_config --include)
LDFLAGS = -shared
CFLAGS := $(FLAGS) $(LIBS) $(INCLUDE) -D_GNU_SOURCE -fPIC -O2

CC = clang++

TARGET  = gcloud_access_token_auth_plugin.so
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -v -o $(TARGET) $(OBJECTS)

all: $(TARGET)

clean:
	@rm -fv $(OBJECTS) $(TARGET) *.so *.o

.PHONY : clean all