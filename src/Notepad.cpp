#include "Notepad.h"

void Notepad::ExportNotes(WINDOW *window, std::vector<std::shared_ptr<Note>> &results) {
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 3, 10, "Export options:");
    mvwprintw(window, 4, 10, "A - export in .txt");
    mvwprintw(window, 5, 10, "B - export in .md");
    while (true) {
        int decision = wgetch(window);
        if (decision == 'A' || decision == 'a') {
            for (std::shared_ptr<Note> &note : results) {
                note->ExportTxt();
            }
            break;
        } else if (decision == 'B' || decision == 'b') {
            for (std::shared_ptr<Note> &note : results) {
                note->ExportMarkdown();
            }
            break;
        }
    }
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 5, 10, "Notes exported to /export folder");
    wrefresh(window);
    sleep(2);
}

bool Notepad::SaveNotes() {
    std::ofstream save;
    save.open("examples/savefiles/" + this->saveFile); //ofstream overwrites existing file by default
    std::vector<std::vector<std::string>> parsedNotes;
    for (std::shared_ptr<Note> &note : this->notes)
        parsedNotes.emplace_back(note->ParseNoteForSaving());
    for (const std::vector<std::string> &note : parsedNotes)
        for (const std::string &item : note)
            save << item << std::endl;
    save.close();
    return true;
}

void Notepad::FindNote(WINDOW *window) {
    werase(window);
    keypad(window, true);
    box(window, 0, 0);
    findNoteChoice type;
    mvwprintw(window, 1, 10, "Find Note");
    mvwprintw(window, 2, 10, "Backspace - Back");
    mvwprintw(window, 3, 5, "Please select criteria by which you want to search:");
    mvwprintw(window, 5, 10, "A - by category");
    mvwprintw(window, 6, 10, "B - by tags");
    mvwprintw(window, 7, 10, "C - by importance");
    mvwprintw(window, 8, 10, "D - by text");
    wrefresh(window);
    while (true) {
        int decision = getch();
        if (decision == 'A' || decision == 'a') {
            type = BY_CATEGORY;
            break;
        } else if (decision == 'B' || decision == 'b') {
            type = BY_TAGS;
            break;
        } else if (decision == 'C' || decision == 'c') {
            type = BY_IMPORTANCE;
            break;
        } else if (decision == 'D' || decision == 'd') {
            type = BY_TEXT;
            break;
        } else if (decision == KEY_BACKSPACE) {
            return;
        }
    }
    std::vector<std::shared_ptr<Note>> results = this->FindBy(type, window);
    this->ShowNotes(window, results);
}

std::vector<std::shared_ptr<Note>> Notepad::FindBy(const findNoteChoice &type, WINDOW *window) {
    std::shared_ptr<Criteria> criteria;

    if (type == BY_CATEGORY)
        criteria = std::make_shared<CategoryCriteria>();
    else if (type == BY_TAGS)
        criteria = std::make_shared<TagsCriteria>();
    else if (type == BY_IMPORTANCE)
        criteria = std::make_shared<ImportanceCriteria>();
    else
        criteria = std::make_shared<TextCriteria>();

    criteria->GetSpecifics(window);
    return criteria->Find(this->notes);
}

void Notepad::ShowNotes(WINDOW *window, std::vector<std::shared_ptr<Note>> &results) {
    while (true) {
        werase(window);
        keypad(window, true);
        noecho();
        box(window, 0, 0);
        mvwprintw(window, 1, 10, this->notepadName.data());
        mvwprintw(window, 2, 10, "Backspace - Back");
        mvwprintw(window, 3, 10, "E - Export notes");
        mvwprintw(window, 4, 10, "Press a number to view certain note:");
        int line = 5;
        for (unsigned int i = 0; i < results.size(); i++) {
            std::string noteNumber = std::to_string(i);
            mvwprintw(window, line, 5, noteNumber.data());
            mvwprintw(window, line, 10, results[i]->Name().data());
            line++;
        }
        wrefresh(window);
        unsigned int decision = wgetch(window);
        if (decision == KEY_BACKSPACE) return;
        else if (decision == 'E' || decision == 'e') this->ExportNotes(window, results);
        else if (decision - '0' < results.size()) {
            this->NoteMenu(window, results, decision);
        }
    }
}

