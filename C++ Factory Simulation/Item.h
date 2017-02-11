#pragma once
#include <vector>
namespace milestone {


	class Item {
		std::vector<std::vector<std::string> > ItemList;
	public:
		Item() {}
		Item(const std::vector<std::vector<std::string> >& pass)
		{
			ItemList = pass;
			//display();
		}
		~Item()
		{
			//delete this;
		}
		long sizethree()const {
			return sizeof(ItemList);
		}
		size_t sizeone()const //returns how many lines their are in the 2D vector
		{ 
			return ItemList.size();
		}
		size_t sizetwo(const int line)const  // returns how many fields their are in the vector line
		{
			return ItemList[line].size();
		}
		std::string Parameter(const int line, const int field)
		{
			return ItemList[line][field];
		}
		std::vector<std::vector<std::string> >& GetList()
		{
			return ItemList;
		}
		void display()
		{
			std::cout << "diplsaying Task List Entity\n";
			for (auto line : ItemList) {
				for (auto field : line)
					std::cout << field << ", ";
				std::cout << "\n";
			}
			std::cout << "Ending Task List entity \n\n";
		}
	};
}
