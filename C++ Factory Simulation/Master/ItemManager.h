#pragma once
#include <fstream>
#include<vector>
#include <iostream>
#include <sstream>
#include <string>
#include "Item.h"
extern const std::string Graph = "GraphItem.gv";

namespace milestone {

	class ItemManager : public Item {
	public:
		class ItemDataElements {// This class holds all the information regarding the Item, slots, installer and remover, Description which will be written to the graph
			std::string name, installer, remover, description;
			int slots;
		public:
			ItemDataElements* next; 
			ItemDataElements() :slots(1), next(nullptr) {  }  // Default constructor
			ItemDataElements(ItemDataElements fakeitems, ItemDataElements* items) // intializes a new ItemManager, copying from faktasks and assigning the next* to the previous node in the list
			{
				name = fakeitems.GetName(); installer = fakeitems.GetInstall(); remover = fakeitems.GetRemover();
				slots = fakeitems.GetSlot(); description = fakeitems.GetDesc();  next = items;
			}
			~ItemDataElements() { }
			void SetName(std::string n) { name = ConvertToUpperCase(trim(n)); }  // set the name string and erase and leftover whitespaces
			void setInstall(std::string n) { installer = ConvertToUpperCase(trim(n)); } // set installer string after trim
			void setRemove(std::string n) { remover = ConvertToUpperCase(trim(n)); } // set remover string after trim
			void SetDesc(std::string n) { description = trim(n); } // set description string after trim
			void SetSlot(int n) { slots = n; }
			std::string GetName() { return name; }
			std::string GetInstall() { return installer; }
			std::string GetRemover() { return remover; }
			std::string GetDesc() { return description; }
			int GetSlot() { return slots; }
			bool NotEmpty() { 
				if (name[0] != '\0') return true; 
				else return false; }  // if object not empty return true, otherwise return false 
			void Reset() { name[0] = '\0'; installer[0] = '\0'; remover[0] = '\0'; slots = 0; description[0] = '\0'; } // Resets object to safe empty state
			std::string& trim(std::string& str)  // trims white spaces at the start and end of a string
			{
				//std::cout << "size begin-->" << str.size() << "size after-->";  
				while (!str.empty() && isspace(str[0]))
					str.erase(0, 1);
				while (!str.empty() && isspace(str[str.size() - 1]))
					str.erase(str.size() - 1, 1);
				//std::cout << "size-->" << str.size() << "\n";
				return str;
			}
			std::string& ConvertToUpperCase(std::string& str)  // converts a string to upper case
			{
				if (str[0] != '\0')
				{
					for (int i = 0; i < str.length(); i++)
					{
						str[i] = toupper(str[i]);
					}
				}
				return str;
			}
			void displayData() {  //generically print data to screen
				std::cout << name << installer << remover << slots << description << "\n";
			}
		};  // class datatasks
		ItemDataElements* items;
		ItemManager(){
			//display();
		}
		ItemManager(const std::vector<std::vector<std::string> > temp) : Item(temp)
		{
			auto error = [](std::string message, std::string type, std::string found){ // checks for errors when parsing
				throw message + "expected ->" + type + "found->" + found + "\n";
			};

			auto isTaskName = [](std::string name)->bool{
				if (name.empty()) return false;  // check if parameter is empty
				if (!isalnum(name[0]) && name[0] != '_' && name[0] != '-') return false;
				else return true;
			};
			auto isTaskNum = [](std::string n)->bool{
				std::stringstream ss(n);
				int i;
				if (ss >> i) return true;
				else return false;
			};
		
			if (sizeone() > 0){ // checks to make sure their is data in vector and allocates memory;		
				items = new ItemDataElements; // Create a new task data element to point to nullptr
				items->next = nullptr;
				for (int line = sizeone() - 1; line >= 0; line--){  // copies line by line all the fields in the vector holding the task/list/customerorder
					ItemDataElements fakeitems; // Make an object to hold properties to be copied
					for (int field = sizetwo(line); field > 0; field--){  // figure out the size of the 2nd dimension in the fileholder array
						switch (field){ // grab the vector field of i and parse data
						case 5:
							fakeitems.SetDesc(Parameter(line, 4));
							break;
						case 4:
							if (isTaskNum(Parameter(line, 3))){ fakeitems.SetSlot(stoi(Parameter(line, 3))); }
							else error(std::string("looking for Slots"), std::string("Items"), (Parameter(line, 3)));
							break;
						case 3:
							if (isTaskName(Parameter(line, 2))) fakeitems.setRemove(Parameter(line, 2));
							else error(std::string("looking for remover"), std::string("Items"), Parameter(field, 2));
							break;
						case 2:
							if (isTaskName(Parameter(line, 1))) fakeitems.setInstall(Parameter(line, 1));
							else error(std::string("looking for installer"), std::string("Items"), Parameter(line, 1));
							break;
						case 1:
							if (isTaskName(Parameter(line, 0))) fakeitems.SetName(Parameter(line, 0));
							else error(std::string("looking for Name"), std::string("Items"), Parameter(line, 0));
							break;
						default:
							error(std::string("No Parameters Found in "), std::string("items"), std::string("none"));
						} // switch
					} // for field
					items = new ItemDataElements(fakeitems, items);
					fakeitems.Reset();  // delete faketasks to reset all valeus.
				} // for line
				//graphTasks();  // Call Graph for Tasks
				/*for (TaskDataElements* p = tasks; p; p = p->next) {
				p->displayData();
				}*/
			}// if
		} // end of constructor
		~ItemManager(){	}
		void display()  //displays item list in Task Class
		{
			std::cout << "ItemManager display \n";
			int line = sizeone();
			for (int i = 0; i < line; i++)
			{
				int field = sizetwo(i);
				std::cout << "size of array-->" << line << "/ Size of field--->" << field << "\n";
				std::cout << "Values pulled are: ";
				for (int k = 0; k < field; k++)
				{
					std::string parm(Parameter(i, k));
					std::cout << parm << ", ";
				}
				std::cout << "\n";
			}
			std::cout << "end of taskmanager display\n";
		}
		void graphTasks()
		{
			std::vector<std::string> consistency;  // for consistency check
			std::ofstream os;
			os.open(Graph); // create file
			if (os.is_open()) {	
				// Create .gv file with conditions to print graph using graphviz
				os << "digraph Task{\n";
				os << R"(node[style="filled",fillcolor="grey"])" << "\n";
				for (ItemDataElements* p = items; p->NotEmpty(); p = p->next) {
					std::string tempName = p->GetName();  // set temp Variables for name, pass and fail
					std::string tempPass = p->GetInstall();
					consistency.push_back(p->GetInstall());
					std::string tempFail = p->GetRemover();
					InstallRemoveCheck(p);
					os << "\"" << "Item-" + p->GetName() << "\"" << "[shape=box][color=blue]\n";
					if (tempPass[0] != '\0')
					{
						os << "\"" << "Installer-" + p->GetInstall() + "\"" << "[color=green]\n";
						if (tempFail[0] != '\0')
							os << "\"" << "Remover-" + p->GetRemover() + "\"" << "[color=red]\n";
					}
					if (tempPass[0] != '\0')
						os << "\"" << "Item-" + tempName << "\"" << "->" << "\"" << "Installer-" + tempPass << "\"" << "\n";		
						if (tempFail[0] != '\0')
							os << "\"" << "Item-" + tempName << "\"" << "->" << "\"" << "Remover-" + tempFail << "\"" << "\n";					
					}  // end of for
				os << "}";
			        os.close();
				const std::string cmd = "dot -Tpng " + Graph + " > " + Graph + ".png";
				system(cmd.c_str());
			}
			else throw std::string("unable to open file");
		}
		void InstallRemoveCheck(ItemDataElements* p)  //Ensures all installer or remover task refereces exist
		{
			std::string n = p->GetName();
			if (n.empty()) throw std::string("No name in data for Item data, revalidate data");
			std::string inst = p->GetInstall();
			if (inst.empty()) throw std::string("No Installer in data for Item data, revalidate data");
			std::string rem = p->GetRemover();
			if (rem.empty()) p->setRemove("Remove " + inst);
		}

	};


} // namespace
