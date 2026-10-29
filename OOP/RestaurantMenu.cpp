#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Base Class
// Encapsulation
class MenuItem {
private:
    string name;
    double price;
public:
    MenuItem(string n = "", double p = 0.0){
        name = n;
        price = p;
    }
    // Virtual Destructor
    virtual ~MenuItem() {}

    string getName() const { return name; }
    double getPrice() const { return price; }
    // Pure Virtual Function
    virtual void displayCategory() const = 0;
    // Virtual Function
    virtual void printDetails() const {
        cout << left << setw(25) << name << "- PHP " << price << endl;
    }

};

//Derive Class
// Inheritance
class MainCourse : public MenuItem {
public:
    MainCourse (string n, double p) : MenuItem(n, p) {}
    void displayCategory() const override {
        cout << "[*] Main Course Selection [*]" << endl;
    }
};
class Dessert : public MenuItem {
public:
    Dessert (string n, double p) : MenuItem(n, p) {}
    void displayCategory() const override {
        cout << "[*] Dessert Selection [*]" << endl;
    }
};
class Beverage : public MenuItem {
public:
    Beverage (string n, double p) : MenuItem(n, p) {}
    void displayCategory() const override {
        cout << "[*] Beverage Selection [*]" << endl;
    }
};
class Appetizer : public MenuItem {
public:
    Appetizer (string n, double p) : MenuItem(n, p) {}
    void displayCategory() const override {
        cout << "[*] Appetizer Selection [*]" << endl;
    }
};

class Receipt {
private:
    double total;
public:
    Receipt() : total(0.0) {}
    // Operator Overloading
    Receipt operator+(const MenuItem* item){
        Receipt temp;
        if (item != nullptr){
            temp.total = this->total + item->getPrice();
        }
        return temp;
    }
    double getTotal() const { return total; }
};

class OrderManager {
public:

    // Function Overloading
    void processSelection(const MenuItem* items[], int size){
        items[0]->displayCategory();
        
        for (int i = 0; i < size; i++) {
            cout << "\t" << (i + 1) << ". ";
            items[i]->printDetails();
        }
    }
    void processSelection(string prompt){
        cout << prompt;
    }
};


int main(){
    OrderManager manager;
    Receipt finalBill;
    // Polymorphism
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
        new Dessert("Halo-halo", 120),
        new Dessert("Leche Flan", 200),
        new Dessert("Buko Pandan", 110),
        new Dessert("Ube Ice Cream", 150)
    };


    const MenuItem* chosenAppetizer = nullptr;
    const MenuItem* chosenMainCourse = nullptr;
    const MenuItem* chosenBeverage = nullptr;
    const MenuItem* chosenDessert = nullptr;

    int choice;

    cout << "****Orig Filipino Resto****" << endl;
    cout << "------Restaurant Menu------" << endl;

    manager.processSelection(appetizers, 4);
    manager.processSelection("[-] Pick Appetizer: ");
    cin >> choice;
    if(choice >= 1 && choice <= 4) chosenAppetizer = appetizers[choice - 1];
    cout << endl;

    manager.processSelection(mainCourses, 4);
    manager.processSelection("[-] Pick Main Course: ");
    cin >> choice;
    if(choice >= 1 && choice <= 4) chosenMainCourse = mainCourses[choice - 1];
    cout << endl;

    manager.processSelection(beverages, 4);
    manager.processSelection("[-] Pick Beverage: ");
    cin >> choice;
    if(choice >= 1 && choice <= 4) chosenBeverage = beverages[choice - 1];
    cout << endl;

    manager.processSelection(desserts, 4);
    manager.processSelection("[-] Pick Dessert: ");
    cin >> choice;
    if(choice >= 1 && choice <= 4) chosenDessert = desserts[choice - 1];
    cout << endl;
    
    finalBill = finalBill + chosenAppetizer;
    finalBill = finalBill + chosenBeverage;
    finalBill = finalBill + chosenDessert;
    finalBill = finalBill + chosenMainCourse;

    //Runtime Polymorphism
    cout << "****Takeout Summary****" << endl << endl;
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

    for (int i = 0; i < 4; i++){
        delete appetizers[i];
        delete mainCourses[i];
        delete beverages[i];
        delete desserts[i];
    }

    return 0;
}