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

evernote::NoteFilter::NoteFilter () : order(0), ascending(0), words(""), timeZone(""), inactive(0), emphasized("") {
    notebookGuid = new GUID ("");
}

evernote::edam::NoteFilter* evernote::NoteFilter::getEdamObject () {
    evernote::edam::NoteFilter* eNoteFilter = new evernote::edam::NoteFilter ();
    return eNoteFilter;
}

        bool includeTitle;
        bool includeContentLength;
        bool includeCreated;
        bool includeUpdated;
        bool includeDeleted;
        bool includeUpdateSequenceNum;
        bool includeNotebookGuid;
        bool includeTagGuids;
        bool includeAttributes;
        bool includeLargestResourceMime;
        bool includeLargestResourceSize;


evernote::edam::NotesMetadataResultSpec* evernote::NotesMetadataResultSpec::getEdamObject () {
    evernote::edam::NotesMetadataResultSpec* enmrs = new evernote::edam::NotesMetadataResultSpec ();
    enmrs->includeTitle = includeTitle;
    enmrs->includeContentLength = includeContentLength;
    enmrs->includeCreated = includeCreated;
    enmrs->includeUpdated = includeUpdated;
    enmrs->includeDeleted = includeDeleted;
    enmrs->includeUpdateSequenceNum = includeUpdateSequenceNum;
    enmrs->includeNotebookGuid = includeNotebookGuid;
    enmrs->includeTagGuids = includeTagGuids;
    enmrs->includeAttributes = includeAttributes;
    enmrs->includeLargestResourceMime = includeLargestResourceMime;
    enmrs->includeLargestResourceSize = includeLargestResourceSize;

    enmrs->__isset.includeTitle = includeTitle;
    enmrs->__isset.includeContentLength = includeContentLength;
    enmrs->__isset.includeCreated = includeCreated;
    enmrs->__isset.includeUpdated = includeUpdated;
    enmrs->__isset.includeDeleted = includeDeleted;
    enmrs->__isset.includeUpdateSequenceNum = includeUpdateSequenceNum;
    enmrs->__isset.includeNotebookGuid = includeNotebookGuid;
    enmrs->__isset.includeTagGuids = includeTagGuids;
    enmrs->__isset.includeAttributes = includeAttributes;
    enmrs->__isset.includeLargestResourceMime = includeLargestResourceMime;
    enmrs->__isset.includeLargestResourceSize = includeLargestResourceSize;
    return enmrs;
}

evernote::NotesMetadataList::NotesMetadataList (evernote::edam::NotesMetadataList* enml) {
    totalNotes = enml->totalNotes;
    startIndex = enml->startIndex;
    stoppedWords = enml->stoppedWords;
    searchedWords = enml->searchedWords;
    updateCount = enml->updateCount;
    notes.reserve (enml->notes.size ());
    for (int i = 0; i < enml->notes.size (); i++) {
        notes.push_back (new NoteMetadata (&(enml->notes[i])));
    }
}

evernote::NoteMetadata::NoteMetadata (const evernote::edam::NoteMetadata* enm) {
    guid = new GUID (enm->guid);
    title = enm->title;
    contentLength = enm->contentLength;
    created = new Timestamp (enm->created);
    updated = new Timestamp (enm->updated);
    deleted = new Timestamp (enm->deleted);
    updateSequenceNum = enm->updateSequenceNum;
    notebookGuid = enm->notebookGuid;
    std::vector<GUID*> tagGuids;
    tagGuids.reserve (enm->tagGuids.size ());
    for (int i = 0; i < enm->tagGuids.size (); i++) {
        tagGuids.push_back (new GUID (enm->tagGuids[i]));
    }
    attributes = new NoteAttributes (&(enm->attributes));
    largestResourceMime = enm->largestResourceMime;
    largestResourceSize = enm->largestResourceSize;
}

evernote::NoteAttributes::NoteAttributes (const evernote::edam::NoteAttributes* ena) {
    Timestamp* subjectDate = new Timestamp (ena->subjectDate);
    double latitude = ena->latitude;
    double longitude = ena->longitude;
    double altitude = ena->altitude;
    std::string author = ena->author;
    std::string source = ena->source;
    std::string sourceURL = ena->sourceURL;
    std::string sourceApplication = ena->sourceApplication;
    Timestamp* shareDate = new Timestamp (ena->shareDate);
    long reminderOrder = ena->reminderOrder;
    Timestamp* reminderDoneTime = new Timestamp (ena->reminderDoneTime);
    Timestamp* reminderTime = new Timestamp (ena->reminderTime);
    std::string placeName = ena->placeName;
    std::string contentClass = ena->contentClass;
    std::string lastEditedBy = ena->lastEditedBy;
/*    
    std::map<std::string, std::string> classifications;
    LazyMap* applicationData = ena->;
    UserID* creatorId;
    UserID* lastEditorId;
*/
}

