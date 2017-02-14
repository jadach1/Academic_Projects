#pragma once
#include <vector>
namespace milestone {


	class Task {
		std::vector<std::vector<std::string> > TaskList;
	public:
		Task() {}
		Task(const std::vector<std::vector<std::string> >& pass)
		{
			TaskList = pass;
			//display();
		}
		~Task()
		{
			//delete this;
		}
		long sizethree()const {
			return sizeof(TaskList);
		}
		size_t sizeone()const { //returns size of task array
			return TaskList.size();
		}
		size_t sizetwo(const int field)const { // returns size of one vector inside array
			return TaskList[field].size();
		}
		std::string Parameter(const int line, const int field)
		{
			return TaskList[line][field];
		}
		std::vector<std::vector<std::string> >& GetList()
		{
			return TaskList;
		}
		 void display()
		{
			std::cout << "diplsaying Task List Entity\n";
			for (auto line : TaskList) {
				for (auto field : line)
					std::cout << field << ", ";
				std::cout << "\n";
			}
			std::cout << "Ending Task List entity \n\n";
		}
	};
}
