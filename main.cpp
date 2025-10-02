#include <iostream>
#include <vector>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "bank.h"

enum PrimaryPrompt{LOGIN, REGISTER, EXIT, ADMIN_LOGIN};
enum RegisterPrompt{CREATE_BUYER, CREATE_SELLER, BACK};
using namespace std;

void printBuyerDetails( Buyer& buyer) {
    cout << "---------------------------------" << endl;
    cout << "Buyer ID: " << buyer.getId() << endl; //
    cout << "Buyer Name: " << buyer.getName() << endl; //
    cout << "--- Linked Bank Account ---" << endl;
    buyer.getAccount().printInfo(); //
    cout << "---------------------------------" << endl;
}

void printSellerDetails( seller& currentSeller) {
    cout << "---------------------------------" << endl;
    cout << "Seller (User ID): " << currentSeller.getId() << endl; //
    cout << "Seller Name: " << currentSeller.getName() << endl; //
    cout << "Store ID: " << currentSeller.getSellerId() << endl; //
    cout << "Store Name: " << currentSeller.getSellerName() << endl; //
    
    cout << "--- Linked Bank Account ---" << endl;
    currentSeller.getAccount().printInfo(); //

    cout << "--- Store Inventory ---" << endl;
    const auto& items = currentSeller.getItems(); //
    if (items.empty()) {
        cout << "Inventory is empty." << endl;
    } else {
        for (const auto& item : items) {
            cout << "  - Item ID: " << item.getId() << ", Name: " << item.getName()
                 << ", Qty: " << item.getQuantity() << ", Price: $" << item.getPrice() << endl; //
        }
    }
    cout << "---------------------------------" << endl;
}

void createBankAccount(vector<BankCustomer>& accounts, int& nextId) {
    string name, address, phone;
    double initialBalance;

    cout << "\n--- Creating New Bank Account ---" << endl;
    cin.ignore(); // Clear input
    cout << "Enter Full Name: ";
    getline(cin, name);
    cout << "Enter Address: ";
    getline(cin, address);
    cout << "Enter Phone Number: ";
    getline(cin, phone);
    cout << "Enter Initial Deposit Amount: ";
    cin >> initialBalance;

    accounts.emplace_back(nextId, name, initialBalance, address, phone);
    cout << "Bank Account for " << name << " with ID " << nextId << " created successfully!" << endl;
    nextId++; 
}

void createBuyer(vector<Buyer>& buyers, vector<BankCustomer>& accounts, int& userId, int& bankId) {
    cout << "\nA new Buyer needs a new Bank Account." << endl;
    createBankAccount(accounts, bankId);

    // Use the details from the newly created bank account
    BankCustomer& newAccount = accounts.back(); // Get a reference to the last element added
    
    buyers.emplace_back(userId, newAccount.getName(), newAccount);
    cout << "Buyer profile for " << newAccount.getName() << " with User ID " << userId << " created successfully!" << endl;
    userId++;
}

void createSeller(vector<seller>& sellers, vector<BankCustomer>& accounts, int& userId, int& bankId, int& sellerId) {
    cout << "\nA new Seller needs a new Bank Account." << endl;
    createBankAccount(accounts, bankId);

    // Use the details from the new bank account
    BankCustomer& newAccount = accounts.back();

    string storeName;
    cout << "Enter Store Name: ";
    cin.ignore();
    getline(cin, storeName);

    // The seller constructor requires a Buyer object
    Buyer tempBuyerForSeller(userId, newAccount.getName(), newAccount);
    sellers.emplace_back(tempBuyerForSeller, sellerId, storeName);

    cout << "Seller profile for " << newAccount.getName() << " (Store: " << storeName << ") created successfully!" << endl;
    userId++;
    sellerId++;
}

