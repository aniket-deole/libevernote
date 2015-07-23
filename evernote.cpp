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

std::set<std::string> evernote::NoteStore::enmlProhibitedTags;
std::set<std::string> evernote::NoteStore::enmlProhibitedAttributes;


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


	    enmlProhibitedTags.insert ("applet");
	    enmlProhibitedTags.insert ("base");
	    enmlProhibitedTags.insert ("basefont");
	    enmlProhibitedTags.insert ("bgsound");
	    enmlProhibitedTags.insert ("blink");
	    enmlProhibitedTags.insert ("button");
	    enmlProhibitedTags.insert ("dir");
	    enmlProhibitedTags.insert ("embedlet");
	    enmlProhibitedTags.insert ("fieldset");
	    enmlProhibitedTags.insert ("form");
	    enmlProhibitedTags.insert ("frame");
	    enmlProhibitedTags.insert ("frameset");
	    enmlProhibitedTags.insert ("head");
	    enmlProhibitedTags.insert ("iframe");
	    enmlProhibitedTags.insert ("ilayer");
	    enmlProhibitedTags.insert ("input");
	    enmlProhibitedTags.insert ("isindex");
	    enmlProhibitedTags.insert ("label");
	    enmlProhibitedTags.insert ("layer");
	    enmlProhibitedTags.insert ("legend");
	    enmlProhibitedTags.insert ("link");
	    enmlProhibitedTags.insert ("marquee");
	    enmlProhibitedTags.insert ("menu");
	    enmlProhibitedTags.insert ("meta");
	    enmlProhibitedTags.insert ("noframes");
	    enmlProhibitedTags.insert ("noscript");
	    enmlProhibitedTags.insert ("object");
	    enmlProhibitedTags.insert ("optgroup");
	    enmlProhibitedTags.insert ("option");
	    enmlProhibitedTags.insert ("param");
	    enmlProhibitedTags.insert ("plaintext");
	    enmlProhibitedTags.insert ("script");
	    enmlProhibitedTags.insert ("select");
	    enmlProhibitedTags.insert ("style");
	    enmlProhibitedTags.insert ("textarea");
	    enmlProhibitedTags.insert ("xml");

	    enmlProhibitedAttributes.insert ("id");
	    enmlProhibitedAttributes.insert ("class");
	    enmlProhibitedAttributes.insert ("ondblclick");
	    enmlProhibitedAttributes.insert ("accesskey");
	    enmlProhibitedAttributes.insert ("data");
	    enmlProhibitedAttributes.insert ("dynsrc");
	    enmlProhibitedAttributes.insert ("tabindex");

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

evernote::Note::Note () {

}

