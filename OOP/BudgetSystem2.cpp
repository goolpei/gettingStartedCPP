#include <iostream>
#include <string>

// ==========================================
// 1. ABSTRACTION (Abstract Base Class)
// ==========================================
class BudgetSector {
protected: 
    std::string sectorName;
    double allocatedBudget;
    double spentBudget;

public:
    BudgetSector(std::string name, double budget) 
        : sectorName(name), allocatedBudget(budget), spentBudget(0.0) {}

    virtual ~BudgetSector() {
        std::cout << "[Base Destructor] Releasing core ledger resources for: " << sectorName << "\n";
    }

    virtual void generateReport() const = 0;
    
    void executeSpending(double amount) {
        if (amount <= (allocatedBudget - spentBudget)) {
            spentBudget += amount;
            std::cout << ">> Approved: Spend of $" << amount << " processed for " << sectorName << ".\n";
        } else {
            std::cout << ">> Denied: Insufficient funds in " << sectorName << " for $" << amount << " request.\n";
        }
    }

    void executeSpending(double amount, std::string emergencyReason) {
        spentBudget += amount;
        std::cout << ">> APPROVED [EMERGENCY AUTHORIZATION - " << emergencyReason << "]: Allocated $" 
                  << amount << " to " << sectorName << " immediately.\n";
    }

    BudgetSector& operator+=(double stimulusAmount) {
        this->allocatedBudget += stimulusAmount;
        std::cout << ">> Stimulus Applied: Added $" << stimulusAmount << " to the " << sectorName << " total allocation.\n";
        return *this;
    }

    // Helper functions added to manually extract whole and fractional numbers for formatting
    long long getWholeDollars() const { return static_cast<long long>(spentBudget); }
    int getCents() const { 
        // Extracts the two digits after the decimal point
        double fractional = spentBudget - static_cast<long long>(spentBudget);
        return static_cast<int>(fractional * 100 + 0.5); // 0.5 prevents rounding dropoff errors
    }
};

// ==========================================
// 2. INHERITANCE: Derived Classes
// ==========================================
class HealthcareSector : public BudgetSector {
private:
    int hospitalCount;
public:
    HealthcareSector(double budget, int hospitals) 
        : BudgetSector("Healthcare", budget), hospitalCount(hospitals) {}

    ~HealthcareSector() override {
        std::cout << "[Derived Destructor] Cleaning up Healthcare-specific database links.\n";
    }

    void generateReport() const override {
        std::cout << "\n=========================================\n"
                  << "        NATIONAL HEALTHCARE REPORT        \n"
                  << "=========================================\n"
                  << "Hospitals Operational : " << hospitalCount << "\n"
                  << "Total Budget Allocated: $" << allocatedBudget << "\n"
                  << "Total Budget Spent    : $" << spentBudget << "\n" 
                  << "=========================================\n";
    }
};

class InfrastructureSector : public BudgetSector {
private:
    int activeMegaProjects;
public:
    InfrastructureSector(double budget, int projects) 
        : BudgetSector("Infrastructure", budget), activeMegaProjects(projects) {}

    ~InfrastructureSector() override {
        std::cout << "[Derived Destructor] Cleaning up Infrastructure asset inventory records.\n";
    }

    void generateReport() const override {
        std::cout << "\n=========================================\n"
                  << "      INFRASTRUCTURE DEVELOPMENT REPORT   \n"
                  << "=========================================\n"
                  << "Active Mega-Projects  : " << activeMegaProjects << "\n"
                  << "Total Budget Allocated: $" << allocatedBudget << "\n"
                  << "Total Budget Spent    : $" << spentBudget << "\n"
                  << "=========================================\n";
    }
};

// ==========================================
// MAIN EXECUTION ENGINE (No vector, No iomanip)
// ==========================================
int main() {
    std::cout << "--- INITIALIZING GOVERNMENT BUDGET TRACKING SYSTEM ---\n\n";

    // ----------------------------------------------------
    // REPLACING STD::VECTOR: C-Style Array of Pointers
    // ----------------------------------------------------
    // We must manually define a rigid tracker capacity size up front
    const int MAX_SECTORS = 2; 
    BudgetSector* nationalLedger[MAX_SECTORS];

    // Manually assigning index slots
    nationalLedger[0] = new HealthcareSector(50000000.00, 142);
    nationalLedger[1] = new InfrastructureSector(120000000.00, 24);

    std::cout << "--- STEP 1: DEMONSTRATING FUNCTION OVERLOADING ---\n";
    nationalLedger[0]->executeSpending(12000000.00); 
    // Triggering an emergency spent that results in cents (.75) to test replacement formatting later
    nationalLedger[0]->executeSpending(45000000.75, "Pandemic Supply Chain Shock"); 
    
    nationalLedger[1]->executeSpending(95000000.00);
    std::cout << "\n";

    std::cout << "--- STEP 2: DEMONSTRATING OPERATOR OVERLOADING ---\n";
    *nationalLedger[1] += 15000000.00;
    std::cout << "\n";

    std::cout << "--- STEP 3: DEMONSTRATING RUNTIME POLYMORPHISM ---\n";
    // Traditional raw indexing loop instead of container iterators
    for (int i = 0; i < MAX_SECTORS; ++i) {
        nationalLedger[i]->generateReport();
    }
    std::cout << "\n";

    // ----------------------------------------------------
    // REPLACING IOMANIP: Custom formatting logic
    // ----------------------------------------------------
    std::cout << "--- STEP 3B: MANUAL FORMATTING OUTSIDE IOMANIP ---\n";
    // Without iomanip, trailing zeros drop off ($57000000.75 vs standard unformatted double display)
    std::cout << "Healthcare raw double print: $" << 57000000.75 << "\n";
    
    // Instead, we manipulate manually using custom class methods to separate dollars and cents:
    std::cout << "Healthcare manual clean print: $" 
              << nationalLedger[0]->getWholeDollars() << ".";
    if (nationalLedger[0]->getCents() < 10) {
        std::cout << "0"; // Handle manual padding if cents is single-digit (e.g. .05)
    }
    std::cout << nationalLedger[0]->getCents() << "\n\n";

    std::cout << "--- STEP 4: CLEANUP & VIRTUAL DESTRUCTOR TEST ---\n";
    // Deleting raw array pointers manually
    for (int i = 0; i < MAX_SECTORS; ++i) {
        delete nationalLedger[i];
        std::cout << "-----------------------------------------\n";
    }

    return 0;
}