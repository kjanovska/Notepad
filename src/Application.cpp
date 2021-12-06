#include "Application.h"
#include <string.h>
#include <zconf.h>

void Application::Run() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    this->MainMenu();
    endwin();
}

void Application::NewNotepad(WINDOW *window) {
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 1, 10, "New notepad");
    if (this->notepads.size() > 9) {
        mvwprintw(window, 2, 10, "Reached maximum amount of notepads");
        mvwprintw(window, 5, 10, "A - back");
        while (true) {
            int decision = getch();
            if (decision == 'A' || decision == 'a') return;
        }
    }
    while (true) {
        if (this->SetNewNotepadName(window)) break;
    }
    wrefresh(window);
    this->notepads[notepads.size() - 1].NotepadMenu(window);
}

bool Application::SetNewNotepadName(WINDOW *window) {
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 1, 10, "New notepad");
    char name[20];
    mvwprintw(window, 2, 10, "Set name: ");
    wrefresh(window);
    echo();
    wgetnstr(window, name, 20);
    std::string str(name);
    if (str.empty()) {
        unsigned int index = this->notepads.size();
        while (true) {
            str = "notepad" + std::to_string(index);
            bool foundMatch = false;
            for (Notepad &notepad : this->notepads)
                if (notepad.Name() == str) {
                    foundMatch = true;
                    index++;
                    break;
                }
            if (!foundMatch) {
                break;
            }
        }
    } else {
        for (Notepad &notepad : this->notepads) {
            if (notepad.Name() == str) {
                mvwprintw(window, 3, 1, "Notepad of that name already exists! Try again.");
                wrefresh(window);
                sleep(2);
                return false;
            }
        }
    }
    this->notepads.emplace_back(str);
    mvwprintw(window, 4, 10, name);
    return true;
}


void Application::BrowseNotepads(WINDOW *window) {
    while (true) {
        this->BrowseInterface(window);
        echo();
        unsigned int decision = wgetch(window);
        if (decision == KEY_BACKSPACE) return;
        else if (decision - '0' < this->notepads.size()) {
            NotepadChoicesInterface(window);
            while (true) {
                if (this->NotepadChoices(window, decision)) break;
            }
        }
    }
}

void Application::BrowseInterface(WINDOW *window) {
    werase(window);
    keypad(window, true);
    noecho();
    box(window, 0, 0);
    mvwprintw(window, 2, 10, "Backspace - Back");
    mvwprintw(window, 3, 10, "Press a number to view a notepad:");
    int line = 4;
    for (unsigned int i = 0; i < this->notepads.size(); i++) {
        std::string noteNumber = std::to_string(i);
        mvwprintw(window, line, 5, noteNumber.data());
        mvwprintw(window, line, 10, this->notepads[i].Name().data());
        line++;
    }
    wrefresh(window);
}

bool Application::NotepadChoices(WINDOW *window, const unsigned int &decision) {
    unsigned int d2 = wgetch(window);
    if (d2 == 'A' || d2 == 'a') {
        this->notepads[decision - '0'].NotepadMenu(window);
        return true;
    } else if (d2 == 'B' || d2 == 'b') {
        this->notepads.erase(this->notepads.begin() + decision - '0');
        std::string a = "examples/savefiles/" + this->notepads[decision - '0'].Name() + ".txt";
        remove(a.data());
        this->SaveNotepads();
        return true;
    } else return false;
}

void Application::NotepadChoicesInterface(WINDOW *window) {
    werase(window);
    box(window, 0, 0);
    noecho();
    mvwprintw(window, 2, 5, "What do you wish to do?");
    mvwprintw(window, 3, 10, "A - show notepad");
    mvwprintw(window, 4, 10, "B - delete notepad");
    wrefresh(window);
}


void Application::MainMenu() {
    int start_y = 0;
    int start_x = 0;
    WINDOW *window = newwin(WIN_HEIGHT, WIN_WIDTH, start_y, start_x);
    refresh();
    this->Load(window);
    while (true) {
        werase(window);
        box(window, 0, 0);
        mvwprintw(window, 1, 10, "NOTEPAD");
        mvwprintw(window, 2, 10, "press a key: ");
        mvwprintw(window, 3, 10, "A - select notepad");
        mvwprintw(window, 4, 10, "B - new notepad");
        mvwprintw(window, 6, 10, "C - exit");
        wrefresh(window);
        int c = getch();
        wrefresh(window);
        if (c == 'C' || c == 'c') {
            return;
        } else if (c == 'A' || c == 'a') {
            this->BrowseNotepads(window);
        } else if (c == 'B' || c == 'b') {
            this->NewNotepad(window);
            this->SaveNotepads();
        }
    }
}

bool Application::Load(WINDOW *window) {
    bool success = true;
    std::ifstream input;
    input.open("examples/savefiles/savefile.txt");
    if (input.bad()) return false;
    std::string line;
    std::vector<std::string> notepadNames;
    while (std::getline(input, line)) {
        if (!line.empty())
            notepadNames.push_back(line);
    }
    for (std::string name : notepadNames) {
        this->notepads.emplace_back(name);
        success = this->notepads[this->notepads.size() - 1].LoadNotes();
    }
    if (!success) {
        werase(window);
        box(window, 0, 0);
        mvwprintw(window, 2, 20, "Oh no!");
        mvwprintw(window, 3, 1, "It appears some of the save files may have been corrupted.");
        mvwprintw(window, 4, 10, "Some of the notes may not have been loaded.");
        wrefresh(window);
        sleep(4);
    }
    return success;
}

void Application::SaveNotepads() {
    std::ofstream output;
    output.open("examples/savefiles/savefile.txt");
    for (Notepad n : this->notepads)
        output << n.Name() << std::endl;
}