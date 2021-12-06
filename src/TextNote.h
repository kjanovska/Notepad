#include <curses.h>
#include "CategoryCriteria.h"

class TextNote : public Note{
    std::vector<std::string> paragraphs;

public:
    TextNote(std::string &newName, std::vector <Tag> &newTags, Importance &newImportance);
    TextNote(const TextNote & note) = delete;
    TextNote & operator= (const TextNote & list) = delete; //used in LoadNote method
    ~TextNote() = default;

    /**
     * Adds new paragraph to paragraphs
     * @param window
     */
    void NewParagraph(WINDOW * window);
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
    * @param textToParse - text from which it forms a new Note from TextNote category
    * @return parsed note
    */
    bool ParseNoteFromLoading(const std::vector<std::string> &textToParse) override;
    /**
     * lets user write a note and triggers a window specific for TextNote
     */
    void ShowAndWriteNote(WINDOW *window) override;
    /**
     * for printing content of a note on a window
     */
    void NoteInterface(WINDOW * window) override;
    /**
     * prints part of paragraphs on screen, number of printed elements depends on window height
     * @param window
     * @param firstItemIndex
     */
    unsigned int PrintItems(WINDOW *window, unsigned int &firstItemIndex);
    /**
     * for finding an occurrence of a given string in both name and paragraphs
     * @param str
     * @return true if note contains given string
     */
    bool HasText(const std::string & str) override;
};