evernote::NotesMetadataList* evernote::NoteStore::findNotesMetadata (std::string authenticationToken,
            evernote::NoteFilter* filter, int offset, int maxNotes,
            evernote::NotesMetadataResultSpec* resultSpec) {

    evernote::edam::NotesMetadataList notesMetadataList;
    evernote::edam::NoteFilter* eNoteFilter = filter->getEdamObject ();
    evernote::edam::NotesMetadataResultSpec* eNmrs = resultSpec->getEdamObject ();
    noteStore->findNotesMetadata (notesMetadataList, authenticationToken, *eNoteFilter, offset, maxNotes, *eNmrs);

    evernote::NotesMetadataList* nml = new evernote::NotesMetadataList (&notesMetadataList);

    return nml;
}

std::string evernote::NoteStore::getNoteContent (std::string authenticationToken,
            GUID* guid) {
    std::string content;
    noteStore->getNoteContent (content, authenticationToken, guid->guid);
    return content;
}

evernote::Note* evernote::NoteStore::getNote (std::string authenticationToken,
    GUID* guid, bool withContent, bool withResourcesData,
    bool withResourcesRecognition, bool withResourcesAlternateData) {

    evernote::edam::Note evernoteNote;
    noteStore->getNote (evernoteNote, authenticationToken, guid->guid, withContent, 
        withResourcesData, withResourcesRecognition, withResourcesAlternateData);
    evernote::Note* note = new evernote::Note (&evernoteNote);
    return note;
}

evernote::Note::Note (evernote::edam::Note* evernoteNote) {
    guid = new GUID(evernoteNote->guid);
    title = evernoteNote->title;
    std::string content = evernoteNote->content;
    std::string contentHash = evernoteNote->contentHash;
    int contentLength = evernoteNote->contentLength;
    Timestamp* created = new Timestamp (evernoteNote->created);
    Timestamp* updated = new Timestamp (evernoteNote->updated);
    Timestamp* deleted = new Timestamp (evernoteNote->deleted);
    bool active = evernoteNote->active;
    int updateSequenceNum = evernoteNote->updateSequenceNum;
    notebookGuid = evernoteNote->notebookGuid;

    tagGuids.reserve (evernoteNote->tagGuids.size ());
    for (int i = 0; i < evernoteNote->tagGuids.size (); i++) {
        tagGuids.push_back (new GUID (evernoteNote->tagGuids[i]));
    }

    resources.reserve (evernoteNote->resources.size ());
    for (int i = 0; i < evernoteNote->resources.size (); i++) {
        resources.push_back (new evernote::Resource (&(evernoteNote->resources[i])));
    }        
    attributes = new NoteAttributes (&(evernoteNote->attributes));
    
    tagNames.reserve (evernoteNote->tagNames.size ());
    for (int i = 0; i < evernoteNote->tagNames.size (); i++) {
        tagNames.push_back (evernoteNote->tagNames[i]);
    }
}

evernote::Resource::Resource (evernote::edam::Resource* resource) {
    guid = new GUID (resource->guid);
    noteGuid = new GUID (resource->noteGuid);
    data = new Data (&(resource->data));
    mime = resource->mime;
    width = resource->width;
    height = resource->height;
    duration = resource->duration;
    active = resource->active;
    recognition = new Data (&(resource->recognition));
    attributes = new ResourceAttributes (&(resource->attributes));
    updateSequenceNum = resource->updateSequenceNum;
    alternateData = new Data (&(resource->alternateData));
}

evernote::Data::Data (evernote::edam::Data* evernoteData) {
    bodyHash = evernoteData->bodyHash;
    size = evernoteData->size;
    body = evernoteData->body;
}

evernote::ResourceAttributes::ResourceAttributes (evernote::edam::ResourceAttributes* ra) {
    sourceURL = ra->sourceURL;
    timestamp = new Timestamp(ra->timestamp);
    latitude = ra->latitude;
    longitude = ra->longitude;
    altitude = ra->altitude;
    cameraMake = ra->cameraMake;
    cameraModel = ra->cameraModel;
    clientWillIndex = ra->clientWillIndex;
    recoType = ra->recoType;
    fileName = ra->fileName;
    attachment = ra->attachment;
    /**    LazyMap* applicationData; **/
}


// the class factories

extern "C" evernote::UserStore* createUserStore(std::string a, int b, std::string c, std::string d) {
    return new evernote::UserStore (a, b, c, d);
}

extern "C" void destroy(void* p) {
    delete p;
}

extern "C" std::string UserStore_getNoteStoreUrl (evernote::UserStore* u, std::string a) {
    return u->getNoteStoreUrl (a);
}
