#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include "Job.h"
#include <time.h>

namespace milestone {
	class Machine: public TaskManager, ItemManager {
		bool installer;
		bool remover;
		bool sink;
		bool source;
		std::string name, pass, fail;
		std::queue<std::string> inputQ;
		Job job;
	public:
		Machine() :installer(false), remover(false), sink(false), source(false) {}
		Machine(TaskDataElements* td, ItemDataElements* p)
		{
			name = td->GetName();  // establish task name
			pass = td->GetPass();
			fail = td->GetFail();
			installer = false; remover = false;
			if (td->GetSink() == 0)  //establish if its a sink or not
				sink = true;
			else
				sink = false;
			if (td->GetSource() == 0) // establish if this is a source!
				source = true;
			else
				source = false;
			// establish whether its a remover or installer
			for (p; p->NotEmpty(); p = p->next)
			{
				if (name == p->GetInstall()) {
					installer = true;
					break;
				}
				if (name == p->GetRemover()) {
					remover = true;
					break;
				}
			}	 // end of for			
		}
		std::string MachineName() { return name; }
		std::string getpass() { return pass; }
		std::string getfail() { return fail; }
		bool isInstaller() { return installer; }
		bool isRemover() { return remover; }
		bool SourceMachine() { return source; }
		void RunMachine(Job &j, ItemDataElements* ID, int &Day)
		{
			bool set = false;
			std::string Item; // item to be installed
			if (sink != true) {
				std::cout << "***********************MACHINE " << name << " *******************************************\n\n";
				/****************CHECK WHICH ITEMS NEEDS INSTALLATION******************************/
				for (int i = 0; i < j.GetInstallSize(); i++) {
					if (!j.GetInstalled(i)) { // if false we will run machine further
						 Item = j.GetJob(i);
						std::cout << "Looking to install " << Item << "\n";
						set = false;
						// run query to find what kind of item it is snf if it needs to be removed/installed
						ItemDataElements* id = ID; // reset the list of pointers to the start
						for (id; id->NotEmpty() && installer == true && set == false; id = id->next) {
							//std::cout << "Installer Machine Looking to install -> " << j.GetJob(i);
							std::string ins = id->GetInstall();
							
								if (installer == true && Item == id->GetName() && ins == name) {		// if item matches item in id then install and update						
									std::cout << "*****SUCCESS-INSTALLING****->" << j.GetJob(i) << "\n";									
										j.UpdateInstall(true, i);
										set = true;
									break;
								}
						}// for
					} //if				
				} // for
				if (set == false) // if the set flag is false, their were no matches throughout the whole loop
				 { std::cout << "NOT US! This Machine is for " << name << "\n---------------------------\n"; } 
				if (remover == true)
				{
					std::cout << name << "Move job to Next Machine!\n-----------------------------------\n";
				}
				std::cout << "***********************NEXT MACHINE*******************************************\n\n";
			} // sink 
			else  { 
				++Day;  
				std::cout << "This Machine is a sink....Day Over...Ending at" << 
					name << "\n***********************************Day-" << 
					Day <<"-Is Complete***********************\n\n"; } // else sink is this
			}  // end of function
		std::string GetNext() {
			// This is like an inspector, checks if the machine has a passed and/or fail and will send it to that machine
			if (fail.empty() && !pass.empty()) return pass;
			if (pass.empty() && !fail.empty()) return fail;
			if (!pass.empty() && !fail.empty())
			{
				// flip a coin.... if > 7 then it will fail
				int rando = rand() % 10;
				if (rando > 7)
					return fail;
				else
					return pass;
			}
			else return "bogey";  //if their is no pass or fail this will be marked as a bogey
		}
	};
	}
