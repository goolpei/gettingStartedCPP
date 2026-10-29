#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// 1. ENCAPSULATION & PURE VIRTUAL FUNCTION
// ==========================================
class MenuItem {
private:
    string name;
    double price;

public:
    MenuItem(string n = "", double p = 0.0) : name(n), price(p) {}

    // 9. VIRTUAL DESTRUCTOR (Prevents memory leaks in polymorphic deletion)
    virtual ~MenuItem() {}

    // Encapsulation: Getters and Setters
    string getName() const { return name; }
    double getPrice() const { return price; }
    
    // 7. PURE VIRTUAL FUNCTION (Makes this class Abstract)
    virtual void displayCategory() const = 0; 

    // 8. VIRTUAL FUNCTION
    virtual void printDetails() const {
        cout << name << "\t\t- PHP " << price << endl;
    }
};

// ==========================================
// 2. INHERITANCE
// ==========================================
class Appetizer : public MenuItem {
public:
    Appetizer(string n, double p) : MenuItem(n, p) {}
    
    // Implementing the pure virtual function
    void displayCategory() const override {
        cout << "[*] Appetizer Selection [*]" << endl;
    }
};

class MainCourse : public MenuItem {
public:
    MainCourse(string n, double p) : MenuItem(n, p) {}
    
    void displayCategory() const override {
        cout << "[*] Main Course Selection [*]" << endl;
    }
};

class Beverage : public MenuItem {
public:
    Beverage(string n, double p) : MenuItem(n, p) {}
    
    void displayCategory() const override {
        cout << "[*] Beverage Selection [*]" << endl;
    }
};

class Dessert : public MenuItem {
public:
    Dessert(string n, double p) : MenuItem(n, p) {}
    
    void displayCategory() const override {
        cout << "[*] Dessert Selection [*]" << endl;
    }
};

// ==========================================
// 5. OPERATOR OVERLOADING
// ==========================================
class Receipt {
private:
    double total;
public:
    Receipt() : total(0.0) {}
    
    // Overloading '+' operator to easily add MenuItem prices to the total
    Receipt operator+(const MenuItem* item) {
        Receipt temp;
        if (item != nullptr) {
            temp.total = this->total + item->getPrice();
        }
        return temp;
    }
    
    double getTotal() const { return total; }
};

// ==========================================
// 4. FUNCTION OVERLOADING
// ==========================================
class OrderManager {
public:
    // Version 1: Handles normal selection display using polymorphism
    void processSelection(const MenuItem* items[], int size) {
        items[0]->displayCategory();
        for (int i = 0; i < size; i++) {
            cout << "\t" << (i + 1) << ". " << items[i]->getName() << "\t\t- PHP " << items[i]->getPrice() << endl;
        }
    }

    // Version 2: Overloaded function to handle custom prompt text
    void processSelection(string prompt) {
        cout << prompt;
    }
};

// ==========================================
// MAIN FUNCTION
// ==========================================
int main() {
    OrderManager manager;
    Receipt finalBill;

    // Hardcoded Menu Items categorized using Base class pointers (Polymorphism)
    const MenuItem* appetizers[] = {
        new Appetizer("Chicharon", 180),
        new Appetizer("Kinilaw", 280),
        new Appetizer("Lumpia", 180),
        new Appetizer("Calamares", 260)
    };

    const MenuItem* mainCourses[] = {
        new MainCourse("Adobo", 250),
        new MainCourse("Sinigang", 350),
        new MainCourse("Kare-kare", 450),
        new MainCourse("Sizzling Sisig", 280)
    };

    const MenuItem* beverages[] = {
        new Beverage("Buko Juice", 90),
        new Beverage("Kalamansi Juice", 70),
        new Beverage("Mango Shake", 110),
        new Beverage("Tubig", 20)
    };

    const MenuItem* desserts[] = {
        new Dessert("Hallo-halo", 120),
        new Dessert("Leche Flan", 200),
        new Dessert("Buko Pandan", 110),
        new Dessert("Ube Ice Cream", 150)
    };

    // Selected items storage pointers
    const MenuItem* chosenAppetizer = nullptr;
    const MenuItem* chosenMainCourse = nullptr;
    const MenuItem* chosenBeverage = nullptr;
    const MenuItem* chosenDessert = nullptr;

    int choice;

    cout << "****Orig Filipino Resto****" << endl;
    cout << "------Restaurant Menu------" << endl << endl;

    // 1. Appetizer Selection Flow
    manager.processSelection(appetizers, 4);
    manager.processSelection("[-] Pick Appetizer: ");
    cin >> choice;
    if(choice >= 1 && choice <= 4) chosenAppetizer = appetizers[choice - 1];
    cout << endl;

    // 2. Main Course Selection Flow
    manager.processSelection(mainCourses, 4);
    manager.processSelection("[-] Pick Main Course: ");
    cin >> choice;
    if(choice >= 1 && choice <= 4) chosenMainCourse = mainCourses[choice - 1];
    cout << endl;

    // 3. Beverage Selection Flow
    manager.processSelection(beverages, 4);
    manager.processSelection("[-] Pick Beverage: ");
    cin >> choice;
    if(choice >= 1 && choice <= 4) chosenBeverage = beverages[choice - 1];
    cout << endl;

    // 4. Dessert Selection Flow
    manager.processSelection(desserts, 4);
    manager.processSelection("[-] Pick Dessert: ");
    cin >> choice;
    if(choice >= 1 && choice <= 4) chosenDessert = desserts[choice - 1];
    cout << endl;

    // Calculate total using Operator Overloading
    finalBill = finalBill + chosenAppetizer;
    finalBill = finalBill + chosenMainCourse;
    finalBill = finalBill + chosenBeverage;
    finalBill = finalBill + chosenDessert;

    // ==========================================
    // 3 & 6. RUNTIME POLYMORPHISM IN ACTION
    // ==========================================
    cout << "****Takeout Summary****" << endl;
    
    cout << "Appetizer: ";
    if(chosenAppetizer) cout << chosenAppetizer->getName() << ", PHP " << chosenAppetizer->getPrice() << endl;
    else cout << "None" << endl;

    cout << "Main Course: ";
    if(chosenMainCourse) cout << chosenMainCourse->getName() << ", PHP " << chosenMainCourse->getPrice() << endl;
    else cout << "None" << endl;

    cout << "Beverage: ";
    if(chosenBeverage) cout << chosenBeverage->getName() << ", PHP " << chosenBeverage->getPrice() << endl;
    else cout << "None" << endl;

    cout << "Dessert: ";
    if(chosenDessert) cout << chosenDessert->getName() << ", PHP " << chosenDessert->getPrice() << endl;
    else cout << "None" << endl;

    cout << endl << "Total: PHP " << finalBill.getTotal() << endl;

    // Cleanup allocated memory
    for (int i = 0; i < 4; i++) {
        delete appetizers[i];
        delete mainCourses[i];
        delete beverages[i];
        delete desserts[i];
    }

    return 0;
}