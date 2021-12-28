#include "boost-hello.hpp"


namespace boost_ser{
std::ostream & operator<<(std::ostream &os, const gps_position &gp)
{
    return os << ' ' << gp.degrees << (unsigned char)186 << gp.minutes << '\'' << gp.seconds << '"';
}

std::ostream & operator<<(std::ostream &os, const bus_stop &bs)
{
    return os << bs.latitude << bs.longitude << ' ' << bs.description();
}

std::ostream & operator<<(std::ostream &os, const bus_route &br)
{
    std::list<bus_stop *>::const_iterator it;
    // note: we're displaying the pointer to permit verification
    // that duplicated pointers are properly restored.
    for(it = br.stops.begin(); it != br.stops.end(); it++){
        os << '\n' << std::hex << "0x" << *it << std::dec << ' ' << **it;
    }
    return os;
}

std::ostream & operator<<(std::ostream &os, const bus_schedule::trip_info &ti)
{
    return os << '\n' << ti.hour << ':' << ti.minute << ' ' << ti.driver << ' ';
}

std::ostream & operator<<(std::ostream &os, const bus_schedule &bs)
{
    std::list<std::pair<bus_schedule::trip_info, bus_route *> >::const_iterator it;
    for(it = bs.schedule.begin(); it != bs.schedule.end(); it++){
        os << it->first << *(it->second);
    }
    return os;
}

void save_schedule(const bus_schedule &s, const char * filename){
    // make an archive
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << s;
}

void restore_schedule(bus_schedule &s, const char * filename)
{
    // open the archive
    std::ifstream ifs(filename);
    boost::archive::text_iarchive ia(ifs);

    // restore the schedule from the archive
    ia >> s;
}

}

int output_bus_info() {
    // create and open a character archive for output
    std::ofstream ofs("filename");

    using namespace boost_ser;

        // make the schedule
    bus_schedule original_schedule;
    // create class instance
    const boost_ser::gps_position g(35, 59, 24.567f);

    std::cout << g << std::endl;

    // save data to archive
    {
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << g;
    	// archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its orginal state
    gps_position newg;
    {
        // create and open an archive for input
        std::ifstream ifs("filename");
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> newg;
        // archive and stream closed when destructors are called
    }

    
    // fill in the data
    // make a few stops
    std::unique_ptr<bus_stop> bs0 {std::make_unique<bus_stop_corner>(
        gps_position(34, 135, 52.560f),
        gps_position(134, 22, 78.30f),
        "24th Street", "10th Avenue"
    )};

    bus_stop *bs1 = new bus_stop_corner(
        gps_position(35, 137, 23.456f),
        gps_position(133, 35, 54.12f),
        "State street", "Cathedral Vista Lane"
    );
    bus_stop *bs2 = new bus_stop_destination(
        gps_position(35, 136, 15.456f),
        gps_position(133, 32, 15.300f),
        "White House"
    );
    bus_stop *bs3 = new bus_stop_destination(
        gps_position(35, 134, 48.789f),
        gps_position(133, 32, 16.230f),
        "Lincoln Memorial"
    );

    std::cout << *bs0 << std::endl;
    std::cout << *bs1 << std::endl;
    std::cout << *bs2 << std::endl;
    std::cout << *bs3 << std::endl;

     // make a  routes
    bus_route route0;
    route0.append(bs0.get());
    route0.append(bs1);
    route0.append(bs2);

     // make aother routes
    bus_route route1;
    route1.append(bs3);
    route1.append(bs2);
    route1.append(bs1);

    std::cout << "displaying route0: " << route0 << std::endl << std::endl;
    std::cout << "displaying route1: " << route1 << std::endl << std::endl;

        // add trips to schedule
    original_schedule.append("ted", 7, 17, &route1);
    original_schedule.append("ted", 9, 38, &route1);
    original_schedule.append("alice", 11, 47, &route1);

    // display the complete schedule
    std::cout << "original schedule";
    std::cout << original_schedule;

    std::string filename(boost::archive::tmpdir());
    filename += "/demofile.txt";

    // save the schedule
    save_schedule(original_schedule, filename.c_str());

    // ... some time later
    // make  a new schedule
    bus_schedule new_schedule;

    restore_schedule(new_schedule, filename.c_str());

    // and display
    std::cout << "\nrestored schedule";
    std::cout << new_schedule;


    delete bs1;
    delete bs2;
    delete bs3;

    return 0;
}