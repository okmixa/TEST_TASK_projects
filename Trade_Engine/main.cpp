#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

struct Order {
    char traderIdentifier[2];
    char side;
    uint32_t quantity;
    uint32_t price;

    friend istream& operator>>(istream& input, Order& order)
    {
            input >> order.traderIdentifier >> order.side >> order.quantity >> order.price;
            return input;
    }
};

void saveOrder(list<Order>& sell, Order&& aggressor)
{
     auto i = sell.begin();
     for( ; i != sell.end(); ++i)
             if(i->price > aggressor.price ^ aggressor.side == 'B')
                 break;

     if(aggressor.quantity)
       sell.insert(i, aggressor);
}

void processAgressor(list<Order>& sell, Order& aggressor)
{
list<Order> tradeResult;
    for(auto& i: sell)
            if((i.price < aggressor.price ^ aggressor.side == 'S') || i.price == aggressor.price)
             {
                uint32_t count = static_cast<uint32_t>(min(aggressor.quantity, i.quantity));
                aggressor.quantity -= count; i.quantity -= count;

                uint32_t samePrice = (aggressor.side == 'B') ? min(i.price, aggressor.price) : max (i.price, aggressor.price);
                auto cumulativeOrder = find_if(tradeResult.begin(), tradeResult.end(), [&i, samePrice] (const Order& order) { return order.traderIdentifier[1] == i.traderIdentifier[1] && order.price == samePrice; } );
                if( cumulativeOrder != tradeResult.end())
                    cumulativeOrder->quantity += count;
                else
                    tradeResult.emplace_front( Order{ {'T', i.traderIdentifier[1]}, i.side, count, samePrice });
                cumulativeOrder = find_if(tradeResult.begin(), tradeResult.end(), [&aggressor, samePrice] (const Order& order) { return order.traderIdentifier[1] == aggressor.traderIdentifier[1] && order.price == samePrice; } );
                if( cumulativeOrder != tradeResult.end())
                     cumulativeOrder->quantity += count;
                else
                  tradeResult.emplace_back(Order{ {'T', aggressor.traderIdentifier[1]}, aggressor.side, count, samePrice });

                if (aggressor.quantity == 0)
                    break;
             }

    if(!tradeResult.empty())
    {
      cout << endl;
       for ( auto& i : tradeResult)
           cout << 'T' << i.traderIdentifier[1] << (i.side == 'S' ? '-' : '+') << i.quantity << "@" << i.price  << " ";
      cout << endl;
    }

     sell.remove_if([](Order& x){ return !x.quantity; });
}


int main()
{
    list<Order> sell, buy;

    Order agressor;
       while (std::cin >> agressor)
       {
           if  (agressor.side == 'B')
              processAgressor(sell,agressor);
           else if(agressor.side == 'S')
                processAgressor(buy,agressor);

            if(agressor.quantity)
                if  (agressor.side == 'B')
                  saveOrder(buy, std::move(agressor));
                else if(agressor.side == 'S')
                  saveOrder(sell, std::move(agressor));
       }

    return 0;
}
