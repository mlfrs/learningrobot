/*
 * Helper class for various conversion functions.
 *
 * Authors : Cormac Guerin.
 *
 */
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;
 
class BadConversion : public std::runtime_error {
public:
  BadConversion(const std::string& s)
    : std::runtime_error(s)
    { }
};

inline string IntToString(int i)
{
  stringstream ss;
  ss << i;
  const std::string& s = ss.str(); 
  return s;
}

inline int StringToInt(const std::string& s)
{
  std::istringstream i(s);
  int x;
  if (!(i >> x))
    throw BadConversion("StringToInt(\"" + s + "\")");
  return x;
}

inline double StringToDouble(const std::string& s)
{
  std::istringstream i(s);
  double x;
  if (!(i >> x))
    throw BadConversion("StringToDouble(\"" + s + "\")");
  return x;
}

inline float StringToFloat(const std::string& s)
{
  std::istringstream i(s);
  float x;
  if (!(i >> x))
    throw BadConversion("StringToFloat(\"" + s + "\")");
  return x;
}

inline vector<string> SplitString(const string& s, const string& d)
{
    vector<string> temp;
    if ( d.empty() ) {
        temp.push_back( s );
        return temp;
    }
    typedef string::const_iterator iter;
    const iter::difference_type f_size( distance( d.begin(), d.end() ) );
    iter i( s.begin() );
    for ( iter pos; ( pos = search( i , s.end(), d.begin(), d.end() ) ) != s.end(); ) {
        temp.push_back( string( i, pos ) );
        advance( pos, f_size );
        i = pos;
    }
    temp.push_back( string( i, s.end() ) );
    return temp;
}

// this is probably really inefficient
inline vector<int> compare(vector<int> v1, vector<int> v2) {
	vector<int> sortedV1(v1);
	vector<int> sortedV2(v2);

	sort(sortedV1.begin(),sortedV1.end());
	sort(sortedV2.begin(),sortedV2.end());

	vector<int> vDifferences;
	set_symmetric_difference(
		sortedV1.begin(),
		sortedV1.end(),
		sortedV2.begin(),
		sortedV2.end(),
	back_inserter(vDifferences));
	
	return vDifferences;
}

inline int
pow2roundup (int x)
{
	if (x < 0)
	return 0;
	--x;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x-1;
}

inline int
roundDown (int x)
{
	if (x >= 92)
		return 92;
	if (x >= 44)
		return 44;
	if (x >= 20)
		return 20;
	if (x >= 8)
		return 8;
	if (x >= 2)
		return 2;
	if (x >= 1)
		return 1;
}

