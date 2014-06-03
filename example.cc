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
#include "evernote.hh"

/*
 * Sample program that gets notes from Evernote's dev server account.
 */
int main () {
    /*
     * AuthToken would be received after oauth. 
     * Right now we are using the one provided by evernote for accessing 
     * sandbox.evernote.com
     */
    std::string authToken = "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0";
    
    evernote::UserStore* userStore = new evernote::UserStore ("sandbox.evernote.com", 80, "/edam/user", authToken);
    evernote::NoteStore* noteStore = new evernote::NoteStore (userStore->getNoteStoreUrl (authToken));

    std::cout << "*. List all notebooks\n";
    std::cout << "----------------------------------------------------------------\n";
    std::vector<evernote::Notebook*>* notebookList = noteStore->listNotebooks (authToken);

    for (int i = 0; i < notebookList->size (); i++) {
        std::cout << notebookList->at (i)->stack << ":" << notebookList->at (i)->name << std::endl;
    }

    std::cout << "----------------------------------------------------------------\n";
    std::cout << "----------------------------------------------------------------\n";

    std::cout << "*. List all notes titles.\n";
    std::cout << "----------------------------------------------------------------\n";
    evernote::NoteFilter* nf = new evernote::NoteFilter ();
    evernote::NotesMetadataResultSpec* nmrs = new evernote::NotesMetadataResultSpec ();
    nmrs->includeTitle = true;
    evernote::NotesMetadataList* nml = noteStore->findNotesMetadata (authToken, nf, 0, 20, nmrs);
    std::cout << nml->totalNotes << std::endl;
    for (int i = 0; i < nml->notes.size (); i++) {
        std::cout << nml->notes[i]->title << std::endl;
        // std::cout << noteStore->getNoteContent (authToken, nml->notes[i]->guid);
        evernote::Note* note = noteStore->getNote (authToken, nml->notes[i]->guid, false, true, false, false);
        for (int j = 0; j < note->resources.size (); j++) {
            std::cout << note->resources[j]->data->body << std::endl;
        }
    }

    std::cout << "----------------------------------------------------------------\n";
    std::cout << "----------------------------------------------------------------\n";

    std::cout << "----------------------------------------------------------------\n";
    std::cout << "----------------------------------------------------------------\n";

    std::cout << "*. Find Note.\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "*. Create Note.\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "*. Create Notebook.\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "*. Exit" << std::endl;
    std::cout.flush ();

    delete noteStore;
    delete userStore;

    return 0;
}