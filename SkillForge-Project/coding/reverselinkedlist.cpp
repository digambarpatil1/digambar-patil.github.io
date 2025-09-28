#include<iostream>
#include<memory>
struct reverselinkedlist
{
    int data;
    std::shared_ptr<reverselinkedlist> next;
    reverselinkedlist(int val):data(val),next(nullptr){}
};
void reverselinkedlistfunction(std::shared_ptr<reverselinkedlist> head)
{
    std::shared_ptr<reverselinkedlist> prev=nullptr;
    std::shared_ptr<reverselinkedlist> curr=head;
    std::shared_ptr<reverselinkedlist> next=nullptr;
    while(curr)
    {
        next=curr->next;
        curr->next=prev;
        prev=curr;
        curr=next;
    }
    head=prev;
    while(head)
    {
        std::cout<<head->data<<" ";
        head=head->next;
    }
    std::cout<<std::endl;
}


int main(){
    std::cout<<"Reverse Linked List using smart pointer"<<std::endl;
    // Create linked list 1->2->3->4->5 using smart pointers
    std::shared_ptr<reverselinkedlist> head=std::make_shared<reverselinkedlist>(1);
    head->next=std::make_shared<reverselinkedlist>(2);
    head->next->next=std::make_shared<reverselinkedlist>(3);
    head->next->next->next=std::make_shared<reverselinkedlist>(4);
    head->next->next->next->next=std::make_shared<reverselinkedlist>(5);
    reverselinkedlistfunction(head);
    return 0;
}