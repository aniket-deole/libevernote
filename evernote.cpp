/***
Copyright (C) 2013-2014 Aniket Deole <aniket.deole@gmail.com>
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License version 2.1, as published
by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranties of
MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <protocol/TBinaryProtocol.h>
#include <transport/THttpClient.h>
#include <transport/TSSLSocket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/md5.h>

#include "rapidxml_print.hpp"
#include "rapidxml.hpp"

#include "UserStore.h"
#include "NoteStore.h"

#include "evernote.hh"

void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}

evernote::UserStore::UserStore (std::string evernoteUrl, int port, std::string parameterThree,
            std::string authenticationToken) {
    auth_http = boost::shared_ptr<apache::thrift::transport::THttpClient>(
        new apache::thrift::transport::THttpClient(evernoteUrl, port, parameterThree));
    auth_http->open();
    boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> userStoreProt(
        new apache::thrift::protocol::TBinaryProtocol(auth_http));
    userStore = new evernote::edam::UserStoreClient (userStoreProt, userStoreProt);
}

evernote::UserStore::~UserStore () {
    auth_http->close ();
}

std::string evernote::UserStore::getNoteStoreUrl (std::string authToken) {
    std::string noteStoreUrl;
    userStore->getNoteStoreUrl (noteStoreUrl, authToken);
    return noteStoreUrl;
}

evernote::NoteStore::NoteStore (std::string noteStoreUrl) {
    boost::shared_ptr<apache::thrift::transport::TSSLSocketFactory> sslSocketFactory = 
    boost::shared_ptr<apache::thrift::transport::TSSLSocketFactory>(
        new apache::thrift::transport::TSSLSocketFactory());;

    boost::shared_ptr<apache::thrift::transport::TSocket> sslSocket = sslSocketFactory->
        createSocket("sandbox.evernote.com", 443);
    boost::shared_ptr<apache::thrift::transport::TBufferedTransport> bufferedTransport(
        new apache::thrift::transport::TBufferedTransport(sslSocket));
    userStoreHttpClient = boost::shared_ptr<apache::thrift::transport::THttpClient>(
        new apache::thrift::transport::THttpClient(bufferedTransport, "sandbox.evernote.com", noteStoreUrl));

    userStoreHttpClient->open();

    boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> noteStoreProt(
        new apache::thrift::protocol::TBinaryProtocol(userStoreHttpClient) );
    noteStore = new evernote::edam::NoteStoreClient (noteStoreProt, noteStoreProt);
}

evernote::NoteStore::~NoteStore () {
    userStoreHttpClient->close ();    
}


/*
 * Sample program that gets notes from Evernote's dev server account.
 */
int main () {
    /*
     * AuthToken would be recieved after oauth. 
     * Right now we are using the one provided by evernote for accessing 
     * sandbox.evernote.com
     */
    std::string authToken = "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0";
    
    evernote::UserStore* userStore = new evernote::UserStore ("sandbox.evernote.com", 80, "/edam/user", authToken);
    evernote::NoteStore* noteStore = new evernote::NoteStore (userStore->getNoteStoreUrl (authToken));

    delete noteStore;
    delete userStore;

    return 0;
}