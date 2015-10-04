#include <iostream>
#include <assert.h>
#include "../factory.h"

class Base
{
public:
	using factory = dp14::factory<Base, std::string, int>;

	explicit Base(const std::string& name, int q)
		: _name(name)
		, _q(q)
	{
		std::cout << "constructor " << _name << " - " << _q << std::endl;
	}
	virtual ~Base() { std::cout << "destruction" << std::endl; }

protected:
	std::string _name;
	int _q;
};

class A : public Base
{
public:
	explicit A(const std::string& name, int q) : Base(name, q) { ; }
	virtual ~A() = default;
};
DEFINE_HASH(A)

class B : public Base
{
public:
	explicit B(const std::string& name, int q) : Base(name, q) { ; }
	virtual ~B() = default;
};
DEFINE_HASH(B)

// register implementations to static factory
namespace regA
{
	Base::factory::registrator<A> reg;
}
namespace regB
{
	Base::factory::registrator<B> reg;
}

// factory with singleton is useful for DLL/plugin systems:
// if you want publish your factory in a DLL, only need:
//		template LIBNAME_API Base::factory;

int main()
{
	{
		// equivalent ways of create A
		std::shared_ptr<Base> a1 = Base::factory::instance().create<A>("first parameter", 2);
		std::shared_ptr<A> a2 = Base::factory::instance().create<A>("first parameter", 2);
		std::shared_ptr<Base> a3 = Base::factory::instance().create("A", "first parameter", 2);

		// equivalent ways of create B
		std::shared_ptr<Base> b1 = Base::factory::instance().create<B>("first parameter", 2);
		std::shared_ptr<B> b2 = Base::factory::instance().create<B>("first parameter", 2);
		std::shared_ptr<Base> b3 = Base::factory::instance().create("B", "first parameter", 2);

		assert(a1 != a2);
		assert(a3 != b1);
		assert(b1 != b2);
	}

	return(0);
}
