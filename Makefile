TARGET := sensor

CC := mipsel-openwrt-linux-gcc
CXX := mipsel-openwrt-linux-g++
# CC := gcc
# CXX := g++

#这里添加其他头文件路径
INC_DIR = -I./include -I./src2
LDLIBS = -ldl -lm -lpthread  
# LDLIBS +=  -lz

CFLAGS := $(INC_DIR) -Wno-deprecated -std=c++11 -g

LNK_FLAGS := -L/usr/local/lib -I./include $(LDLIBS)

SRC_DIR = src
BUILD_DIR = output
OBJ_DIR = $(BUILD_DIR)/obj
DEPS_DIR  = $(BUILD_DIR)/deps

#这里递归遍历3级子目录
DIRS := $(shell find $(SRC_DIR) -maxdepth 3 -type d)
#将每个子目录添加到搜索路径
VPATH = $(DIRS)

SOURCES   = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))  
OBJS  = $(addprefix $(OBJ_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))  
DEPS  = $(addprefix $(DEPS_DIR)/, $(patsubst %.cpp,%.d,$(notdir $(SOURCES))))

$(TARGET):$(OBJS)
	$(CXX) $^ $(LNK_FLAGS) -o $@

#编译之前要创建OBJ目录，确保目录存在
$(OBJ_DIR)/%.o:%.cpp
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi;\
	$(CXX) -c $(CFLAGS) -o $@ $<

#编译之前要创建DEPS目录，确保目录存在
#前面加@表示隐藏命令的执行打印
$(DEPS_DIR)/%.d:%.cpp
	if [ ! -d $(DEPS_DIR) ]; then mkdir -p $(DEPS_DIR); fi;\
	set -e; rm -f $@;\
	$(CXX) -MM $(CFLAGS) $< > $@.$$$$;\
	sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$
#前面加-表示忽略错误
-include $(DEPS)

.PHONY : clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
