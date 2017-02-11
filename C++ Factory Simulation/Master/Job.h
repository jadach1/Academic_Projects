#pragma once
#include "CustomerOrderManager.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace milestone {
	class Job : public OrderManager {
		std::vector<bool>installed;
		std::vector<std::string>jobs;
		std::string CustomerName;
		std::string CustomerProduct;
	public:
		Job(){}
		Job(OrderDataElements* od)
		{// returns list of jobs from customer
			jobs = od->GetList();
			CustomerName = od->Getcustomer();
			CustomerProduct = od->GetProduct();
			installed.resize(jobs.size(),false);
		}
		int GetJobSize() { return jobs.size(); }
		int GetInstallSize() { return installed.size(); }
		void OderDetails() {
			std::cout << " Customer : " << CustomerName;
			std::cout << " Ordered: " << CustomerProduct;
			std::cout << " Parts: ";
			for (int i = 0; i < jobs.size(); i++) {
				std::cout << jobs[i] << " ";
			}
			std::cout << "\n";
		}
		void UpdateInstall(bool boo, int i) { installed[i] = boo; }
		bool GetInstalled(int i) { return installed[i]; }
		std::vector<std::string> GetJobs() {
			return jobs;
		}
		std::string GetJob(int i)
		{
			if (jobs.size() > i)
				return jobs[i];
			else
				return "Empty Job Bracket";
		}
		bool IsComplete() {
			bool complete;
			for (int i = 0; i < installed.size(); i++)
				complete = installed[i];
			return complete;
		} // Function will query through installed, if anything i uninstalled it will return false
	};

}