void Notepad::NoteMenu(WINDOW *window, std::vector<std::shared_ptr<Note>> &results, unsigned int &decision) {
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 2, 5, "What do you wish to do?");
    mvwprintw(window, 3, 10, "A - show note");
    mvwprintw(window, 4, 10, "B - delete note");
    mvwprintw(window, 5, 10, "C - export note in txt");
    mvwprintw(window, 6, 10, "D - export note in Markdown");
    while (true) {
        int d2 = wgetch(window);
        if (d2 == 'A' || d2 == 'a') {
            results[decision - '0']->ShowAndWriteNote(window);
            this->SaveNotes();
            break;
        } else if (d2 == 'B' || d2 == 'b') {
            results.erase(results.begin() + decision - '0');
            this->SaveNotes();
            break;
        } else if (d2 == 'C' || d2 == 'c') {
            results[decision - '0']->ExportTxt();
            this->SaveNotes();
            break;
        } else if (d2 == 'D' || d2 == 'd') {
            results[decision - '0']->ExportMarkdown();
            this->SaveNotes();
            break;
        }
    }
}

void Notepad::NotepadMenu(WINDOW *window) {
    while (true) {
        this->MenuInterface(window);
        keypad(window, true);
        int decision = wgetch(window);
        if (decision == 'A' || decision == 'a') {
            if (this->notes.size() > 9) {
                werase(window);
                keypad(window, true);
                box(window, 0, 0);
                mvwprintw(window, 2, 10, "Reached maximum amount of notes");
                mvwprintw(window, 5, 10, "A - back");
                wrefresh(window);
                while (true) {
                    int decision2 = getch();
                    if (decision2 == 'A' || decision2 == 'a') return;
                }
            }
            this->NewNoteMenu(window);
            //when called from here, user didn't have chance to create another note, therefore it is always the last one
            this->SaveNotes();
        } else if (decision == 'B' || decision == 'b') this->SaveNotes();
        else if (decision == 'C' || decision == 'c') this->ExportNotes(window, this->notes);
        else if (decision == 'D' || decision == 'd') this->FindNote(window);
        else if (decision == 'E' || decision == 'e') {
            this->ImportMenu(window);
            this->SaveNotes();
        } else if (decision == 'F' || decision == 'f') this->ShowNotes(window, this->notes);
        else if (decision == KEY_BACKSPACE) return;
    }
}

void Notepad::MenuInterface(WINDOW *window) {
    noecho();
    werase(window);
    keypad(window, true);
    box(window, 0, 0);
    mvwprintw(window, 2, 10, "Notepad ");
    mvwprintw(window, 2, 20, this->notepadName.data());
    mvwprintw(window, 5, 10, "A - new note");
    mvwprintw(window, 6, 10, "B - save notes");
    mvwprintw(window, 7, 10, "C - export all");
    mvwprintw(window, 8, 10, "D - find note");
    mvwprintw(window, 9, 10, "E - import note");
    mvwprintw(window, 10, 10, "F - show all notes");
    mvwprintw(window, 11, 10, "Backspace - back");
    wrefresh(window);
}

void Notepad::NewNoteMenu(WINDOW *window) {
    char name[40];
    echo();
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 1, 10, "NEW NOTE");
    mvwprintw(window, 2, 10, "Set note name: ");
    wrefresh(window);
    wgetnstr(window, name, 40);
    std::string noteName(name);
    if (noteName.empty()) noteName = "note" + std::to_string(this->notes.size());
    noecho();
    Importance importance = SetImportance(window);
    std::vector<Tag> tags = SetTags(window);
    Category category = SetCategory(window);
    std::shared_ptr<Note> newNote;
    if (category == TEXT)
        newNote = std::make_shared<TextNote>(noteName, tags, importance);
    else if (category == SHOPPING_LIST)
        newNote = std::make_shared<ShoppingList>(noteName, tags, importance);
    else if (category == TASK_LIST)
        newNote = std::make_shared<TaskList>(noteName, tags, importance);
    this->notes.push_back(std::move(newNote));
    this->notes[notes.size() - 1]->ShowAndWriteNote(window);
}

