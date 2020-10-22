#include "repast_hpc/RepastProcess.h"
#include "boost/filesystem.hpp"

#include "RInside.h"
#include "Rcpp.h"

#include "network_utils.h"
#include "file_utils.h"
#include "Parameters.h"
#include "PersonToVAL.h"
#include "common.h"
#include "utils.h"

#include "Serializer.h"

namespace fs = boost::filesystem;

namespace TransModel {


std::string get_net_out_filename(const std::string& file_name) {
    long tick = floor(repast::RepastProcess::instance()->getScheduleRunner().currentTick());
    fs::path filepath(file_name);
    std::string stem = filepath.stem().string();

    std::stringstream ss;
    ss << stem << "_" << tick << filepath.extension().string();
    fs::path newName(filepath.parent_path() / ss.str());

    return newName.string();
}

Serializer::Serializer(Network<Person>* net, Jail* jail, std::shared_ptr<RInside> R) : net_{net}, jail_{jail}, R_{R} {}

void Serializer::operator()() {
    Rcpp::List rnet;
    std::map<unsigned int, unsigned int> idx_map;
    PersonToVAL p2val;

    // Put the jailed persons back in the network with their edges
    jail_->addInmatesToNetwork();

    long tick = floor(repast::RepastProcess::instance()->getScheduleRunner().currentTick());
    create_r_network(tick, rnet, *net_, idx_map, p2val, STEADY_NETWORK_TYPE);

    Rcpp::NumericVector schedule_jail_events;
    // This matches the way "create_r_network" works -- iterate through the 
    // vertices such that first vertex gets an id of 0 and so on. So, the 
    // ids pushed into schedule_jail_events correspond with those created when
    // the network is deserialized. Each event is represented by a tuple
    // (id, scheduled for time, evt type).
    unsigned int seralized_id = 0;
    for (auto iter = net_->verticesBegin(); iter != net_->verticesEnd(); ++iter) {
        auto art_evt_iter = jail_->art_evts.find((*iter)->id());
        if (art_evt_iter != jail_->art_evts.end()) {
            schedule_jail_events.push_back(seralized_id);
            schedule_jail_events.push_back(art_evt_iter->second->scheduledFor());
            schedule_jail_events.push_back(0);
        }

        auto prep_evt_iter =  jail_->prep_evts.find((*iter)->id());
        if (prep_evt_iter != jail_->prep_evts.end()) {
            schedule_jail_events.push_back(seralized_id);
            schedule_jail_events.push_back(prep_evt_iter->second->scheduledFor());
            schedule_jail_events.push_back(1);
        }

        ++seralized_id;
    }
    
    //Rcpp::NumericVector v(jail_->jail_inf_calc.rates.begin(), jail_->jail_inf_calc.rates.end());
    List gal = rnet["gal"];
    //gal["external.infection.rates"] = v;
    gal["jail.events"] = schedule_jail_events;
    rnet["gal"] = gal;
    
    std::string file_name = output_directory(Parameters::instance()) + "/"
            + Parameters::instance()->getStringParameter(NET_SAVE_FILE);
    Rcpp::as<Rcpp::Function>((*R_)["nw_save"])(rnet, unique_file_name(get_net_out_filename(file_name)), tick);

    if (Parameters::instance()->contains(CASUAL_NET_SAVE_FILE)) {
        idx_map.clear();
        List cas_net;
        create_r_network(tick, cas_net, *net_, idx_map, p2val, CASUAL_NETWORK_TYPE);
        file_name = output_directory(Parameters::instance()) + "/"
                + Parameters::instance()->getStringParameter(CASUAL_NET_SAVE_FILE);
        Rcpp::as<Rcpp::Function>((*R_)["nw_save"])(cas_net, unique_file_name(get_net_out_filename(file_name)), tick);
    }
    jail_->removeInmatesFromNetwork();

}

}
