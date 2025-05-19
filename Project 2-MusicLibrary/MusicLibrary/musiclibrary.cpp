/// @author Muhammad Hakim
/// @date CS 251, Spring 2023.
/// @brief Project 2 - Music Library, University of Illinois Chicago
///
/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdlib>
#include <sstream>

using namespace std;


/// @brief trims the leading whitespace from a string
/// @param s the string to trim whitespace off of
/// @return a string with no more leading whitespace
string trimWhiteSpace(const string s)
{
    size_t index = 0;
    while (index < s.size() && isspace(s[index++]));
    return s.substr(index - 1);
}

/// @brief splits a string into two pieces, the first word and the rest
/// @param s the string to split
/// @param first a string reference to store the first word in
/// @param remains a string reference to store everything else from s
void splitFirstWord(const string& s, string& first, string& remains)
{
    // first word or possibly the only word
    first = s.substr(0, s.find(" "));

    // skip all whitespace and get everything after the first word
    if (s.size() != first.size())
    {
        remains = trimWhiteSpace(s.substr(s.find(" ")));
    }
    else
    {
        remains = "";
    }

}

/// @brief converts an entire string to lowercase
/// @param s a reference to the string to convert
void tolower(string& s)
{
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) {return tolower(c); });
}

/// @brief implementation of the menu command for help
//          prints out descriptions of all the commands
void helpCommand()
{
    cout << "Music Library Help" << endl;
    cout << "------------------" << endl;

    cout << "Command Options: clear, exit, export, help, load, search";
    cout << endl << endl;

    // print out all the help descriptions for every command
    ifstream fstr("help-commands.txt");
    if (fstr.is_open())
    {
        cout << fstr.rdbuf();
        cout << endl;
        fstr.close();
    }
}
string to_lower(string s) {
    for (char& c : s) {
        c = tolower(c);
    }
    return s;
}
bool contains_token(string s, string token) {
    string delimiter = " ";
    size_t pos = 0;
    string word;

    while ((pos = s.find(delimiter)) != string::npos) {
        word = s.substr(0, pos);
        if (to_lower(word) == to_lower(token)) {
            return true;
        }
        s.erase(0, pos + delimiter.length());
    }

    if (to_lower(s) == to_lower(token)) {
        return true;
    }

    return false;
}



// Struct to represent a song
struct Song {
    string title;
    string track_number;
};

// Struct to represent an album
struct Album {
    string title;
    set<string> artists;
    vector<Song> songs;
};

// Function to load a data file and add its contents to the music library
void load(const string& filename, map<string, Album>& library) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open music library file - " <<filename<< endl;
        return;
    }

    string line;
    Album current_album;
    bool in_album = false;
    string album_title;
    string artist_name;

    while (getline(file, line)) {
        if (line.empty()) {
            // Skip empty lines
            continue;
        }
        else if (!in_album) {
            // Start of a new album
            album_title = line;
            artist_name.clear();
            current_album.title = album_title;
            current_album.artists.clear();
            current_album.songs.clear();
            in_album = true;
        }
        else if (line == album_title) {
            // End of current album
            if (!artist_name.empty()) {
                current_album.artists.insert(artist_name);
            }
            library[album_title] = current_album;
            in_album = false;
        }
        else if (isdigit(line[0])) {
            // Song information for the current album
            size_t delimiter_pos = line.find(" ");
            Song song;
            song.track_number = line.substr(0, delimiter_pos);
            song.title = line.substr(delimiter_pos + 1);
            current_album.songs.push_back(song);
        }
        else {
            // Artist name for the current album
            artist_name = line;
            current_album.artists.insert(artist_name);
        }
    }

}

// Function to print out stats regarding the music library
void stats(map<string, Album>& library, string remains) {
    // Create a set to hold unique artists across all albums
    set<string> tempArtists;
    // Keep track of the total number of songs in the library
    int numberSongs = 0;

    // Print overall library stats
    cout << "Overall Music Library Stats" << endl;
    cout << "===========================" << endl;
    cout << "Total Records: " << library.size() << endl;

    // Iterate over each album in the library
    for (const auto& i : library) {
        // Add each artist from the album to the set of unique artists
        for (const auto& artist : i.second.artists) {
            tempArtists.insert(artist);
        }
        // Add the number of songs in the album to the total number of songs in the library
        numberSongs += i.second.songs.size();
    }
    // Print additional library stats
    cout << "Total Unique Artists: " << tempArtists.size() << endl;
    cout << "Total Songs: " << numberSongs << endl;
    cout << endl;

    // If the remains string is "-d", print detailed information about the library contents
    if (remains == "-d") {
        cout << "Your Current Music Library Includes" << endl;
        cout << "===================================" << endl;

        // Iterate over each album in the library
        for (const auto& i : library) {
            // Print the album name and each artist associated with the album
            cout << i.first << endl;
            for (const auto& artist : i.second.artists) {
                cout << " " << artist << endl;
            }

            // Print each song in the album with its track number and title
            for (const auto& song : i.second.songs) {
                cout << "   " << song.track_number << " " << song.title << endl;
            }
        }
        cout << endl;
    }
}

