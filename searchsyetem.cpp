#include "searchsyetem.h"
#include "qlogging.h"
#include "snippetbaseclass.h"

int searchSystem::getIndex(char ch) {
    if (ch == ' ') return 54;
    if (ch == '-') return 53;
    if (isupper(ch)) return ch - 'A';
    if (islower(ch)) return ch - 'a' + 26;
    return -1; // Invalid character
}

void searchSystem::clear(Node *curr) {
    if (!curr) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        clear(curr->next[i]);
    }
    delete curr;
}

void searchSystem::collectAll(Node *curr, std::string prefix, std::vector<std::pair<std::string, snippetBaseClass *> > &results) {
    if (!curr) return;
    if (curr->endpoint) {
        results.emplace_back(prefix, curr->target);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curr->next[i]) {
            char ch;
            if (i < 26) ch = 'A' + i;
            else if (i < 52) ch = 'a' + (i - 26);
            else if (i == 53) ch = '-';
            else if (i == 54) ch = ' ';
            collectAll(curr->next[i], prefix + ch, results);
        }
    }
}

searchSystem::searchSystem() {
    root = new Node;
}

searchSystem::~searchSystem() {
    clear(root);
}

std::string searchSystem::getSnippet()
{
    return NULL;
}

std::vector<std::string> searchSystem::getTags()
{
    return std::vector<std::string>() ;
}

void searchSystem::setEditor(QTextDocument *)
{

}

std::string searchSystem::getLang()
{
    return NULL;
}

bool searchSystem::isCustom()
{
    return true;
}

void searchSystem::putColors(std::unordered_map<std::string, std::vector<std::string> > &colorsData)
{

}

void searchSystem::putTags(std::vector<std::string> &)
{

}

bool searchSystem::isLocked()
{
    return true;
}

void searchSystem::insert(std::string str, int n){
    //absolutely nothing
}

void searchSystem::insert(const std::string &str,   snippetBaseClass *targ) {
    qDebug("entered the insert");
    Node* curr = root;
    for (char ch : str) {
        int idx = getIndex(ch);
        if (idx == -1) continue; // Skip invalid characters
        if (!curr->next[idx]) {
            curr->next[idx] = new Node;
        }
        curr = curr->next[idx];
    }
    curr->endpoint = true;
    curr->target.push_back(targ);
}

bool searchSystem::search(const std::string &str,   std::vector<snippetBaseClass*> ret) {
    Node* curr = root;
    for (char ch : str) {
        int idx = getIndex(ch);
        if (idx == -1 || !curr->next[idx]) {
            return false;
        }
        curr = curr->next[idx];
    }
    ret=curr->target;
    return curr->endpoint;
}

std::vector<std::pair<std::string, snippetBaseClass *> > searchSystem::searchWithPrefix(const std::string &prefix) {
    qDebug("this was called 222\n");
    Node* curr = root;
    std::vector<std::pair<std::string, snippetBaseClass*>> results;

    // Traverse to the end of the prefix
    for (char ch : prefix) {
        int idx = getIndex(ch);
        if (idx == -1 || !curr->next[idx]) {
            return results; // Prefix not found
        }
        curr = curr->next[idx];
    }

    // Collect all strings and targets under this node
    collectAll(curr, prefix, results);
    return results;
}

void searchSystem::display(Node *curr, std::string str) {
    if (!curr) {
        std::cout << "\nTree has:\n";
        curr = root;
    }
    if (curr->endpoint) {
        std::cout << str << std::endl;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curr->next[i]) {
            char ch;
            if (i < 26) ch = 'A' + i;
            else if (i < 52) ch = 'a' + (i - 26);
            else if (i == 53) ch = '-';
            else if (i == 54) ch = ' ';
            display(curr->next[i], str + ch);
        }
    }
}

void searchSystem::getData(std::string& str, int& n)
{
    //fuck you
}
