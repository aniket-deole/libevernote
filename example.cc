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
#include "evernote.hh"

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
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
      std::cout << "Please Install libevernote.so along with its dependencies.\n Error: " << dlsym_error << std::endl;
      return 1;
    }
    dlerror ();

    /*
     * AuthToken would be received after oauth. 
     * Right now we are using the one provided by evernote for accessing 
     * sandbox.evernote.com
     */
    std::string authToken = "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0";
    // load the symbols.
    createUserStore_t* create_triangle = (createUserStore_t*) dlsym(handle, "createUserStore");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }
    evernote::UserStore* userStore = create_triangle ("sandbox.evernote.com", 80, "/edam/user", authToken);

    UserStore_getNoteStoreUrl_t* getNoteStoreUrlP = (UserStore_getNoteStoreUrl_t*) dlsym (handle,"UserStore_getNoteStoreUrl");

    std:: cout << getNoteStoreUrlP (userStore, authToken) << std::endl;
/*
    evernote::UserStore* userStore = new evernote::UserStore ("sandbox.evernote.com", 80, "/edam/user", authToken);
    evernote::NoteStore* noteStore = new evernote::NoteStore (userStore->getNoteStoreUrl (authToken));

    // std::cout << "*. List all notebooks\n";
    // std::cout << "----------------------------------------------------------------\n";
    std::vector<evernote::Notebook*>* notebookList = noteStore->listNotebooks (authToken);

    for (int i = 0; i < notebookList->size (); i++) {
        // std::cout << notebookList->at (i)->stack << ":" << notebookList->at (i)->name << std::endl;
    }

    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "----------------------------------------------------------------\n";

    // std::cout << "*. List all notes titles.\n";
    // std::cout << "----------------------------------------------------------------\n";
    evernote::NoteFilter* nf = new evernote::NoteFilter ();
    evernote::NotesMetadataResultSpec* nmrs = new evernote::NotesMetadataResultSpec ();
    nmrs->includeTitle = true;
    evernote::NotesMetadataList* nml = noteStore->findNotesMetadata (authToken, nf, 0, 20, nmrs);
    // std::cout << nml->totalNotes << std::endl;
    for (int i = 0; i < nml->notes.size (); i++) {
        // std::cout << nml->notes[i]->title << std::endl;
        // // std::cout << noteStore->getNoteContent (authToken, nml->notes[i]->guid);
        evernote::Note* note = noteStore->getNote (authToken, nml->notes[i]->guid, false, true, false, false);
        for (int j = 0; j < note->resources.size (); j++) {
//            std::ofstream myfile;
//                  myfile.open ("asd");
//              myfile << note->resources[j]->data->body;
//              myfile.close();
  std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(note->resources[j]->data->body.c_str ()), note->resources[j]->data->size);

  std::cout << "encoded: " << encoded << std::endl;
            return 1;
        }
    }

    // std::cout << "*. Find Note.\n";
    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "*. Create Note.\n";
    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "*. Create Notebook.\n";
    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "*. Exit" << std::endl;
    // std::cout.flush ();

    delete noteStore;
    delete userStore;
*/
    dlclose (handle);
    return 0;
}