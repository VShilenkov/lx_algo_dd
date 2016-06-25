template <typename T>
void counting_sort(std::vector<uint8_t>& v)
{
   size_t counters_size = sizeof(uint8_t)*CHAR_BIT; 
   size_t counters[counters_size] = { 0 };

   for (auto& x : v) ++counters[x];

   size_t pos = 0;
   for (size_t i = 0; i < counters_size; ++i)
   {
      for (size_t j = 0; j < counters[i]; ++j)
      {
         v[pos] = i;
         pos++;
      }
   }
}

template <typename T>
uint8_t radix_value(T x, size_t radix)
{
   return static_cast<uint8_t>(x >> (radix * CHAR_BIT));
}

template <typename T>
std::vector<size_t> cumulative_sums(const std::vector<size_t>& freq)
{
   std::vector<size_t> sums(freq.size() + 1);
   copy(freq.begin(), freq.end(), back_inserter(sums));
   for (size_t i = 1; i < sums.size(); ++i)
   {
      sums[i] += sums[i-1];
   }

   return sums;
}


template <typename T>
std::vector<size_t> compute_frequencies(std::vector<T>& v, size_t radix)
{
   size_t counters_size = sizeof(uint8_t)*CHAR_BIT;
   std::vector<size_t> freq(counters_size, 0);
   for(const auto& x: v)
   {
      auto digit = radix_value(x, radix);
      ++freq[digit];
   }

   return freq;
}

template <typename T>
void radix_sort(std::vector<T>& v)
{
   std::vector<T> buff(v.size());

   auto radix_count = sizeof(T);
   for (size_t radix = 0; radix < radix_count; ++radix)
   {
      auto freq = compute_frequencies(v, radix);
      auto sums = cumulative_sums(freq);

      for (auto x : v)
      {
         auto digit = radix_value(x, radix);
         auto index = sums[digit];
         buff[index] = x;
         ++sums[digit];
      }

      v.swap(buff);
   }
}