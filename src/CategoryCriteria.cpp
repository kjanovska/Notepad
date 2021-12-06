#include "CategoryCriteria.h"

void CategoryCriteria::GetSpecifics(WINDOW *window) {
    while (true) {
        werase(window);
        keypad(window, true);
        noecho();
        box(window, 0, 0);
        mvwprintw(window, 2, 10, "Backspace - Back");
        mvwprintw(window, 3, 10, "Which category do you wish to view?");
        mvwprintw(window, 4, 10, "A - Text notes");
        mvwprintw(window, 5, 10, "B - Shopping lists");
        mvwprintw(window, 6, 10, "C - Task lists");
        wrefresh(window);
        int decision = wgetch(window);
        if (decision == KEY_BACKSPACE) return;
        else if (decision == 'A' || decision == 'a') {
            this->category = TEXT;
            return;
        } else if (decision == 'B' || decision == 'b') {
            this->category = SHOPPING_LIST;
            return;
        } else if (decision == 'C' || decision == 'c') {
            this->category = TASK_LIST;
            return;
        }
    }
}

std::vector<std::shared_ptr<Note>> CategoryCriteria::Find(const std::vector<std::shared_ptr<Note>> &notes) {
    std::vector<std::shared_ptr<Note>> result;
    for (const std::shared_ptr<Note> &note : notes)
        if (note->GetCategory() == this->category) result.push_back(note);
    return result;
}
