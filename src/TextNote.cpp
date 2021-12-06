#include <iostream>
#include "TextNote.h"

std::vector<std::string> TextNote::ParseNoteForSaving() {
    std::vector<std::string> parsedNote = this->ParseHeader();
    for (const std::string &paragraph : this->paragraphs)
        parsedNote.emplace_back(paragraph);
    parsedNote.emplace_back(Hash(this->name));
    return parsedNote;
}

void TextNote::ExportTxt() {
    std::ofstream output;
    output.open("examples/export/" + this->name + ".txt");

    output << this->name << std::endl;
    for (const std::string &paragraph : this->paragraphs)
        output << paragraph << std::endl;
    output.close();
}

void TextNote::ExportMarkdown() {
    std::ofstream output;
    output.open("examples/export/" + this->name + ".md");
    output << "## " << this->name << std::endl;
    for (const std::string &paragraph : this->paragraphs)
        output << paragraph << std::endl << std::endl;
    output.close();
}

bool TextNote::ParseNoteFromLoading(const std::vector<std::string> &textToParse) { // 1 line == 1 paragraph
    this->paragraphs = textToParse;
    return true;
}

void TextNote::ShowAndWriteNote(WINDOW *window) {
    unsigned int firstLineIndex = 0;
    while (true) {
        this->NoteInterface(window);
        this->PrintItems(window, firstLineIndex);
        noecho();
        int character = wgetch(window);
        if (character == KEY_RIGHT) this->NewParagraph(window);
        else if (character == KEY_BACKSPACE) return;
        else if (character == KEY_UP && firstLineIndex > 0) firstLineIndex--;
        else if (character == KEY_DOWN && firstLineIndex < this->paragraphs.size() - 1) firstLineIndex++;
    }
}

void TextNote::NoteInterface(WINDOW *window) {
    this->CommonInterface(window);
    mvwprintw(window, 2, WIN_WIDTH - 25, "R. arrow - New Paragraph");
    wrefresh(window);
}

unsigned int TextNote::PrintItems(WINDOW *window, unsigned int &firstItemIndex) {
    int paraNumber = 8;
    int position = 3;

    for (unsigned int i = firstItemIndex;
         i < this->paragraphs.size(); i++) {
        if (paraNumber < WIN_HEIGHT - 3) {
            mvwprintw(window, paraNumber, position, this->paragraphs[i].data());
            wrefresh(window);
            paraNumber++;
            int size = this->paragraphs[i].size();
            while (size > WIN_WIDTH - 2) {
                paraNumber++;
                size -= WIN_WIDTH - 2;
            }
        }
    }
    return paraNumber;
}

TextNote::TextNote(std::string &newName, std::vector<Tag> &newTags, Importance &newImportance)
        : Note(newName, newTags, newImportance) {
    this->category = TEXT;
}

bool TextNote::ImportNote(std::ifstream &file) {
    std::string line;
    if (!getline(file, line) || line.empty()) return false;
    else this->name = line;
    while (getline(file, line)) {
        this->paragraphs.push_back(line);
    }
    return true;
}

void TextNote::NewParagraph(WINDOW *window) {
    char text[150];
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 1, 10, "New paragraph");
    mvwprintw(window, 2, 10, "Set text: ");
    wrefresh(window);
    echo();
    wgetnstr(window, text, 150);
    std::string newParagraph(text);

    this->paragraphs.push_back(newParagraph);
}

bool TextNote::HasText(const std::string &str) {
    size_t position = this->name.find(str);
    if (position != std::string::npos)
        return true;
    for (const std::string &paragraph : this->paragraphs) {
        position = paragraph.find(str);
        if (position != std::string::npos)
            return true;
    }
    return false;
}

