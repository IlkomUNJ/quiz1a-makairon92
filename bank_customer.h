#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <string>

using namespace std;

class BankCustomer {
private:
    int id;
    string name;
    double balance;
    string address;
    string phoneNumber;

public:
    BankCustomer(int id, const string& name, double balance, const string& address, const string& phoneNumber)
        : id(id), name(name), balance(balance), address(address), phoneNumber(phoneNumber) {}

    int getId() const;
    string getName() const;
    double getBalance() const;
    string getAddress() const { return address; } 
    string getPhoneNumber() const { return phoneNumber; } 

    void printInfo() const;
    void setName(const string& name);
    void setBalance(double balance);
    void addBalance(double amout);
    bool withdrawBalance(double amout);
};

#endif // BANK_CUSTOMER_H