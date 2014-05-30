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

#include <protocol/TBinaryProtocol.h>
#include <transport/THttpClient.h>
#include <transport/TSSLSocket.h>

#include "evernote-sdk/UserStore.h"
#include "evernote-sdk/NoteStore.h"

#include "evernote.hh"

void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}

evernote::UserStore::UserStore (std::string eU, int p, std::string pT,
            std::string authenticationToken) {
    try {

        evernoteUrl = eU;
        port = p;
        parameterThree = pT;

        auth_http = boost::shared_ptr<apache::thrift::transport::THttpClient>(
            new apache::thrift::transport::THttpClient(evernoteUrl, port, parameterThree));
        auth_http->open();
        boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> userStoreProt(
            new apache::thrift::protocol::TBinaryProtocol(auth_http));
        userStore = new evernote::edam::UserStoreClient (userStoreProt, userStoreProt);
        /**
         * We have the userStore working.
         */
     } catch (apache::thrift::transport::TTransportException e) {
        std::cerr << "Exception in evernote::UserStore::UserStore (std::string eU, int p, std::string pT, std::string authenticationToken)" << std::endl;
     }
}

evernote::UserStore::~UserStore () {
    auth_http->close ();
}

std::string evernote::UserStore::getNoteStoreUrl (std::string authToken) {
    try {        
        std::string noteStoreUrl;
        userStore->getNoteStoreUrl (noteStoreUrl, authToken);
        return noteStoreUrl;
    } catch (apache::thrift::transport::TTransportException e) {
        std::cerr << "Exception in std::string evernote::UserStore::getNoteStoreUrl (std::string authToken)" << std::endl;
        auth_http->close ();
        delete userStore;
        auth_http = boost::shared_ptr<apache::thrift::transport::THttpClient>(
            new apache::thrift::transport::THttpClient(evernoteUrl, port, parameterThree));
        auth_http->open();
        boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> userStoreProt(
            new apache::thrift::protocol::TBinaryProtocol(auth_http));
        userStore = new evernote::edam::UserStoreClient (userStoreProt, userStoreProt);
        std::string noteStoreUrl;
        userStore->getNoteStoreUrl (noteStoreUrl, authToken);
        auth_http->close ();
        delete userStore;
        return noteStoreUrl;
    }
}

evernote::NoteStore::NoteStore (std::string noteStoreUrl) {
    try {
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
    } catch (apache::thrift::transport::TTransportException e) {
        std::cerr << "Exception in evernote::NoteStore::NoteStore (std::string noteStoreUrl)" << std::endl;
    }
}

evernote::NoteStore::~NoteStore () {
    userStoreHttpClient->flush ();
    userStoreHttpClient->close ();
}

std::vector<evernote::Notebook*>* evernote::NoteStore::listNotebooks(std::string authenticationToken) {
    std::vector<evernote::Notebook*>* notebookList = new std::vector<evernote::Notebook*> ();
    try {            
        std::vector<evernote::edam::Notebook> notebooks;
        noteStore->listNotebooks(notebooks, authenticationToken);
        for (unsigned int i = 0; i < notebooks.size (); i++) {
            evernote::Notebook* n = new evernote::Notebook (notebooks[i]);
            notebookList->push_back (n);
        }
    } catch (apache::thrift::transport::TTransportException e) {
        std::cerr << "Exception in evernote::NoteStore::listNotebooks (std::string authenticationToken)" << std::endl;
    }
    return notebookList;
}

evernote::Notebook::Notebook (evernote::edam::Notebook nb) {
    guid = new evernote::GUID (nb.guid);
    name = nb.name;
    updateSequenceNum = nb.updateSequenceNum;
    defaultNotebook = nb.defaultNotebook;
    serviceCreated = new Timestamp (nb.serviceCreated);
    serviceUpdated = new Timestamp (nb.serviceUpdated);
    publishing = new Publishing (nb.publishing);
    published = nb.published;
    stack = nb.stack;

/*
    std::vector<SharedNotebook*> sharedNotebooks;
    BusinessNotebook* businessNotebook;
    User* contact;
    NotebookRestrictions* restrictions;
*/
}

evernote::GUID::GUID (std::string g) {
    guid = g;
}

evernote::Timestamp::Timestamp (long t) {
    timestamp = t;
}

evernote::Publishing::Publishing ( evernote::edam::Publishing p) {

}