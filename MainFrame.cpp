#include "MainFrame.h"
#include <wx/wx.h> 
#include <iostream>

using namespace std;

class Node
{
public:
    string phone_number;
    Node* next;
    Node* prev;
    string name;

    Node()
    {
        name = "";
        next = nullptr;
        prev = nullptr;
        phone_number = "";
    }
};

ContactBookFrame::ContactBookFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	createControls();
	BindEventHandlers();
}

void ContactBookFrame::createControls()
{
    SetBackgroundColour(wxColour(102, 178, 255)); // Establecer color de fondo gris

    wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
    wxFont bottons(wxFontInfo(wxSize(0, 20)).AntiAliased());

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* headlineText = new wxStaticText(panel, wxID_ANY, "Contact Book",  //header title
        wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER);
    headlineText->SetForegroundColour(wxColour(255,255,255));


    headlineText->SetFont(headlineFont);

    addButton = new wxButton(panel, wxID_ANY, "Add Contact", wxPoint(300, 180), wxSize(200, 35));
    addButton->SetForegroundColour(wxColour(*wxBLACK));
    addButton->SetFont(bottons);

    displayButton = new wxButton(panel, wxID_ANY, "Display Contacts", wxPoint(300, 280), wxSize(200, 35));
    displayButton->SetForegroundColour(wxColour(*wxBLACK));
    displayButton->SetFont(bottons);

    searchButton = new wxButton(panel, wxID_ANY, "Search contact", wxPoint(300, 380), wxSize(200, 35));
    searchButton->SetForegroundColour(wxColour(*wxBLACK)); 
    searchButton->SetFont(bottons);

    deleteButton = new wxButton(panel, wxID_ANY, "Delete Contact", wxPoint(300, 480), wxSize(200, 35));
    deleteButton->SetForegroundColour(wxColour(*wxBLACK));
    deleteButton->SetFont(bottons);

    deleteAll = new wxButton(panel, wxID_ANY, "Delete All", wxPoint(300, 580), wxSize(200, 35));
    deleteAll->SetForegroundColour(wxColour(*wxBLACK));
    deleteAll ->SetFont(bottons);

    panel->SetSizer(sizer);
    sizer->Fit(this);
    sizer->SetSizeHints(this);
}

void ContactBookFrame::BindEventHandlers()
{
    addButton->Bind(wxEVT_BUTTON, &ContactBookFrame::OnAddButtonClick, this);
    displayButton->Bind(wxEVT_BUTTON, &ContactBookFrame::OnDisplayButtonClick, this);
    deleteButton->Bind(wxEVT_BUTTON, &ContactBookFrame::OnDeleteButtonClick, this);
    deleteAll->Bind(wxEVT_BUTTON, &ContactBookFrame::OnDeleteAll, this);
    searchButton->Bind(wxEVT_BUTTON, &ContactBookFrame::OnSearchButton, this);
    reOpen();

    // Create an instance of the Node class and access the head member through it
    Node* head = new Node;
    head->name = "";
    head->phone_number = "";
}


void ContactBookFrame::CreateNode()
{
    wxString nameInput = wxGetTextFromUser("Enter the name of the contact:", "Add Contact");
    if (nameInput == "") wxMessageBox("Operation Cancelled");
    else
    {
        wxString numberInput = wxGetTextFromUser("Enter the phone number:", "Add Contact");
        if (numberInput == "")
        {
            wxMessageBox("Operation Cancelled");
            return;
        }
        if (head == nullptr)
        {
            Node* newer = new Node;

            newer->name = nameInput.ToStdString();
            newer->phone_number = numberInput.ToStdString();

            newer->next = nullptr;
            newer->prev = nullptr;
            head = newer;

            wxMessageBox("Contact added");
        }
        else
        {
            Node* newer = new Node;

            newer->name = nameInput.ToStdString();
            newer->phone_number = numberInput.ToStdString();

            newer->next = nullptr;

            Node* temp = head;
            while (temp != nullptr)  // Update the condition here
            {
                if (temp->next == nullptr)
                    break;

                temp = temp->next;
            }
            temp->next = newer;
            newer->prev = temp;

            wxMessageBox("Contact added");
        }

    offlineSave();

    }
}

