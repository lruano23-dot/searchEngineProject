/*------------------------------------
Program 2: Search Engine Project
Course: CS 251, Fall 2023, UIC
System: VSCode
Author: Lisette Ruano
--------------------------------------*/

#pragma once

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;


// Purpose: takes in a word and checks if it's valid by checking if it has at least 1 letter
// gets rid of punctuation at the beginning and end of the word and also makes all it's letters lowercase
// Parameters: 1 string variable
// Returns: a string
string cleanToken(string s) {

    int charNum = 0;

    //iterates through the word's array of char and counts how many letters there are in the word
    for(int i = 0; i < s.size(); i++){
        if(isalpha(s[i])){
            charNum = charNum +1;

        }
    }

    //if else statement that returns a token with no letters as blank or cleans the token if it's valid
    if(charNum == 0){
        s = "";
        return s;
    }
    else{
        while(ispunct(s[0])){
            s.erase(0,1);
        }

        while(ispunct(s[s.size()-1])){
            s.erase(s.size()-1,1);
        }

        for(int i = 0; i < s.size(); i++){
            s[i] = tolower(s[i]);
        }
    }

    return s;
}


// Purpose: Utilizes the cleanToken function in order to tokenize each word from the string of text of the website. It
// then inserts the token into a set and then returns that set.
// Parameters: 1 string variable
// Returns: set of strings
set<string> gatherTokens(string text) {
    set<string> tokens;
    string word;
    stringstream strstream;

    // TODO:  Write this function.

    strstream << text;

    while(strstream >> word){
        word = cleanToken(word);
        if(word != ""){
            tokens.insert(word);
        }
    }

    return tokens;  // TODO:  update this.
}

// Purpose: Reads in the website string and uses the website address that is read in outside of the function in order to
// first put all the words of the string line into a set using the gatherTokens function. After the set is made a for loop
// is used to iterate through the tokens set and creates a map key of the word if it doesn't already exist in the index map
// and it adds the website address to the word's set.
// Parameters: ifstream variable inFS by refrence, index map variable by refrence, int pageCount variable by refrence,
// string websiteAddress variable
// Returns: nothing
void buildMap(ifstream& inFS, map<string, set<string>>& index, int& pageCount, string websiteAddress){

    string websiteString;
    set<string> websiteTokens;
    set<string> webSiteSet;

    getline(inFS,websiteString);
    websiteTokens = gatherTokens(websiteString);
    for (auto token : websiteTokens){
        if(index.count(token) == 0){
            index.emplace(token,webSiteSet);
            index.at(token).insert(websiteAddress);
        }
        else{
            index.at(token).insert(websiteAddress);
        }
    }


}

// Purpose: Creates the inverted index needed to know which word is associated with which website(s). First opens the text file
// and while it hasn't reached the end of the file it reads in both the URL and text string. The function calls buildMap function
// in order to update the map with the new URl and text string pair. Also keeps count of the pages that have been added.
// Parameters: string filename variable, map index variable by refrence
// Returns: int variable pageCount
int buildIndex(string filename, map<string, set<string>>& index) {


    // TODO:  Write this function.
    ifstream inFS;
    int pageCount = 0;
    string websiteAddress;

    inFS.open(filename);

    if(!inFS.is_open()){
        return 0;
    }
    else{
        getline(inFS,websiteAddress);

        while(!inFS.eof()){
            buildMap(inFS,index,pageCount,websiteAddress);
            pageCount = pageCount + 1;
            getline(inFS, websiteAddress);
        }

        inFS.close();
    }

    return pageCount;
}

// Purpose: takes in the single word that user input and using an if else statement it either returns result set right away
// if the word doesn't exist as a key in the map index or if the key does exist it returns that key's set
// Parameters: vector of string type searchWords, map of string and string set type index by refrence
// Returns: a set of string type
set<string> singleQuery(vector<string>& searchWords, map<string, set<string>>& index){
    set<string> result;
    string wordToSearch;

    wordToSearch = cleanToken((searchWords.at(0)));
    if(index.count(wordToSearch) == 0){
        return result;
    }
    else{
        return index.at(wordToSearch);
    }
}


// Purpose: reads in the sentence that is input by the user and using string stream divides the string into individual
// words. If it's only 1 word that is searched the function calls the singleQuery function. Else the funciton goes through the
// process of calculating the results set based on the operators that the user has input.
// Parameters: map of type string and set of string index by refrence, string sentence variable
// Returns: set of string type
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> result;
    set<string> temp;
    vector<string> searchWords;
    vector<string> cleanWords;
    string wordToSearch;
    string word;
    stringstream strstream;

    strstream << sentence;

    while(strstream >> word){
        searchWords.push_back(word);
    }

    if(searchWords.size() == 1){
        result = singleQuery(searchWords, index);
    }
    else{

        for(int i = 0; i < searchWords.size(); i++){
            wordToSearch = cleanToken(searchWords.at(i));
            cleanWords.push_back(wordToSearch);
        }

        //using an if statement determines which calculation needs to be done between the first word's set and the second word's set
        //the set that is calculated will be used in the next for statement
        if(isalpha(searchWords.at(1)[0])){
            set_union(index.at(cleanWords.at(0)).begin(), index.at(cleanWords.at(0)).end(), index.at(cleanWords.at(1)).begin(), index.at(cleanWords.at(1)).end(),
                      inserter(result,result.begin()));
        }
        else if(searchWords.at(1)[0] == '+'){
            set_intersection(index.at(cleanWords.at(0)).begin(), index.at(cleanWords.at(0)).end(), index.at(cleanWords.at(1)).begin(), index.at(cleanWords.at(1)).end(),
                             inserter(result,result.begin()));
        }
        else{
            set_difference(index.at(cleanWords.at(0)).begin(), index.at(cleanWords.at(0)).end(), index.at(cleanWords.at(1)).begin(), index.at(cleanWords.at(1)).end(),
                           inserter(result,result.begin()));
        }


        //using a for statement it iterates thorugh the searchWord array starting at the 3rd word and
        //determines which calculation needs to be done between the previous equation's set and the next word's set
        for(int i = 2; i < searchWords.size(); i++){
            temp.clear();
            if(isalpha(searchWords.at(i)[0])){
                set_union(result.begin(), result.end(), index.at(cleanWords.at(i)).begin(), index.at(cleanWords.at(i)).end(),
                          inserter(temp,temp.begin()));
                result=temp;
            }
            else if(searchWords.at(i)[0] == '+'){
                set_intersection(result.begin(), result.end(), index.at(cleanWords.at(i)).begin(), index.at(cleanWords.at(i)).end(),
                                 inserter(temp,temp.begin()));
                result=temp;
            }
            else{
                set_difference(result.begin(), result.end(), index.at(cleanWords.at(i)).begin(), index.at(cleanWords.at(i)).end(),
                               inserter(temp,temp.begin()));

                result=temp;
            }
        }

    }

    return result;  // TODO:  update this.
}

