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

#include "UserStore.h"
#include "NoteStore.h"

#include "evernotedataprovider.hh"

/*
Note(contentHash='\xbbS\x8ad\x05\xd0%\x98\xf9\xa6L[\xc2\xce\x8bY', 
    updated=1388741518000, created=1388741518000, deleted=None, contentLength=221, 
    title='Test note from EDAMTest.py', notebookGuid='dafa6b30-402c-4920-b251-2827881d2ac3', 
    content='<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE en-note SYSTEM "http://xml.evernote.com/pub/enml2.dtd"><en-note>Here is the Evernote logo:<br/><en-media type="image/png" hash="a54fe8bcd146e20a8a5742834558543c"/></en-note>', 
    tagNames=None, updateSequenceNum=53, tagGuids=None, active=True, 
    attributes=NoteAttributes(lastEditorId=None, placeName=None, sourceURL=None, 
        classifications=None, creatorId=None, author=None, reminderTime=None, altitude=None, 
        reminderOrder=None, shareDate=None, reminderDoneTime=None, longitude=None, lastEditedBy=None, 
        source=None, applicationData=None, sourceApplication=None, latitude=None, contentClass=None, subjectDate=None), 
    guid='c3aab5ac-947f-4d94-8e8c-a54f11674ec0', 
    resources=[Resource(noteGuid='c3aab5ac-947f-4d94-8e8c-a54f11674ec0', height=60, 
        width=60, alternateData=None, mime='image/png', updateSequenceNum=54, 
        duration=None, attributes=ResourceAttributes(recoType=None, sourceURL=None, cameraMake=None, 
        timestamp=None, altitude=None, clientWillIndex=None, longitude=None, fileName=None, 
        attachment=None, latitude=None, applicationData=None, cameraModel=None), 
        guid='05ac4079-b3e5-4648-bc45-26fbe834d88b', data=Data(body=None, bodyHash='\xa5O\xe8\xbc\xd1F\xe2\n\x8aWB\x83EXT<', size=6035), 
        active=True, recognition=Data(body=None, bodyHash='\x91G\x01\xa5\x11\\Xw\xca\xc1\x00"\x9d\xf3\x16Y', size=291))]
)
*/


std::vector<evernote::Notebook> gNotebooks;
std::vector<evernote::Note> gNotes;


const std::string evernote::EvernoteDataProvider::evernoteUrl = "sandbox.evernote.com";
const int evernote::EvernoteDataProvider::port = 80;
const int evernote::EvernoteDataProvider::sslPort = 443;
const std::string evernote::EvernoteDataProvider::parameterThree = "/edam/user";
const std::string evernote::EvernoteDataProvider::attachmentFolder = "data/attachments/";

evernote::EvernoteDataProvider::EvernoteDataProvider () {
    authToken = "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0";
    connectionOpened = false;
    open ();
}

evernote::EvernoteDataProvider::~EvernoteDataProvider () {
}