bool Notepad::LoadNotes() {
    std::ifstream input;
    input.open("examples/savefiles/" + this->saveFile);
    std::string currentLine;
    while (std::getline(input, currentLine)) {
        std::shared_ptr<Note> newNote;
        Category category;
        Importance importance;
        std::vector<Tag> tags;
        std::string noteName;
        std::vector<std::string> content;
        if (currentLine != "NOTE") return false;
        if (!std::getline(input, currentLine)) return false; //category
        else if (currentLine != "TEXT" && currentLine != "SHOPPING_LIST" && currentLine != "TASK_LIST") return false;
        else category = GetCategory(currentLine);
        if (!std::getline(input, currentLine)) return false; //importance
        else if (currentLine != "IMPORTANT" && currentLine != "NOT_IMPORTANT") return false;
        else importance = GetImportance(currentLine);
        if (!std::getline(input, currentLine)) return false; //"TAGS"
        else if (currentLine != "TAGS") return false;
        else {
            while (std::getline(input, currentLine)) { //WORK, TRAVEL, HOME, EVENT, SCHOOL, FUN
                if (currentLine != "WORK" && currentLine != "FUN" && currentLine != "TRAVEL" && currentLine != "HOME" &&
                    currentLine != "EVENT" && currentLine != "SCHOOL") {
                    if (currentLine == "NAME") break;
                    else return false;
                }
                tags.push_back(GetTag(currentLine));
            }
        }
        if (!std::getline(input, currentLine)) return false; //noteName
        else noteName = currentLine;
        if (!std::getline(input, currentLine)) return false; //"TEXT"
        else if (currentLine != "TEXT") return false;
        else {
            while (std::getline(input, currentLine)) {
                if (currentLine == newNote->Hash(noteName)) break;
                else content.push_back(currentLine);
            }
            if (currentLine != newNote->Hash(noteName)) return false;
        }
        if (category == TEXT)
            newNote = std::make_shared<TextNote>(noteName, tags, importance);
        else if (category == SHOPPING_LIST)
            newNote = std::make_shared<ShoppingList>(noteName, tags, importance);
        else if (category == TASK_LIST)
            newNote = std::make_shared<TaskList>(noteName, tags, importance);

        this->notes.push_back(std::move(newNote));
        if (!this->notes[notes.size() - 1]->ParseNoteFromLoading(content)) return false;
    }
    return true;
}

void Notepad::ImportMenu(WINDOW *window) {
    std::ifstream input;
    char name[40];
    echo();
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 1, 10, "Import note");
    mvwprintw(window, 2, 1, "Enter the name of a file you want to import:");
    mvwprintw(window, 3, 10, "");
    wrefresh(window);
    wgetnstr(window, name, 40);
    std::string noteName(name);
    input.open("examples/import/" + noteName);
    if (!input) {
        werase(window);
        box(window, 0, 0);
        mvwprintw(window, 2, 10, "File doesn't exist.");
        wrefresh(window);
        sleep(2);
        return;
    }
    noecho();
    Importance importance = SetImportance(window);
    std::vector<Tag> tags = SetTags(window);
    Category category = SetCategory(window);
    std::shared_ptr<Note> newNote;
    if (category == TEXT)
        newNote = std::make_shared<TextNote>(noteName, tags, importance);
    else if (category == SHOPPING_LIST)
        newNote = std::make_shared<ShoppingList>(noteName, tags, importance);
    else if (category == TASK_LIST)
        newNote = std::make_shared<TaskList>(noteName, tags, importance);
    bool successfulImport = newNote->ImportNote(input);
    if (!successfulImport) {
        werase(window);
        box(window, 0, 0);
        mvwprintw(window, 2, 1, "Import failed, please check your file and its placement");
        wrefresh(window);
        sleep(2);
        return;
    }
    this->notes.push_back(std::move(newNote));
}

