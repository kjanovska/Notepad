#include "TagsCriteria.h"

void TagsCriteria::GetSpecifics(WINDOW *window) {
    std::vector<Tag> myTags;
    while (true) {
        werase(window);
        keypad(window, true);
        noecho();
        box(window, 0, 0);
        mvwprintw(window, 2, 10, "Enter - Done");
        mvwprintw(window, 3, 10, "Which tags should the notes have?");
        mvwprintw(window, 4, 10, "A - WORK");
        mvwprintw(window, 5, 10, "B - TRAVEL");
        mvwprintw(window, 6, 10, "C - HOME");
        mvwprintw(window, 7, 10, "D - EVENT");
        mvwprintw(window, 8, 10, "E - SCHOOL");
        mvwprintw(window, 9, 10, "F - FUN");
        wrefresh(window);
        int decision = wgetch(window);
        if (decision == 10) {
            this->tags = myTags;
            return;
        } else if (decision == 'A' || decision == 'a') {
            if (std::find(this->tags.begin(), this->tags.end(), WORK) == this->tags.end()) {
                Tag tag = WORK;
                myTags.push_back(tag);
            }
        } else if (decision == 'B' || decision == 'b') {
            if (std::find(this->tags.begin(), this->tags.end(), TRAVEL) == this->tags.end()) {
                Tag tag = TRAVEL;
                myTags.push_back(tag);
            }
        } else if (decision == 'C' || decision == 'c') {
            if (std::find(this->tags.begin(), this->tags.end(), HOME) == this->tags.end()) {
                Tag tag = HOME;
                myTags.push_back(tag);
            }
        } else if (decision == 'D' || decision == 'd') {
            if (std::find(this->tags.begin(), this->tags.end(), EVENT) == this->tags.end()) {
                Tag tag = EVENT;
                myTags.push_back(tag);
            }
        } else if (decision == 'E' || decision == 'e') {
            if (std::find(this->tags.begin(), this->tags.end(), SCHOOL) == this->tags.end()) {
                Tag tag = SCHOOL;
                myTags.push_back(tag);
            }
        } else if (decision == 'F' || decision == 'f') {
            if (std::find(this->tags.begin(), this->tags.end(), FUN) == this->tags.end()) {
                Tag tag = FUN;
                myTags.push_back(tag);
            }
        }
    }
}

std::vector<std::shared_ptr<Note>> TagsCriteria::Find(const std::vector<std::shared_ptr<Note>> &notes) {
    std::vector<std::shared_ptr<Note>> result;
    for (const std::shared_ptr<Note> &note : notes) {
        if (note->Tags().empty() && this->tags.empty()) result.push_back(note);
        for (Tag criteriaTag : this->tags) {
            for (Tag noteTag : note->Tags())
                if (noteTag == criteriaTag) {
                    if (std::find(result.begin(), result.end(), note) == result.end())
                        result.push_back(note);
                    break;
                }
        }
    }
    return result;
}
