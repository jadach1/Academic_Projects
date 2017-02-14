#pragma once
#include <fstream>
#include<vector>
#include <iostream>
#include <sstream>
#include <string>
#include "Task.h"
extern const std::string GraphTask = "GraphTask.gv";

namespace milestone {

	class TaskManager : public Task {
	public:
		class TaskDataElements {  // This class holds all the information regarding the task, slots, passed and failed which will be written to the graph
			std::string name, passed, failed;
			int slots;
			bool PassedExists;
			bool FailedExists;
			int sink;  // this value will increase if this node is pointing to another node.  0 means its a sink
			int source;  // this value will increase if another node is pointing to this node.  0 means its a source
			public:
				TaskDataElements* next;
				TaskDataElements() :slots(1), sink(0), source(0), PassedExists(false), FailedExists(false), next(nullptr) {  }
			TaskDataElements(TaskDataElements faketasks, TaskDataElements* tasks)
			{
				name = faketasks.GetName(); passed = faketasks.GetPass(); failed = faketasks.GetFail();
				slots = faketasks.GetSlot(); next = tasks; sink = faketasks.sink; source = faketasks.source; 
				PassedExists = faketasks.PassedExists; FailedExists = faketasks.FailedExists;
			}
			~TaskDataElements() { }
			void SetName(std::string n) { ConvertToUpperCase(n); name = trim(n); }  // set the name string and erase and leftover whitespaces
			void SetPass(std::string n) { ConvertToUpperCase(n); passed = trim(n); } // set passed string after trim
			void SetFail(std::string n) { ConvertToUpperCase(n); failed = trim(n); } // set failed string after trim
			void SetSlot(int n) { slots = n; }
			void PassOn(bool n) { PassedExists = n; } // Sets a flag for whether their is a value for the passed param
			void FailOn(bool n) { FailedExists = n; } // Sets a flag for whether their is a value for the failed param
			void AddSink() { sink++; }  // increment sink, 0 means it's a sink. gg
			void AddSource() { source++; }// increment source, 0 means it's a source
			bool PassExists() const{ return PassedExists;}
			bool FailExists() const{ return FailedExists; }
			int GetSink() const { return sink; }
			int GetSource() const { return source; }
 			std::string GetName() const{ return name; }
			std::string GetPass() const{ return passed; }
			std::string GetFail() const{ return failed; }
			int GetSlot() const{ return slots; }
			bool NotEmpty() { if (name[0] != '\0') return true; else return false; }  // if object not empty return true, otherwise return false 
			void Reset() { name[0] = '\0'; passed[0] = '\0'; failed[0] = '\0'; slots = 0; } // Resets object to safe empty state
			std::string& trim(std::string& str)  // trims white spaces at the start and end of a string
			{
				//std::cout << "size begin-->" << str.size() << "size after-->";  
				while (!str.empty() && isspace(str[0]))
					str.erase(0, 1);
				while (!str.empty() && isspace(str[str.size() - 1]))
					str.erase(str.size()-1, 1);
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
				std::cout << name << slots << passed << failed << "\n";
			}
		};  // class datatasks
		TaskDataElements* tasks;
		TaskManager(){
			//display();
		}
		TaskManager(const std::vector<std::vector<std::string> > temp) : Task(temp)
		{
		auto error = [](std::string message, std::string type, std::string found){ // checks for errors when parsing
			throw message + "expected ->" + type + "found->" + found + "\n";
		};

		auto isTaskName = [](std::string str)->bool{ 
			if (str.empty()) return false;  // check if parameter is empty
			if (!isalnum(str[0]) && str[0] != '_' && str[0] != '-') return false;
			else return true;
		};
		auto isTaskNum = [](std::string n)->bool{
			std::stringstream ss(n);
			int i;
			if (ss >> i) return true;
			else return false;
		};
		if (sizeone() > 0){ // checks to make sure their is data in vector and allocates memory;		
			tasks = new TaskDataElements; // Create a new task data element to point to nullptr
			tasks->next = nullptr;
			for (int line = sizeone()-1; line >= 0; line--){  // copies line by line all the fields in the vector holding the task/list/customerorder
				TaskDataElements faketasks; // Make an object to hold properties to be copied
				for (int field = sizetwo(line); field > 0; field--){  // figure out the size of the 2nd dimension in the fileholder array
					switch (field){ // grab the vector field of i and parse data
					case 4:
						if (isTaskName(Parameter(line, 3))) { faketasks.SetFail(Parameter(line, 3)); faketasks.FailOn(true); }
						else faketasks.FailOn(false);
						break;
					case 3:
						if (isTaskName(Parameter(line, 2))) { faketasks.SetPass(Parameter(line, 2)); faketasks.PassOn(true); }
						else faketasks.PassOn(false);
						break;
					case 2:
						if (isTaskNum(Parameter(line, 1))){ faketasks.SetSlot(stoi(Parameter(line, 1))); }
						else error(std::string("looking for Slots"), std::string("Tasks"), (Parameter(line, 1)));
						break;
					case 1:
						if (isTaskName(Parameter(line, 0))) faketasks.SetName(Parameter(line, 0));
						else error(std::string("looking for Name"), std::string("Tasks"), Parameter(line, 0));
						break;
					default:
						error(std::string("No Parameters Found in "), std::string("Tasks"), std::string("none"));
					} // switch
				} // for field
				tasks = new TaskDataElements(faketasks, tasks);
				faketasks.Reset();  // delete faketasks to reset all valeus.
			} // for line
		}// if
		} // end of constructor
		~TaskManager()
		{
			//delete this;
		}
	
