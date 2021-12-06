#include <utility>
#include "Note.h"

Note::Note(std::string new_name, std::vector<Tag> new_tags, const Importance &new_importance)
        : name(std::move(new_name)), tags(std::move(new_tags)), importance(new_importance) {
    this->category = NONE;
}

std::vector<std::string> Note::ParseHeader() {
    std::vector<std::string> parsedNote;
    //first part of parsed text is 'header'
    parsedNote.emplace_back("NOTE");
    parsedNote.emplace_back(CategoryToString(this->category)); // 1. line of parsed text is note category
    parsedNote.emplace_back(ImportanceToString(this->importance)); // 2. line is importance
    parsedNote.emplace_back("TAGS");
    for (Tag tag : tags) parsedNote.emplace_back(TagToString(tag));
    parsedNote.emplace_back("NAME");
    parsedNote.emplace_back(this->name);
    parsedNote.emplace_back("TEXT");
    return parsedNote;
}

std::string Note::TagToString(const Tag &tag) {
    switch (tag) {
        case WORK:
            return "WORK";
        case TRAVEL:
            return "TRAVEL";
        case HOME:
            return "HOME";
        case EVENT:
            return "EVENT";
        case SCHOOL:
            return "SCHOOL";
        case FUN:
            return "FUN";
        default:
            return "FUN";
    }
}

std::string Note::CategoryToString(const Category &cat) {
    switch (cat) {
        case TEXT:
            return "TEXT";
        case SHOPPING_LIST:
            return "SHOPPING_LIST";
        case TASK_LIST:
            return "TASK_LIST";
        case NONE:
            return "TEXT";
        default:
            return "TEXT";
    }
}

std::string Note::ImportanceToString(const Importance &imp) {
    switch (imp) {
        case IMPORTANT:
            return "IMPORTANT";
        case NOT_IMPORTANT:
            return "NOT_IMPORTANT";
        default:
            return "NOT_IMPORTANT";
    }
}

void Note::CommonInterface(WINDOW *window) {
    werase(window);
    keypad(window, true);
    box(window, 0, 0);
    mvwprintw(window, 2, 5, "Note ");
    mvwprintw(window, 2, 10, this->name.data());
    mvwprintw(window, 1, WIN_WIDTH - 25, "Backspace - Back");
    mvwprintw(window, 2, WIN_WIDTH - 25, "Right arrow - New Item");
    mvwprintw(window, 3, WIN_WIDTH - 25, "Up arrow - Up");
    mvwprintw(window, 4, WIN_WIDTH - 25, "Down arrow - Down");
    mvwprintw(window, 5, 4, "Importance: ");
    mvwprintw(window, 5, 16, ImportanceToString(this->importance).data());
    mvwprintw(window, 6, 4, "Tags: ");
    int pos = 10;
    for (Tag tag : this->tags) {
        mvwprintw(window, 6, pos, TagToString(tag).data());
        pos += TagToString(tag).size() + 1;
    }
}

std::string Note::Hash(const std::string &str) {
    std::string result;
    unsigned int hashMultiplier = 1;
    bool add = true;
    for (const char &ch : str) {
        result += std::to_string(ch * hashMultiplier);
        if (add && hashMultiplier < 9) {
            hashMultiplier++;
        } else if (add && hashMultiplier >= 9) {
            add = false;
            hashMultiplier--;
        } else if (!add && hashMultiplier > 1) {
            hashMultiplier--;
        } else if (!add && hashMultiplier <= 1) {
            add = true;
            hashMultiplier++;
        }
    }
    return result;
}
