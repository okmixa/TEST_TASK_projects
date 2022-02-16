An efficient solution is while traversing the array, store sum so far in currsum. Also, maintain the count of different values of currsum in a map. If the value of currsum is equal to the desired sum at any instance increment count of subarrays by one. The value of currsum exceeds the desired sum by currsum – sum. If this value is removed from currsum then the desired sum can be obtained. From the map find the number of subarrays previously found having sum equal to currsum-sum. Excluding all those subarrays from the current subarray, gives new subarrays having the desired sum. So increase count by the number of such subarrays. Note that when currsum is equal to the desired sum then also check the number of subarrays previously having a sum equal to 0. Excluding those subarrays from the current subarray gives new subarrays having the desired sum. Increase count by the number of subarrays having sum 0 in that case.