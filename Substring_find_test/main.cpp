#include <stdio.h>
#include <string.h>
#include <functional>
#include <vector>
#include <tuple>
#include <algorithm>

inline static const char * to_str( const char * const str )
{
    return (nullptr == str ? "(null)" : str);
}



namespace pyr {


    inline bool str_cmp(const char* str, const char* substr)
    {

        const char * c = str;
        const char* cc = substr;
        while( *cc != '\0' && *c != '\0')
        {
            if (*c != *cc)
               return false;

            ++cc;
            ++c;
         }


             return *cc == '\0';
    }

    const char * strstr( const char * str, const char * substr )
    {
        if ( *str == '\0' && *substr == '\0')
          return str;

        while(*str != '\0')
        {
            if ( str_cmp(str, substr) )
               return str;
            else
               ++str;
        }

        // Implementation code here


        return nullptr;
    }
}


///   typename * const c;
///   const typename * c;
////  const typename * const c;
////  typename const * const c;
////  const typename c;
////  typename const c;

bool run_test( char const * const str, const char * const substr )
{
    const char * const pyr_ptr = pyr::strstr( str, substr );
    const char * const ptr = ::strstr( str, substr );
    const bool status = (ptr == pyr_ptr);
    printf( "%s: '%s' <--> '%s' : pyr: '%s', std: '%s'\n",
           (status ? "PASSED" : "FAILED"), str, substr, to_str( pyr_ptr ), to_str( ptr ) );
    return status;
}

using TestArgs = std::tuple<const char *, const char *>;
using Tests = std::vector<TestArgs>;
using TestsResults = std::vector<bool>;

bool run_test_suit( const Tests& tests )
{
    TestsResults results( tests.size() );
    std::transform(tests.cbegin(), tests.cend(), results.begin(), []( const TestArgs& args ) {
        const auto &[str, substr] = args;
        return run_test( str, substr );
    });

    const bool status = (results.cend() == std::find( results.cbegin(), results.cend(), false ));
    printf( "\n***** %s *****\n\n", (status ? "ALL PASSED" : "SOME FAILED") );
    return status;
}

int main( int argc, const char * argv[] ) {
    const char * const str = "Good morning!";

    Tests tests = {
        {str, "Good"},
        {str, "Good"},
        {str, "G"},
        {str, "!"},
        {str, "ing!"},
        {str, " mor"},
        {str, "unknown"},
        {"eq", "equal"},
        {"Substring eq", "equal"},
        {"dup dup", "dup"},
        {"small", "bigger"},
        {"", ""},
        {"small", ""},
        {"", "foo"},
        {"asdfasdd", "asdd"},
        {"GooGooGood morning", "GooGood"}
    };

    return (run_test_suit( tests ) ? 0 : 1);
}
