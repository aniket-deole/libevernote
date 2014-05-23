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

#ifndef _EVERNOTEDATAPROVIDER_H_
#define _EVERNOTEDATAPROVIDER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <set>

#include "rapidxml.hpp"

namespace evernote {

static std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

template <typename T>
std::string NumberToString(T pNumber)
{
 std::ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}

/**
 * Classes 
 */

class Data {
public:
    std::string bodyHash;
    int size;
    std::string body;
};

class Resource {
public:
    std::string guid;
    std::string noteGuid;
    Data data;
    std::string mime;
    int width;
    int height;
    int duration;
    bool active;
    int updateSequenceNum;
};

class NotesMetadata {
public:
    std::string guid;
};

class Note {
public:
    std::string title;
    std::string guid;
    std::string contentEnml;
    std::string contentHtml;
    std::string notebook_guid;
    long int created;
    long int updated;
    bool deleted;

    enum NoteType {HTML, ENML};
    NoteType type;

    Note (std::string t, std::string g, std::string c, std::string n_g, long int c_time, long int u_time, NoteType nt, bool d = false) {
        title = t; guid = g; contentEnml = c; notebook_guid = n_g;
        created = c_time / 1000; updated = u_time / 1000; deleted = d;
        type = nt;
        if (type == HTML) {
            htmlToEnml ();
        }else {
            enmlToHtml ();
        }
    }
    
    rapidxml::xml_document<>* docP;
    void convertNodesFromEnmlToHtml (rapidxml::xml_node<>* );
    void convertNodesFromHtmlToEnml (rapidxml::xml_node<>* );

    void enmlToHtml ();
    void htmlToEnml ();

    void fetchResources ();
};

class Notebook {
public:
    std::string name;
    std::string guid;
    bool isDefaultNotebook;
    long int created;
    long int updated;
    Notebook (std::string n,std::string g,bool d ,long int c, long int u) {
        name = n; guid = g; isDefaultNotebook = d; created = c / 1000; updated = u / 1000;
    }
};



class EvernoteDataProvider{
private:
	std::string authToken;
	int notebookCount;
    int lastUpdateCount;
    bool connectionOpened;

    std::string notestoreUrl;

    static const std::string evernoteUrl;
    static const int port;
    static const std::string parameterThree;
    static const int sslPort;

    static const std::string attachmentFolder;


public:

    static std::set<std::string> enmlProhibitedTags;
    static std::set<std::string> enmlProhibitedAttributes;

	EvernoteDataProvider ();
	~EvernoteDataProvider ();

    int open ();
	int close ();
	int sync ();

    /**
     * Stubs
     */
    std::vector<Notebook> getNotebooks ();
    std::vector<NotesMetadata> getNotesMetadataList ();
    Note getNoteContent ();

    /*
     * Getters and Setters
     */
	void setAuthToken (std::string at) {
		authToken = at;
	}

	std::string getAuthToken () {
		return authToken;
	}

};

}
#endif