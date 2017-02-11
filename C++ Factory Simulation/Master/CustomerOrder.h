#pragma once
#include <vector>
namespace milestone {


	class Order {
		std::vector<std::vector<std::string> > OrderList;
	public:
		Order() {}
		Order(const std::vector<std::vector<std::string> >& pass)
		{
			OrderList = pass;
			//display();
		}
		~Order()
		{
			//delete this;
		}
		long sizethree()const {
			return sizeof(OrderList);
		}
		size_t sizeone()const //returns how many lines their are in the 2D vector
		{
			return OrderList.size();
		}
		size_t sizetwo(const int line)const  // returns how many fields their are in the vector line
		{
			return OrderList[line].size();
		}
		std::string Parameter(const int line, const int field)
		{
			return OrderList[line][field];
		}
		std::vector<std::vector<std::string> >& GetList()
		{
			return OrderList;
		}
		void display()
		{
			std::cout << "diplsaying Task List Entity\n";
			for (auto line : OrderList) {
				for (auto field : line)
					std::cout << field << ", ";
				std::cout << "\n";
			}
			std::cout << "Ending Task List entity \n\n";
		}
	};
}


