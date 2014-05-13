I hope to make this into an Evernote C++ library with a command line tool 
to manage Evernote and which in turn showcases the features of the library
along with its api.

Requires:
Thrift 0.91 and a TSSLSocket.cpp hack which from this thread:
https://discussion.evernote.com/topic/10979-c-in-linuxcheckversion-errorplease-help-me/
It basically states to remove the authorize () call on the last line of checkHandshake () method in the TSSLSocket.cpp file. 

=======
Use make to compile and make test to test it out with my authKeys.
