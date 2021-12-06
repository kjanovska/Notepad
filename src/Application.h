#include "Notepad.h"

class Application {
private:
    std::vector<Notepad> notepads;
public:
    Application() = default;
    ~Application() = default;

    /**
     * initializes screen, cursor, cbreak, calls MainMenu
     */
    void Run();

    /**
     * first screen user sees after launch, lets user choose to select or create a notepad or exit the application
     */
    void MainMenu();

    /**
     * lets user make a new notepad if the max amount of notepad hasn't been already passed
     * @param window
     */
    void NewNotepad(WINDOW * window);
    /**
     * lets user choose a name for the new notepad
     * @param window
     */
    bool SetNewNotepadName(WINDOW *window);
    /**
     * saves notepad names into a main savefile
     */
    void SaveNotepads();

    /**
     * lets user choose a notepad out of already saved notepads
     * @param window
     */
    void BrowseNotepads(WINDOW * window);
    /**
     * helper function for BrowseNotepads, only cosmetic
     */
    void BrowseInterface(WINDOW * window);
    /**
    * helper function for BrowseNotepads, makes user choose to either view certain notepad or delete it.
     * Delete option also removes the notepad's save file
    */
    bool NotepadChoices(WINDOW * window, const uint & decision);
    /**
    * helper function for BrowseNotepads, only cosmetic
    */
    static void NotepadChoicesInterface(WINDOW * window);

    /**
     * takes each line from savefile as a notepad name, then tries to find a file of that name and load it as one notepad
     * @param window
     * @return true if load was successfull
     */
    bool Load(WINDOW *window);
};

