PRJ_NAME       =        LuaDebugger
CC             =        g++
C              =	gcc
AR             =        ar

CUR_DIR        =        $(shell pwd)
LIB_DIR        =        $(CUR_DIR)/../lib
BIN_DIR        =        $(CUR_DIR)/../../output
SRC_DIR        =        $(CUR_DIR)
INC_DIR        =        $(CUR_DIR)
OBJ_DIR        =        $(CUR_DIR)/../BuildOut/$(PRJ_NAME)

CFLAGS         =        -g -fPIC -I$(INC_DIR)
VPATH          =        $(SRC_DIR)

TARGET         =        $(BIN_DIR)/$(PRJ_NAME)


vpath %.o $(OBJ_DIR)

include $(SRC_DIR)/BuildList.mk $(OBJS)

OBJ_FILES := $(foreach n,$(OBJS), $(OBJ_DIR)/$(n))


all: pre_build_event $(TARGET) post_build_event

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJ_FILES) -ldl -lpthread -g

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $(OBJ_DIR)/$@

%.o: %.c
	$(C) -c $(CFLAGS) $< -o $(OBJ_DIR)/$@


pre_build_event:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

post_build_event:

install:

clean:
	-rm -f -r $(TARGET) $(OBJ_FILES)