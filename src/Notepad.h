#include <ncurses.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <zconf.h>
#include "TaskList.h"

enum findNoteChoice{BY_IMPORTANCE, BY_CATEGORY, BY_TAGS, BY_TEXT};

class Notepad {
private:
    std::string notepadName;
    std::vector<std::shared_ptr<Note>> notes;
    std::string saveFile;

public:
    explicit Notepad(std::string & name) : notepadName(name){ this -> saveFile = this -> notepadName + ".txt";}
    std::string Name(){return this -> notepadName;}

    /**
     * menu for individual notepad
     * @param window
     */
    void NotepadMenu(WINDOW * window);
    /**
     * prints notepad menu interface
     * @param window
     */
    void MenuInterface(WINDOW * window);

    /**
     * method for adding a new note
     * @param window
     */
    void NewNoteMenu(WINDOW *window);

    /**
     * load notes from save file and parse them according to their categories
     * @return true if notes were successfully loaded, false if save file was missing or corrupted
     */
    bool LoadNotes();
    /**
     * exports all notes in format specified by user (plain text or markdown). Polymorphic calling
     * - export every Note element from notes vector
     */
    static void ExportNotes(WINDOW *window, std::vector<std::shared_ptr<Note>> &results);
    /**
     * triggers window where user inputs a letter according to by which criteria he wants to find certain
     * notes (by importance/category/tags). Then FindBy method is called returning desired notes, whose name attributes
     * are then printed on screen. Every name has assigned a number, and if user then inputs that number, full note
     * is shown (polymorphic call).
     */
    void FindNote(WINDOW *window);

    /** CASE IMPORTANCE
    * user is asked to specify if notes he wants to find should be those marked 'important' or those marked
    * 'not important'. Then notes fulfilling that criteria are found and returned.
    * CASE CATEGORY
    * user is asked to specify which category of notes he wants to see (shopping lists, ...). Then notes
    * fulfilling that criteria are found and returned.
    * CASE TAGS
    * user is asked to specify tags his notes are marked as ('work', 'events'...). Then notes fulfilling
    * that criteria are found and returned.
    */
    std::vector<std::shared_ptr<Note>> FindBy(const findNoteChoice &type, WINDOW *window);

    /**
     * displays notes from 'results' vector - might be a certain group or all notes
     * @param window
     * @param results
     */
    void ShowNotes(WINDOW *window, std::vector<std::shared_ptr<Note>> &results);

    /**
     * shows one of notes from results depending on decision value
     * @param window
     * @param results
     * @param decision
     */
    void NoteMenu(WINDOW * window, std::vector<std::shared_ptr<Note>> &results, unsigned int & decision);

    /**
     * saves all notes to a file (named as saveFile variable). Triggers polymorphic SaveNote method of individual Notes.
     * @return true if saving process completed successfully
     */
    bool SaveNotes();

    /**
     * import note in .txt format valid for this notepad
     * @param window
     */
    void ImportMenu(WINDOW * window);
    /**
     * method used for accepting user's choices about tags in a new note settings
     * @param window
     * @return
     */
    static std::vector<Tag> SetTags(WINDOW * window);
    /**
     * method used for accepting user's choices about tags in a new note settings
     * @param window
     * @return
     */
    static Importance SetImportance(WINDOW * window);
    /**
     * method used for accepting user's choices about tags in a new note settings
     * @param window
     * @return
     */
    static Category SetCategory(WINDOW * window);
    /**
     * parses a string to category enum
     * @param str
     * @return
     */
    static Category GetCategory(const std::string& str);
    /**
     * parses a string to importance enum
     * @param str
     * @return
     */
    static Importance GetImportance(const std::string& str);
    /**
     * parses a string to tag enum
     * @param str
     * @return
     */
    static Tag GetTag(const std::string& str);
};

