#pragma once
#include <wx/wx.h>
#include<iostream>
#include <fstream>
#include <string.h>

class Node;
class ContactBookFrame : public wxFrame
{
public:
	ContactBookFrame(const wxString& title);
	void createControls();
	void BindEventHandlers();

	void CreateNode();
	void Display();
	void BubbleSort();


	void OnDeleteButtonClick(wxCommandEvent& event);
	void OnAddButtonClick(wxCommandEvent& event);
	void OnDisplayButtonClick(wxCommandEvent& event);
	void OnDeleteAll(wxCommandEvent& event);
	void OnSearchButton(wxCommandEvent& event);

	void offlineSave();
	void reOpen();

	wxButton* addButton;
	wxButton* displayButton;
	wxButton* deleteButton;
	wxButton* deleteAll;
	wxButton* searchButton;
	Node* head;
};