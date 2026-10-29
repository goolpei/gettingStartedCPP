#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
 
using namespace std;
 
class Product
{
    public:

    string code;
    string name;
    int quantity_sold;
    double selling_price; 
    double unit_cost;

    Product (string code_ = "NA", string name_ = "NA", int qsold_ = 0, double selling_price_ = 0, double unit_cost_ = 0){
        code = code_;
        name = name_;
        quantity_sold = qsold_;
        selling_price = selling_price_;
        unit_cost = unit_cost_;
    }
 
    double total_sales(){return quantity_sold * selling_price;}
    double total_cost(){return quantity_sold * unit_cost;}
    double profit(){return total_sales() - total_cost();}
};
 
 
int main(void)
{
    string decision;
    

    vector<Product> dummyProducts;
    dummyProducts.push_back(Product("01A", "HP Laptop", 90, 30000, 28000));
    dummyProducts.push_back(Product("02A", "MacBook Laptop", 70, 39000, 38900));
    dummyProducts.push_back(Product("03A", "Acer Laptop", 40, 70000, 75000));
    dummyProducts.push_back(Product("04A", "Asus Laptop", 80, 45000, 40000));
    dummyProducts.push_back(Product("05A", "Lenovo Laptop", 90, 30000, 28000));
    
    vector<Product> products(dummyProducts);
    int product_count = products.size();

    
    // cout << "Enter at least 10 products.\n";
    while (0)
    {
        Product product_holder;
        cout << "-------------------------------------------\n";
        cout << "Product " << product_count + 1 << endl;
        cout << "Enter product code: ";
        cin >> product_holder.code;
 
        cin.ignore();
        cout << "Enter product name: ";
        getline(cin, product_holder.name);
 
        cout << "Enter quantity sold: ";
        cin >> product_holder.quantity_sold;
        cout << "Enter selling price: ";
        cin >> product_holder.selling_price;
        cout << "Enter unit cost: ";
        cin >> product_holder.unit_cost;
 
        products.push_back(product_holder);
        product_count += 1;
 
        cout << "Done? (y/n) ";
        cin >> decision;
        if (decision == "y" || decision == "Y")
        {
            if (product_count >= 10) {break;}
            else
            {
                cout << "Enter at least 10 products.\n";
            }
        }
    }
 
    int max_idx;
    Product temp;
    // Sort by profitability
    for (int i = 0; i < product_count; i++)
    {
        max_idx = i;
        for (int j = i + 1; j < product_count; j++)
        {
            if (products[j].profit() > products[max_idx].profit()) max_idx = j;
        }
        temp = products[i];
        products[i] = products[max_idx];
        products[max_idx] = temp;
    }
 
 
    // Profitability classification
    // 1. Profitable
    // 2. Not profitable

    int profitable_count = 0;
    int non_profitable_count = 0;
 
    for (int i = 0; i < product_count; i++)
    {
        if (products[i].profit() > 0) profitable_count += 1;
        else non_profitable_count += 1;
    }

    for (Product p : products)
    {
        cout << p.name << " " <<p.profit() << endl;
    }
    
    cout << profitable_count << endl;
    cout << non_profitable_count << endl;
 
    return 0;
}