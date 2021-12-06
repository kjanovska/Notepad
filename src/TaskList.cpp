#include "TaskList.h"

std::vector<std::string> TaskList::ParseNoteForSaving() {
    std::vector<std::string> parsedNote = this->ParseHeader();
    for (const TaskItem &item : this->taskList) {
        parsedNote.emplace_back(item.name);
        parsedNote.emplace_back(item.text);
    }
    parsedNote.emplace_back(Hash(this->name));
    return parsedNote;
}

void TaskList::ExportTxt() {
    std::ofstream output;
    output.open("examples/export/" + this->name + ".txt");

    output << this->name << std::endl;
    for (const TaskItem &item : this->taskList) {
        output << item.name << std::endl;
        output << item.text << std::endl;
    }
    output.close();
}

void TaskList::ExportMarkdown() {
    std::ofstream output;
    output.open("examples/export/" + this->name + ".md");

    output << "## " << this->name << std::endl;
    for (const TaskItem &item : this->taskList) {
        output << "- **" << item.name << "**" << std::endl;
        output << "\t- " << item.text << std::endl;
    }
    output.close();
}

bool TaskList::ParseNoteFromLoading(const std::vector<std::string> &textToParse) { // 2 lines == 1 task
    if (textToParse.size() % 2 != 0) return false; // name - price - count
    for (unsigned int current = 0; current < textToParse.size(); current += 2) {
        TaskItem newItem;
        newItem.name = textToParse[current]; //first of the three
        newItem.text = textToParse[current + 1];
        this->taskList.push_back(newItem);
    }
    return true;
}

void TaskList::NewTask(WINDOW *window) {
    TaskItem newTask;
    char name[40];
    char content[WIN_WIDTH - 2];
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 1, 10, "New task");
    mvwprintw(window, 2, 1, "Set name: ");
    wrefresh(window);
    echo();
    wgetnstr(window, name, 40);
    std::string str(name);
    newTask.name = str;
    mvwprintw(window, 3, 1, "Set content: ");
    mvwprintw(window, 4, 1, "");
    wrefresh(window);
    echo();
    wgetnstr(window, content, WIN_WIDTH - 2);
    std::string con(content);
    newTask.text = con;

    this->taskList.push_back(newTask);
}

void TaskList::ShowAndWriteNote(WINDOW *window) {
    unsigned int firstItemIndex = 0;
    while (true) {
        this->NoteInterface(window);
        this->PrintItems(window, firstItemIndex);
        noecho();
        int character = wgetch(window);
        if (character == KEY_RIGHT) {
            this->NewTask(window);
            if (this->taskList[this->taskList.size() - 1].name.empty())
                this->taskList[this->taskList.size() - 1].name = "task" + std::to_string(this->taskList.size());
        } else if (character == KEY_BACKSPACE) return;
        else if (character == KEY_UP && firstItemIndex > 0) firstItemIndex--;
        else if (character == KEY_DOWN && firstItemIndex < this->taskList.size() - 1) firstItemIndex++;
    }
}

void TaskList::NoteInterface(WINDOW *window) {
    this->CommonInterface(window);
    mvwprintw(window, 2, WIN_WIDTH - 25, "Right arrow - New Task");
    mvwprintw(window, 7, 4, "Tasks:");
    wrefresh(window);
}

void TaskList::PrintItems(WINDOW *window, unsigned int &firstItemIndex) {
    int line = 8;
    for (unsigned int i = firstItemIndex; i < this->taskList.size(); i++) {
        if (line < WIN_HEIGHT - 3) {
            mvwprintw(window, line, 5, this->taskList[i].name.data());
            line++;
            mvwprintw(window, line, 10, this->taskList[i].text.data());
            line++;
        }
    }
    wrefresh(window);
}


TaskList::TaskList(std::string &newName, std::vector<Tag> &newTags, Importance &newImportance)
        : Note(newName, newTags, newImportance) {
    this->category = TASK_LIST;
}

bool TaskList::ImportNote(std::ifstream &file) {
    std::string line;
    if (!getline(file, line) || line.empty()) return false;
    else this->name = line;
    while (getline(file, line)) {
        TaskItem item;
        item.name = line;
        if (getline(file, line)) item.text = line;
        else {
            this->taskList.push_back(item);
            return true;
        }
        this->taskList.push_back(item);
    }
    return true;
}

bool TaskList::HasText(const std::string &str) {
    size_t position = this->name.find(str);
    if (position != std::string::npos)
        return true;
    for (const TaskItem &task : this->taskList) {
        position = task.name.find(str);
        if (position != std::string::npos)
            return true;
        position = task.text.find(str);
        if (position != std::string::npos)
            return true;
    }
    return false;
}


