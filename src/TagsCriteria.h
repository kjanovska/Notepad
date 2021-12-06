#include "ImportanceCriteria.h"
#include<algorithm>

class TagsCriteria : public Criteria {
private:
    std::vector<Tag> tags;
public:
    TagsCriteria() {
        this -> tags.clear();
    }
    ~TagsCriteria() = default;
    /**
    * overridden method, user chooses tags one of which searched notes have to have
    * @param window
    */
    void GetSpecifics(WINDOW *window) override;
    /**
    * overridden method, find all notes of given tags in given vector of notes
    * @param notes
    * @return vector containing all notes from given tags that are in param notes
    */
    std::vector<std::shared_ptr<Note>> Find(const std::vector<std::shared_ptr<Note>> &notes) override;
};
