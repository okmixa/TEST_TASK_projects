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
        return input >> order.traderIdentifier >> order.side >> order.quantity >> order.price;
    }
};

void saveOrder(list<Order>& resting, Order&& aggressor)
{
     auto i = resting.begin();
     for( ; i != resting.end(); ++i)
             if(i->price > aggressor.price ^ aggressor.side == 'B')
                 break;

     if(aggressor.quantity)
       resting.insert(i, aggressor);
}

void processAgressor(list<Order>& sell, list<Order>& buy, Order& aggressor)
{
    list<Order> tradeResult;

    for(auto& i: aggressor.side == 'B' ? sell : buy)
            if((i.price < aggressor.price ^ aggressor.side == 'S') || i.price == aggressor.price)
             {
                uint32_t count = static_cast<uint32_t>(min(aggressor.quantity, i.quantity));
                aggressor.quantity -= count;
                i.quantity -= count;

                uint32_t bestPrice = (aggressor.side == 'B') ? min(i.price, aggressor.price) : max (i.price, aggressor.price);
                auto cumulativeOrder = find_if(tradeResult.begin(), tradeResult.end(), [&i, bestPrice] (const Order& order) { return order.traderIdentifier[1] == i.traderIdentifier[1] && order.price == bestPrice; } );
                if( cumulativeOrder != tradeResult.end())
                    cumulativeOrder->quantity += count;
                else
                    tradeResult.emplace_front( Order{ {i.traderIdentifier[0], i.traderIdentifier[1]}, i.side, count, bestPrice });
                cumulativeOrder = find_if(tradeResult.begin(), tradeResult.end(), [&aggressor, bestPrice] (const Order& order) { return order.traderIdentifier[1] == aggressor.traderIdentifier[1] && order.price == bestPrice; } );

                if( cumulativeOrder != tradeResult.end())
                     cumulativeOrder->quantity += count;
                else
                  tradeResult.emplace_back(Order{ {aggressor.traderIdentifier[0], aggressor.traderIdentifier[1]}, aggressor.side, count, bestPrice });

                if (aggressor.quantity == 0)
                    break;
             }

    if(!tradeResult.empty())
    {
      cout << endl;
      for (auto& i : tradeResult)
          cout << i.traderIdentifier[0] << i.traderIdentifier[1] << (i.side == 'S' ? '-' : '+') << i.quantity << "@" << i.price  << " ";
      cout << endl;
    }

    (aggressor.side == 'B' ? sell : buy).remove_if([](Order& emptyOrder){ return !emptyOrder.quantity; });

    if(aggressor.quantity)
       saveOrder(aggressor.side == 'S' ? sell : buy, move(aggressor));
}


int main()
{
    list<Order> sell, buy;
    Order aggressor;

    while (cin >> aggressor)
        processAgressor(sell, buy, aggressor);
}