int main() {
    vector<BankCustomer> allBankAccounts;
    vector<Buyer> allBuyers;
    vector<seller> allSellers;

    int nextBankId = 101;
    int nextUserId = 1;
    int nextSellerId = 201;
    //create a loop prompt 
    PrimaryPrompt prompt = LOGIN;
    RegisterPrompt regPrompt = CREATE_BUYER;
    const string ADMIN_USERNAME = "root";
    const string ADMIN_PASSWORD = "toor";
    string username, password;

    // sample
    while (prompt != EXIT) {
        cout << "Select an option: " << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "4. Admin Login" << endl;
        int choice;
        cin >> choice;
        prompt = static_cast<PrimaryPrompt>(choice - 1);
        switch (prompt) {
            case LOGIN:
                cout << "Login selected." << endl;
                /* if Login is selected, based on authority then provide options:
                assume user is logged in as Buyer for now
                1. Chek Account Status (will display if user is Buyer or Seller or both and linked banking account status)
                Will display Buyer, Seller and Banking Account details
                2. Upgrade Account to Seller
                Will prompt user to enter Seller details and create a Seller account linked to Buyer account
                Will reject if a user dont have a banking account linked
                3. Create Banking Account (if not already linked), will be replaced with banking functions
                Must provides: initial deposit amount, Address, Phone number, Email
                Banking functions will provides: Balance checking, Transaction History, Deposit, Withdraw
                4. Browse Store Functionality
                Will display all stores initially
                Need to select a store to browse each store inventory
                Will display all items in the store inventory
                After selecting an item, will display item details and option to add to cart
                After adding to cart, will notify user item is added to cart
                5. Order Functionality
                Will display all items in cart
                Will provide option to remove item from cart
                Will provide option to checkout
                After checkout invoide will be generated (will go to payment functionality)
                6. Payment Functionality
                Will display all listed invoices
                Pick an invoice to pay
                Will display invoice details and total amount
                Will provide option to pay invoice
                Payment is done through confirmation dialogue
                In confirmation dialogue, will display account balance as precursor
                User will need to manually enter invoice id to pay
                After paying balance will be redacted from buyer and added to the responding seller account
                After payment, order status will be changed to paid
                7. Logout (return to main menu)
                Display confirmation dialogue
                If confirmed, return to main menu
                If not, return to Buyer menu
                8. Delete Account (remove both Buyer and Seller account and relevant banking account)
                Display confirmation dialogue
                If confirmed, delete account and return to main menu
                If not, return to Buyer menu

                assume user is logged in as Seller for now
                9. Check Inventory
                10. Add Item to Inventory
                11. Remove Item from Inventory
                12. View Orders (will display all orders placed to this seller
                Only orders with paid status will be listed
                Order details will listing items, quantity, total amount, buyer details, order status (paid, cancelled, completed)
                extra functions
                9. Exit to main Menu
                10. Exit Program
                **/
                break;
            case REGISTER:
                regPrompt = CREATE_BUYER; // reset regPrompt to CREATE_BUYER when entering register menu
                while (regPrompt != BACK){
                    cout << "Register selected. " << endl;
                    cout << "Select an option: " << endl;
                    cout << "1. Create Buyer Account" << endl;
                    cout << "2. Create Seller Account" << endl;
                    cout << "3. Back" << endl;
                    int regChoice;
                    cin >> regChoice;
                    regPrompt = static_cast<RegisterPrompt>(regChoice - 1);
                    switch (regPrompt) {
                        case CREATE_BUYER:
                            cout << "Create Buyer Account selected." << endl;
                            break;
                        case CREATE_SELLER:
                            cout << "Create Seller Account selected." << endl;
                            break;
                        case BACK:
                            cout << "Back selected." << endl;
                            break;
                        default:
                            cout << "Invalid option." << endl;
                            break;
                    }
                }
                /* if register is selected then went throuhh registration process:
                1. Create a new Buyer Account
                Must provides: Name, Home Address, Phone number, Email
                2. Option to create a Seller Account (will be linked to Buyer account)
                Must Provides 1: Home Address, Phone number, Email
                Must provides 2: Store Name, Store Address, Store Phone number, Store Email
                Must provides 3: initial deposit amount
                After finished immediately logged in as Buyer/Seller
                */
                break;
            case EXIT:
                cout << "Exiting." << endl;
                break;
            case ADMIN_LOGIN:
                /* Prompt for username & password then check the entries with our hard coded features */
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;
                                // Check if login is successful
                if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
                    cout << "Admin login successful." << endl;
                    int adminChoice = 0;

                    // Admin sub-menu loop
                    while (adminChoice != 5) { 
                        cout << "\n--- Admin Menu ---" << endl;
                        cout << "1. View User Summary" << endl;
                        cout << "2. View User Details" << endl;
                        cout << "3. Search for a User" << endl;
                        cout << "4. Create New User/Account" << endl; 
                        cout << "5. Logout" << endl;                   
                        cout << "Select an option: ";
                        cin >> adminChoice;

                        switch (adminChoice) {
                            case 1:
                                // === VIEW ALL USERS LOGIC ===
                                cout << "\n--- All Buyers ---" << endl;
                                if (allBuyers.empty()) {
                                    cout << "No buyers found." << endl;
                                } else {
                                    for (const auto& buyer : allBuyers) {
                                        cout << "Buyer ID: " << buyer.getId() << ", Name: " << buyer.getName() << endl;
                                    }
                                }

                                cout << "\n--- All Sellers ---" << endl;
                                if (allSellers.empty()) {
                                    cout << "No sellers found." << endl;
                                } else {
                                    for (const auto& currentSeller : allSellers) {
                                        // A seller is a Buyer, so we can use getName() and getId()
                                        cout << "Seller (User ID): " << currentSeller.getId() << ", Name: " << currentSeller.getName() << endl;
                                    }
                                }
                                break;
                            case 2:
                                cout << "\n--- Detailed Buyer Information ---" << endl;
                                for (auto& buyer : allBuyers) {
                                    printBuyerDetails(buyer);
                                }
                                cout << "\n--- Detailed Seller Information ---" << endl;
                                for (auto& seller : allSellers) {
                                    printSellerDetails(seller);
                                }

                                break;
                            case 3: { // Use braces to create a local scope for variables
                                int searchChoice = 0;
                                cout << "\n--- Search Menu ---" << endl;
                                cout << "1. By Name" << endl;
                                cout << "2. By Account ID (User ID)" << endl;
                                cout << "3. By Address" << endl;
                                cout << "4. By Phone Number" << endl;
                                cout << "Select search criteria: ";
                                cin >> searchChoice;

                                string searchTerm;
                                int searchId;
                                bool found = false;

                                if (searchChoice == 2) {
                                    cout << "Enter Account ID to search for: ";
                                    cin >> searchId;
                                } else {
                                    cout << "Enter search term: ";
                                    cin.ignore(); // Important: clear the input buffer
                                    getline(cin, searchTerm);
                                }

                                cout << "\n--- Search Results ---" << endl;

                                // Search through buyers
                                for (  auto& buyer : allBuyers) {
                                    bool match = false;
                                    if (searchChoice == 1 && buyer.getName().find(searchTerm) != string::npos) match = true;
                                    if (searchChoice == 2 && buyer.getId() == searchId) match = true;
                                    if (searchChoice == 3 && buyer.getAccount().getAddress().find(searchTerm) != string::npos) match = true;
                                    if (searchChoice == 4 && buyer.getAccount().getPhoneNumber().find(searchTerm) != string::npos) match = true;
                                    
                                    if (match) {
                                        cout << "Found a match in Buyers:" << endl;
                                        printBuyerDetails(buyer);
                                        found = true;
                                    }
                                }

                                // Search through sellers
                                for (  auto& seller : allSellers) {
                                    bool match = false;
                                    if (searchChoice == 1 && seller.getName().find(searchTerm) != string::npos) match = true;
                                    if (searchChoice == 2 && seller.getId() == searchId) match = true;
                                    if (searchChoice == 3 && seller.getAccount().getAddress().find(searchTerm) != string::npos) match = true;
                                    if (searchChoice == 4 && seller.getAccount().getPhoneNumber().find(searchTerm) != string::npos) match = true;

                                    if (match) {
                                        cout << "Found a match in Sellers:" << endl;
                                        printSellerDetails(seller);
                                        found = true;
                                    }
                                }

                                if (!found) {
                                    cout << "No users found matching the criteria." << endl;
                                }
                                break;
                            }
                            case 4: {
                                int createChoice = 0;
                                cout << "\n--- Create Menu ---" << endl;
                                cout << "1. Create Bank Account Only" << endl;
                                cout << "2. Create New Buyer" << endl;
                                cout << "3. Create New Seller" << endl;
                                cout << "Select an option: ";
                                cin >> createChoice;

                                switch (createChoice) {
                                    case 1:
                                        createBankAccount(allBankAccounts, nextBankId);
                                        break;
                                    case 2:
                                        createBuyer(allBuyers, allBankAccounts, nextUserId, nextBankId);
                                        break;
                                    case 3:
                                        createSeller(allSellers, allBankAccounts, nextUserId, nextBankId, nextSellerId);
                                        break;
                                    default:
                                        cout << "Invalid creation choice." << endl;
                                        break;
                                }
                                break;
                            }

                            case 5:
                                cout << "Logging out from admin panel." << endl;
                                break;
                            default:
                                cout << "Invalid admin option." << endl;
                                break;
                        }
                    }
                } else {
                    cout << "Invalid admin credentials." << endl;
                }
                break;
                /** After login create a sub prompt that provides the following features
                1. Account Management
                    - View All Buyers, Sellers
                    - View All details of Buyers, Sellers
                    - Seek certain buyer of seller based on Name / account Id / address / phone number
                    - Create new buyer/seller/Bank account
                    - Remove buyer/seller based on ID (all related info will be deleted)
                2. System Report
                    - Total number of Buyers, Sellers
                    - Total number of Banking Accounts
                */
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
        cout << endl;
    }

    //BankCustomer customer1(1, "Alice", 1000.0);
    //Buyer buyer1(1, customer1.getName(), customer1);
    return 1;
}