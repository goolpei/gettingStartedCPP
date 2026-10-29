#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

class Project {
private:
    string name;
    double expense;

public:
    Project(string n = "Unnamed", double e = 0.0) : name(n), expense(e) {}

    string getName() const { return name; }
    double getExpense() const { return expense; }

    friend ostream& operator<<(ostream& os, const Project& p) {
        os << "    [-] Project: " << left << setw(25) << p.name 
           << " | Expense: PHP " << fixed << setprecision(2) << p.expense;
        return os;
    }
};

class GovernmentSector {
protected:
    string sectorName;
    vector<Project> projects;

public:
    GovernmentSector(string name) : sectorName(name) {}

    virtual ~GovernmentSector() {
        cout << "[System] Cleaning up resources for " << sectorName << "...\n";
    }

    virtual void printSectorHeader() const = 0;

    void printSectorReport() const {
        printSectorHeader(); 
        
        if (projects.empty()) {
            cout << "    [No projects recorded for this sector]\n";
        } else {
            for (const auto& p : projects) cout << p << "\n";
        }
        
        cout << "  ----------------------------------------------\n";
        cout << "  TOTAL SECTOR EXPENSES: PHP " << calculateTotalSectorExpenses() << "\n";
    }

    void addProject(const string& projName, double projExpense) {
        projects.push_back(Project(projName, projExpense));
    }

    void addProject(const Project& p) {
        projects.push_back(p);
    }

    double calculateTotalSectorExpenses() const {
        double total = 0;
        for (const auto& p : projects) {
            total += p.getExpense();
        }
        return total;
    }

    string getSectorName() const { return sectorName; }
    int getProjectCount() const { return projects.size(); }
};

class HealthSector : public GovernmentSector {
public:
    HealthSector() : GovernmentSector("Department of Health (DOH)") {}
    void printSectorHeader() const override {
        cout << "\n[MEDICINE & CARE] " << sectorName << "\n";
    }
};

class EducationSector : public GovernmentSector {
public:
    EducationSector() : GovernmentSector("Department of Education (DepEd)") {}
    void printSectorHeader() const override {
        cout << "\n[FUTURE & EDUCATION] " << sectorName << "\n";
    }
};

class DefenseSector : public GovernmentSector {
public:
    DefenseSector() : GovernmentSector("Department of National Defense (DND)") {}
    void printSectorHeader() const override {
        cout << "\n[SECURE & PROTECT] " << sectorName << "\n";
    }
};

int main() {
    vector<GovernmentSector*> trackingSystem;
    trackingSystem.push_back(new HealthSector());
    trackingSystem.push_back(new EducationSector());
    trackingSystem.push_back(new DefenseSector());

    int choice = 0;

    cout << "|************************************************|\n";
    cout << "| GOVERNMENT BUDGET TRACKING SYSTEM INITIALIZED  |\n";
    cout << "| Access Level: Head of Budget                   |\n";
    cout << "**************************************************";

    while (true) {
        cout << "\n--- MAIN MENU ---\n";
        cout << "Select a sector to add an expense:\n";
        for (size_t i = 0; i < trackingSystem.size(); ++i) {
            cout << " [" << i + 1 << "] " << trackingSystem[i]->getSectorName() 
                 << " (" << trackingSystem[i]->getProjectCount() << " Active Projects)\n";
        }
        cout << " [4] Exit and View Comprehensive Budget Summary\n";
        cout << "Enter choice (1-4): ";
        
        if (!(cin >> choice)) {
            cout << "[Error] Invalid input type. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 4) {
            break; 
        }

        if (choice >= 1 && choice <= 3) {
            int index = choice - 1;
            
            string name;
            double expense;

            cout << "\n>>> Adding Project to " << trackingSystem[index]->getSectorName() << "\n";
            while (true) {
                cout << "    Enter Project Name: ";
                getline(cin, name);
                if (!name.empty()) {
                    break;
                }
                cout << "    [Error] Project name cannot be empty. Please try again.\n";
            }

            cout << "    Enter Project Expense (PHP): ";
            while (!(cin >> expense) || expense <= 0 || (cin.peek() != '\n' && cin.peek() != EOF)) {
                cout << "    [Error] Invalid expense value. Enter a valid amount: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            trackingSystem[index]->addProject(name, expense);
            cout << "[Success] Project recorded successfully!\n";

        } else {
            cout << "[Error] Option out of range. Please choose a valid menu item.\n";
        }
    }

    cout << "\n\n==================================================";
    cout << "\n      GENERATING FINAL GOVERNMENT REPORT          ";
    cout << "\n==================================================\n";

    double grandTotal = 0.0;

    for (const auto& sector : trackingSystem) {
        sector->printSectorReport();
        grandTotal += sector->calculateTotalSectorExpenses();
    }

    cout << "\n==================================================\n";
    cout << " SYSTEM-WIDE GRAND TOTAL EXPENSES: PHP " << fixed << setprecision(2) << grandTotal << "\n";
    cout << "==================================================\n\n";

    for (auto sector : trackingSystem) {
        delete sector; 
    }

    return 0;
}