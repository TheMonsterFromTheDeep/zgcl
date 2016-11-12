# zstr.o: zstr.c zstr.h
# 	gcc -fPIC -g -c -Wall $< -o $@

# zlist.o: zlist.c zlist.h
#	gcc -c $< -o $@

SRC_DIR= ./src
BUILD_DIR= ./build

SRCS= zstr.c zlist.c zexception.c

OBJS= $(SRCS:%.c=$(BUILD_DIR)/%.o)

DEPS= $(OBJS:%.o=$(BUILD_DIR)/%.d)

CFLAGS = -Wall

LIB_NAME= zlib
LIB_BUILDNAME= $(LIB_NAME:%=lib%.a)
# LIB_SONAME= $(LIB_NAME:%=lib%.so.1)
# LIB_FULLNAME= $(LIB_SONAME:=.0.1)

INSTALL_DIR?=/usr
LIB_PATH=$(INSTALL_DIR)/lib
INCLUDE_PATH=$(INSTALL_DIR)/include

$(LIB_BUILDNAME): $(OBJS)
	#gcc -shared -Wl,-soname,$(LIB_SONAME) \
	#-o $(LIB_FULLNAME) $(OBJS) -lc
	ar rcs $(BUILD_DIR)/$(LIB_BUILDNAME) $(OBJS)

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -MMD -c $< -o $@ $(CFLAGS)

.PHONY: clean install
clean:
	rm -rf $(BUILD_DIR)

install: $(LIB_FULLNAME)
	cp $(BUILD_DIR)/$(LIB_BUILDNAME) $(LIB_PATH)
	chmod 0755 $(LIB_PATH)/$(LIB_BUILDNAME)
	mkdir -p $(INCLUDE_PATH)/zlib
	cp $(SRC_DIR)/*.h $(INCLUDE_PATH)/zlib
	ldconfig
