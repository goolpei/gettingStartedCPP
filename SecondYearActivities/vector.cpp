// Squares the values inside the vector
#include <iostream>
#include <vector>

using namespace std;
int main(void)
{

    vector<int> nums;
    vector<int>::iterator it; 
    int n;

    cout << "Enter 5 numbers: ";
    for (int i = 0; i < 5; ++i)
    {
        cin >> n;
        nums.push_back(n);
    }

    cout << "\nUsing iterator: ";
    for (it = nums.begin(); it != nums.end(); ++it)
    {
        *it = (*it) * (*it);
        cout << *it << " ";
    }
    
    cout << "\nUsing array indexing: ";
    for (int i = 0; i < nums.size(); ++i)
    {
        cout << nums[i] << " ";
    }

    
    return 0;
}