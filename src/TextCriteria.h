#include "TagsCriteria.h"

class TextCriteria : public Criteria {
private:
    std::string text;
public:
    TextCriteria() {
        this->text = "";
    };

    ~TextCriteria() = default;
    /**
     * overridden method, user chooses text searched notes have to contain
     * @param window
     */
    void GetSpecifics(WINDOW *window) override;
    /**
     * overridden method, find all notes containing given text in given vector of notes
     * @param notes
     * @return vector containing all notes containing given text that are in param notes
     */
    std::vector<std::shared_ptr<Note>> Find(const std::vector<std::shared_ptr<Note>> &notes) override;

};


