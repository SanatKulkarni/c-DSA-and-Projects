#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <sstream>
#include <vector>
#include <algorithm>  // Added for remove_if

using namespace std;

unsigned int hashString(const string &str) {
    unsigned int hash = 0;
    for (char ch : str) {
        hash = hash * 121212139 + ch;
    }
    return hash;
}




void registerUser() {
    string username, password;
    cout << "Enter a username: ";
    cin >> username;

    ifstream file("users.txt");
    string line;
    bool usernameExists = false;
    while (getline(file, line)) {
        size_t pos = line.find(' ');
        if (pos != string::npos) {
            string storedUsername = line.substr(0, pos);
            if (username == storedUsername) {
                usernameExists = true;
                break;
            }
        }
    }
    file.close();

    if (usernameExists) {
        cout << "Username already exists. Please choose a different username." << endl;
    } else {
        cout << "Enter a password: ";
        cin >> password;

        unsigned int hashedPassword = hashString(password);

        ofstream outFile("users.txt", ios::app);
        outFile << username << " " << hashedPassword << endl;
        outFile.close();

        cout << "Registration successful! Hashed Password: " << hashedPassword << endl;
    }
}





struct ShopItem {
    string name;
    double price;
};

struct TreeNode {
    ShopItem item;
    TreeNode* left;
    TreeNode* right;
    TreeNode(const ShopItem& item) : item(item), left(nullptr), right(nullptr) {}
};

class ShopBST {
private:
    TreeNode* root;

    void insert(TreeNode*& node, const ShopItem& item) {
        if (node == nullptr) {
            node = new TreeNode(item);
        } else if (item.name < node->item.name) {
            insert(node->left, item);
        } else if (item.name > node->item.name) {
            insert(node->right, item);
        }
    }

    TreeNode* search(TreeNode* node, const string& itemName) {
        if (node == nullptr || node->item.name == itemName) {
            return node;
        }
        if (itemName < node->item.name) {
            return search(node->left, itemName);
        } else {
            return search(node->right, itemName);
        }
    }

    void inorderTraversal(TreeNode* node, vector<ShopItem>& items) {
        if (node != nullptr) {
            inorderTraversal(node->left, items);
            items.push_back(node->item);
            inorderTraversal(node->right, items);
        }
    }

    void destroyTree(TreeNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    ShopBST() : root(nullptr) {}

    void insert(const ShopItem& item) {
        insert(root, item);
    }

    ShopItem* search(const string& itemName) {
        TreeNode* result = search(root, itemName);
        return (result != nullptr) ? &(result->item) : nullptr;
    }

    vector<ShopItem> getShopItems() {
        vector<ShopItem> items;
        inorderTraversal(root, items);
        return items;
    }

    ~ShopBST() {
        destroyTree(root);
    }
};

// Global shop instance
ShopBST shopBST;

void addItemToShop() {
    ShopItem newItem;
    cout << "Enter item name: ";
    cin >> newItem.name;

    // Check if the item already exists
    if (shopBST.search(newItem.name) != nullptr) {
        cout << "Item already exists. Choose a different name." << endl;
        return;
    }

    cout << "Enter item price: ";
    cin >> newItem.price;
    shopBST.insert(newItem);
    cout << "Item added successfully!" << endl;
}

void removeItemFromShop() {
    string itemName;
    cout << "Enter the name of the item to remove: ";
    cin >> itemName;

    // Check if the item exists
    ShopItem* itemToRemove = shopBST.search(itemName);
    if (itemToRemove == nullptr) {
        cout << "Item not found in the shop." << endl;
        return;
    }

    // Remove the item from the BST
    vector<ShopItem> currentShopItems = shopBST.getShopItems();
    currentShopItems.erase(
        remove_if(currentShopItems.begin(), currentShopItems.end(),
                  [itemName](const ShopItem& item) { return item.name == itemName; }),
        currentShopItems.end());

    // Rebuild the BST with the updated items
    shopBST = ShopBST();
    for (const auto& item : currentShopItems) {
        shopBST.insert(item);
    }

    cout << "Item removed successfully!" << endl;
}

void searchItemInShop() {
    string itemName;
    cout << "Enter the name of the item to search: ";
    cin >> itemName;

    // Search for the item in the BST
    ShopItem* foundItem = shopBST.search(itemName);

    if (foundItem != nullptr) {
        cout << "Item found in the shop. Price: " << foundItem->price << endl;
    } else {
        cout << "Item not found in the shop." << endl;
    }
}

void displayShop(const vector<ShopItem>& shopItems) {
    cout << "Available items in the shop:" << endl;
    cout << setw(20) << "Item" << setw(10) << "Price" << endl;
    for (const auto& item : shopItems) {
        cout << setw(20) << item.name << setw(10) << item.price << endl;
    }
}

vector<ShopItem> loadShopFromFile() {
    vector<ShopItem> shopItems;
    ifstream inFile("shop_items.txt");
    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        ShopItem newItem;
        iss >> newItem.name >> newItem.price;
        shopItems.push_back(newItem);
    }
    inFile.close();
    return shopItems;
}

void saveShopToFile(const vector<ShopItem>& shopItems) {
    ofstream outFile("shop_items.txt");
    for (const auto& item : shopItems) {
        outFile << item.name << " " << item.price << endl;
    }
    outFile.close();
}

bool loginUser() {
    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    unsigned int hashedPassword = hashString(password);

    ifstream file("users.txt");
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(' ');
        if (pos != string::npos) {
            string storedUsername = line.substr(0, pos);
            string hashedPasswordStr = line.substr(pos + 1);

            try {
                unsigned int storedHashedPassword;
                istringstream(hashedPasswordStr) >> storedHashedPassword;

                if (username == storedUsername && hashedPassword == storedHashedPassword) {
                    file.close();
                    return true;
                }
            } catch (const std::invalid_argument& e) {
                cerr << "Error: Invalid password format in the file." << endl;
                file.close();
                return false;
            } catch (const std::out_of_range& e) {
                cerr << "Error: Out of range exception during password conversion." << endl;
                file.close();
                return false;
            }
        }
    }
    file.close();

    return false;
}

void showSpinner(int durationInSeconds) {
    cout << "Logging in";
    for (int i = 0; i < durationInSeconds; ++i) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << endl;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void manageShop() {
    // Load shop items from the file into the BST
    vector<ShopItem> shopItems = loadShopFromFile();
    for (const auto& item : shopItems) {
        shopBST.insert(item);
    }

    int shopChoice;
    do {
        cout << "Shop Management: " << endl;
        cout << "1. Add Item" << endl;
        cout << "2. Remove Item" << endl;
        cout << "3. Search Item" << endl;
        cout << "4. Display Items" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> shopChoice;

        switch (shopChoice) {
            case 1:
                addItemToShop();
                break;
            case 2:
                removeItemFromShop();
                break;
            case 3:
                searchItemInShop();
                break;
            case 4:
                displayShop(shopBST.getShopItems());
                break;
            case 0:
                cout << "Exiting shop management." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (shopChoice != 0);

    // Save shop items from the BST to the file
    shopItems = shopBST.getShopItems();
    saveShopToFile(shopItems);
}

int main() {
    int choice;

    do {
        cout << "Choose an option:" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    cout << "Login successful!" << endl;
                    showSpinner(2);
                    clearScreen();
                    // Directly go to shop management after successful login
                    manageShop();
                } else {
                    cout << "Invalid username or password. Please try again." << endl;
                }
                break;
            case 0:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (choice != 0);
    return 0;
}
