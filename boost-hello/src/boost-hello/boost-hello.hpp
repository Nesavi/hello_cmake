#ifndef boost_hello_h
#define boost_hello_h

#include <cstddef> // NULL
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>

/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
namespace boost_ser{
class gps_position
{
    friend std::ostream & operator<<(std::ostream &os, const gps_position &gp);
    friend class boost::serialization::access;
    int degrees;
    int minutes;
    float seconds;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /* file_version */){
        ar & degrees & minutes & seconds;
    }
public:
    // every serializable class needs a constructor
    gps_position(){};
    gps_position(int _d, int _m, float _s) :
        degrees(_d), minutes(_m), seconds(_s)
    {}
};

std::ostream & operator<<(std::ostream &os, const gps_position &gp);

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

std::ostream & operator<<(std::ostream &os, const bus_stop &bs);

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

std::ostream & operator<<(std::ostream &os, const bus_route &br);

/////////////////////////////////////////////////////////////
// a bus schedule is a collection of routes each with a starting time
//
// Illustrates serialization of STL objects(pair) in a non-intrusive way.
// See definition of operator<< <pair<F, S> >(ar, pair) and others in
// serialization.hpp
//
// illustrates nesting of serializable classes
//
// illustrates use of version number to automatically grandfather older
// versions of the same class.

class bus_schedule
{
private:
    // note: this structure was made public. because the friend declarations
    // didn't seem to work as expected.
    struct trip_info
    {
        friend class bus_schedule;
        template<class Archive>
        void serialize(Archive &ar, const unsigned int file_version)
        {
            // in versions 2 or later
            if(file_version >= 2)
                // read the drivers name
                ar & driver;
            // all versions have the follwing info
            ar & hour & minute;
        }

        // starting time
        int hour;
        int minute;
        // only after system shipped was the driver's name added to the class
        std::string driver;

        trip_info(){}
        trip_info(int _h, int _m, const std::string &_d) :
            hour(_h), minute(_m), driver(_d)
        {}
    };
private:
    friend class boost::serialization::access;
    friend std::ostream & operator<<(std::ostream &os, const bus_schedule &bs);
    friend std::ostream & operator<<(std::ostream &os, const bus_schedule::trip_info &ti);
    std::list<std::pair<trip_info, bus_route *> > schedule;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & schedule;
    }
public:
    void append(const std::string &_d, int _h, int _m, bus_route *_br)
    {
        schedule.insert(schedule.end(), std::make_pair(trip_info(_h, _m, _d), _br));
    }
    bus_schedule(){}
};

}

using namespace boost_ser;

BOOST_CLASS_VERSION(bus_schedule::trip_info, 2)

namespace boost_ser{

std::ostream & operator<<(std::ostream &os, const bus_schedule::trip_info &ti);

std::ostream & operator<<(std::ostream &os, const bus_schedule &bs);

void save_schedule(const bus_schedule &s, const char * filename);

void restore_schedule(bus_schedule &s, const char * filename);
}

int output_bus_info();

#endif