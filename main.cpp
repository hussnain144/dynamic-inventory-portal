#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#ifdef _WIN32
#include <windows.h> // For Windows
#else
#include <unistd.h> // For Linux/Mac
#endif
using namespace std;

// Function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Linux/Mac
#endif
}

class Item {
public:
    int id;
    string name;
    int quantity;

    Item(int Id = 0, string Name = "", int Quantity = 0)
        : id(Id), name(Name), quantity(Quantity) {}
};

class Inventory {
private:
    Item* items;
    int size;
    int capacity;
    int lowStockLimit;

public:
    Inventory(int limit, int initialCapacity = 10)
        : size(0), capacity(initialCapacity), lowStockLimit(limit) {
        items = new Item[capacity];
    }

    ~Inventory() {
        delete[] items;
    }

    void addItem() {
        clearScreen(); // Clear screen when function starts
        if (size >= capacity) {
            resizeArray();
        }

        int id, quantity;
        string name;
        cout << "Enter Item ID: ";
        cin >> id;
        cout << "Enter Item Name: ";
        cin >> name;
        cout << "Enter Quantity: ";
        cin >> quantity;

        items[size++] = Item(id, name, quantity);
        cout << "Item added successfully!\n";
    }

    void deleteItem() {
        clearScreen(); // Clear screen when function starts
        int id;
        cout << "Enter Item ID to delete: ";
        cin >> id;

        for (int i = 0; i < size; ++i) {
            if (items[i].id == id) {
                for (int j = i; j < size - 1; ++j) {
                    items[j] = items[j + 1];
                }
                --size;
                cout << "Item deleted successfully!\n";
                return;
            }
        }
        cout << "Item not found!\n";
    }

    void displayAllItems() {
        clearScreen(); // Clear screen when function starts
        if (size == 0) {
            cout << "No items in the inventory.\n";
            return;
        }

        cout << "ID\tName\t\tQuantity\n";
        for (int i = 0; i < size; ++i) {
            cout << items[i].id << "\t" << items[i].name << "\t\t" << items[i].quantity << "\n";
        }
    }

    void searchItemById() {
        clearScreen(); // Clear screen when function starts
        int id;
        cout << "Enter Item ID to search: ";
        cin >> id;

        for (int i = 0; i < size; ++i) {
            if (items[i].id == id) {
                cout << "ID: " << items[i].id << "\n";
                cout << "Name: " << items[i].name << "\n";
                cout << "Quantity: " << items[i].quantity << "\n";
                return;
            }
        }
        cout << "Item not found!\n";
    }

    void lowStockAlert() {
        clearScreen(); // Clear screen when function starts
        cout << "Enter low stock limit: ";
        cin >> lowStockLimit;

        cout << "Items with quantity below " << lowStockLimit << ":\n";
        bool found = false;

        for (int i = 0; i < size; ++i) {
            if (items[i].quantity < lowStockLimit) {
                cout << "ID: " << items[i].id << ", Name: " << items[i].name << ", Quantity: " << items[i].quantity << "\n";
                found = true;
            }
        }

        if (!found) {
            cout << "No items below the limit.\n";
        }
    }

    void recordSale() {
        clearScreen(); // Clear screen when function starts
        int id, quantity;
        cout << "Enter Item ID to sell: ";
        cin >> id;
        cout << "Enter Quantity to sell: ";
        cin >> quantity;

        for (int i = 0; i < size; ++i) {
            if (items[i].id == id) {
                if (items[i].quantity >= quantity) {
                    items[i].quantity -= quantity;
                    cout << "Sale recorded successfully!\n";
                    saveTransaction("Sale", id, quantity);
                } else {
                    cout << "Insufficient stock!\n";
                }
                return;
            }
        }
        cout << "Item not found!\n";
    }

    void recordPurchase() {
        clearScreen(); // Clear screen when function starts
        int id, quantity;
        cout << "Enter Item ID to purchase: ";
        cin >> id;
        cout << "Enter Quantity to purchase: ";
        cin >> quantity;

        for (int i = 0; i < size; ++i) {
            if (items[i].id == id) {
                items[i].quantity += quantity;
                cout << "Purchase recorded successfully!\n";
                saveTransaction("Purchase", id, quantity);
                return;
            }
        }
        cout << "Item not found!\n";
    }

    void saveToFile() {
        clearScreen(); // Clear screen when function starts
        ofstream outFile("inventory.txt");
        if (!outFile) {
            cout << "Error opening file for saving!\n";
            return;
        }

        for (int i = 0; i < size; ++i) {
            outFile << items[i].id << " " << items[i].name << " " << items[i].quantity << "\n";
        }
        outFile.close();
        cout << "Inventory saved to file successfully!\n";
    }

private:
    void resizeArray() {
        capacity *= 2;
        Item* newItems = new Item[capacity];
        for (int i = 0; i < size; ++i) {
            newItems[i] = items[i];
        }
        delete[] items;
        items = newItems;
    }

    void saveTransaction(const string& type, int id, int quantity) {
        ofstream outFile("transactions.txt", ios::app);
        if (!outFile) {
            cout << "Error opening transactions file!\n";
            return;
        }

        outFile << type << " - ID: " << id << ", Quantity: " << quantity << "\n";
        outFile.close();
    }
};

void displayWelcomeMessage() {
    const int consoleWidth = 60; // Adjust based on your console width
    string welcomeMessage = "Welcome to the Dynamic Inventory Portal";

    // Calculate padding to center the message
    int padding = (consoleWidth - welcomeMessage.length()) / 2;

    // Display the centered welcome message
    cout << string(consoleWidth, '*') << "\n";
    cout << string(padding, ' ') << welcomeMessage << "\n";
    cout << string(consoleWidth, '*') << "\n";
    cout << "\n";
}

void displayMenu() {
    cout << "\nDynamic Inventory Portal\n";
    cout << "1. Add Item\n";
    cout << "2. Delete Item\n";
    cout << "3. Display All Items\n";
    cout << "4. Search Item by ID\n";
    cout << "5. Low Stock Alert\n";
    cout << "6. Record Sale\n";
    cout << "7. Record Purchase\n";
    cout << "8. Save Inventory to File\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    displayWelcomeMessage(); // Display the centered welcome message

    Inventory obj(0); // Initialize with a default low stock limit (will be set later)
    int choice;

    do {
        displayMenu(); // Display the menu
        cin >> choice;

        switch (choice) {
            case 1:
                obj.addItem();
                break;
            case 2:
                obj.deleteItem();
                break;
            case 3:
                obj.displayAllItems();
                break;
            case 4:
                obj.searchItemById();
                break;
            case 5:
                obj.lowStockAlert();
                break;
            case 6:
                obj.recordSale();
                break;
            case 7:
                obj.recordPurchase();
                break;
            case 8:
                obj.saveToFile();
                break;
            case 9:
                cout << "Exiting program. Thank you for using the Dynamic Inventory Portal!\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }

        // Pause and wait for user input before clearing the screen
        if (choice != 9) {
            cout << "\nPress Enter to continue...";
            cin.ignore(); // Ignore any leftover newline character
            cin.get();    // Wait for user to press Enter
            clearScreen(); // Clear the screen before redisplaying the menu
        }
    } while (choice != 9);

    return 0;
}