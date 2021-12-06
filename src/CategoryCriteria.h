#include "TextCriteria.h"

class CategoryCriteria : public Criteria{
private:
    Category category;
public:
    CategoryCriteria(){
        this -> category = TEXT;
    }
    ~CategoryCriteria() = default;
    /**
     * overridden method, user chooses category searched notes have to have
     * @param window
     */
    void GetSpecifics(WINDOW *window) override;
    /**
     * overridden method, find all notes of given category in given vector of notes
     * @param notes
     * @return vector containing all notes from given category that are in param notes
     */
    std::vector<std::shared_ptr<Note>> Find(const std::vector<std::shared_ptr<Note>> &notes) override;
};

