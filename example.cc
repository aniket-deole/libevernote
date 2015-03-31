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
#include <fstream>
#include <dlfcn.h>
#include <liboauthcpp/liboauthcpp.h>
#include "evernote.hh"

std::string consumerKey = "analogx";
std::string consumerSecret = "953b9c5d5f64c09b";
std::string request_token_url = "https://sandbox.evernote.com/oauth";
std::string request_token_query_args = "oauth_callback=sandbox.evernote.com";
std::string authorize_url = "https://sandbox.evernote.com/OAuth.action";
std::string access_token_url = "https://sandbox.evernote.com/oauth";

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

static std::string getUserString(std::string prompt) {
    std::cout << prompt << " ";

    std::string res;
    std::cin >> res;
    std::cout << std::endl;
    return res;
}
 

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}


/*
 * Sample program that gets notes from Evernote's dev server account.
 */
int main () {
    /* Check if library is loaded. Else ask the user to install it. */
    void* handle;
    handle = dlopen ("libevernote.so", RTLD_LAZY);
    if (!handle) {
      const char* dlsym_error = dlerror();
      std::cout << "Please Install libevernote.so along with its dependencies.\nDlSym Error: " << dlsym_error << std::endl;
      return 1;
    }
    dlerror ();

    /*
     * AuthToken would be received after oauth. 
     * Right now we are using the one provided by evernote for accessing 
     * sandbox.evernote.com
     */
/*          OAuth::SetLogLevel(OAuth::LogLevelDebug);
 
    OAuth::Consumer consumer (consumerKey, consumerSecret);
    OAuth::Client oauth (&consumer);

//////////////////////////////////////
    std::string base_request_token_url = request_token_url + (request_token_query_args.empty() ? std::string("") : (std::string("?")+request_token_query_args) );
    std::string oAuthQueryString =
        oauth.getURLQueryString( OAuth::Http::Get, base_request_token_url);

    std::cout << "Enter the following in your browser to get the request token: " << std::endl;
    // Note that getting the query string includes the arguments we
    // passed in, so we don't need to include request_token_query_args
    // again.
    std::cout << request_token_url << "?" << oAuthQueryString << std::endl;
    std::cout << std::endl;

    // Extract the token and token_secret from the response
    std::string request_token_resp = getUserString("Enter the response:");
    // This time we pass the response directly and have the library do the
    // parsing (see next extractToken call for alternative)
    OAuth::Token request_token = OAuth::Token::extract( request_token_resp );

    // Get access token and secret from OAuth object
    std::cout << "Request Token:" << std::endl;
    std::cout << "    - oauth_token        = " << request_token.key() << std::endl;
    std::cout << "    - oauth_token_secret = " << request_token.secret() << std::endl;
    std::cout << std::endl;

    // Step 2: Redirect to the provider. Since this is a CLI script we
    // do not redirect. In a web application you would redirect the
    // user to the URL below.
    std::cout << "Go to the following link in your browser to authorize this application on a user's account:" << std::endl;
    std::cout << authorize_url << "?oauth_token=" << request_token.key() << std::endl;

    // After the user has granted access to you, the consumer, the
    // provider will redirect you to whatever URL you have told them
    // to redirect to. You can usually define this in the
    // oauth_callback argument as well.
    std::string pin = getUserString("What is the PIN?");
    request_token.setPin(pin);

    // Step 3: Once the consumer has redirected the user back to the
    // oauth_callback URL you can request the access token the user
    // has approved. You use the request token to sign this
    // request. After this is done you throw away the request token
    // and use the access token returned. You should store the oauth
    // token and token secret somewhere safe, like a database, for
    // future use.
    oauth = OAuth::Client(&consumer, &request_token);
    // Note that we explicitly specify an empty body here (it's a GET) so we can
    // also specify to include the oauth_verifier parameter
    oAuthQueryString = oauth.getURLQueryString( OAuth::Http::Get, access_token_url, std::string( "" ), true );
    std::cout << "Enter the following in your browser to get the final access token & secret: " << std::endl;
    std::cout << access_token_url << "?" << oAuthQueryString;
    std::cout << std::endl;

    // Once they've come back from the browser, extract the token and token_secret from the response
    std::string access_token_resp = getUserString("Enter the response:");
    // On this extractToken, we do the parsing ourselves (via the library) so we
    // can extract additional keys that are sent back, in the case of twitter,
    // the screen_name
    OAuth::KeyValuePairs access_token_resp_data = OAuth::ParseKeyValuePairs(access_token_resp);
    OAuth::Token access_token = OAuth::Token::extract( access_token_resp_data );

    std::cout << "Access token:" << std::endl;
    std::cout << "    - oauth_token        = " << access_token.key() << std::endl;
    std::cout << "    - oauth_token_secret = " << access_token.secret() << std::endl;
    std::cout << std::endl;
    std::cout << "You may now access protected resources using the access tokens above." << std::endl;
    std::cout << std::endl;

    std::pair<OAuth::KeyValuePairs::iterator, OAuth::KeyValuePairs::iterator> screen_name_its = access_token_resp_data.equal_range("screen_name");
    for(OAuth::KeyValuePairs::iterator it = screen_name_its.first; it != screen_name_its.second; it++)
        std::cout << "Also extracted screen name from access token response: " << it->second << std::endl;

    // E.g., to use the access token, you'd create a new OAuth using
    // it, discarding the request_token:
    // oauth = OAuth::Client(&consumer, &access_token);

/////////////////////////////////
*/

    createOAuthManager_t* createOAuthManager_p = (createOAuthManager_t*) dlsym (handle, "createOAuthManager");
    evernote::OAuthManager* oAuthManager = createOAuthManager_p (consumerKey, consumerSecret,
          request_token_url, request_token_query_args, authorize_url, access_token_url);
    
    OAuthManager_generateRequestTokenUrl_t* OAuthManager_generateRequestTokenUrl_p =
      (OAuthManager_generateRequestTokenUrl_t*) dlsym (handle, "OAuthManager_generateRequestTokenUrl");

    std::string rqu = OAuthManager_generateRequestTokenUrl_p (oAuthManager);
    
    std::cout << "Enter this in the browser:\n";
    std::cout << rqu << std::endl;

    std::string rtr = getUserString ("Enter the response:");

    OAuthManager_generateAuthorizationUrl_t* OAuthManager_generateAuthorizationUrl_p =
      (OAuthManager_generateAuthorizationUrl_t*) dlsym (handle, "OAuthManager_generateAuthorizationUrl");
    std::string authUrl = OAuthManager_generateAuthorizationUrl_p (oAuthManager, rtr);

    std::cout << "Enter this in the browser:\n";
    std::cout << authUrl << std::endl;

    std::string ppin = getUserString ("What is the PIN?");

    OAuthManager_generateFinalAccessTokenUrl_t* OAuthManager_generateFinalAccessTokenUrl_p =
      (OAuthManager_generateFinalAccessTokenUrl_t*) dlsym (handle, "OAuthManager_generateFinalAccessTokenUrl");
    std::string fat = OAuthManager_generateFinalAccessTokenUrl_p (oAuthManager, ppin);

    std::cout << "Enter this in browser:" << fat << std::endl;
    std::string ffat = getUserString ("Enter final decoded token:");

    OAuthManager_generateAccessToken_t* OAuthManager_generateAccessToken_p = 
      (OAuthManager_generateAccessToken_t*) dlsym (handle, "OAuthManager_generateAccessToken");

    std::string authToken = OAuthManager_generateAccessToken_p (oAuthManager, ffat);



    // load the symbols.
    createUserStore_t* createUserStore_p = (createUserStore_t*) dlsym(handle, "createUserStore");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }
    evernote::UserStore* userStore = createUserStore_p ("sandbox.evernote.com", 80, "/edam/user", authToken);

    UserStore_getNoteStoreUrl_t* UserStore_getNoteStoreUrl_p = (UserStore_getNoteStoreUrl_t*) dlsym (handle,"UserStore_getNoteStoreUrl");

    createNoteStore_t* createNoteStore_p = (createNoteStore_t*) dlsym (handle, "createNoteStore");
    evernote::NoteStore* noteStore = createNoteStore_p (UserStore_getNoteStoreUrl_p (userStore, authToken));

    // std::cout << "*. List all notebooks\n";
    // std::cout << "----------------------------------------------------------------\n";
    NoteStore_listNotebooks_t* NoteStore_listNotebooks_p = (NoteStore_listNotebooks_t*) dlsym (handle, "NoteStore_listNotebooks");
    std::vector<evernote::Notebook*>* notebookList = NoteStore_listNotebooks_p (noteStore, authToken);

    for (int i = 0; i < notebookList->size (); i++) {
         std::cout << notebookList->at (i)->stack << ":" << notebookList->at (i)->name << std::endl;
    }

    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "----------------------------------------------------------------\n";

    NoteStore_createNoteFilter_t* NoteStore_createNoteFilter_p = (NoteStore_createNoteFilter_t*) dlsym (handle, "NoteStore_createNoteFilter");
    evernote::NoteFilter* nf = NoteStore_createNoteFilter_p ();
    NoteStore_createNotesMetadataResultSpec_t* NoteStore_createNotesMetadataResultSpec_p = 
        (NoteStore_createNotesMetadataResultSpec_t*) dlsym (handle, "NoteStore_createNotesMetadataResultSpec");
    evernote::NotesMetadataResultSpec* nmrs = NoteStore_createNotesMetadataResultSpec_p ();
    nmrs->includeTitle = true;

    NoteStore_findNotesMetadata_t* NoteStore_findNotesMetadata_p = (NoteStore_findNotesMetadata_t*) dlsym (handle, "NoteStore_findNotesMetadata");

    evernote::NotesMetadataList* nml = NoteStore_findNotesMetadata_p (noteStore, authToken, nf, 0, 20, nmrs);
    // std::cout << nml->totalNotes << std::endl;
    for (int i = 0; i < nml->notes.size (); i++) {
        // std::cout << nml->notes[i]->title << std::endl;
        // // std::cout << noteStore->getNoteContent (authToken, nml->notes[i]->guid);
       NoteStore_getNote_t* NoteStore_getNote_p = (NoteStore_getNote_t*) dlsym (handle, "NoteStore_getNote");

       evernote::Note* note = NoteStore_getNote_p (noteStore, authToken, nml->notes[i]->guid, true, true, false, false);
       
       Note_enmlToHtml_t* Note_enmlToHtml_p = (Note_enmlToHtml_t*) dlsym (handle, "Note_enmlToHtml");
       Note_enmlToHtml_p (note);

       std::cout << note->contentEnml << std::endl;
       std::cout << "-----------------------------";
       std::cout << "\n" << note->contentHtml << std::endl;
       std::cout << "-----------------------------\n";
       std::cout << "-----------------------------\n";
        for (int j = 0; j < note->resources.size (); j++) {
//            std::ofstream myfile;
//                  myfile.open ("asd");
//              myfile << note->resources[j]->data->body;
//              myfile.close();
          std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(note->resources[j]->data->body.c_str ()), note->resources[j]->data->size);

         //std::cout << "encoded: " << encoded << std::endl;                
        }
    }

    // Create a Note.
    NoteStore_createNote_t* NoteStore_createNote_p = (NoteStore_createNote_t*) dlsym (handle, "NoteStore_createNote");
    evernote::Note* note = NoteStore_createNote_p ();
    note->title = "First UpSync Note.";
    note->content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE en-note SYSTEM \"http://xml.evernote.com/pub/enml2.dtd\"><en-note> HelloWorld </en-note>";

    NoteStore_createNote2_t* NoteStore_createNote2_p = (NoteStore_createNote2_t*) dlsym (handle, "NoteStore_createNote2");

    NoteStore_createNote2_p (noteStore, authToken, note);

    // std::cout << "*. Find Note.\n";
    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "*. Create Note.\n";
    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "*. Create Notebook.\n";
    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "*. Exit" << std::endl;
    // std::cout.flush ();

    dlclose (handle);
    return 0;
}
