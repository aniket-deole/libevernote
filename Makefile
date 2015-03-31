CC=g++
CFLAGS=-c -g -fPIC -Ievernote-sdk/thrift  -Ievernote-sdk -L`pwd` -lthrift
LDFLAGS=-L`pwd` -lthrift

SRCS= \
    evernote-sdk/Limits_types.cpp \
    evernote-sdk/NoteStore.cpp \
    evernote-sdk/Types_constants.cpp \
    evernote-sdk/UserStore_constants.cpp \
    evernote-sdk/UserStore_types.cpp \
    evernote-sdk/Errors_types.cpp \
    evernote-sdk/Limits_constants.cpp \
    evernote-sdk/NoteStore_constants.cpp \
    evernote-sdk/NoteStore_types.cpp \
    evernote-sdk/Types_types.cpp \
    evernote-sdk/UserStore.cpp \
    evernote.cpp
 
OBJECTS=$(SRCS:.cpp=.o)
EXECUTABLE=libevernote.so

all: $(SRCS) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -shared  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o libevernote.so *.out evernotecppsdktest

test: all
	$(CC) -g example.cc -Ievernote-sdk -Ievernote-sdk/thrift -L`pwd` -lthrift -ldl -o evernotecppsdktest && export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH && ./evernotecppsdktest

debug: all
	$(CC) -g example.cc -Ievernote-sdk -Ievernote-sdk/thrift -L`pwd` -lthrift -ldl -o evernotecppsdktest && export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH && gdb ./evernotecppsdktest
