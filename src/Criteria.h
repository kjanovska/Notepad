#include <vector>
#include <memory>
#include "Note.h"

class Criteria {

public:
    Criteria() = default;
    ~Criteria() = default;

    /**
     * makes user choose a criteria by which he/she wants to search
     * @param window
     */
    virtual void GetSpecifics(WINDOW * window) = 0;
    /**
     * searches across the given notes vector for notes fulfilling given criteria, returns those notes
     * @param notes vector to be searched
     * @return results satisfying given criteria
     */
    virtual std::vector<std::shared_ptr<Note>> Find(const std::vector<std::shared_ptr<Note>> &notes) = 0;

};

