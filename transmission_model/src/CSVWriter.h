/*
 * CSVWriter.h
 *
 *  Created on: May 15, 2019
 *      Author: Babak
 */

#ifndef SRC_CSVWRITER_H_
#define SRC_CSVWRITER_H_

#include <fstream>
#include <vector>
#include <set>
#include <iterator>
#include <string>
//using namespace std;

namespace TransModel {

/*
 * A class to create and write data in a csv file.
 *  Examples: 
 *  std::vector<std::string> aContainer = { "bla", "10", "blo" };
 *	std::set<int> aContainer = { 8, 10, 44, 59};
 *	std::string aContainer = "abcd";
 *  writer.addRow(aContainer);
 */

class CSVWriter
{
	std::string fileName;
	std::string delimeter;
	int linesCount;
 
public:
	CSVWriter(std::string filename, std::string delim = ",") :
			fileName(filename), delimeter(delim), linesCount(0)	{}

	//Member functions
	template<class iterable_type>
	void addHeader(iterable_type aContainter);

	template<class iterable_type>
	void addRow(iterable_type aContainter);

	template<class iterator_type>
	void addRowRange(iterator_type first, iterator_type last);
	
	template<typename T>
	void addSingleValue(T val);

};


/*
 * Function accepts a range of values and appends all its elements
 * to the last row, seperated by delimeter
 */
template<class iterator_type>
void CSVWriter::addRowRange(iterator_type first, iterator_type last)
{
	std::fstream file;
	// Open csv file in truncate mode if is first line else in append mode
	file.open(fileName, std::ios::out | (linesCount ? std::ios::app : std::ios::trunc));
 
	for (; first != last; )	{
		file << *first;
		if (++first != last)
			file << delimeter;
	}
	file << "\n";
	linesCount++;
 	file.close();
}


/*
 * Function accepts an itererable container and appends all its elements 
 * to the last row, seperated by delimeter
 */
template<class iterable_type>
void CSVWriter::addRow(iterable_type aContainter)
{
	std::fstream file;
	// Open csv file in truncate mode if is first line else in append mode
	file.open(fileName, std::ios::out | (linesCount ? std::ios::app : std::ios::trunc));

	for (auto it = aContainter.begin(); it != aContainter.end(); ++it) {
		file << *it;
		file << delimeter;
	}
	file << "\n";
	linesCount++;
	file.close();
}

/*
 * Function accepts an itererable container containing header labels of the CSV file
 * and appends all its elements to the first row, seperated by delimeter 
 */
template<class iterable_type>
void CSVWriter::addHeader(iterable_type aContainter)
{
	std::fstream file;
	// Open csv file in truncate mode 
	file.open(fileName, std::ios::out | (std::ios::trunc));

    if (aContainter.size()== 1){

		file << *aContainter.begin();
	}
    else {
	    for (auto it = aContainter.begin(); it != aContainter.end(); ++it) {
		   file << *it;
		   file << delimeter;
	    }
	}

	file << "\n";
	linesCount++;
 
	file.close();
}
 
/*
 * Function adds a single row value to the last line of cvs file
 */
template<typename T>
void CSVWriter::addSingleValue(T val)
{
	std::fstream file;
	// Open csv file in truncate mode if is first line else in append mode
	file.open(fileName, std::ios::out | (linesCount ? std::ios::app : std::ios::trunc));
 
	file << val;
	file << "\n";
	linesCount++;
 
	// Close csv file
	file.close();
}


} /* namespace TransModel */

#endif /* SRC_CSVWRITER_H_ */