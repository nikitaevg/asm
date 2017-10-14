#include <cstddef>
#include <string>
#include <immintrin.h>
#include <iostream>
#include <assert.h>

template <typename Pred>
std::pair<size_t, size_t> stupid_count(const char *str, size_t size, Pred&& pred, bool was_space = true)
{
    size_t ans = 0;
    size_t i = 0;
    const char *ch = str;
    for (i = 0; i < size && pred(ch); i++) {
        ans += (was_space && *ch != ' ');
        was_space = (*ch == ' ');
        ch++;
    }
    return {i, ans};
}

size_t flush(__m128i &x)
{
    __m128i marr = _mm_setzero_si128();
    marr = _mm_sad_epu8(marr, x);
    const uint64_t *arr = reinterpret_cast<const uint64_t*>(&marr);
    x = _mm_setzero_si128();
    return arr[0] + arr[1];
}

size_t smart_count(const char *str, size_t size)
{
    static const __m128i spaces = _mm_set_epi8(' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    static const __m128i true_mask = _mm_set_epi8(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    __m128i mans = _mm_setzero_si128();
    static constexpr size_t STEP_SIZE = 16;

    auto res = stupid_count(str, size, [&str](const char *ch){return ch == str || (size_t)ch % STEP_SIZE;});
    if (res.first == size)
        return res.second;
    size_t ans = res.second, cnt = 1;
    str += res.first;
    size_t i;
    for (i = res.first; i < size - STEP_SIZE; i += STEP_SIZE) {
        __m128i curr_spaces = _mm_cmpeq_epi8(_mm_load_si128(reinterpret_cast<const __m128i*>(str)), spaces);
        __m128i shifted_spaces = _mm_cmpeq_epi8(_mm_loadu_si128(reinterpret_cast<const __m128i*>(str - 1)), spaces);
        mans = _mm_add_epi8(mans, _mm_and_si128(_mm_andnot_si128(curr_spaces, shifted_spaces), true_mask));
        str += STEP_SIZE;
        if (!(cnt = (cnt + 1) % 256))
            ans += flush(mans);
    }
    ans += flush(mans);
    return ans + stupid_count(str, size - i - 1, [](const char *ch){return true;}, !(i && str[i - 1] != ' ')).second;
}

int main()
{
    constexpr char str0[] = "f       FFFF FFFFFFFFFFFFFFFFFFFFFFF dFFFFFFFFFFFFFFFFFFFFFF      d";
    constexpr char str1[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    assert(smart_count(str0, sizeof(str0)) == stupid_count(str0, sizeof(str0), [](const char *ch){return true;}).second);
    assert(smart_count(str1, sizeof(str1)) == 1);
}
