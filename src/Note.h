
#include <string>
#include <vector>
#include <fstream>
#include <curses.h>

#define WIN_HEIGHT 20
#define WIN_WIDTH 60

enum Tag{WORK, TRAVEL, HOME, EVENT, SCHOOL, FUN};
enum Importance{IMPORTANT, NOT_IMPORTANT};
enum Category{TEXT, SHOPPING_LIST, TASK_LIST, NONE};

/**
 * superclass Note - subclasses are ShoppingList, TaskList, TextNote
 */
class Note {
protected:
    std::string name;
    std::vector<Tag> tags;
    Importance importance;
    Category category; //for finding note by category
public:
    Note(std::string  new_name, std::vector<Tag>  new_tags, const Importance & new_importance);
    std::string Name() const { return this -> name; }
    std::vector<Tag> Tags() const { return this -> tags; }
    Importance GetImportance() const { return this -> importance; }
    Category GetCategory() const { return this -> category; }
    /**
     * method called from ParseNoteForSaving for parsing attributes shared by subclasses
     */
    std::vector<std::string> ParseHeader();
    /**
     * method for transforming a Tag enum to a string
     * @param Tag enum to transform
     * @return enum name as a string
     */
    static std::string TagToString(const Tag &tag);
    /**
    * method for transforming a Category enum to a string
    * @param Category enum to transform
    * @return enum name as a string
    */
    static std::string CategoryToString(const Category &cat);
    /**
    * method for transforming an Importance enum to a string
    * @param Importance enum to transform
    * @return enum name as a string
    */
    static std::string ImportanceToString(const Importance &imp);
    /**
     * exports note to individual .txt file containing that note
     */
    virtual void ExportTxt() = 0;
    /**
     * exports note to individual .md file containing that note in preset format
     */
    virtual void ExportMarkdown() = 0;
    /**
     * import file formatted as if it was exported by ExportTxt() method
     * @param file
     * @return true if import was successful
     */
    virtual bool ImportNote(std::ifstream &file) = 0;
    /**
     * parses attributes to be saved in a save file
     * @return vector of strings to be saved into a save file
     */
    virtual std::vector<std::string> ParseNoteForSaving() = 0;
    /**
     * @param textToParse - text from which it forms a new Note from specific category
     * @return parsed note
     */
    virtual bool ParseNoteFromLoading(const std::vector<std::string> &textToParse) = 0;
    /**
     * lets user write a note and triggers a window specific for the note type
     */
    virtual void ShowAndWriteNote(WINDOW *window) = 0;
    /**
     * for printing content of a note on a window
     */
    virtual void NoteInterface(WINDOW * window) = 0;
    /**
     * for printing that part of the window which all types of notes share - name, tags,...
     * @param window
     */
    void CommonInterface(WINDOW * window);
    /**
     * for finding an occurrence of a given string in both name and content of a note
     * @param str
     * @return true if note contains given string
     */
    virtual bool HasText(const std::string & str) = 0;
    /**
     * counts hash from note's name. Hash is used in a save file for marking the end of a note.
     * @return hash counted from note's name
     */
    static std::string Hash(const std::string &str);

};
