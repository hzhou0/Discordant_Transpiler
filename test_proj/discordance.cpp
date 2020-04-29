// discordance.cpp
//

#include "discordance.h"
#include "__Discordance.h"
using namespace discordance;
using discordance::deque; using discordance::vector; using discordance::var;
#define LZZ_INLINE inline
using namespace std;
discordance::var return_any (discordance::var any)
                                                   {
  return any;
}
int main ()
          {
    class apple{
        string color="green";
        discordance::var  weight="2kg";
        apple(){
            weight=2;
            weight=2.0+weight;
            cout<<weight;
        }
    };
    discordance::deque<discordance::vector<int >> numbers={
    {1,2,3,4,5,6,7,8,9,10},
    {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10}
    };
    for ( auto row : numbers ){
        for ( auto column : row ){
            cout<<column<<endl;
        }
    }
    discordance::deque<discordance::var  > bucket={2.0, "chocolate milk", "15789"};
    for ( auto item : bucket ){
        cout<<item<<endl;
    }
    auto small_bucket=bucket.slice(1,2);
    for ( auto item : small_bucket ){
        cout<<item<<endl;
    }
    cout<<return_any(2.0)<<endl;
    cout<<return_any(5)<<endl;
    cout<<return_any("hello")<<endl;
    return 0;
}
#undef LZZ_INLINE
