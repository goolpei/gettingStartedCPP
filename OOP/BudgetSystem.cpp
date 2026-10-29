/*
 * ============================================================
 *   GOVERNMENT BUDGET TRACKING SYSTEM
 *   Demonstrates: Inheritance, Encapsulation, Polymorphism,
 *                 Abstraction, Function Overloading,
 *                 Operator Overloading, Virtual Destructors,
 *                 Runtime Polymorphism (pure virtual functions)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stdexcept>

// ============================================================
//  UTILITY: Color codes for terminal output
// ============================================================
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string CYAN    = "\033[36m";
    const std::string MAGENTA = "\033[35m";
    const std::string WHITE   = "\033[37m";
    const std::string BG_DARK = "\033[40m";
}

// ============================================================
//  HELPER: Format large numbers with commas
// ============================================================
std::string formatMoney(double amount) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << amount;
    std::string s = oss.str();
    // Insert commas
    size_t dot = s.find('.');
    int insertPos = (int)dot - 3;
    while (insertPos > 0) {
        s.insert((size_t)insertPos, ",");
        insertPos -= 3;
    }
    return "PHP " + s;
}

// ============================================================
//  TRANSACTION RECORD  (Encapsulation)
// ============================================================
class Transaction {
private:
    std::string description;
    double      amount;
    std::string type;   // "credit" or "debit"
    std::string date;

public:
    Transaction(const std::string& desc, double amt,
                const std::string& t, const std::string& d)
        : description(desc), amount(amt), type(t), date(d) {}

    // Getters (Encapsulation)
    std::string getDescription() const { return description; }
    double      getAmount()      const { return amount; }
    std::string getType()        const { return type; }
    std::string getDate()        const { return date; }

    void display() const {
        std::string color = (type == "credit") ? Color::GREEN : Color::RED;
        std::string sign  = (type == "credit") ? "  +" : "  -";
        std::cout << "  " << Color::WHITE << std::left << std::setw(12) << date
                  << std::setw(35) << description
                  << color << sign << formatMoney(amount)
                  << Color::RESET << "\n";
    }
};

// ============================================================
//  BASE CLASS: BudgetDepartment  (Abstraction + Inheritance)
// ============================================================
class BudgetDepartment {
protected:
    std::string          name;
    std::string          departmentCode;
    double               allocatedBudget;
    double               spentAmount;
    std::vector<Transaction> transactions;
    int                  fiscalYear;

public:
    // Constructor
    BudgetDepartment(const std::string& n, const std::string& code,
                     double budget, int year)
        : name(n), departmentCode(code),
          allocatedBudget(budget), spentAmount(0.0), fiscalYear(year) {}

    // Virtual Destructor (required for safe polymorphic deletion)
    virtual ~BudgetDepartment() {
        std::cout << Color::CYAN << "  [Destructor] " << name
                  << " department record released." << Color::RESET << "\n";
    }

    // ── PURE VIRTUAL FUNCTIONS (Abstraction) ──────────────────
    virtual void   generateReport()         const = 0;
    virtual double calculatePenalty()       const = 0;
    virtual std::string getDepartmentType() const = 0;

    // ── VIRTUAL FUNCTIONS (Runtime Polymorphism) ──────────────
    virtual void recordExpense(const std::string& desc, double amount,
                               const std::string& date) {
        if (amount <= 0)
            throw std::invalid_argument("Expense amount must be positive.");
        if (spentAmount + amount > allocatedBudget)
            throw std::runtime_error("Budget limit exceeded!");
        spentAmount += amount;
        transactions.emplace_back(desc, amount, "debit", date);
    }

    virtual void recordIncome(const std::string& desc, double amount,
                              const std::string& date) {
        if (amount <= 0)
            throw std::invalid_argument("Income amount must be positive.");
        allocatedBudget += amount;
        transactions.emplace_back(desc, amount, "credit", date);
    }

    // ── FUNCTION OVERLOADING ──────────────────────────────────
    // Overload 1: Display summary (no args)
    void displaySummary() const {
        displaySummary(false);
    }
    // Overload 2: Display summary with optional full transaction list
    void displaySummary(bool showTransactions) const {
        double remaining = allocatedBudget - spentAmount;
        double usagePct  = (allocatedBudget > 0)
                           ? (spentAmount / allocatedBudget) * 100.0 : 0.0;

        std::cout << Color::BOLD << Color::CYAN
                  << "  ┌─────────────────────────────────────────────────┐\n"
                  << "  │  " << std::left << std::setw(45) << (name + "  [" + departmentCode + "]")
                  << "│\n"
                  << "  │  Type : " << std::setw(39) << getDepartmentType() << "│\n"
                  << "  │  Year : " << std::setw(39) << fiscalYear          << "│\n"
                  << "  └─────────────────────────────────────────────────┘\n"
                  << Color::RESET;

        std::cout << "  " << Color::WHITE  << "Allocated : " << Color::GREEN  << formatMoney(allocatedBudget) << "\n";
        std::cout << "  " << Color::WHITE  << "Spent     : " << Color::RED    << formatMoney(spentAmount)     << "\n";
        std::cout << "  " << Color::WHITE  << "Remaining : "
                  << (remaining >= 0 ? Color::GREEN : Color::RED)
                  << formatMoney(remaining) << Color::RESET << "\n";

        // Usage bar
        int barWidth  = 40;
        int filled    = std::min((int)(usagePct / 100.0 * barWidth), barWidth);
        std::cout << "  " << Color::WHITE << "Usage     : [";
        std::cout << (usagePct > 80 ? Color::RED : Color::GREEN);
        for (int i = 0; i < barWidth; ++i)
            std::cout << (i < filled ? "█" : "░");
        std::cout << Color::WHITE << "] " << std::fixed << std::setprecision(1)
                  << usagePct << "%" << Color::RESET << "\n";

        double penalty = calculatePenalty();
        if (penalty > 0)
            std::cout << "  " << Color::YELLOW << "⚠  Penalty Applied : "
                      << formatMoney(penalty) << Color::RESET << "\n";

        if (showTransactions && !transactions.empty()) {
            std::cout << "\n  " << Color::BOLD << Color::BLUE
                      << "  Transaction History:\n" << Color::RESET;
            std::cout << "  " << Color::WHITE
                      << std::left << std::setw(12) << "Date"
                      << std::setw(35) << "Description"
                      << "Amount" << Color::RESET << "\n";
            std::cout << "  " << std::string(60, '-') << "\n";
            for (const auto& t : transactions)
                t.display();
        }
        std::cout << "\n";
    }

    // Overload 3: Search transactions by keyword
    void displaySummary(const std::string& keyword) const {
        std::cout << Color::BOLD << Color::YELLOW
                  << "  Transactions matching \"" << keyword << "\":\n"
                  << Color::RESET;
        bool found = false;
        for (const auto& t : transactions) {
            std::string desc = t.getDescription();
            std::string kw   = keyword;
            // Case-insensitive search
            std::transform(desc.begin(), desc.end(), desc.begin(), ::tolower);
            std::transform(kw.begin(),   kw.end(),   kw.begin(),   ::tolower);
            if (desc.find(kw) != std::string::npos) {
                t.display();
                found = true;
            }
        }
        if (!found)
            std::cout << "  " << Color::RED << "No matching records found.\n" << Color::RESET;
        std::cout << "\n";
    }

    // ── OPERATOR OVERLOADING ──────────────────────────────────
    // (+)  Merge two departments' spending into a combined total
    BudgetDepartment& operator+(const BudgetDepartment& other) {
        allocatedBudget += other.allocatedBudget;
        spentAmount     += other.spentAmount;
        for (const auto& t : other.transactions)
            transactions.push_back(t);
        return *this;
    }

    // (>)  Compare spending percentages
    bool operator>(const BudgetDepartment& other) const {
        double myPct    = spentAmount / allocatedBudget;
        double otherPct = other.spentAmount / other.allocatedBudget;
        return myPct > otherPct;
    }

    // (==) Check same fiscal year
    bool operator==(const BudgetDepartment& other) const {
        return fiscalYear == other.fiscalYear;
    }

    // (<<) Stream output operator (declared friend)
    friend std::ostream& operator<<(std::ostream& os, const BudgetDepartment& dept);

    // ── GETTERS ───────────────────────────────────────────────
    std::string getName()           const { return name; }
    std::string getDepartmentCode() const { return departmentCode; }
    double getAllocatedBudget()      const { return allocatedBudget; }
    double getSpentAmount()         const { return spentAmount; }
    double getRemainingBudget()     const { return allocatedBudget - spentAmount; }
    int    getFiscalYear()          const { return fiscalYear; }
};

// Stream operator implementation
std::ostream& operator<<(std::ostream& os, const BudgetDepartment& dept) {
    os << Color::BOLD << "[" << dept.departmentCode << "] "
       << dept.name << Color::RESET
       << " | Allocated: " << formatMoney(dept.allocatedBudget)
       << " | Spent: "     << formatMoney(dept.spentAmount)
       << " | Year: "      << dept.fiscalYear;
    return os;
}

// ============================================================
//  DERIVED CLASS 1: HealthDepartment
// ============================================================
class HealthDepartment : public BudgetDepartment {
private:
    int    hospitalCount;
    double emergencyReserve;

public:
    HealthDepartment(const std::string& n, const std::string& code,
                     double budget, int year, int hospitals, double reserve)
        : BudgetDepartment(n, code, budget, year),
          hospitalCount(hospitals), emergencyReserve(reserve) {}

    ~HealthDepartment() override {
        std::cout << Color::GREEN << "  [HealthDepartment Destructor] "
                  << name << " resources freed.\n" << Color::RESET;
    }

    // Override pure virtual: Generate Report
    void generateReport() const override {
        std::cout << Color::BOLD << Color::GREEN
                  << "\n  ╔══════════════════════════════════════════════════╗\n"
                  << "  ║        HEALTH DEPARTMENT BUDGET REPORT           ║\n"
                  << "  ╚══════════════════════════════════════════════════╝\n"
                  << Color::RESET;
        displaySummary(true);
        std::cout << "  " << Color::CYAN << "Hospitals Covered  : " << hospitalCount   << "\n";
        std::cout << "  " << Color::CYAN << "Emergency Reserve  : " << formatMoney(emergencyReserve) << "\n";
        std::cout << "  " << Color::CYAN << "Effective Budget   : "
                  << formatMoney(allocatedBudget - emergencyReserve) << Color::RESET << "\n\n";
    }

    // Override pure virtual: Calculate Penalty
    // Penalty: 5% of overspent amount if reserve is dipped into
    double calculatePenalty() const override {
        double effective = allocatedBudget - emergencyReserve;
        if (spentAmount > effective)
            return (spentAmount - effective) * 0.05;
        return 0.0;
    }

    // Override pure virtual: Department Type
    std::string getDepartmentType() const override {
        return "Health & Medical Services";
    }

    // Override recordExpense: Health checks cannot exceed emergency reserve
    void recordExpense(const std::string& desc, double amount,
                       const std::string& date) override {
        if (spentAmount + amount > allocatedBudget - emergencyReserve * 0.5) {
            std::cout << Color::YELLOW
                      << "  ⚠  Warning: Nearing emergency reserve threshold!\n"
                      << Color::RESET;
        }
        BudgetDepartment::recordExpense(desc, amount, date);
    }
};

// ============================================================
//  DERIVED CLASS 2: EducationDepartment
// ============================================================
class EducationDepartment : public BudgetDepartment {
private:
    int    schoolCount;
    double scholarshipFund;

public:
    EducationDepartment(const std::string& n, const std::string& code,
                        double budget, int year, int schools, double scholarship)
        : BudgetDepartment(n, code, budget, year),
          schoolCount(schools), scholarshipFund(scholarship) {}

    ~EducationDepartment() override {
        std::cout << Color::BLUE << "  [EducationDepartment Destructor] "
                  << name << " resources freed.\n" << Color::RESET;
    }

    void generateReport() const override {
        std::cout << Color::BOLD << Color::BLUE
                  << "\n  ╔══════════════════════════════════════════════════╗\n"
                  << "  ║       EDUCATION DEPARTMENT BUDGET REPORT         ║\n"
                  << "  ╚══════════════════════════════════════════════════╝\n"
                  << Color::RESET;
        displaySummary(true);
        std::cout << "  " << Color::CYAN << "Schools Covered    : " << schoolCount      << "\n";
        std::cout << "  " << Color::CYAN << "Scholarship Fund   : " << formatMoney(scholarshipFund) << "\n";
        double costPerSchool = (schoolCount > 0)
                               ? (spentAmount / schoolCount) : 0.0;
        std::cout << "  " << Color::CYAN << "Avg Cost/School    : "
                  << formatMoney(costPerSchool) << Color::RESET << "\n\n";
    }

    // Penalty: 3% of surplus if more than 20% budget is unused (under-utilization)
    double calculatePenalty() const override {
        double unusedPct = 1.0 - (spentAmount / allocatedBudget);
        if (unusedPct > 0.20)
            return (allocatedBudget - spentAmount) * 0.03;
        return 0.0;
    }

    std::string getDepartmentType() const override {
        return "Education & Academic Development";
    }
};

// ============================================================
//  DERIVED CLASS 3: InfrastructureDepartment
// ============================================================
class InfrastructureDepartment : public BudgetDepartment {
private:
    int    activeProjects;
    double contingencyFund;
    double projectCompletionRate; // 0.0 to 1.0

public:
    InfrastructureDepartment(const std::string& n, const std::string& code,
                             double budget, int year, int projects,
                             double contingency, double completionRate)
        : BudgetDepartment(n, code, budget, year),
          activeProjects(projects), contingencyFund(contingency),
          projectCompletionRate(completionRate) {}

    ~InfrastructureDepartment() override {
        std::cout << Color::MAGENTA << "  [InfrastructureDepartment Destructor] "
                  << name << " resources freed.\n" << Color::RESET;
    }

    void generateReport() const override {
        std::cout << Color::BOLD << Color::MAGENTA
                  << "\n  ╔══════════════════════════════════════════════════╗\n"
                  << "  ║     INFRASTRUCTURE DEPARTMENT BUDGET REPORT      ║\n"
                  << "  ╚══════════════════════════════════════════════════╝\n"
                  << Color::RESET;
        displaySummary(true);
        std::cout << "  " << Color::CYAN << "Active Projects    : " << activeProjects << "\n";
        std::cout << "  " << Color::CYAN << "Contingency Fund   : " << formatMoney(contingencyFund) << "\n";
        std::cout << "  " << Color::CYAN << "Completion Rate    : "
                  << std::fixed << std::setprecision(1)
                  << (projectCompletionRate * 100) << "%" << Color::RESET << "\n\n";
    }

    // Penalty: 8% if completion rate < 60% (poor project performance)
    double calculatePenalty() const override {
        if (projectCompletionRate < 0.60)
            return allocatedBudget * 0.08;
        return 0.0;
    }

    std::string getDepartmentType() const override {
        return "Infrastructure & Public Works";
    }

    // Additional domain-specific method
    void updateCompletionRate(double rate) {
        if (rate < 0.0 || rate > 1.0)
            throw std::invalid_argument("Completion rate must be 0.0–1.0");
        projectCompletionRate = rate;
        std::cout << Color::GREEN << "  ✔  Completion rate updated to "
                  << std::fixed << std::setprecision(1)
                  << (rate * 100) << "%\n" << Color::RESET;
    }
};

// ============================================================
//  DERIVED CLASS 4: DefenseDepartment
// ============================================================
class DefenseDepartment : public BudgetDepartment {
private:
    std::string classificationLevel;
    double      operationalReserve;

public:
    DefenseDepartment(const std::string& n, const std::string& code,
                      double budget, int year,
                      const std::string& classification, double reserve)
        : BudgetDepartment(n, code, budget, year),
          classificationLevel(classification), operationalReserve(reserve) {}

    ~DefenseDepartment() override {
        std::cout << Color::RED << "  [DefenseDepartment Destructor] "
                  << name << " resources freed.\n" << Color::RESET;
    }

    void generateReport() const override {
        std::cout << Color::BOLD << Color::RED
                  << "\n  ╔══════════════════════════════════════════════════╗\n"
                  << "  ║        DEFENSE DEPARTMENT BUDGET REPORT          ║\n"
                  << "  ╚══════════════════════════════════════════════════╝\n"
                  << Color::RESET;
        displaySummary(true);
        std::cout << "  " << Color::CYAN << "Classification     : " << classificationLevel   << "\n";
        std::cout << "  " << Color::CYAN << "Operational Reserve: " << formatMoney(operationalReserve) << Color::RESET << "\n\n";
    }

    // Penalty: 10% of overspent if reserve is breached
    double calculatePenalty() const override {
        if (spentAmount > allocatedBudget - operationalReserve)
            return (spentAmount - (allocatedBudget - operationalReserve)) * 0.10;
        return 0.0;
    }

    std::string getDepartmentType() const override {
        return "National Defense & Security  [" + classificationLevel + "]";
    }
};

// ============================================================
//  BUDGET TRACKER  (System Controller)
// ============================================================
class BudgetTracker {
private:
    std::vector<BudgetDepartment*> departments;
    int                             fiscalYear;
    std::string                     countryName;

public:
    BudgetTracker(const std::string& country, int year)
        : fiscalYear(year), countryName(country) {}

    ~BudgetTracker() {
        std::cout << Color::BOLD << Color::YELLOW
                  << "\n  ──────────────────────────────────────────────\n"
                  << "  Shutting down Budget Tracker. Freeing memory...\n"
                  << "  ──────────────────────────────────────────────\n"
                  << Color::RESET;
        for (auto* dept : departments)
            delete dept;  // Virtual destructors called correctly
    }

    void addDepartment(BudgetDepartment* dept) {
        departments.push_back(dept);
    }

    // ── RUNTIME POLYMORPHISM ──────────────────────────────────
    // Calls overridden generateReport() on each derived object at runtime
    void printAllReports() const {
        printHeader("FULL GOVERNMENT BUDGET REPORT");
        for (const auto* dept : departments)
            dept->generateReport();   // <-- Runtime polymorphism
    }

    void printHeader(const std::string& title) const {
        int width = 54;
        std::string line(width, '═');
        std::cout << "\n" << Color::BOLD << Color::YELLOW
                  << "  ╔" << line << "╗\n"
                  << "  ║  " << std::left << std::setw(width - 2) << countryName
                  << "║\n"
                  << "  ║  " << std::setw(width - 2) << title
                  << "║\n"
                  << "  ║  " << std::setw(width - 2) << ("Fiscal Year: " + std::to_string(fiscalYear))
                  << "║\n"
                  << "  ╚" << line << "╝\n"
                  << Color::RESET;
    }

    // Consolidated summary of all departments
    void printConsolidatedSummary() const {
        printHeader("CONSOLIDATED BUDGET SUMMARY");
        double totalAllocated = 0, totalSpent = 0;
        for (const auto* dept : departments) {
            std::cout << "  " << *dept << "\n";   // operator<< overload
            totalAllocated += dept->getAllocatedBudget();
            totalSpent     += dept->getSpentAmount();
        }
        std::cout << "\n  " << Color::BOLD << Color::WHITE
                  << std::string(60, '─') << "\n";
        std::cout << "  TOTAL ALLOCATED : " << Color::GREEN << formatMoney(totalAllocated) << "\n";
        std::cout << Color::WHITE << "  TOTAL SPENT     : " << Color::RED   << formatMoney(totalSpent)     << "\n";
        std::cout << Color::WHITE << "  TOTAL REMAINING : "
                  << ((totalAllocated - totalSpent >= 0) ? Color::GREEN : Color::RED)
                  << formatMoney(totalAllocated - totalSpent)
                  << Color::RESET << "\n\n";
    }

    // Demonstrate operator> overloading — compare departments
    void compareTopSpenders() const {
        printHeader("DEPARTMENT SPENDING COMPARISON");
        if (departments.size() < 2) {
            std::cout << "  Not enough departments to compare.\n";
            return;
        }
        for (size_t i = 0; i < departments.size(); ++i) {
            for (size_t j = i + 1; j < departments.size(); ++j) {
                const auto* a = departments[i];
                const auto* b = departments[j];
                std::cout << "  " << Color::CYAN << a->getName()
                          << Color::WHITE << " vs " << Color::CYAN << b->getName()
                          << Color::RESET << " → ";
                if (*a > *b)
                    std::cout << Color::RED << a->getName()
                              << " has higher budget utilization.\n" << Color::RESET;
                else
                    std::cout << Color::RED << b->getName()
                              << " has higher budget utilization.\n" << Color::RESET;
            }
        }
        std::cout << "\n";
    }

    // Demonstrate operator== overloading
    void checkSameFiscalYear() const {
        printHeader("FISCAL YEAR CONSISTENCY CHECK");
        for (size_t i = 0; i < departments.size(); ++i) {
            for (size_t j = i + 1; j < departments.size(); ++j) {
                const auto* a = departments[i];
                const auto* b = departments[j];
                bool same = (*a == *b);
                std::cout << "  " << Color::CYAN
                          << a->getName() << " & " << b->getName()
                          << Color::WHITE << " — Same fiscal year? "
                          << (same ? Color::GREEN + "YES" : Color::RED + "NO")
                          << Color::RESET << "\n";
            }
        }
        std::cout << "\n";
    }

    // Keyword search across ALL departments (function overloading demo)
    void searchAllDepartments(const std::string& keyword) const {
        printHeader("SEARCH: \"" + keyword + "\"");
        for (const auto* dept : departments) {
            std::cout << Color::BOLD << "  [ " << dept->getName() << " ]\n" << Color::RESET;
            dept->displaySummary(keyword);   // Overload 3
        }
    }

    // Find highest-spending department
    void findHighestSpender() const {
        if (departments.empty()) return;
        BudgetDepartment* highest = departments[0];
        for (auto* d : departments)
            if (d->getSpentAmount() > highest->getSpentAmount())
                highest = d;
        std::cout << Color::BOLD << Color::YELLOW
                  << "  🏆 Highest Spender: " << highest->getName()
                  << " — " << formatMoney(highest->getSpentAmount())
                  << Color::RESET << "\n\n";
    }
};

// ============================================================
//  SEPARATOR HELPERS
// ============================================================
void section(const std::string& label) {
    std::cout << "\n" << Color::BOLD << Color::YELLOW
              << "  ══════════════════════════════════════════════════\n"
              << "  " << label << "\n"
              << "  ══════════════════════════════════════════════════\n"
              << Color::RESET;
}

void pause() {
    std::cout << Color::WHITE << "\n  [Press ENTER to continue...]\n" << Color::RESET;
    std::cin.ignore();
    std::cin.get();
}

// ============================================================
//  MAIN ENTRY POINT
// ============================================================
int main() {
    std::cout << Color::BOLD << Color::YELLOW << R"(
  ╔══════════════════════════════════════════════════════╗
  ║     PHILIPPINE GOVERNMENT BUDGET TRACKING SYSTEM    ║
  ║     Fiscal Year 2025                                ║
  ║     Demonstrating C++ OOP Principles                ║
  ╚══════════════════════════════════════════════════════╝
)" << Color::RESET;

    // ── CREATE TRACKER ────────────────────────────────────────
    BudgetTracker tracker("Republic of the Philippines", 2025);

    // ── INSTANTIATE DERIVED CLASSES ───────────────────────────
    // (Base class pointers → Runtime Polymorphism)
    BudgetDepartment* health = new HealthDepartment(
        "Department of Health", "DOH",
        /*budget=*/  85'000'000.00,
        /*year=*/    2025,
        /*hospitals*/120,
        /*reserve=*/ 10'000'000.00
    );

    BudgetDepartment* education = new EducationDepartment(
        "Department of Education", "DepEd",
        /*budget=*/  120'000'000.00,
        /*year=*/    2025,
        /*schools=*/ 3500,
        /*scholarship=*/ 15'000'000.00
    );

    BudgetDepartment* infra = new InfrastructureDepartment(
        "Department of Public Works", "DPWH",
        /*budget=*/  200'000'000.00,
        /*year=*/    2025,
        /*projects=*/45,
        /*contingency=*/ 20'000'000.00,
        /*completion=*/  0.55          // 55% — triggers penalty
    );

    BudgetDepartment* defense = new DefenseDepartment(
        "Department of National Defense", "DND",
        /*budget=*/  95'000'000.00,
        /*year=*/    2025,
        /*classification=*/ "CONFIDENTIAL",
        /*reserve=*/ 25'000'000.00
    );

    tracker.addDepartment(health);
    tracker.addDepartment(education);
    tracker.addDepartment(infra);
    tracker.addDepartment(defense);

    // ── RECORD TRANSACTIONS ───────────────────────────────────
    section("STEP 1 — RECORDING TRANSACTIONS");

    try {
        // Health
        health->recordExpense("PhilHealth Medical Supplies",   12'500'000, "2025-01-15");
        health->recordExpense("Rural Health Unit Construction", 8'000'000, "2025-02-01");
        health->recordExpense("Doctors & Nurses Salary Q1",    18'000'000, "2025-03-31");
        health->recordIncome ("DOH Supplemental Allocation",    5'000'000, "2025-04-10");
        health->recordExpense("Vaccine Procurement",            7'200'000, "2025-04-20");

        // Education
        education->recordExpense("Textbook Distribution",       22'000'000, "2025-01-20");
        education->recordExpense("Teacher Salaries Q1",         30'000'000, "2025-03-31");
        education->recordExpense("Classroom Construction",      15'000'000, "2025-05-01");
        education->recordIncome ("Private Sector Grants",        3'500'000, "2025-05-15");

        // Infrastructure
        infra->recordExpense("North Luzon Highway Phase 1",     55'000'000, "2025-01-30");
        infra->recordExpense("Metro Manila Flood Control",      40'000'000, "2025-02-28");
        infra->recordExpense("Mindanao Bridge Repair",          25'000'000, "2025-04-15");
        infra->recordIncome ("Toll Revenue Reinvestment",       10'000'000, "2025-05-01");

        // Defense
        defense->recordExpense("Military Equipment Upgrade",    30'000'000, "2025-01-25");
        defense->recordExpense("Border Security Operations",    20'000'000, "2025-03-10");
        defense->recordExpense("Personnel Benefits Q1",         12'000'000, "2025-03-31");

        std::cout << Color::GREEN << "  ✔  All transactions recorded successfully.\n" << Color::RESET;

    } catch (const std::exception& e) {
        std::cout << Color::RED << "  ✘  Error: " << e.what() << Color::RESET << "\n";
    }

    pause();

    // ── GENERATE INDIVIDUAL REPORTS (Runtime Polymorphism) ────
    section("STEP 2 — INDIVIDUAL DEPARTMENT REPORTS (Runtime Polymorphism)");
    std::cout << Color::WHITE
              << "  generateReport() is called via base class pointer.\n"
              << "  The correct derived version executes at runtime.\n\n"
              << Color::RESET;

    // Runtime polymorphism: base class pointer calls the correct override
    tracker.printAllReports();
    pause();

    // ── CONSOLIDATED SUMMARY (operator<< overload) ────────────
    section("STEP 3 — CONSOLIDATED SUMMARY (operator<< Overload)");
    tracker.printConsolidatedSummary();
    pause();

    // ── COMPARISON (operator> overload) ───────────────────────
    section("STEP 4 — SPENDING COMPARISON (operator> Overload)");
    tracker.compareTopSpenders();
    pause();

    // ── FISCAL YEAR CHECK (operator== overload) ───────────────
    section("STEP 5 — FISCAL YEAR CHECK (operator== Overload)");
    tracker.checkSameFiscalYear();
    pause();

    // ── FUNCTION OVERLOADING DEMO ─────────────────────────────
    section("STEP 6 — FUNCTION OVERLOADING: displaySummary()");
    std::cout << Color::WHITE
              << "  Calling displaySummary()         → no args (brief)\n"
              << "  Calling displaySummary(true)     → show transactions\n"
              << "  Calling displaySummary(\"keyword\") → filtered search\n\n"
              << Color::RESET;

    std::cout << Color::BOLD << Color::CYAN
              << "  ► displaySummary() — Brief View:\n" << Color::RESET;
    health->displaySummary();   // Overload 1

    std::cout << Color::BOLD << Color::CYAN
              << "  ► displaySummary(true) — With Transactions:\n" << Color::RESET;
    defense->displaySummary(true);   // Overload 2

    pause();

    // ── KEYWORD SEARCH (Overload 3) ───────────────────────────
    section("STEP 7 — KEYWORD SEARCH: displaySummary(\"keyword\")");
    tracker.searchAllDepartments("salary");
    pause();

    // ── HIGHEST SPENDER ───────────────────────────────────────
    section("STEP 8 — HIGHEST SPENDING DEPARTMENT");
    tracker.findHighestSpender();

    // ── EXCEPTION HANDLING DEMO ───────────────────────────────
    section("STEP 9 — EXCEPTION HANDLING DEMO");
    std::cout << Color::WHITE << "  Attempting to overspend DOH budget...\n\n" << Color::RESET;
    try {
        health->recordExpense("INVALID: Oversized Purchase", 999'999'999, "2025-12-01");
    } catch (const std::runtime_error& e) {
        std::cout << Color::RED << "  ✘  Runtime Error caught: " << e.what() << Color::RESET << "\n";
    }
    try {
        education->recordExpense("INVALID: Negative Amount", -500, "2025-12-01");
    } catch (const std::invalid_argument& e) {
        std::cout << Color::RED << "  ✘  Invalid Argument caught: " << e.what() << Color::RESET << "\n";
    }

    std::cout << Color::GREEN << "\n  ✔  All exceptions handled gracefully.\n" << Color::RESET;
    pause();

    // ── VIRTUAL DESTRUCTOR DEMO ───────────────────────────────
    section("STEP 10 — VIRTUAL DESTRUCTOR DEMO");
    std::cout << Color::WHITE
              << "  BudgetTracker destructor fires → deletes all dept pointers.\n"
              << "  Virtual destructors ensure each derived class destructor runs.\n\n"
              << Color::RESET;

    // ── Tracker goes out of scope here, triggering destructors ──
    std::cout << Color::BOLD << Color::YELLOW << "\n  Exiting program...\n\n" << Color::RESET;

    // ── tracker destructor fires here (end of scope) ──
    return 0;
}