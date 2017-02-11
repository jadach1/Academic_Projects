#pragma once
#include <fstream>
#include<vector>
#include <iostream>
#include <sstream>
#include <string>
#include "CustomerOrder.h"
extern const std::string GraphOrder = "GraphOrder.gv";

namespace milestone {

	class OrderManager : public Order {
		int count; //counts number of elements in TaskDateElements
	public:
		class OrderDataElements {  // This class holds all the information regarding the task, slots, passed and failed which will be written to the graph
			std::string customer;
			std::string product;
			std::vector<std::string>orderlist;
		public:
			OrderDataElements* next;
			OrderDataElements() :next(nullptr) {  }
			OrderDataElements(OrderDataElements fakeorders, OrderDataElements* orders)  //overloaded constructor for when we pass in fakeorders
			{
				customer = fakeorders.Getcustomer(); product = fakeorders.GetProduct();
				orderlist = fakeorders.GetList();  next = orders;
			}
			~OrderDataElements() {}
			void Setcustomer(std::string n) { trim(n); if (n[0] != '\0') customer = n; }  // set the customer string and erase and leftover whitespaces
			void Setproduct(std::string n) { trim(n); if (n[0] != '\0') product = n;   }
			void SetList(std::string n) { trim(n); ConvertToUpperCase(n);  if (n[0] != '\0') orderlist.push_back(n); }
			std::string Getcustomer() { return customer; }
			std::string GetProduct() { return product; }
			std::vector<std::string> GetList() { return orderlist; }  // returns a vector
			std::string GetListField(int field) { return orderlist[field]; }// returns a string from inside the vector
			int SizeList() { return orderlist.size(); }  // returns the size of the orderlist
			bool NotEmpty() { if (customer[0] != '\0') return true; else return false; }  // if object not empty return true, otherwise return false 
			void Reset() { customer[0] = '\0';  
			for (int i = 0; i < orderlist.size(); i++)
				orderlist[i] = '\0';
							} // Resets object to safe empty state
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
				std::cout << customer;
				for (int i = 0; i < orderlist.size(); i++){
					std::cout << " ,"<< orderlist[i];
				}
				std::cout << "\n";
			}
		};  // class datatasks
		OrderDataElements* orders;
		OrderManager(){
			count = 0;
			//display();
		}
		OrderManager(const std::vector<std::vector<std::string> > temp) : Order(temp)
		{
			auto error = [](std::string message, std::string type, std::string found){ // checks for errors when parsing
				throw message + "expected ->" + type + "found->" + found + "\n";
			};

			auto isNotEmpty = [](std::string str)->bool{
				if (str.empty()) return false;  // check if parameter is empty
				else return true;
			};
			
			if (sizeone() > 0){ // checks to make sure their is data in vector and allocates memory;		
				orders = new OrderDataElements; // Create a new task data element to point to nullptr
				orders->next = nullptr;
				for (int line = sizeone()-1; line >= 0; line--){  // copies line by line all the fields in the vector holding the task/list/customerorder
					OrderDataElements fakeorders; // Make an object to hold properties to be copied
					if (isNotEmpty(Parameter(line, 0))) fakeorders.Setcustomer(Parameter(line, 0));
					else error(std::string("looking for customer"), std::string(" in Customer Orders, "), (Parameter(line, 0)));
						if (isNotEmpty(Parameter(line, 1))) fakeorders.Setproduct(Parameter(line, 1));
						else error(std::string("looking for product"), std::string(" in Customer Orders, "), (Parameter(line, 1)));
							for (int field = 2; field < sizetwo(line); field++){  // figure out the size of the 2nd dimension in the fileholder array
								// define elements and parse into fakeorders
								if (isNotEmpty(Parameter(line, field))) fakeorders.SetList(Parameter(line, field));
								else error(std::string("looking for Order customer"), std::string(" in Customer Orders, "), (Parameter(line, field)));			
					} // for field
					orders = new OrderDataElements(fakeorders, orders);
					fakeorders.Reset();  // delete faketasks to reset all valeus.
				} // for line
				//std::cout << "Order Data Elements--->\n";
				//graphOrders();  // Call Graph for Tasks
			}// if
		} // end of constructor
		~OrderManager(){}
		void display()  //displays item list in Task Class
		{
			std::cout << "OrderManager display \n";
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
			std::cout << "end of Ordermanager display\n";
		}
		void graphOrders()
		{
			std::cout << "open graph orders\n";
			std::vector<std::string> consistency;  // for consistency check
			std::ofstream os;
			os.open(GraphOrder); // create file
			if (os.is_open()) {
				os << "digraph Orders{\n";
				os << R"(node[style="filled",fillcolor="grey"])" << "\n";
				/*for (TaskDataElements* p = tasks; p->NotEmpty(); p = p->next) {
				p->displayData(os);
				}*/
				// Create .gv file with conditions to print graph using graphviz
				for (OrderDataElements* p = orders; p->NotEmpty(); p = p->next) {															
					for (int i = 0; i < p->SizeList(); i++)
						os << "\"" << p->GetListField(i) << "\"" << "->" << "\"" << p->Getcustomer() << "==" << p->GetProduct() << "\"" << "[color=black]"<<"\n";				
				}  // end of for
				os << "}";
				os.close();  // done with file, CLOSE IT!!!

				//std::cout << "leaving graph\n";
				const std::string cmd
					= "dot -Tpng " + GraphOrder + " > " + GraphOrder + ".png";
					system(cmd.c_str());
			}
			else throw std::string("unable to open file");
		}
		
	};

} // customerspace
