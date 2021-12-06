#include "TextCriteria.h"

void TextCriteria::GetSpecifics(WINDOW *window) {
    char toFind[50];
    werase(window);
    keypad(window, true);
    echo();
    box(window, 0, 0);
    mvwprintw(window, 3, 10, "Input text to find: ");
    mvwprintw(window, 4, 10, "");
    wrefresh(window);
    wgetnstr(window, toFind, 50);
    std::string strToFind(toFind);
    this->text = strToFind;
}

std::vector<std::shared_ptr<Note>> TextCriteria::Find(const std::vector<std::shared_ptr<Note>> &notes) {
    std::vector<std::shared_ptr<Note>> result;
    for (const std::shared_ptr<Note> &note : notes)
        if (note->HasText(this->text)) result.push_back(note);
    return result;
}
