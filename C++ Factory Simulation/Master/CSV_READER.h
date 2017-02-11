#pragma once
#include "Task.h"
#include "TaskManager.h"
#include "ItemManager.h"
#include "CustomerOrderManager.h"
#include "Factory.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace milestone {
	class CSV {
		TaskManager* taskmanager;
		ItemManager* itemmanager;
		OrderManager* ordermanager;
		Factory* factory;
		char delimeter;
		std::vector<std::vector<std::string> > FileHolder;
	public:
		CSV() {}
		CSV(const char* filename, const char* del, const char* filenameTwo, const char* delTwo, const char* filenameThree, const char* delThree)
		{
			ParseData(filename, del);
			FileHolder.clear();
			ParseData(filenameTwo, delTwo);
			FileHolder.clear();
			ParseData(filenameThree, delThree);
			FileHolder.clear();
			taskmanager->graphTasks();
			itemmanager->graphTasks();
			ordermanager->graphOrders();
			factory = new Factory(taskmanager, itemmanager, ordermanager);
			//factory->display();
		} // ctor overloaded
		~CSV(){
			delete taskmanager->tasks;
			delete taskmanager;
			delete itemmanager->items;
			delete itemmanager;
			delete ordermanager->orders;
			delete ordermanager;
		}
char GetDelimeter() { return delimeter; }  // returns the delimeter to parse dat
void ParseData(const char* filename, const char* del)  // receives a file and parses the data, then stores it in a vector before creating an instance of task/item or customerorer
{
	delimeter = del[0];
	std::ifstream is(filename);
	//SetDelimeter(is);
	//std::cout << GetDelimeter() << "<---- delimeter\n";
	if (is.is_open())
	{
		while (!is.eof()) {
			bool whitespace = false;
			while (is.peek() == ' ' || is.peek() == '\t' || is.peek() == '\0')
			{
				//std::cout << "caught";	
				is.ignore();
			}
			std::string line;
			getline(is, line);
			//std::cout << line << "\n";
			std::vector<std::string>temp;
			std::stringstream ss(line);
			if (line != "" && line != "\t" && !line.empty() && line != "\n" && line != "\r" && line != " ") {  //checks to see if line has a value
				while (getline(ss, line, GetDelimeter()))
				{
					if (line != " " && line != "") // checks if line is a whitespace
					{
						//std::cout << "this line being copied is-->" << line << "\n";
						temp.push_back(line);
						while (ss.peek() == ' ')
						{
							ss.ignore();
						}
					}
					else { whitespace = true; }
				}// while
			} // if line != ""
			else { whitespace = true; } // if line has no valye

			if (whitespace == false)
				FileHolder.push_back(temp);
		} // WHILE NOT EOF
		CreateItem(filename);  // will create either a task, item or customerorder class
	}
	else {
		throw std::string("Cannot open file: ") + filename;
	}
	is.close();
}
void CreateItem(const std::string filename) {
	int count = 0;
			// Check for the number of fields in file holder to determin if it is a task, item or customer order 4 = task, 5 = item, 6 = customer order
	for (int line = 0; line < FileHolder.size(); line++){
		if (FileHolder[line].size() > count )
		count = FileHolder[line].size();
		//std::cout << count << "<---couynt\n";
	}
	if (count == 4 ) {
		taskmanager = new TaskManager(FileHolder);  // Create new Task Manager with list from CSV reader
		//std::cout << "Size of Vector FileHolder ->" << sizeof(FileHolder) << "\n";
		//std::cout << "Size of taskmanager class ->" << sizeof(taskmanager) << "\n";
		//std::cout << "Size of taskmanager Vector ->" << taskmanager->sizethree() << "\n";
		//std::cout << "Size of this ->" << sizeof(this) << "\n";
		//std::cout << "Size of *this ->" << sizeof(*this) << "\n";
	}
	else if (count == 5)
	{
		//displaythree();
		itemmanager = new ItemManager(FileHolder);
		//display();		
	}
	else if (count > 5)
	{
		ordermanager = new OrderManager(FileHolder);
		//display();		
	}
	else
		throw std::string("Found No Matching Class Types  for") + filename;
}

void display()
{
	for (int i = 0; i < FileHolder.size(); i++)
	{
		for (int k = 0; k < FileHolder[i].size(); k++)
		{
			std::cout << FileHolder[i][k] << ", ";
		}
		std::cout << "\n";
	}
	//std::cout << FileHolder.size() << "  Size\n";
} // display

void displaytwo()
{
	for (auto cline : FileHolder) {
		for (auto fieldvs : cline)
			std::cout << fieldvs << "\n";
		std::cout << "\n";
	}
}
void displaythree()
{
	//std::cout << FileHolder.begin() << " , " << FileHolder.end() << ", Fileholder\n";
	for (auto line = FileHolder.begin(); line < FileHolder.end(); line++) {
		for (auto field = line->begin(); field < line->end(); field++)
			std::cout << *field;
		std::cout << "\n";
	}
}
void SetDelimeter(std::ifstream& is)
{
	while (!is.eof())
	{
		if (is.peek() == ',') { delimeter = ','; break; }
		if (is.peek() == '|') { delimeter = '|'; break; }
		is.ignore();
	}
	is.clear();
	is.seekg(0);
}
	};
} // namespace


