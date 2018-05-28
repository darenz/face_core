#include<iostream>

#ifndef _LINK_LIST
#define _LINK_LIST
#define ERRO 0
#define OK 1
typedef int Status;

using namespace std;

template<typename ElemType>
class LinkList
{
    public:
        class ListNode
        {
             public:
                 ListNode()
                 {
                     next = NULL;
                 }
                 ListNode(const ElemType& t_elem):elem(t_elem)
                 {
                    next = NULL;
                 }
                 ElemType elem;
                 ListNode* next;
        };
        typedef ListNode* NodePointer;

        LinkList();
        ~LinkList();
        LinkList(LinkList<ElemType>& otherL);
        
        Status append(const ElemType& e);
        ElemType get(int i);
        Status insert(int i,ElemType e);
        Status deleteElem(ElemType e);
        Status clear();
        int getLength();
        uint size()
        {
            return (uint)getLength();
        }
        NodePointer getHead();
        Status show();

        ElemType& operator [](int i);

    private:
        NodePointer head;
        int length;
};

template<typename ElemType>
LinkList<ElemType>::LinkList()
{
    head = new ListNode;
    //require elem init
    head->next = NULL;
    length = 0;
}

template<typename ElemType>
LinkList<ElemType>::~LinkList()
{
    clear();
    delete head;
}

template<typename ElemType>
LinkList<ElemType>::LinkList(LinkList<ElemType> &otherL)
{
    NodePointer pthis = head->next;
    NodePointer pt = head;
    NodePointer pother = otherL.head->next;
    while(pother!=NULL)
    {
        pthis = new ListNode;
        pthis->next= NULL;
        pthis->elem = pother->elem;
        pt->next = pthis;
        pt = pthis;
        pother = pother->next;
        length++;
    }
}

template<typename ElemType>
Status LinkList<ElemType>::insert(int i,ElemType e)
{
    if(i<1 || i>length+1 || length==0)
    {
        cout<<"index erro"<<endl;
        return ERRO;
    }
    int j=0;
    NodePointer p=head;
    while(j!=i-1)
    {
        p = p->next;
        j++;
    }
    NodePointer newp = new ListNode;
    newp->elem = e; 
    newp->next = p->next;
    p->next = newp;
    return OK;
}

template<typename ElemType>
ElemType LinkList<ElemType>::get(int i)
{
    NodePointer p = head->next;
    int j=1;
    while(j<i && p)
    {
        p = p->next;
        j++;
    }
    if(p == NULL)
    {
        cout<<"get elem not found or out of range"<<endl;
        exit(0);
    }
    else
    {
        return p->elem;
    }
}

template<typename ElemType>
Status LinkList<ElemType>::deleteElem(ElemType e)
{
    NodePointer p = head;
    while(p!=NULL)
    {
        if(p->next->elem == e)
        {
            NodePointer pt = p->next;
            p->next = pt->next;
            delete pt;
	    length--;
            return OK;
        }
        p = p->next;
    }
    return OK;
}

template<typename ElemType>
Status LinkList<ElemType>::clear()
{
    /*
    NodePointer p = head->next;
    NodePointer q = p->next;
    while(q->next!=NULL)
    {
        delete p;
        p = q;
        q = q->next;
    }
    delete p;
    delete q;
    length = 0;
    head->next = NULL;
    return OK;
    */
    NodePointer p = head->next;
    head->next=0;
    while(p)
    {
        NodePointer q = p->next;
        delete p;
        p = q;
    }
    length=0;
    return OK;
}

template<typename ElemType>
int LinkList<ElemType>::getLength()
{
    return length;
}

template<typename ElemType>
typename LinkList<ElemType>::NodePointer LinkList<ElemType>::getHead()
{
    return head;
}

template<typename ElemType>
Status LinkList<ElemType>::show()
{
    NodePointer p = head->next;
    while(p)
    {
        cout<<p->elem<<' ';
        p = p->next;
    }
    cout<<endl;
    return OK;
}

template<typename ElemType>
Status LinkList<ElemType>::append(const ElemType& e)
{
    NodePointer p = new ListNode;
    p->elem = e;
    p->next = head->next;
    head->next = p;
    length++;
    return OK;
}

template<typename ElemType>
ElemType& LinkList<ElemType>::operator [](int i)
{
    NodePointer p = head->next;
    int j = 0;
    while(j<i && p)
    {
        j++;
        p = p->next;
    }
    if(p == NULL)
    {
        cout<<"[] not found or out of range"<<endl;
        exit(0);
    }
    else
        return (p->elem);         // elem won't been release?
                                  // or return &(p->elem) ?
}

#endif
