#include <fstream>
#include <iostream>

// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
namespace boost_ser{
class gps_position
{
private:
    friend std::ostream & operator<<(std::ostream &os, const gps_position &gp);
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & degrees;
        ar & minutes;
        ar & seconds;
    }
    int degrees;
    int minutes;
    float seconds;
public:
    gps_position(){};
    gps_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {}
};



std::ostream & operator<<(std::ostream &os, const gps_position &gp)
{
    return os << ' ' << gp.degrees << (unsigned char)186 << gp.minutes << '\'' << gp.seconds << '"';
}

/////////////////////////////////////////////////////////////
// One bus stop
//
// illustrates serialization of serializable members
//

class bus_stop
{
    friend class boost::serialization::access;
    friend std::ostream & operator<<(std::ostream &os, const bus_stop &gp);
    virtual std::string description() const = 0;
    gps_position latitude;
    gps_position longitude;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & latitude;
        ar & longitude;
    }
protected:
    bus_stop(const gps_position & _lat, const gps_position & _long) :
        latitude(_lat), longitude(_long)
    {}
public:
    bus_stop(){}
    virtual ~bus_stop(){}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(bus_stop)

std::ostream & operator<<(std::ostream &os, const bus_stop &bs)
{
    return os << bs.latitude << bs.longitude << ' ' << bs.description();
}

/////////////////////////////////////////////////////////////
// Several kinds of bus stops
//
// illustrates serialization of derived types
//
class bus_stop_corner : public bus_stop
{
    friend class boost::serialization::access;
    std::string street1;
    std::string street2;
    virtual std::string description() const
    {
        return street1 + " and " + street2;
    }
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        // save/load base class information
        ar & boost::serialization::base_object<bus_stop>(*this);
        ar & street1 & street2;
    }

public:
    bus_stop_corner(){}
    bus_stop_corner(const gps_position & _lat, const gps_position & _long,
        const std::string & _s1, const std::string & _s2
    ) :
        bus_stop(_lat, _long), street1(_s1), street2(_s2)
    {
    }
};

class bus_stop_destination : public bus_stop
{
    friend class boost::serialization::access;
    std::string name;
    virtual std::string description() const
    {
        return name;
    }
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<bus_stop>(*this) & name;
    }
public:

    bus_stop_destination(){}
    bus_stop_destination(
        const gps_position & _lat, const gps_position & _long, const std::string & _name
    ) :
        bus_stop(_lat, _long), name(_name)
    {
    }
};

/////////////////////////////////////////////////////////////
// a bus route is a collection of bus stops
//
// illustrates serialization of STL collection templates.
//
// illustrates serialzation of polymorphic pointer (bus stop *);
//
// illustrates storage and recovery of shared pointers is correct
// and efficient.  That is objects pointed to by more than one
// pointer are stored only once.  In such cases only one such
// object is restored and pointers are restored to point to it
//
class bus_route
{
    friend class boost::serialization::access;
    friend std::ostream & operator<<(std::ostream &os, const bus_route &br);
    typedef bus_stop * bus_stop_pointer;
    std::list<bus_stop_pointer> stops;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        // in this program, these classes are never serialized directly but rather
        // through a pointer to the base class bus_stop. So we need a way to be
        // sure that the archive contains information about these derived classes.
        //ar.template register_type<bus_stop_corner>();
        ar.register_type(static_cast<bus_stop_corner *>(NULL));
        //ar.template register_type<bus_stop_destination>();
        ar.register_type(static_cast<bus_stop_destination *>(NULL));
        // serialization of stl collections is already defined
        // in the header
        ar & stops;
    }
public:
    bus_route(){}
    void append(bus_stop *_bs)
    {
        stops.insert(stops.end(), _bs);
    }
};
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
}

using namespace boost_ser;

int main() {
    // create and open a character archive for output
    std::ofstream ofs("filename");

    using namespace boost_ser;
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


    delete bs1;
    delete bs2;
    delete bs3;

    return 0;
}