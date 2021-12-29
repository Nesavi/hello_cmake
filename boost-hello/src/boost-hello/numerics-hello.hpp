#ifndef numerics_hello_h
#define numerics_hello_h


#include <boost/array.hpp>
#include <boost/random.hpp>

#include <boost/numeric/odeint.hpp>

namespace boost_num{

template< size_t N > class stochastic_euler
{
public:

    typedef boost::array< double , N > state_type;
    typedef boost::array< double , N > deriv_type;
    typedef double value_type;
    typedef double time_type;
    typedef unsigned short order_type;
    typedef boost::numeric::odeint::stepper_tag stepper_category;

    static order_type order( void ) { return 1; }

    template< class System >
    void do_step( System system , state_type &x , time_type t , time_type dt ) const
    {
        deriv_type det , stoch ;
        system.first( x , det );
        system.second( x , stoch );
        for( size_t i=0 ; i<x.size() ; ++i )
            x[i] += dt * det[i] + sqrt( dt ) * stoch[i];
    }
};


const static size_t N = 1;
typedef boost::array< double , N > state_type;

struct ornstein_det
{
    void operator()( const state_type &x , state_type &dxdt ) const
    {
        dxdt[0] = -x[0];
    }
};

struct ornstein_stoch
{
    boost::mt19937 &m_rng;
    boost::normal_distribution<> m_dist;

    ornstein_stoch( boost::mt19937 &rng , double sigma ) : m_rng( rng ) , m_dist( 0.0 , sigma ) { }

    void operator()( const state_type &x , state_type &dxdt )
    {
        dxdt[0] = m_dist( m_rng );
    }
};

struct streaming_observer
{
    template< class State >
    void operator()( const State &x , double t ) const
    {
        std::cout << t << "\t" << x[0] << "\n";
    }
};
}
#endif