#pragma once

namespace helper
{
	// overload pattern type definition 
	template <typename ... Ts>
	struct overload : Ts ...	// Ts ... derives a sequence of base-classes
	{
		// this brings the call operator of all the bases classes into scope
		// each base class has got to provide a call operator
		using Ts::operator() ...;
	};

	// Apparently this part is not required in C++20 in MSVC
	// defining a class deduction guide
	// When we construct an overload(...) the type of arguments ...
	// would be used to determine the template arguments of Overload<>
	template <typename... Ts> overload(Ts...) -> overload<Ts ...>;
}
