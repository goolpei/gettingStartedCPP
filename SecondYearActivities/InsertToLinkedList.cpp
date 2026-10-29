// Insert value from the user

#include <iostream>

using namespace std;

class Node {
public:
    int val;
    Node *next;
};

int main(void)
{
    int num;
    char choice = 'y';
    
    Node *head = nullptr;
    Node *ptr;

    while (choice == 'y' || choice == 'Y')
    {
        Node *temp = new Node();
        cout << "Insert number to linked list: ";
        cin >> num;
        temp->val = num;
        temp->next = nullptr;

        if (head == nullptr)
        {
            head = temp;
        }
        else
        {
            ptr = head;
            while (ptr->next != nullptr)
            {
                ptr = ptr->next;
            }
            ptr->next = temp;
        }
        

        cout << "Continue? (y/n) ";
        cin >> choice;
          
    }
    
    ptr = head;
    while (ptr != nullptr)
    {
        cout << ptr->val << " ";
        Node *nxt = ptr->next;

        delete ptr;

        ptr = nxt;
    }

    return 0;
}
