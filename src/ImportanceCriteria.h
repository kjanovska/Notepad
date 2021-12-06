#include "Criteria.h"

class ImportanceCriteria : public Criteria{
private:
    Importance importance;
public:
    ImportanceCriteria() {
        this -> importance = NOT_IMPORTANT;
    }
    ~ImportanceCriteria() = default;
    /**
    * overridden method, user chooses importance searched notes have to have
    * @param window
    */
    void GetSpecifics(WINDOW *window) override;
    /**
    * overridden method, find all notes of given importance in given vector of notes
     * @param notes
    * @return vector containing all notes from given importance that are in param notes
    */
    std::vector<std::shared_ptr<Note>> Find(const std::vector<std::shared_ptr<Note>> &notes) override;

};

