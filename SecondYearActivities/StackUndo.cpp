// Implement stack undo

#include <iostream>
#include <stack>

using namespace std;

void undo_stack(stack<int> &s)
{
    while (!s.empty())
    {
        s.pop();
    }
    return;
}

int main(void)
{
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    undo_stack(s);

    cout << "Is stack empty? " << (s.empty() ? "Yes" : "No") << "\n";

    return 0;
}
