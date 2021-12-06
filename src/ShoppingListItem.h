#include <string>
/*!
 * represents one item on a shopping list
 */
struct ShoppingListItem {
    std::string name;
    unsigned int price;
    unsigned int count;

    /**
     * count sum of one particular item in the list
     * @return sum = price * count
     */
    unsigned int Sum() const {return this -> price * this -> count;}
};

