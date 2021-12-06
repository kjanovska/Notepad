#inspired by bernhdav
CXX = g++
CXX_FLAGS = -Wall -pedantic -Wextra -std=c++14
BUILD_DIR = build

.PHONY: all
all: compile doc

.PHONY: compile
compile: janovkri

.PHONY: run
run: janovkri
	./janovkri

.PHONY: clean
clean:
	rm -rf  janovkri $(BUILD_DIR)/ 2>/dev/null

doc: doc src/Application.h src/CategoryCriteria.h src/Criteria.h src/ImportanceCriteria.h \
	src/Note.h src/Notepad.h src/ShoppingList.h src/ShoppingListItem.h src/TagsCriteria.h \
	src/TaskItem.h src/TaskList.h src/TextCriteria.h src/TextNote.h
	doxygen Doxyfile

janovkri: $(BUILD_DIR)/Application.o $(BUILD_DIR)/CategoryCriteria.o $(BUILD_DIR)/ImportanceCriteria.o $(BUILD_DIR)/main.o $(BUILD_DIR)/Note.o $(BUILD_DIR)/Notepad.o $(BUILD_DIR)/ShoppingList.o $(BUILD_DIR)/TagsCriteria.o $(BUILD_DIR)/TaskList.o $(BUILD_DIR)/TextCriteria.o $(BUILD_DIR)/TextNote.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ -lncurses

$(BUILD_DIR)/%.o: src/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $< -c -o $@

$(BUILD_DIR)/Application.o: src/Application.cpp src/Application.h src/Notepad.h src/TaskList.h src/ShoppingList.h src/TextNote.h \
	src/CategoryCriteria.h src/TextCriteria.h src/TagsCriteria.h src/ImportanceCriteria.h src/Criteria.h src/Note.h \
	src/ShoppingListItem.h src/TaskItem.h
$(BUILD_DIR)/CategoryCriteria.o: src/CategoryCriteria.cpp src/CategoryCriteria.h \
	src/TextCriteria.h src/TagsCriteria.h src/ImportanceCriteria.h \
	src/Criteria.h src/Note.h
$(BUILD_DIR)/ImportanceCriteria.o: src/ImportanceCriteria.cpp src/ImportanceCriteria.h \
	src/Criteria.h src/Note.h
$(BUILD_DIR)/main.o: src/main.cpp src/Application.h src/Notepad.h src/TaskList.h \
	src/ShoppingList.h src/TextNote.h src/CategoryCriteria.h \
	src/TextCriteria.h src/TagsCriteria.h src/ImportanceCriteria.h \
	src/Criteria.h src/Note.h src/ShoppingListItem.h src/TaskItem.h
$(BUILD_DIR)/Note.o: src/Note.cpp src/Note.h
$(BUILD_DIR)/Notepad.o: src/Notepad.cpp src/Notepad.h src/TaskList.h \
	src/ShoppingList.h src/TextNote.h src/CategoryCriteria.h \
	src/TextCriteria.h src/TagsCriteria.h src/ImportanceCriteria.h \
	src/Criteria.h src/Note.h src/ShoppingListItem.h src/TaskItem.h
$(BUILD_DIR)/ShoppingList.o: src/ShoppingList.cpp src/ShoppingList.h src/TextNote.h \
	src/CategoryCriteria.h src/TextCriteria.h src/TagsCriteria.h \
	src/ImportanceCriteria.h src/Criteria.h src/Note.h \
	src/ShoppingListItem.h
$(BUILD_DIR)/TagsCriteria.o: src/TagsCriteria.cpp src/TagsCriteria.h \
	src/ImportanceCriteria.h src/Criteria.h src/Note.h
$(BUILD_DIR)/TaskList.o: src/TaskList.cpp src/TaskList.h src/ShoppingList.h \
	src/TextNote.h src/CategoryCriteria.h src/TextCriteria.h \
	src/TagsCriteria.h src/ImportanceCriteria.h src/Criteria.h src/Note.h \
	src/ShoppingListItem.h src/TaskItem.h
$(BUILD_DIR)/TextCriteria.o: src/TextCriteria.cpp src/TextCriteria.h \
	src/TagsCriteria.h src/ImportanceCriteria.h src/Criteria.h src/Note.h
$(BUILD_DIR)/TextNote.o: src/TextNote.cpp src/TextNote.h src/CategoryCriteria.h \
	src/TextCriteria.h src/TagsCriteria.h src/ImportanceCriteria.h \
	src/Criteria.h src/Note.h
