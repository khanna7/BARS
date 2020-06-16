/*
 * JailParameters.h
 *
 *  Created on: March 1, 2019
 *      Author: Babak
 */

#ifndef SRC_JAILPARAMETERS_H_
#define SRC_JAILPARAMETERS_H_

#include <cmath>

namespace TransModel {

struct JailParameters {

    bool is_in_jail;
    bool is_first_time_jailed;
    double time_of_jail, serving_time;
    //float age_at_first_jail;  //@TODO: to remove:: not used currenlty 
    float age_at_jail;
    double time_since_jailed=0;
    int accumulative_injail_count;  //accumulative number of times agent has been jailed
    double accumulative_time_in_jail; //accumulative time agent spent time in jail 
    double time_of_release;
    
    // IF THESE DEFAULT VALUES CHANGE, MAKE SURE THE JAIL INITIALIZATION CODE IN
    // PERSONCREATOR IS STILL CORRECT
    JailParameters() : is_in_jail(false), is_first_time_jailed(true),
            time_of_jail(-1),
            serving_time(-1),
            //age_at_first_jail(-1),
            age_at_jail(-1),
            time_since_jailed(NAN),
            accumulative_injail_count(-1),
            accumulative_time_in_jail(0),
            time_of_release(-1)
            {}
    
    //JailParameters(bool in_jail = false): in_jail(in_jail) {}
            //bool in_jail;
    
    /**
     * Debugging helper function to print the jail record of a person  
    */ 
    void printJailRecord(double current_time) {
        //std::cout << "JailParameters.h:: printJailRecord is called " << std::endl;
        std::cout << "-----------Jail Record (@time: " <<  current_time << ")------------" << std::endl;
        std::cout << "accumulative_injail_count= " <<  accumulative_injail_count << std::endl;
        std::cout << "accumulative_time_in_jail= " <<   (accumulative_time_in_jail + (current_time - time_of_jail)) << std::endl;
        //std::cout << "age_at_first_jail= " <<  age_at_first_jail<< std::endl;
        std::cout << "isFirstTimeJailed?= " <<  is_first_time_jailed<< std::boolalpha << std::endl;
        std::cout << "isJailed?= " <<  is_in_jail << std::boolalpha << std::endl;
        std::cout << "time_of_jail= " << time_of_jail;
        std::cout << "time_since_jailed= " << (current_time -time_of_jail);
        //std::cout << "age since jailed= " << (age -age_at_jail);
        std::cout << ", age_at_jail= " << age_at_jail << std::endl;
        std::cout << "time_of_release= " << time_of_release << std::endl;
        std::cout << "serving_time= " << serving_time;
        std::cout << ", time_since_jailed= " << (current_time - time_of_jail) << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
};


} /* namespace TransModel */

#endif /* SRC_JAILPARAMETERS_H_ */
