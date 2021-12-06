#include "ImportanceCriteria.h"

void ImportanceCriteria::GetSpecifics(WINDOW *window) {
    while (true) {
        werase(window);
        keypad(window, true);
        noecho();
        box(window, 0, 0);
        mvwprintw(window, 2, 10, "Backspace - Back");
        mvwprintw(window, 3, 5, "Do you wish to view important or not important notes?");
        mvwprintw(window, 4, 10, "A - Important");
        mvwprintw(window, 5, 10, "B - Not important");
        wrefresh(window);
        int decision = wgetch(window);
        if (decision == KEY_BACKSPACE) return;
        else if (decision == 'A' || decision == 'a') {
            this->importance = IMPORTANT;
            return;
        } else if (decision == 'B' || decision == 'b') {
            this->importance = NOT_IMPORTANT;
            return;
        }
    }
}

std::vector<std::shared_ptr<Note>> ImportanceCriteria::Find(const std::vector<std::shared_ptr<Note>> &notes) {
    std::vector<std::shared_ptr<Note>> result;
    for (const std::shared_ptr<Note> &note : notes)
        if (note->GetImportance() == this->importance) result.push_back(note);
    return result;
}
