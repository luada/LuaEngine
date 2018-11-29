PRJ_NAME       =        lua
CC             =        gcc
AR             =        ar

CUR_DIR        =        $(shell pwd)
LIB_DIR        =        $(CUR_DIR)/../lib
SRC_DIR        =        $(CUR_DIR)/src
INC_DIR        =        $(CUR_DIR)/src
OBJ_DIR        =        $(CUR_DIR)/../BuildOut/$(PRJ_NAME)

CFLAGS         =        -g -fPIC -I$(INC_DIR)
VPATH          =        $(SRC_DIR)

TARGET         =        $(LIB_DIR)/lib$(PRJ_NAME).a


vpath %.o $(OBJ_DIR)

include $(SRC_DIR)/BuildList.mk $(OBJS)

OBJ_FILES := $(foreach n,$(OBJS), $(OBJ_DIR)/$(n))


all: pre_build_event $(TARGET) post_build_event

$(TARGET):$(OBJS)
	$(AR) -rcs $(TARGET) $(OBJ_FILES)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(OBJ_DIR)/$@

pre_build_event:
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)

post_build_event:

install:

clean:
	-rm -f -r $(TARGET) $(OBJ_FILES)