void ContactBookFrame::Display()
{
    BubbleSort();
    Node* temp = head;
    int count = 0;
    wxString contactsInfo;

    if (temp == nullptr) //If there are no contacts
    {
        contactsInfo = "No contacts found!";
    }
    else
    {
        contactsInfo = "Contact list:\n\n";
        while (temp != nullptr)
        {
            count++;
            contactsInfo += wxString::Format("%s", temp->name.c_str());
            contactsInfo += wxString::Format("      %s\n", temp->phone_number.c_str());
            temp = temp->next;
        }
        contactsInfo += wxString::Format("\nTotal contacts: %d", count);
    }

    wxMessageBox(contactsInfo);
}

void ContactBookFrame::BubbleSort()
{
    Node* temp = head;
    Node* i, * j;
    string n;
    string n2;

    if(temp != nullptr)
    {
        for (i = head; i->next != nullptr; i = i->next)
        {
            for (j = i->next; j != nullptr; j = j->next)
            {
                if (i->name > j->name)
                {
                    n = i->name;
                    i->name = j->name;
                    j->name = n;

                    n2 = i->phone_number;
                    i->phone_number = j->phone_number;
                    j->phone_number = n2;
                }
            }
        }
    }
    offlineSave();
}

void ContactBookFrame::OnSearchButton(wxCommandEvent& event)
{
    wxString nameInput = wxGetTextFromUser("Enter the name to search:", "Search Contact");
    string nameToSearch = nameInput.ToStdString();
    
    Node* temp = head;
    Node* prevNode = nullptr;
    wxString contactsInfo;
    
    bool check = true;

    while (temp != nullptr)
    {
        if (temp->name == nameToSearch)
        {
            contactsInfo = "Contac Information:\n\n";
            contactsInfo += wxString::Format("%s", temp->name.c_str());
            contactsInfo += wxString::Format("      %s\n", temp->phone_number.c_str());

            check = false;

            wxMessageBox(contactsInfo);
            break;
        }
        prevNode = temp;
        temp = temp->next;
    }
    if (nameToSearch == "")
    {
        wxMessageBox("Operation cancelled");
        return;
    }
    if (check)
    {
        contactsInfo = "Contact not found";
        wxMessageBox(contactsInfo);
    }
}
void ContactBookFrame::OnDeleteButtonClick(wxCommandEvent& event)
{
    wxMessageDialog searchDialog(this, "Choose an option to search:", "Delete Contact", wxYES_NO | wxCANCEL);
    searchDialog.SetYesNoLabels("Search By Name", "Search By Number");
    int searchOption = searchDialog.ShowModal();

    if (searchOption == wxID_YES) //search by name
    {
        wxString nameInput = wxGetTextFromUser("Enter the name to delete:", "Delete Contact");
        string nameToDelete = nameInput.ToStdString();
        if (nameToDelete == "")
        {
            wxMessageBox("Operation cancelled");
            return;
        }

        Node* temp = head;
        Node* prevNode = nullptr;

        bool contactFound = false;

        while (temp != nullptr)
        {
            if (temp->name == nameToDelete)
            {
                contactFound = true;
                break;
            }
            prevNode = temp;
            temp = temp->next;
        }

        if (contactFound)
        {
            wxMessageDialog* deleteDialog = new wxMessageDialog(this, "Are you sure that you want to delete the contact?", "Delete Contact", wxYES_NO);
            searchDialog.SetYesNoLabels("Delete", "Cancel");
            int deleteOption = deleteDialog->ShowModal();

            if (deleteOption == wxID_YES)
            {
                if (prevNode != nullptr) //not first contact
                {
                    prevNode->next = temp->next; //skip the contact to be deleted
                    if (temp->next != nullptr) //not last contact
                    {
                        temp->next->prev = prevNode; 
                    }
                }
                else //first contact
                {
                    head = temp->next;
                    if (temp->next != nullptr) //not only 1 contact
                    {
                        temp->next->prev = nullptr; //prev after of the eliminated contact to nullptr
                    }
                }

                delete temp;
                wxMessageBox("Contact deleted successfully");
            }
            else
            {
                wxMessageBox("Operation cancelled");
            }
        }
        else
        {
            wxMessageBox("Contact with this name not found.");
        }
    }
    else if(searchOption == wxID_NO) //search by number
    { 
        wxString numberInput = wxGetTextFromUser("Enter the number of the contact to delete:", "Delete Contact");
        string numberToDelete = numberInput.ToStdString();
        if (numberToDelete == "")
        {
            wxMessageBox("Operation cancelled");
            return;
        }

        Node* temp = head;
        Node* prevNode = nullptr;

        bool contactFound = false;

        while (temp != nullptr)
        {
            if (temp->phone_number == numberToDelete)
            {
                contactFound = true;
                break;
            }
            prevNode = temp;
            temp = temp->next;
        }

        if (contactFound)
        {
            wxMessageDialog* deleteDialog = new wxMessageDialog(this, "Are you sure that you want to delete the contact?", "Delete Contact", wxYES_NO);
            searchDialog.SetYesNoLabels("Delete", "Cancel");
            int deleteOption = deleteDialog->ShowModal();

            if (deleteOption == wxID_YES)
            {
                if (prevNode != nullptr)
                {
                    prevNode->next = temp->next;
                    if (temp->next != nullptr)
                    {
                        temp->next->prev = prevNode;
                    }
                }
                else
                {
                    head = temp->next;
                    if (temp->next != nullptr)
                    {
                        temp->next->prev = nullptr;
                    }
                }

                delete temp;
                wxMessageBox("Contact deleted successfully");
            }
            else
            {
                wxMessageBox("Operation cancelled");
            }
        }
        else
        {
            wxMessageBox("Contact with this number not found");
        }
    }
    else
    {
        wxMessageBox("Operation cancelled");
    }

    offlineSave();
}


