#include "ShoppingList.h"
#include <string>
#include <vector>
#include <zconf.h>
#include <iostream>

std::vector<ShoppingListItem> ShoppingList::ItemList() const {
    return this->itemList;
}

unsigned int ShoppingList::TotalSum() {
    this->totalSum = 0;
    for (const ShoppingListItem &item : this->itemList)
        this->totalSum += item.Sum();
    return this->totalSum;
}

ShoppingList::ShoppingList(std::string &newName, std::vector<Tag> &newTags, Importance &newImportance)
        : Note(newName, newTags, newImportance) {
    this->totalSum = 0;
    this->category = SHOPPING_LIST;
}

std::vector<std::string> ShoppingList::ParseNoteForSaving() {
    std::vector<std::string> parsedNote = this->ParseHeader();
    for (const ShoppingListItem &item : this->itemList) {
        parsedNote.emplace_back(item.name);
        parsedNote.emplace_back(std::to_string(item.price));
        parsedNote.emplace_back(std::to_string(item.count));
    }
    parsedNote.emplace_back(Hash(this->name));

    return parsedNote;
}

void ShoppingList::ExportTxt() {
    std::ofstream output;
    output.open("examples/export/" + this->name + ".txt");

    output << this->name << std::endl;
    for (const ShoppingListItem &item : this->itemList) {
        output << "Name: " << item.name << std::endl;
        output << "Price: " << item.price << ",-" << std::endl;
        output << "Count:  " << item.count << std::endl;
        output << "Sum: " << item.Sum() << ",-" << std::endl;
    }
    output << "Total sum = " << this->TotalSum() << ",-" << std::endl;
    output.close();
}

void ShoppingList::ExportMarkdown() {
    std::ofstream output;
    output.open("examples/export/" + this->name + ".md");

    output << "## " << this->name << std::endl;
    for (const ShoppingListItem &item : this->itemList) {
        output << "- Name: **" << item.name << "**" << std::endl;
        output << "\t- Price: " << item.price << ",-" << std::endl;
        output << "\t- Count:" << item.count << std::endl;
        output << "\t- Sum: " << item.Sum() << ",-" << std::endl;
    }
    output << std::endl << "#### Total sum = " << this->TotalSum() << ",-" << std::endl;
    output.close();
}

void ShoppingList::NewItem(WINDOW *window) {
    ShoppingListItem newItem;
    char name[40];
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 1, 10, "New shopping list item");
    mvwprintw(window, 2, 10, "Set name: ");
    wrefresh(window);
    echo();
    wgetnstr(window, name, 40);
    std::string str(name);
    newItem.name = str;
    // input price
    char number[10];
    while (true) {
        werase(window);
        box(window, 0, 0);
        mvwprintw(window, 2, 10, "Set price: ");
        wrefresh(window);
        wgetnstr(window, number, 10);
        try {
            newItem.price = std::stoi(number);
            break;
        } catch (std::exception const &ex) {
            mvwprintw(window, 3, 10, "Invalid number, try again");
            wrefresh(window);
            sleep(1);
        }
    }
    // input quantity
    char quantity[10];
    while (true) {
        werase(window);
        box(window, 0, 0);
        mvwprintw(window, 2, 10, "Set quantity: ");
        wrefresh(window);
        wgetnstr(window, quantity, 10);
        try {
            newItem.count = std::stoi(quantity);
            break;
        } catch (std::exception const &ex) {
            mvwprintw(window, 3, 10, "Invalid number, try again");
            wrefresh(window);
            sleep(1);
        }
    }
    this->itemList.push_back(newItem);
}