std::vector<Tag> Notepad::SetTags(WINDOW *window) {
    std::vector<Tag> tags;
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 2, 10, "Choose tags: ");
    mvwprintw(window, 3, 10, "A - WORK");
    mvwprintw(window, 4, 10, "B - TRAVEL");
    mvwprintw(window, 5, 10, "C - HOME");
    mvwprintw(window, 6, 10, "D - EVENT");
    mvwprintw(window, 7, 10, "E - SCHOOL");
    mvwprintw(window, 8, 10, "F - FUN");
    mvwprintw(window, 9, 10, "G - done");
    wrefresh(window);
    while (true) {
        int decision = getch();
        if (decision == 'A' || decision == 'a') {
            if (std::find(tags.begin(), tags.end(), WORK) == tags.end()) {
                Tag tag = WORK;
                tags.push_back(tag);
            }
        } else if (decision == 'B' || decision == 'b') {
            if (std::find(tags.begin(), tags.end(), TRAVEL) == tags.end()) {
                Tag tag = TRAVEL;
                tags.push_back(tag);
            }
        } else if (decision == 'C' || decision == 'c') {
            if (std::find(tags.begin(), tags.end(), HOME) == tags.end()) {
                Tag tag = HOME;
                tags.push_back(tag);
            }
        } else if (decision == 'D' || decision == 'd') {
            if (std::find(tags.begin(), tags.end(), EVENT) == tags.end()) {
                Tag tag = EVENT;
                tags.push_back(tag);
            }
        } else if (decision == 'E' || decision == 'e') {
            if (std::find(tags.begin(), tags.end(), SCHOOL) == tags.end()) {
                Tag tag = SCHOOL;
                tags.push_back(tag);
            }
        } else if (decision == 'F' || decision == 'f') {
            if (std::find(tags.begin(), tags.end(), FUN) == tags.end()) {
                Tag tag = FUN;
                tags.push_back(tag);
            }
        } else if (decision == 'G' || decision == 'g') {
            return tags; //note does not have to have a tag
        }
    }
}

Importance Notepad::SetImportance(WINDOW *window) {
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 2, 10, "Choose importance: ");
    mvwprintw(window, 3, 10, "A - not important ");
    mvwprintw(window, 4, 10, "B - important ");
    wrefresh(window);
    while (true) {
        int decision = getch();
        if (decision == 'A' || decision == 'a') {
            return NOT_IMPORTANT;
        } else if (decision == 'B' || decision == 'b') {
            return IMPORTANT;
        }
    }
}

Category Notepad::SetCategory(WINDOW *window) {
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 2, 10, "Choose category: ");
    mvwprintw(window, 3, 10, "A - Text note");
    mvwprintw(window, 4, 10, "B - Shopping list");
    mvwprintw(window, 5, 10, "C - Task list");
    wrefresh(window);
    while (true) {
        int decision = getch();
        if (decision == 'A' || decision == 'a') {
            return TEXT;
        } else if (decision == 'B' || decision == 'b') {
            return SHOPPING_LIST;
        } else if (decision == 'C' || decision == 'c') {
            return TASK_LIST;
        }
    }
}

Category Notepad::GetCategory(const std::string &str) {
    if (str == "TEXT") return TEXT;
    else if (str == "TASK_LIST") return TASK_LIST;
    else if (str == "SHOPPING_LIST") return SHOPPING_LIST;
    return NONE;
}

Importance Notepad::GetImportance(const std::string &str) {
    if (str == "IMPORTANT") return IMPORTANT;
    return NOT_IMPORTANT;
}

Tag Notepad::GetTag(const std::string &str) {
    if (str == "WORK") return WORK;
    else if (str == "TRAVEL") return TRAVEL;
    else if (str == "HOME") return HOME;
    else if (str == "EVENT") return EVENT;
    else if (str == "FUN") return FUN;
    else return SCHOOL;
}
