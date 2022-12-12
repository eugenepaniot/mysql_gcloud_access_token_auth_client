current_dir = $(shell pwd)
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

my.cnf:
	@sed -i '' -e 's|#current_dir#|$(current_dir)|g' my.cnf

all: $(TARGET) my.cnf install

install:
	@grep -q $(current_dir) ~/.my.cnf || sed -i '.bak' '1s|^|!include $(current_dir)/my.cnf\n|g' ~/.my.cnf

clean:
	@git chechout -- my.cnf
	@rm -fv $(OBJECTS) $(TARGET) *.so *.o

.PHONY : clean all my.cnf