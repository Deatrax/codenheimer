#ifndef SEARCHSYSTEM_H
#define SEARCHSYSTEM_H

#include "snippetbaseclass.h"
#include <iostream>
#include <ostream>
#include <vector>

class searchSystem : public snippetBaseClass
{

private:
    static const int ALPHABET_SIZE = 66;

    struct Node {
        Node* next[ALPHABET_SIZE];
        bool endpoint;
        std::vector<snippetBaseClass*> target;
        Node() {
            endpoint = false;
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                next[i] = nullptr;
            }
        }
    };

    Node* root;

    int getIndex(char ch);

    void clear(Node* curr);

    void collectAll(Node* curr, std::string prefix, std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>>& results);

public:
    searchSystem();
    ~searchSystem();

    void innit(
        std::string nam,
        std::string filenam,
        int linNum,
        std::string lng,
        std::vector<std::string> tgs
    ) override;

    std::vector<std::string> *getInfo(
        std::string &nam,
        std::string &lng
    ) override;

    std::string getSnippet() override;
    std::vector<std::string> getTags() override;
    void setEditor(QTextDocument*) override;
    std::string getLang() override;
    bool isCustom() override;
    void putColors(std::unordered_map<std::string,std::vector<std::string>>& colorsData) override;
    void putTags(std::vector<std::string>&) override;
    bool isLocked() override;

    void insert(std::string str, int n) override;
    void insert(const std::string& str, snippetBaseClass* targ) override;
    bool search(const std::string& str, std::vector<snippetBaseClass*>& ret) override;
    std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>> searchWithPrefix(const std::string& prefix) override;

    void display(Node* curr = nullptr, std::string str = "");
    void getData(std::string &str, int &n) override;

    bool saveSnippetToFile(std::string snippet) override;
    bool updateSnippetDetails() override;
    std::string& EditSnippet() override;

};

#endif // SEARCHSYSTEM_H