int evernote::EvernoteDataProvider::open () {
    if (connectionOpened) {
        return 0;
    }

    /* Initiate the connection. */
    boost::shared_ptr<apache::thrift::transport::THttpClient> auth_http( new apache::thrift::transport::THttpClient(evernoteUrl, port, parameterThree));
    auth_http->open();
    boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> userStoreProt( new apache::thrift::protocol::TBinaryProtocol(auth_http));
    evernote::edam::UserStoreClient userStore(userStoreProt, userStoreProt);

    userStore.getNoteStoreUrl (notestoreUrl, authToken);
    auth_http->close();   

    mkdir (attachmentFolder.c_str (),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    connectionOpened = true;
    return 0;
}

int evernote::EvernoteDataProvider::close () {
    connectionOpened = false;        
    return 0;
}

int evernote::EvernoteDataProvider::sync () {
/*
    1. get Last Update Count from db. If it does not exist. Initialize it to 0.
    2. Get AuthToken. For now, we have the sandbox evernote token.
    // INCOMING
    3. Check whether to full sync. i.e. First run or not.
    // FULL SYNC
    4. getSyncChunk for a 100 notes.
    5. For All Notebooks
        ProcessNotebook ()
    6. For All Notes
        ProcessNote ()
    7. lastUpdateCount = UpdateCount From Server
    // INCREMENTAL SYNC
    8. getSyncChunk but afterUSN = lastUpdateCount.
    9. For All Notebooks
        ProcessNotebook ()
    10. For All Notes 
        ProcessNotes ()
    11. Process Expunged Items.
    // SEND CHANGES
    12. For resources with dirty tags.
        If USN == 0
        createResource
        else 
        updateResource
    13. END
*/ 

    if (!connectionOpened) {
        open ();
        if (!connectionOpened) {
            /* 
             * Return with an error 
             */
        }
    }

    std::cout << notestoreUrl << std::endl;

    notestoreUrl = "/shard/s1/notestore";

    boost::shared_ptr<apache::thrift::transport::TSSLSocketFactory> sslSocketFactory = boost::shared_ptr<apache::thrift::transport::TSSLSocketFactory>(new apache::thrift::transport::TSSLSocketFactory());;

    boost::shared_ptr<apache::thrift::transport::TSocket> sslSocket = sslSocketFactory->createSocket(evernoteUrl, sslPort);
    boost::shared_ptr<apache::thrift::transport::TBufferedTransport> bufferedTransport(new apache::thrift::transport::TBufferedTransport(sslSocket));
    boost::shared_ptr<apache::thrift::transport::THttpClient> userStoreHttpClient = boost::shared_ptr<apache::thrift::transport::THttpClient>(new apache::thrift::transport::THttpClient(bufferedTransport, evernoteUrl, notestoreUrl));

    userStoreHttpClient->open();

    boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> noteStoreProt(new apache::thrift::protocol::TBinaryProtocol(userStoreHttpClient) );
    evernote::edam::NoteStoreClient noteStore(noteStoreProt, noteStoreProt);

    std::vector<evernote::edam::Notebook> notebooks;
    std::vector<evernote::edam::Note> notes;
    noteStore.listNotebooks(notebooks, authToken);

    evernote::edam::NotesMetadataList notesMetadataList;


    for (unsigned int i = 0; i < notebooks.size (); i++) {
      std::cout << notebooks[i].guid << ":" << notebooks[i].stack << std::endl;
      evernote::Notebook n(notebooks[i].name, notebooks[i].guid, notebooks[i].defaultNotebook, notebooks[i].serviceCreated, notebooks[i].serviceUpdated);
      gNotebooks.push_back (n);
    }

    evernote::edam::NoteFilter noteFilter;
    evernote::edam::NotesMetadataResultSpec nmrs;
    nmrs.includeTitle = true;
    nmrs.__isset.includeTitle = true;
    nmrs.includeCreated = true;
    nmrs.__isset.includeCreated = true;
    nmrs.includeUpdated = true;
    nmrs.__isset.includeUpdated = true;
    nmrs.includeNotebookGuid = true;
    nmrs.__isset.includeNotebookGuid = true;

    noteStore.findNotesMetadata (notesMetadataList, authToken, noteFilter, 0, 20, nmrs);

    for (unsigned int i = 0; i < notesMetadataList.notes.size (); i++) {
        evernote::edam::NoteMetadata noteMetadata = notesMetadataList.notes[i];
        std::string content;
        noteStore.getNoteContent (content, authToken, noteMetadata.guid);
        
        evernote::edam::Note evernoteNote;
        noteStore.getNote (evernoteNote, authToken, noteMetadata.guid, false, true, false, false);
        std::vector<evernote::edam::Resource> resourcesList = evernoteNote.resources;
        std::cout << "Resources #" << resourcesList.size () << std::endl;
        for (unsigned int j = 0; j < resourcesList.size (); j++) {
            std::cout << "ResourceMime:" << resourcesList[j].attributes.fileName << std::endl;
              std::ofstream myfile;
              mkdir ((attachmentFolder + noteMetadata.guid).c_str (),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
              std::string name;
              if(resourcesList[j].attributes.fileName.empty ())
                name = attachmentFolder + noteMetadata.guid + "/" + resourcesList[j].guid;
              else
                name = attachmentFolder + noteMetadata.guid + "/" + resourcesList[j].attributes.fileName;

              myfile.open (name.c_str ());
              myfile << resourcesList[j].data.body;
              myfile.close();
        }

        evernote::Note n(noteMetadata.title, noteMetadata.guid, content, noteMetadata.notebookGuid, noteMetadata.created, noteMetadata.updated);
        gNotes.push_back (n);
    }

    userStoreHttpClient->flush ();
    userStoreHttpClient->close();
    userStoreHttpClient->close();

    std::cout << notebooks.size () << notesMetadataList.notes.size () << std::endl;
    return 0;
}

/*
 * Sample program that gets notes from Evernote's dev server account.
 */
int main () {
    evernote::EvernoteDataProvider* edp = new evernote::EvernoteDataProvider ();
    edp->open ();
    edp->sync ();
    return 0;
}