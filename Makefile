.PHONY:clean distclean install output
.SUFFIXES: .cpp .c

# 编译器版本
CC=g++
# 头文件路径
INCLUDEPATH=./include
# .o文件目录
BUILDPATH=./build
# 源文件目录
SRCPATH=./src
# 安装目录
INSTALLPATH=/usr/bin/
# 预处理选项
CPPFLAGS=-E -D #-O1 -O2 -O3
# 编译选项
CFLAGS=-I$(INCLUDEPATH) -c -g -Wall
# 库选项
LDFLAGS=-L./lib -lpthread -lodb -lodb-mysql
# 目标名
TARGET=app

COPY=sudo cp
RM=rm -rf

#获取src目录下指定后缀文件名字符串
SRCFILE_C=$(wildcard $(SRCPATH)/*.c)
SRCFILE_CPP=$(wildcard $(SRCPATH)/*.cpp)
#通过字符串处理的方式将后缀替换
DEPENDENCY=$(patsubst $(SRCPATH)/%.cpp,$(BUILDPATH)/%.o,$(SRCFILE_CPP)) $(patsubst $(SRCPATH)/%.c,$(BUILDPATH)/%.o,$(SRCFILE_C))

#$@ 表示当前目标名
#$< 表示依赖中的第一个
#$^ 表示所有依赖
$(TARGET):$(DEPENDENCY)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILDPATH)/%.o:$(SRCPATH)/%.cpp
	$(CC) $< -o $@ $(CFLAGS)

$(BUILDPATH)/%.o:$(SRCPATH)/%.c
	$(CC) $< -o $@ $(CFLAGS)

output:
	@echo 原始文件:$(SRCFILE_C) $(SRCFILE_CPP)
	@echo 替换文件:$(DEPENDENCY)

clean:
	$(RM) $(DEPENDENCY) $(TARGET)
#	rm *.o -rf
#	rm app -rf
install:
	$(COPY) $(TARGET) $(INSTALLPATH)
#	sudo cp app /usr/bin/
distclean:
	$(RM) $(INSTALLPATH)$(TARGET)
#	sudo rm /usr/bin/app -rf