		void display()  //displays item list in Task Class
		{
			std::cout << "TaskManager display \n";
			int line = sizeone();
			for (int i = 0; i < line; i++)
			{
				int field = sizetwo(i);
				std::cout << "size of array-->" << line << "/ Size of field--->" << field << "\n";
				std::cout << "Values pulled are: ";
				for (int k = 0; k < field; k++)
				{
					std::string parm(Parameter(i, k));
					std::cout <<  parm << ", ";
				}
				std::cout << "\n";
			}				
			std::cout << "end of taskmanager display\n";
		}
		void graphTasks()
		{
			std::vector<std::string> Consistency;
			std::ofstream os;
			os.open(GraphTask); // create file
			if (os.is_open()) {
				os << "digraph Task{\n";
				os << R"(node[style="filled",fillcolor="grey"])" << "\n";
				/*for (TaskDataElements* p = tasks; p->NotEmpty(); p = p->next) {
					p->displayData(os);
				}*/
				// Create .gv file with conditions to print graph using graphviz
			int i = 0;
			for (TaskDataElements* p = tasks; p->NotEmpty(); p = p->next, i++) {
					std::string tempName = p->GetName();  // set temp Variables for name, pass and fail
					Consistency.push_back(tempName);
					std::string tempPass = p->GetPass();
					std::string tempFail = p->GetFail();
					ConsistencyCheck(p, tempPass, tempFail,Consistency);					
						if (p->PassExists()) 
							os << "\"" << p->GetName() << "\""; os << "->" << "\"" << p->GetPass() << "\"" << "[color=green]" << "\n";
						if (p->FailExists()) 
							os << "\"" << p->GetName() << "\"" << "->" << "\"" << p->GetFail() << "\"" << "[color=red]" << "\n";
				}  // end of for
				os << "}";
			        os.close();  // done with file, CLOSE IT!!!
					// Run command line to call dot and graphviz
				const std::string cmd 
			         = "dot -Tpng " + GraphTask + " > " + GraphTask + ".png";
					  system(cmd.c_str());
			}
			else throw std::string("unable to open file" + GraphTask);
		}
		void ConsistencyCheck(TaskDataElements* p, const std::string pass, const std::string fail, const std::vector<std::string> consisteny){
			bool foundPass = false, foundFailed = false, ItemRemovedbutNotInstalled=false;
			for (TaskDataElements* R = tasks; R->NotEmpty(); R = R->next){
				if (pass.empty() || pass == R->GetName()) foundPass = true;
				if (fail.empty() || fail == R->GetName()) foundFailed = true;			
			}
			if (foundPass == false) throw std::string("Error inside Consistency Check for Task Manager, pass data found but does not match -->" + pass);
			if (foundFailed == false) throw std::string("Error inside Consistency Check for Task Manager, failed data found but does not match -->" + fail);
		}
	};

} // namespace