bool ShoppingList::ParseNoteFromLoading(
        const std::vector<std::string> &textToParse) { // header by uz v tu dobu mel byt urcen
    if (textToParse.size() % 3 != 0) return false; // name - price - count
    for (unsigned int current = 0; current < textToParse.size(); current += 3) {
        ShoppingListItem newItem;
        if (current < textToParse.size()) newItem.name = textToParse[current]; //first of the three
        try {
            if (current < textToParse.size()) newItem.price = std::stoi(textToParse[current + 1]);
        } catch (std::exception const &ex) { //not a number
            return false;
        }
        try {
            if (current < textToParse.size()) newItem.count = std::stoi(textToParse[current + 2]);
        } catch (std::exception const &ex) {
            return false;
        }
        this->itemList.push_back(newItem);
    }
    return true;
}

void ShoppingList::ShowAndWriteNote(WINDOW *window) {
    unsigned int firstItemIndex = 0;
    while (true) {
        this->NoteInterface(window);
        this->PrintItems(window,
                         firstItemIndex); // aka vyprinti se list itemy firstitemIndex - tolik, kolik vyska okna dovoli - window - line - 3 nebo tak nejak
        noecho();
        int character = wgetch(window);
        if (character == KEY_RIGHT) {
            this->NewItem(window);
            if (this->itemList[this->itemList.size() - 1].name.empty())
                this->itemList[this->itemList.size() - 1].name = "item" + std::to_string(this->itemList.size());
        } else if (character == KEY_BACKSPACE) return;
        else if (character == KEY_UP && firstItemIndex > 0) firstItemIndex--;
        else if (character == KEY_DOWN && firstItemIndex < this->itemList.size() - 1) firstItemIndex++;
    }
}

void ShoppingList::NoteInterface(WINDOW *window) {
    this->CommonInterface(window);
    mvwprintw(window, 7, 4, "Items:");
    mvwprintw(window, 8, 5, "Name\t\t\t   Price\t  Count");
    mvwprintw(window, WIN_HEIGHT - 2, 5, "Total sum = ");
    mvwprintw(window, WIN_HEIGHT - 2, 17, std::to_string(this->TotalSum()).data());
    wrefresh(window);
}


void ShoppingList::PrintItems(WINDOW *window, unsigned int &firstItemIndex) {
    int line = 9;
    for (unsigned int i = firstItemIndex; i < this->itemList.size(); i++) {
        if (line < WIN_HEIGHT - 3) {
            mvwprintw(window, line, 5, this->itemList[i].name.data());
            std::string str = std::to_string(this->itemList[i].price);
            mvwprintw(window, line, 35, str.data());
            str = std::to_string(this->itemList[i].count);
            mvwprintw(window, line, 50, str.data());
            line++;
        }
    }
    wrefresh(window);
}

bool ShoppingList::ImportNote(std::ifstream &file) {
    std::string line;
    if (!getline(file, line) || line.empty()) return false;
    else this->name = line;
    while (getline(file, line)) {
        ShoppingListItem item;
        std::string tmp;
        if (line.size() < 6 || line.substr(0, 5) != "Name:") {
            return line.substr(0, 12) == "Total sum = ";
        }
        item.name = line.substr(6, line.size() - 6);
        if (!getline(file, line) || line.size() < 7 || line.substr(0, 6) != "Price:") return false;
        try {
            item.price = std::stoi(line.substr(7, line.size() - 7 - 2));
        } catch (std::exception const &ex) {
            return false;
        }
        if (!getline(file, line) || line.size() < 7 || line.substr(0, 6) != "Count:") return false;
        try {
            item.count = std::stoi(line.substr(7, line.size() - 7 - 2));
        } catch (std::exception const &ex) {
            return false;
        }
        if (!getline(file, line)) return false;
        this->itemList.push_back(item);
    }
    return true;
}

bool ShoppingList::HasText(const std::string &str) {
    size_t position = this->name.find(str);
    if (position != std::string::npos)
        return true;
    for (const ShoppingListItem &item : this->itemList) {
        // searching for substring in a string which is item.name
        position = item.name.find(str);
        if (position != std::string::npos)
            return true; // first occurrence is fine as the whole note is then viewed
    }
    return false;
}



