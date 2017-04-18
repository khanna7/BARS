/*
 * file_utils.h
 *
 *  Created on: Mar 3, 2016
 *      Author: nick
 */

#ifndef SRC_FILE_UTILS_H_
#define SRC_FILE_UTILS_H_

namespace TransModel {

/**
 * Creates a unique file name from the specified file_name. If a
 * file with the file_name path does not exist then file_name is returned
 */
std::string unique_file_name(const std::string& file_name);

}



#endif /* SRC_FILE_UTILS_H_ */
