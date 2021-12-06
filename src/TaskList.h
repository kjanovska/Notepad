#include <curses.h>
#include "ShoppingList.h"
#include "TaskItem.h"

class TaskList : public Note {
private:
    std::vector <TaskItem> taskList;
public:
    TaskList(std::string &newName, std::vector <Tag> &newTags, Importance &newImportance);
    TaskList(const TaskList & list) = delete;
    TaskList & operator= (const TaskList & list) = delete;
    ~TaskList() = default;

    /**
     * Adds new TaskItem to taskList
    * @param window
    */
    void NewTask(WINDOW *window);
    /**
     * exports note to individual .txt file containing that note
     */
    void ExportTxt() override;
    /**
     * exports note to individual .md file containing that note in preset format
     */
    void ExportMarkdown() override;
    /**
    * import file formatted as if it was exported by ExportTxt() method
    * @param file
    * @return true if import was successful
    */
    bool ImportNote(std::ifstream &file) override;
    /**
    * parses attributes to be saved in a save file
    * @return vector of strings to be saved into a save file
    */
    std::vector<std::string> ParseNoteForSaving() override;
    /**
    * @param textToParse - text from which it forms a new Note from TaskList category
    * @return parsed note
    */
    bool ParseNoteFromLoading(const std::vector<std::string> &textToParse) override;
    /**
     * lets user write a note and triggers a window specific for TaskList
     */
    void ShowAndWriteNote(WINDOW *window) override;
    /**
     * for printing content of a note on a window
     */
    void NoteInterface(WINDOW * window) override;
    /**
     * prints part of itemList on screen, number of printed elements depends on window height
     * @param window
     * @param firstItemIndex
     */
    void PrintItems(WINDOW *window, unsigned int &firstItemIndex);
    /**
     * for finding an occurrence of a given string in note's name, names of items in taskList and content of these items
     * @param str
     * @return true if note contains given string
     */
    bool HasText(const std::string & str) override;
    };
