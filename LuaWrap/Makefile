PRJ_NAME       =        LuaWrap
CC             =        g++
AR             =        ar

CUR_DIR        =        $(shell pwd)
LIB_DIR        =        $(CUR_DIR)/../lib
COMMON_DIR     =        $(CUR_DIR)/../Common
SRC_DIR        =        $(CUR_DIR)/src
INC_DIR        =        $(CUR_DIR)/inc
OBJ_DIR        =        $(CUR_DIR)/../BuildOut/$(PRJ_NAME)
LUA_DIR        =        $(CUR_DIR)/../lua-5.1.4/src
TOLUA_DIR      =        $(CUR_DIR)/../tolua++-1.0.93/include

CFLAGS         =        -g -fPIC -I$(INC_DIR) -I$(LUA_DIR) -I$(TOLUA_DIR)
VPATH          =        $(SRC_DIR)

TARGET         =        $(LIB_DIR)/lib$(PRJ_NAME).a


vpath %.o $(OBJ_DIR)

include $(SRC_DIR)/BuildList.mk $(OBJS)

OBJ_FILES := $(foreach n,$(OBJS), $(OBJ_DIR)/$(n))


all: pre_build_event $(TARGET) post_build_event

$(TARGET):$(OBJS)
	$(AR) -rcs $(TARGET) $(OBJ_FILES)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $(OBJ_DIR)/$@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(OBJ_DIR)/$@


pre_build_event:
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)

post_build_event:

install:

clean:
	-rm -f -r $(TARGET) $(OBJ_FILES)