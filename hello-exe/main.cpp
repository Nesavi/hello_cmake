#include <iostream>
#include <vector>
#include <boost-hello/boost-hello.hpp>
#include <boost-hello/numerics-hello.hpp>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/integrate/detail/integrate_const.hpp>
// #include <boost/numeric/odeint/integrate/integrate.hpp>


namespace euler_test{
typedef std::vector< double > state_type;

const double gam = 0.15;

/* The rhs of x' = f(x) */
void harmonic_oscillator( const state_type &x , state_type &dxdt , const double /* t */ )
{
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - gam*x[1];
}

struct push_back_state_and_time
{
    std::vector< state_type >& m_states;
    std::vector< double >& m_times;
    int counter = 0;

    push_back_state_and_time( std::vector< state_type > &states , std::vector< double > &times )
    : m_states( states ) , m_times( times ), counter{42} { }

    void operator()( const state_type &x , double t )
    {
        counter++;
        m_states.push_back( x );
        m_times.push_back( t );
    }
};
}

int main() {
    // using namespace boost_ser;

    // output_bus_info();
    {
        euler_test::state_type x(2);
        x[0] = 1.0; // start at x=1.0, p=0.0
        x[1] = 0.0;
        std::vector<euler_test::state_type> x_vec;
        std::vector<double> times;

        euler_test::push_back_state_and_time state_and_time( x_vec , times );

        // size_t steps = boost::numeric::odeint::integrate<decltype(harmonic_oscillator), state_type,
        // double, push_back_state_and_time&>
        //  ( harmonic_oscillator ,
        //     x , 0.0 , 10.0 , 0.1, std::ref(state_and_time));

        size_t steps = boost::numeric::odeint::integrate
        ( euler_test::harmonic_oscillator ,
            x , 0.0 , 10.0 , 0.1, [&]( const euler_test::state_type &x , double t ) {state_and_time(x, t);});

        /* output */
        for( size_t i=0; i<=steps; i++ )
        {
            std::cout << times[i] << '\t' << state_and_time.m_states[i][0] << '\t' << state_and_time.m_states[i][1] << '\n';
        }

        std::cout << "Counter " << state_and_time.counter << std::endl;
    }

    {
        std::cout << "Stochastic Euler" << std::endl << std::endl;
        boost::mt19937 rng;
        double dt = 0.1;
        boost_num::state_type x_stoch = {{ 1.0 }};
        boost::numeric::odeint::integrate_const( boost_num::stochastic_euler<boost_num::N >() , std::make_pair( boost_num::ornstein_det() , boost_num::ornstein_stoch( rng , 1.0 ) ),
                x_stoch , 0.0 , 10.0 , dt , boost_num::streaming_observer() );
    }

    return 0;
}