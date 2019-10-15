/*
 * CSVReader.h
 *
 *  Created on: Oct 7, 2019
 *      Author: Babak
 */

#ifndef SRC_CSVREADER_H_
#define SRC_CSVREADER_H_

#include <fstream>
#include <vector>
#include <iterator>

//#include <iostream>
//#include <sstream>

namespace TransModel {

/*
 * A class to read a csv file.
 *
 */

class CSVReader
{
	std::string fileName;
	std::string delimeter;

 
public:
	public:
	CSVReader(std::string filename, std::string delim = ",") :
			fileName(filename), delimeter(delim) {}
 
	//Member functions
	std::vector<double> readAsDouble();
};

std::vector<double> CSVReader::readAsDouble()
{
	std::fstream file;

	std::vector<double> vals;

    file.open(fileName);
	if (!file.is_open()) {
        std::cout << "******ERORR: in CSVReader:: could not read the file: " << fileName<< std::endl;
		exit(0);
	}
    std::string line;
    while (getline( file, line,'\n'))
	{
	  std::istringstream templine(line);
	  std::string data;
	  while (getline( templine, data,',')) 
	  {
	    vals.push_back(atof(data.c_str()));  
	  }
	}

    file.close();

	return vals;

}

} /* namespace TransModel */

#endif /* SRC_CSVREADER_H_ */