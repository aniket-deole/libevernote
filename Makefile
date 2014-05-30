CC=g++
CFLAGS=-c -fPIC -Ievernote-sdk/thrift  -I/usr/include/ `pkg-config --cflags --libs thrift`
LDFLAGS=`pkg-config --cflags --libs thrift`

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
	$(CC) -shared $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.so *.out evernotecppsdktest

test: all
	$(CC) -g evernotedataprovider.cc -Ievernote-sdk -Ievernote-sdk/thrift `pkg-config --cflags --libs thrift` -L`pwd` -levernote -o evernotecppsdktest && export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH && ./evernotecppsdktest

testnew: all
	$(CC) -g example.cc -Ievernote-sdk -Ievernote-sdk/thrift `pkg-config --cflags --libs thrift` -L`pwd` -levernote -o evernotecppsdktest && export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH && ./evernotecppsdktest