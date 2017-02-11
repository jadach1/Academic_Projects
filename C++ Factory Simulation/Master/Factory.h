#pragma once
#include "Job.h"
#include "Machine.h"
#include <string>
#include <vector>
#include <queue>
#include <iostream>


namespace milestone {
	class Factory: public TaskManager, ItemManager, OrderManager, Job {
		OrderManager* factoryorders;
		ItemManager* factoryitems;
		TaskManager* factorytasks;
		std::vector<Job> jobs;  // Holds customer orders
		std::vector<Machine> machinez; // Holds Tasks
		int Day; // factory day tracker
	public:
		Factory(): Day(0) {}
		Factory(TaskManager* t, ItemManager* i, OrderManager* o) {  
			// sets pointers to the class instances of task, item and order which have already been created.
			Day = 0;
			std::cout << "Welcome to the Simulation Factory!\n";
			std::cout << "**********************************\n";
			factorytasks = t; factoryitems = i; factoryorders = o;
			CustomerOrderAndItemCheck(); // Verify all items in order exist!
			FindSourceNode(); // Calculate Source Node
			LoadJobs(); // Load jobs from customer orders to jobs vector
			std::cout << "Today we will have " << jobs.size() << " jobs to Install... Lets Begin!\n\n\n\n";
			LoadMachinez();
			RunFactory();
			std::cout << "\nWE HAVE SUCCESSFULLY SIMULATED INSTALLATION OF ALL OCUSTOMER ORDERS\n";
		}
		void display() {
			// Test to see all values are pointing to the correct space in memory
			std::cout << "tasks inside factory =\n";
			for (TaskDataElements* p = factorytasks->tasks; p->NotEmpty(); p = p->next)
			{
				p->displayData();
			}
			std::cout << "items inside factory =\n";
			for (ItemDataElements* p = factoryitems->items; p->NotEmpty(); p = p->next)
			{
				p->displayData();
			}
			std::cout << "orders inside factory =\n";
			for (OrderDataElements* p = factoryorders->orders; p->NotEmpty(); p = p->next)
			{
				p->displayData();
			}
		}
		void CustomerOrderAndItemCheck() { //Checks to make sure the item ordered by the customer exists
			OrderDataElements* ordercheck = factoryorders->orders; // Set pointer to the inherited pointer in the original orders class
			for (ordercheck; ordercheck->NotEmpty(); ordercheck = ordercheck->next)
			{
				bool found = false;
				std::vector<std::string> CustList = ordercheck->GetList();
				for (int i = 0; i < CustList.size(); i++) {
					for (ItemDataElements* itemcheck = factoryitems->items; itemcheck->NotEmpty(); itemcheck = itemcheck->next)
					{
						if (CustList[i] == itemcheck->GetName()) found = true;
					}
					if (found == false)
						throw std::string("No Item found in item data from customer order regarding-->" + CustList[i]);
				}
			}
		}
		void FindSourceNode() {
			//This function will query tasks, and calculate what is a source and what is a sink
			for (TaskDataElements* p = factorytasks->tasks; p->NotEmpty(); p = p->next)
			{
				std::string pname;  
				if (p->PassExists()) // if a passed item exists to install, we will copy the name
				{
					pname = p->GetPass();
					for (TaskDataElements* k = factorytasks->tasks; k->NotEmpty(); k = k->next)
					{
						// Find the Node which will install this passed item and add to its source variable
						if (pname == k->GetName()) {  // node found, increment sink/source
							k->AddSource(); // k cannot be a source
							p->AddSink(); // p cannot be a sink
							break;
						}
					}
				}
				if (p->FailExists()) { // if a item to be removed exists,
					pname = p->GetFail();
					for (TaskDataElements* k = factorytasks->tasks; k->NotEmpty(); k = k->next)
					{
						// Find the Node which will install this passed item and add to its source variable
						if (pname == k->GetName()) {  // node found, increment sink/source
							k->AddSource(); // k cannot be a source
							p->AddSink(); // p cannot be a sink
							break;
						}
					}
				}
			}
			
		}
		void DisplaySource() {
			std::cout << "Nodes source n sink!/n";
			for (TaskDataElements* p = factorytasks->tasks; p->NotEmpty(); p = p->next)
			{
				std::cout << p->GetSource() << "/ " << p->GetSink() << "<--" << p->GetName() << "\n";
			}
		}
		void LoadJobs() {
			OrderDataElements* od = factoryorders->orders;
			for (od; od->NotEmpty(); od = od->next) {
				Job job(od); //Create an instance of job which stores the current pointer of orderdataelements which holds 1 set of orders
				jobs.push_back(job);  // pushes back the job just created into the vector of jobs to be completed
			}
		}//loads jobs into vector
		void DisplayJobs()
		{
			std::cout << "size of jobs is " << jobs.size() << "\n" <<
				"Inside jobs \n";
			for (int i = 0; i < jobs.size(); i++)
			{
				for (int k = 0; k < jobs[i].GetJobSize(); k++)
					std::cout << jobs[i].GetJob(k) << ", ";
				std::cout << "\n";
			}
		} // displays all jobs inside vector jobs
		void LoadMachinez() {
			TaskDataElements* td = factorytasks->tasks;
			for (td; td->NotEmpty(); td = td->next)  //push everything into a  vector
			{
				ItemDataElements* od = factoryitems->items;
				Machine machine(td,od);
				machinez.push_back(machine);
			}
		}
		void RunFactory() {
			// find out which machine the job will be sent too next
			auto FindNextMachine = [=](std::string str, int i, ItemDataElements* id)->int{
				if (str == "bogey"){
					return 0;
				}
				for (int k = 0; k < machinez.size(); k++){
					if (str == machinez[k].MachineName())
					{
						return k;
					}
				}
			};
			// Find the Source Machine
			auto FindSourceMachine = [=]()->int{
				for (int k = 0; k < machinez.size(); k++){
					if (machinez[k].SourceMachine()) {
						return k;
					}
				}
			};
			// Run Machine by check if jobs is complete or not
			std::string str;					
			// List through jobs and complete them
			for (int i = 0; i < jobs.size(); i++) {
				// Find Source Machine!
				int begin = FindSourceMachine();
				std::cout << "Job Details->"; jobs[i].OderDetails();
				while (!jobs[i].IsComplete()) {					
							ItemDataElements* id = factoryitems->items;
							machinez[begin].RunMachine(jobs[i], id, Day);// Passes list of jobs to source machine to check for install , remove / pass,fail
								str = machinez[begin].GetNext();
								begin = FindNextMachine(str, i, id);
								if (str == "bogey"){
									break;
								}
							} // while
				std::cout << "----------------------MOVING TO NEXT JOB-----------------------------\n\n";
				} // for
		}
	};

} // namespace
