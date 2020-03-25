constexpr auto PI = 3.1415926535897932384626433832795;

template <typename T>
DefaultType radians(T degrees)
{
	return degrees * DefaultType(PI) / DefaultType(180);
}