evernote::Note::Note (evernote::edam::Note* evernoteNote) {
    guid = new GUID(evernoteNote->guid);
    title = evernoteNote->title;
    content = evernoteNote->content;
    contentEnml = evernoteNote->content;
    contentHash = evernoteNote->contentHash;
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

evernote::Note* evernote::NoteStore::createNote (std::string authenticationToken, Note* note) {
    evernote::edam::Note* reference_note = new evernote::edam::Note ();
    reference_note->title = note->title;
    reference_note->content = note->content;

    reference_note->__isset.title = true;
    reference_note->__isset.content = true;
    reference_note->__isset.contentHash = false;
    reference_note->__isset.contentLength = true;
    reference_note->__isset.resources = false;
    reference_note->__isset.attributes = false;
    reference_note->__isset.notebookGuid = false;
    
    reference_note->contentLength = note->content.size();
    evernote::edam::Note* returnNote = new evernote::edam::Note ();
    noteStore->createNote (*returnNote, authenticationToken, *reference_note);
    return new evernote::Note (returnNote);
}


void evernote::Note::convertNodesFromEnmlToHtml (rapidxml::xml_node<>* root) {
    char* rootNodeName = root->name ();
    if (!strcmp (rootNodeName, "en-media")) {
        char* hashValue;
        for (rapidxml::xml_attribute<> *attr = root->first_attribute(); attr; 
            attr = attr->next_attribute ()) {
            char* attrName= attr->name ();
            if (!strcmp (attrName, "hash")) {
                hashValue = attr->value ();
            }
            if (!strcmp (attrName, "type")) {
                if (!strncmp (attr->value(), "image", 5)) {
                    root->name (docP->allocate_string("img"));
                    rapidxml::xml_attribute<> *attr = docP->allocate_attribute("src", hashValue);
                    root->append_attribute(attr);
                    std::cout << root->name () << std::endl;
                }
            }
        }
    }

    for (rapidxml::xml_node<> *child = root->first_node (); child;
            child = child->next_sibling ()) {
        char* nodeName = child->name ();
        convertNodesFromEnmlToHtml (child);
    }
}

void evernote::Note::convertNodesFromHtmlToEnml (rapidxml::xml_node<>* root) {
    for (rapidxml::xml_attribute<> *attr = root->first_attribute(); attr; 
            attr = attr->next_attribute ()) {
        char* attrName= attr->name ();  
        if (evernote::NoteStore::enmlProhibitedAttributes.count (attrName)) {
            root->remove_attribute (attr);
        }
    }
    char* rootNodeName = root->name ();

    if (!strcmp (rootNodeName, "body")) {
        root->name (docP->allocate_string ("en-note"));
    }

    /**
     * Convert img tags to en-media 
     */

    for (rapidxml::xml_node<> *child = root->first_node (); child;
            child = child->next_sibling ()) {
        char* nodeName = child->name ();
        if (evernote::NoteStore::enmlProhibitedTags.count (nodeName)) {
            root->remove_node (child);
        } else {
            convertNodesFromEnmlToHtml (child);
        }
    }
}

void evernote::Note::enmlToHtml () {
    rapidxml::xml_document<> doc;
    char *cstr = new char[contentEnml.length() + 1];
    strcpy(cstr, contentEnml.c_str());
    doc.parse<0> (cstr);
    char *rootNode = doc.allocate_string("body");        // Allocate string and copy name into it
    doc.first_node ()->name (rootNode);
    docP = &doc;
    convertNodesFromEnmlToHtml (doc.first_node ());
    rapidxml::print(std::back_inserter(contentHtml), doc);
    delete cstr;
}

void evernote::Note::htmlToEnml () {
    rapidxml::xml_document<> doc;
    char *cstr = new char[contentHtml.length() + 1];
    strcpy (cstr, contentHtml.c_str ());
    doc.parse<0> (cstr);
    docP = &doc;
    convertNodesFromHtmlToEnml (doc.first_node ());
    rapidxml::print(std::back_inserter (contentEnml), doc);
}

evernote::OAuthManager::OAuthManager (std::string ck, std::string cs, std::string rtu,
    std::string rtqa, std::string au, std::string atu) {
  consumerKey = ck;
  consumerSecret = cs;
  requestTokenUrl = rtu;
  requestTokenQueryArgs = rtqa;
  authorizeUrl = au;
  accessTokenUrl = atu;
}

extern "C" evernote::OAuthManager* createOAuthManager (std::string ck, std::string cs,
    std::string rtu, std::string rtqa, std::string au, std::string atu) {
    return new evernote::OAuthManager (ck, cs, rtu, rtqa, au, atu);
}

std::string evernote::OAuthManager::generateRequestTokenUrl () {
  consumer = new OAuth::Consumer (consumerKey, consumerSecret);
  client = new OAuth::Client (consumer);

  std::string baseRequestTokenUrl = requestTokenUrl + (requestTokenQueryArgs.empty () ? 
      std::string ("") : (std::string ("?") + requestTokenQueryArgs));

  return requestTokenUrl + "?" + client->getURLQueryString (OAuth::Http::Get, baseRequestTokenUrl);
}

extern "C" std::string OAuthManager_generateRequestTokenUrl (evernote::OAuthManager* a) {
  return a->generateRequestTokenUrl ();
}

std::string evernote::OAuthManager::generateAuthorizationUrl (std::string rtr) {
  requestToken = new OAuth::Token (OAuth::Token::extract (rtr));
  return authorizeUrl + "?oauth_token=" + requestToken->key ();
}

extern "C" std::string OAuthManager_generateAuthorizationUrl (evernote::OAuthManager* a, 
    std::string rtr) {
  return a->generateAuthorizationUrl (rtr);
}

std::string evernote::OAuthManager::generateFinalAccessTokenUrl (std::string av) {
  accessVerifier = av;
  requestToken->setPin (accessVerifier);

  client = new OAuth::Client (consumer, requestToken);
  return accessTokenUrl + "?" + client->getURLQueryString (OAuth::Http::Get, accessTokenUrl, std::string (""), true);
}

extern "C" std::string OAuthManager_generateFinalAccessTokenUrl (evernote::OAuthManager* a,
    std::string av) {
  return a->generateFinalAccessTokenUrl (av);
}

std::string evernote::OAuthManager::generateAccessToken (std::string ats) {
  OAuth::KeyValuePairs accessTokenResponseData = OAuth::ParseKeyValuePairs (ats);
  OAuth::Token accessTokenToken = OAuth::Token::extract (accessTokenResponseData);
  accessToken = accessTokenToken.key ();
  return accessToken;
}

extern "C" std::string OAuthManager_generateAccessToken (evernote::OAuthManager* a,
    std::string ats) {
  return a->generateAccessToken (ats);
}

std::string evernote::OAuthManager::getAccessToken () {
  return accessToken;
}

extern "C" std::string OAuthManager_getAccessToken (evernote::OAuthManager* a) {
  return a->getAccessToken ();
}

// the class factories

extern "C" evernote::UserStore* createUserStore(std::string a, int b, std::string c, std::string d) {
    return new evernote::UserStore (a, b, c, d);
}

extern "C" evernote::NoteStore* createNoteStore (std::string a) {
    return new evernote::NoteStore (a);
}
extern "C" void destroy(void* p) {
    // Commenting this right now.
    // I think we may need separete delete for each data type
    // as deleting a void pointer is not defined.
    // delete p;
    (void) p;
}

extern "C" std::string UserStore_getNoteStoreUrl (evernote::UserStore* u, std::string a) {
    return u->getNoteStoreUrl (a);
}

extern "C" std::vector<evernote::Notebook*>* NoteStore_listNotebooks (evernote::NoteStore* n, std::string a) {
    return n->listNotebooks (a);
}

extern "C" evernote::NotesMetadataList* NoteStore_findNotesMetadata (evernote::NoteStore* n, std::string a,
        evernote::NoteFilter* filter, int o, int m, evernote::NotesMetadataResultSpec* r) {
    return n->findNotesMetadata (a, filter, o, m, r);
}

extern "C" evernote::Note* NoteStore_getNote (evernote::NoteStore* n, std::string a,
    evernote::GUID* guid, bool b, bool c, bool d, bool e) {
    return n->getNote (a, guid, b,c,d,e);
}
extern "C" evernote::NoteFilter* NoteStore_createNoteFilter () {
    return new evernote::NoteFilter ();
}
extern "C" evernote::NotesMetadataResultSpec* NoteStore_createNotesMetadataResultSpec () {
    return new evernote::NotesMetadataResultSpec ();
}

extern "C" evernote::Note* NoteStore_createNote () {
    return new evernote::Note ();
}

extern "C" evernote::Note* NoteStore_createNote2 (evernote::NoteStore* n, std::string a, evernote::Note* b) {
    return n->createNote (a, b);
}

extern "C" void Note_enmlToHtml (evernote::Note* n) {
	return n->enmlToHtml ();
}

evernote::SyncState::SyncState (evernote::edam::SyncState* ss) {
    updateCount = ss->updateCount;
}

evernote::SyncState* evernote::NoteStore::getSyncState (std::string authToken) {
  try {        

      evernote::edam::SyncState* returnSyncState = new evernote::edam::SyncState ();
      noteStore->getSyncState (*returnSyncState, authToken);

      return new evernote::SyncState (returnSyncState);
  } catch (apache::thrift::transport::TTransportException e) {
      std::cerr << "Exception in evernote::SyncState* evernote::NoteStore::getSyncState (std::string authToken)" 
            << std::endl;
      return NULL;
  }
}

extern "C" evernote::SyncState* NoteStore_getSyncState (evernote::NoteStore* n, std::string authToken) {
    return n->getSyncState (authToken);
}

evernote::SyncChunk::SyncChunk (evernote::edam::SyncChunk* sc) {
		currentTime = new Timestamp (sc->currentTime);
		chunkHighUSN = sc->chunkHighUSN;
		updateCount = sc->updateCount;
    std::cout << "Received Notes:" << sc->notes.size () << std::endl;
    try {            
        for (unsigned int i = 0; i < sc->notebooks.size (); i++) {
            evernote::Notebook* n = new evernote::Notebook (sc->notebooks[i]);
            notebooks.push_back (n);
        }
    } catch (apache::thrift::transport::TTransportException e) {
        std::cerr << "Exception in evernote::NoteStore::listNotebooks (std::string authenticationToken)" << std::endl;
    }

    try {            
        for (unsigned int i = 0; i < sc->notes.size (); i++) {
            evernote::Note* n = new evernote::Note (&(sc->notes[i]));
            notes.push_back (n);
        }
    } catch (apache::thrift::transport::TTransportException e) {
        std::cerr << "Exception in evernote::NoteStore::listNotebooks (std::string authenticationToken)" << std::endl;
    }

    try {            
        for (unsigned int i = 0; i < sc->resources.size (); i++) {
            evernote::Resource* n = new evernote::Resource (&(sc->resources[i]));
            resources.push_back (n);
        }
    } catch (apache::thrift::transport::TTransportException e) {
        std::cerr << "Exception in evernote::NoteStore::listNotebooks (std::string authenticationToken)" << std::endl;
    }

}

evernote::edam::SyncChunkFilter* evernote::SyncChunkFilter::getEdamObject () {
  evernote::edam::SyncChunkFilter* escf = new evernote::edam::SyncChunkFilter ();
  
  escf->includeNotes = includeNotes;
  escf->includeNoteResources = includeNoteResources;
  escf->includeNoteAttributes = includeNoteAttributes;
  escf->includeNotebooks = includeNotebooks;
  escf->includeTags = includeTags;
  escf->includeSearches = includeSearches;
  escf->includeResources = includeResources;
  escf->includeLinkedNotebooks = includeLinkedNotebooks;
  escf->includeExpunged = includeExpunged;
  escf->includeNoteApplicationDataFullMap = includeNoteApplicationDataFullMap;
  escf->includeResourceApplicationDataFullMap = includeResourceApplicationDataFullMap;
  escf->includeNoteResourceApplicationDataFullMap = includeNoteResourceApplicationDataFullMap;
    
  escf->__isset.includeNotes = includeNotes;
  escf->__isset.includeNoteResources = includeNoteResources;
  escf->__isset.includeNoteAttributes = includeNoteAttributes;
  escf->__isset.includeNotebooks = includeNotebooks;
  escf->__isset.includeTags = includeTags;
  escf->__isset.includeSearches = includeSearches;
  escf->__isset.includeResources = includeResources;
  escf->__isset.includeLinkedNotebooks = includeLinkedNotebooks;
  escf->__isset.includeExpunged = includeExpunged;
  escf->__isset.includeNoteApplicationDataFullMap = includeNoteApplicationDataFullMap;
  escf->__isset.includeResourceApplicationDataFullMap = includeResourceApplicationDataFullMap;
  escf->__isset.includeNoteResourceApplicationDataFullMap = includeNoteResourceApplicationDataFullMap;
 
  return escf;
}

evernote::SyncChunk* evernote::NoteStore::getFilteredSyncChunk (std::string authToken, 
    int afterUSN,  int maxEntries, SyncChunkFilter* filter){

  try {        
      evernote::edam::SyncChunk* returnSyncChunk = new evernote::edam::SyncChunk ();

      evernote::edam::SyncChunkFilter* edamSyncChunkFilter = filter->getEdamObject ();

      noteStore->getFilteredSyncChunk (*returnSyncChunk, authToken, 
          afterUSN, maxEntries, *edamSyncChunkFilter);

      return new evernote::SyncChunk (returnSyncChunk);
  } catch (apache::thrift::transport::TTransportException e) {
      std::cerr << "Exception in evernote::SyncChunk* evernote::NoteStore::getSyncChunk (std::string authToken" 
            << ", int afterUSN, int maxEntries, SyncChunkFilter* filter)"
            << std::endl;
      return NULL;
  }
}

extern "C" evernote::SyncChunk* NoteStore_getFilteredSyncChunk (evernote::NoteStore* n, std::string authToken,
    int afterUSN, int maxEntries, evernote::SyncChunkFilter* filter) {
    return n->getFilteredSyncChunk (authToken, afterUSN, maxEntries, filter);
}


