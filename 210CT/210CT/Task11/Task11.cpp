#include "Task11.h"
#include "../IO.h"
#include <fstream>

using namespace TASK_11;


Node::~Node() 
{
	//Recursively deletes entire tree
	if (lhs_child)
		delete lhs_child;
	if (rhs_child)
		delete rhs_child;
}

void Node::AddChild(Node* child) 
{
	if (*child == *this)
	{
		word.count++; //Don't want duplicates
	}
	else if (*child <= *this)
	{
		if (lhs_child)
			lhs_child->AddChild(child);
		else
			lhs_child = child;
	}
	else
	{
		if (rhs_child)
			rhs_child->AddChild(child);
		else
			rhs_child = child;
	}
}

Node* TASK_11::GenerateBSTFromCsv(std::string& file_path) 
{
	Node* root_node = nullptr;
	std::ifstream file;
	file.open(file_path, std::ifstream::in);

	if (!file.is_open())
	{
		IO::out << "Invalid path!\n";
		return nullptr;
	}

	char c;
	bool reading_word = true;
	std::string current_word;
	std::string current_count;

	while (file.get(c))
	{
		//Convert to lower case
		if (c >= 65 && c <= 90)
			c += 32;

		if (c == ',')
			reading_word = false;
		else if (c == '\r' || c == '\n')
		{
			if (current_word != "")
			{
				const int count = std::stoi(current_count);
				Node* node = new Node(Word(current_word, count));
				IO::out_debug << "Adding node: " << current_word << ":" << count << '\n';

				if (!root_node)
					root_node = node;
				else
					root_node->AddChild(node);
			}

			current_word = "";
			current_count = "";
			reading_word = true;
		}
		else 
		{
			if (reading_word)
				current_word += c;
			else
				current_count += c;
		}
	}
	file.close();

	if (current_word != "")
	{
		const int count = std::stoi(current_count);
		Node* node = new Node(Word(current_word, count));
		IO::out_debug << "Adding node: " << current_word << ":" << count << '\n';

		if (!root_node)
			root_node = node;
		else
			root_node->AddChild(node);
	}

	return root_node;
}

Node* TASK_11::GenerateBSTFromTxt(std::string& file_path)
{
	Node* root_node = nullptr;
	std::ifstream file;
	file.open(file_path, std::ifstream::in);

	if (!file.is_open())
	{
		IO::out << "Invalid path!\n";
		return nullptr;
	}

	char c;
	std::string current_word;

	while (file.get(c))
	{
		//Convert to lower case
		if (c >= 65 && c <= 90)
			c += 32;

		if (c == ' ' || c == '\n' || c == '\r' || c == '.' || c == ';' || c == ':' || c == ',' || c == '(' || c == ')')
		{
			if (current_word.size())
			{
				Node* node = new Node(Word(current_word));
				IO::out_debug << "Adding node:" << current_word << '\n';

				if (!root_node)
					root_node = node;
				else
					root_node->AddChild(node);
			}
			current_word = "";
		}
		else
			current_word += c;
	}
	file.close();

	if (current_word.size())
	{
		Node* node = new Node(Word(current_word));
		IO::out_debug << "Adding node:" << current_word << '\n';

		if (!root_node)
			root_node = node;
		else
			root_node->AddChild(node);
	}

	return root_node;
}

uint Node::Count(std::string& string, const bool report/*= false*/)
{
	if (report)
		IO::out << "['" << string << "':Search] Visited '" << word.string << "' node\n";

	if (*this == string)
		return this->word.count;
	
	if (*this < string)
	{
		if (rhs_child)
			return rhs_child->Count(string, report);
		else
			return 0;
	}
	
	if (*this > string)
	{
		if (lhs_child)
			return lhs_child->Count(string, report);
		else
			return 0;
	}

	return 0;
}

void Node::PrintInOrder()
{
	if (lhs_child)
		lhs_child->PrintInOrder();

	IO::out << "'" << word.string << ", ";

	if (rhs_child)
		rhs_child->PrintInOrder();
}

void Node::PrintPreOrder()
{
	IO::out << "'" << word.string << ", ";

	if (lhs_child)
		lhs_child->PrintPreOrder();

	if (rhs_child)
		rhs_child->PrintPreOrder();
}

void Node::PrintPostOrder()
{
	if (lhs_child)
		lhs_child->PrintPostOrder();

	if (rhs_child)
		rhs_child->PrintPostOrder();

	IO::out << "'" << word.string << ", ";
}


void TASK_11::Execute() 
{
	bool read_csv;
	IO::out << "Input can either be from a .csv where each entry is word,count \nor it can be raw text.\n";

	{
		IO::out << "Would you like to read from a .csv (Y/N)? ";
		char input;
		IO::in >> input;
		read_csv = input == 'Y' || input == 'y' ? 1 : 0;
	}

	Node* root_node = nullptr;


	if (read_csv)
	{
		std::string file_path;
		IO::out << "Please input path to csv file:\n";
		IO::in >> file_path;

		root_node = GenerateBSTFromCsv(file_path);
	}
	else 
	{
		std::string file_path;
		IO::out << "Please input path to raw text file:\n";
		IO::in >> file_path;

		root_node = GenerateBSTFromTxt(file_path);
	}

	if (!root_node) //Could not be set(Invalid file/format)
		return;

	std::string word;
	IO::out << "What word would you like to look for? ";
	IO::in >> word;

	IO::out << "Count for '" << word << "' is " << root_node->Count(word, true) << '\n';

	IO::out << "How would you like to print the tree?\n(InOrder = 0, PreOrder = 1, PostOrder = 2, Any other number to skip)";
	int order;
	IO::in >> order;

	switch (order)
	{
	case 0:
		IO::out << "\nTree (InOrder):\n";
		root_node->PrintInOrder();
		break;
	case 1:
		IO::out << "\nTree (PreOrder):\n";
		root_node->PrintPreOrder();
		break;
	case 2:
		IO::out << "\nTree (PostOrder):\n";
		root_node->PrintPostOrder();
		break;
	}
	IO::out << '\n';

	delete root_node;
}