// Purpose: outputs all console output that is needed. Asks user to input the file to process then outputs the stats of the index
// Then asks user what they would like to search for. Then displays the search results. It then asks user again and repeats the process
// until a newline is entered
// Parameters: string variable filename
// Returns: nothing
void searchEngine(string filename) {
    int indexCount;
    map<string,set<string>> index;
    set<string> results;
    string wordString;

    indexCount = buildIndex(filename, index);

    if(indexCount == 0){
        cout << "Invalid filename." << endl;
    }
    cout <<"Stand by while building index..." << endl;
    cout << "Indexed " << indexCount << " pages containing " << index.size() << " unique terms" << endl;
    cout << endl;

    cout << "Enter query sentence (press enter to quit): ";
    getline(cin,wordString);


    while(!wordString.empty()){
        results = findQueryMatches(index,wordString);
        cout << "Found " << results.size() << " matching pages" << endl;
        for(auto website:results){
            cout << website << endl;
        }
        cout << endl;
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin,wordString);
    }

    cout << endl;
    cout << "Thank you for searching!" << endl;

}

//Purpose: tests if cleanToken works properly
//Paramaters: none
//Returns: bool value
bool testCleanToken() {
    string ans = "hello";


    if (ans != cleanToken(".hello")) {
        return false;
    }

    if (ans != cleanToken("...hello")) {
        return false;
    }

    ans="hello^&#you";

    if (ans != cleanToken("&*($hello^&#you*()")){
        return false;
    }

    return true;
}

//Purpose: tests if gatherTokens works properly
//Paramaters: none
//Returns: bool value
bool testGatherToken(){
    int ans = 5;

    if (ans != gatherTokens("hello bye what when where").size()){
        return false;
    }

    if (ans != gatherTokens("* hello bye what when where &").size()){
        return false;
    }

    ans = 0;

    if (ans != gatherTokens("&*^ () ^& ^^^ $%").size()){
        return false;
    }


    return true;

}

//Purpose: tests if buildIndex works properly
//Paramaters: none
//Returns: bool value
bool testBuildIndex(){
    int pageAns = 4;
    int termAns = 20;
    int pageCount = 0;
    map<string,set<string>> index;
    string filename = "tiny.txt";

    pageCount=buildIndex(filename, index);

    if(pageCount != pageAns || index.size()!= termAns){
        return false;
    }

    pageAns = 86;
    termAns = 1498;
    pageCount = 0;
    filename = "cplusplus.txt";
    index.clear();

    pageCount = buildIndex(filename, index);
    if(pageCount != pageAns || index.size()!= termAns){
        return false;
    }

    pageAns = 148;
    termAns = 1874;
    pageCount = 0;
    filename = "wiki-uni.txt";
    index.clear();

    pageCount = buildIndex(filename, index);
    if(pageCount != pageAns || index.size()!= termAns){
        return false;
    }


    return true;


}

//Purpose: tests if findQueryMatches works properly
//Paramaters: none
//Returns: bool value
bool testFindQueryMatches(){
    set<string> returnedSet;
    map<string,set<string>> index;
    int ans = 16;

    buildIndex("cplusplus.txt",index);
    returnedSet= findQueryMatches(index,"container");

    if(returnedSet.size() != ans){
        return false;
    }

    returnedSet.clear();
    ans = 0;

    returnedSet = findQueryMatches(index, "hi");
    if(returnedSet.size() != ans){
        return false;
    }

    returnedSet.clear();
    ans = 7;
    returnedSet = findQueryMatches(index, "vector +container");
    if(returnedSet.size() != ans){
        return false;
    }

    returnedSet.clear();
    ans = 16;
    returnedSet = findQueryMatches(index, "vector +integral -the container");
    if(returnedSet.size() != ans){
        return false;
    }

    return true;


}

//Purpose: runs all tests for each milestone functions then outputs how many tests were passed
//and how many failed
//Parameters: none
//Returns: nothing
void runTests(){

    int pass = 0;
    int fail = 0;

    bool result = testCleanToken();
    if(result == 0){
        fail++;
    }
    else{
        pass++;
    }

    result = testGatherToken();
    if(result == 0){
        fail++;
    }
    else{
        pass++;
    }

    result = testBuildIndex();
    if(result == 0){
        fail++;
    }
    else{
        pass++;
    }


    result = testFindQueryMatches();
    if(result == 0){
        fail++;
    }
    else{
        pass++;
    }


    cout << "Tests passed: " << pass << endl;
    cout << "Tests failed: " << fail << endl;


}