//Function to write music library data to an external provided file
void export1(string& filename, const map<string, Album>& library) {
    // If no filename is provided, set it to a default value
    if (filename == "") {
        filename = "musicdatabase.txt";
    }

    // Open the output file for writing
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        // If the file cannot be opened, print an error message and return
        cout << "Error: could not open file " << filename << " for writing" << endl;
        return;
    }

    // Loop through each album in the library
    for (const auto& i : library) {
        // Write the album name to the file
        outfile << i.first << endl;

        // Write each artist associated with the album to the file
        for (const auto& artist : i.second.artists) {
            outfile << artist << endl;
        }

        // Write each song associated with the album to the file
        for (const auto& song : i.second.songs) {
            outfile << song.track_number << " " << song.title << endl;
        }

        // Write the album name again to the file (as a delimiter)
        outfile << i.first << endl;
    }

    // Close the output file
    outfile.close();
    return;
}


//Search for a set of album, artist, or song in the library.Additional modifiiers for greater search control
void search(const string& query, const map<string, Album>& library) {
    vector<string> tokens;
    istringstream iss(query);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    if (query == "") {
        cout << "Error: Search terms cannot be empty." << endl;
        cout << "No results found." << endl;
        cout << endl;
        return;
    }
    string field = tokens[0];
    set<string> result_set;
    bool intersection_mode = false;
    bool difference_mode = false;
    for (size_t i = 1; i < tokens.size(); i++) {
        string term = tokens[i];
        bool term_mode = true;
        if (term[0] == '+') {
            intersection_mode = true;
            term_mode = true;
            term = term.substr(1);
        }
        else if (term[0] == '-') {
            difference_mode = true;
            term_mode = false;
            term = term.substr(1);
        }
        term = to_lower(term);
        if (field == "album") {
            for (auto it = library.begin(); it != library.end(); ++it) {
                if (contains_token(to_lower(it->second.title), term) == term_mode) {
                    result_set.insert(it->first);
                }
            }
        }
        else if (field == "artist") {
            for (auto it = library.begin(); it != library.end(); ++it) {
                for (const auto& artist : it->second.artists) {
                    if (contains_token(to_lower(artist), term) == term_mode) {
                        result_set.insert(it->first);
                        break;
                    }
                }
            }
        }
        else if (field == "song") {
            for (auto it = library.begin(); it != library.end(); ++it) {
                for (const auto& song : it->second.songs) {
                    if (contains_token(to_lower(song.title), term) == term_mode) {
                        result_set.insert(it->first);
                        break;
                    }
                }
            }
        }
    }
    if (intersection_mode) {
        set<string> intersection_set;
        for (auto it = result_set.begin(); it != result_set.end(); ++it) {
            bool contains_all = true;
            for (size_t i = 1; i < tokens.size(); i++) {
                string term = tokens[i];
                if (term[0] == '+') {
                    term = term.substr(1);
                    term = to_lower(term);
                    bool term_found = false;
                    if (field == "album") {
                        term_found = contains_token(to_lower(library.at(*it).title), term);
                    }
                    else if (field == "artist") {
                        for (const auto& artist : library.at(*it).artists) {
                            if (contains_token(to_lower(artist), term)) {
                                term_found = true;
                                break;
                            }
                        }
                    }
                    else if (field == "song") {
                        for (const auto& song : library.at(*it).songs) {
                            if (contains_token(to_lower(song.title), term)) {
                                term_found = true;
                                break;
                            }
                        }
                    }
                    if (!term_found) {
                        contains_all = false;
                        break;
                    }
                }
            }
            if (contains_all) {
                intersection_set.insert(*it);
            }
        }
        result_set = intersection_set;
    }
    if (difference_mode) {
        set<string> difference_set;
        for (auto it = result_set.begin(); it != result_set.end(); ++it) {
            bool contains_all = true;
            for (size_t i = 1; i < tokens.size(); i++) {
                string term = tokens[i];
                if (term[0] == '-') {
                    term = term.substr(1);
                    term = to_lower(term);
                    bool term_found = false;
                    if (field == "album") {
                        term_found = contains_token(to_lower(library.at(*it).title), term);
                    }
                    else if (field == "artist") {
                        for (const auto& artist : library.at(*it).artists) {
                            if (contains_token(to_lower(artist), term)) {
                                term_found = true;
                                break;
                            }
                        }
                    }
                    else if (field == "song") {
                        for (const auto& song : library.at(*it).songs) {
                            if (contains_token(to_lower(song.title), term)) {
                                term_found = true;
                                break;
                            }
                        }
                    }
                    if (!term_found) {
                        contains_all = false;
                        break;
                    }
                }
            }
            if (contains_all) {
                difference_set.insert(*it);
            }
        result_set = difference_set;
        }
    }

    cout << "Your search results exist in the following albums: " << endl;
    for (auto it = result_set.begin(); it != result_set.end(); ++it) {
        cout << *it << endl;
    }
    cout << endl;
    return;
}


int main()
{
    string userEntry;
    string command, remains;
    map<string, Album> library;

    // TODO: Declare any necessary variables for the music libary

    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            library.clear();
        }
        else if (command == "export")
        {
            export1(remains, library);
        }
        else if (command == "load")
        {
            if (remains != "") {
                load(remains, library);
            }

        }
        else if (command == "stats")
        {
            stats(library, remains);
        }          
        else if (command == "search")
        {
            search(remains, library);
        }

    }while(command != "exit");    

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}