void ContactBookFrame::OnAddButtonClick(wxCommandEvent& event)
{
    CreateNode();
}

void ContactBookFrame::OnDisplayButtonClick(wxCommandEvent& event)
{
    Display();
}

void ContactBookFrame::offlineSave()
{
    Node* temp = head;
    ofstream myfile("contactbook.txt");
    if (myfile.is_open())
    {
        while (temp != NULL)
        {
            myfile << temp->name << endl;
            myfile << temp->phone_number << endl;
            temp = temp->next;
        }
        myfile.close();
    }
}

void ContactBookFrame::reOpen()
{
    ifstream myfile("contactbook.txt");
    if (myfile.is_open() and myfile.peek() != EOF) //check if is correctly open and not empty
    {
        int i = 0;
        string aux, aux2;
        while (getline(myfile, aux))
        {
            if (i % 2 == 0) //save name
            {
                if (head == nullptr) //first name
                {
                    Node* newer = new Node;
                    newer->name = aux;

                    newer->next = nullptr;
                    newer->prev = nullptr;
                    head = newer;
                }
                else //others name
                {
                    Node* newer = new Node;

                    newer->name = aux;
                    newer->next = nullptr;

                    Node* temp = head;
                    while (temp->next != nullptr) //traverse until the end
                    {
                        temp = temp->next;
                    }
                    temp->next = newer;
                    newer->prev = temp; //enlaza con los anteriores nombres
                }
            }
            else //save numbers
            {
                Node* temp = head;
                if (temp->phone_number == "") //first number
                {
                    temp->phone_number = aux;
                }
                else
                {
                    Node* temp = head;
                    while (temp->next != nullptr)
                    {
                        temp = temp->next;
                    }
                    temp->phone_number = aux;
                }
            }
            i++;
        }
        myfile.close();
    }
}

void ContactBookFrame::OnDeleteAll(wxCommandEvent& event)
{
    Node* temp = head;
    Node* temp2 = nullptr;

    wxMessageDialog searchDialog(this, "Are you sure that you want to delete all the contacts?", "Delete Contact", wxYES_NO);
    int deleteAllOption = searchDialog.ShowModal();

    if (deleteAllOption == wxID_YES)
    { 
        if (head == nullptr)
        {
            wxMessageBox("The contact list is empty.");
        }
        else
        {
            while (temp != nullptr)
            {
                temp2 = temp;
                temp = temp->next;
                delete temp2;
            }
            head = nullptr;
            wxMessageBox("All contacts were deleted succesfully");
        }
    }
    else
    {
        wxMessageBox("Operation canceled");
    }
}