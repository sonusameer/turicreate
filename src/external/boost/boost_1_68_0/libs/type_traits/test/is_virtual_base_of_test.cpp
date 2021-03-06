
//  (C) Copyright John Maddock 2009.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <boost/type_traits/is_virtual_base_of.hpp>
#include "test.hpp"
#include "check_integral_constant.hpp"

// for bug report 3317: https://svn.boost.org/trac/boost/ticket/3317
class B
{
public:
   B();
   virtual ~B()throw();
};

class D : public B
{
public:
   D();
   virtual ~D()throw();
};

// for bug report 4453: https://svn.boost.org/trac/boost/ticket/4453
class non_virtual_base
{
public:
   non_virtual_base();
};
class non_virtual_derived : public non_virtual_base
{
public:
   non_virtual_derived();
   virtual int Y();
   virtual int X();
};

//
// These are from https://svn.boost.org/trac/boost/ticket/11309
//
struct bug11309_A { int a; };
struct bug11309_B : public virtual bug11309_A {};
struct bug11309_C : public bug11309_A { virtual ~bug11309_C() {} };

struct bug11323_A { virtual void foo() {} };
struct bug11323_B : public virtual bug11323_A { virtual void foo() {} };
struct bug11323_C : public bug11323_B {};

#ifndef BOOST_NO_CXX11_FINAL

struct bug11323_2A { virtual void foo() = 0; };
struct bug11323_2B : public virtual bug11323_2A { void foo() override {} };
struct bug11323_2C : public bug11323_2B {};


class final_non_virtual_derived final : public non_virtual_base
{
public:
   final_non_virtual_derived();
   virtual int Y();
   virtual int X();
};

class final_virtual_derived final : public virtual non_virtual_base
{
public:
   final_virtual_derived();
   virtual int Y();
   virtual int X();
};

#endif

class protected_virtual_derived : protected virtual non_virtual_base
{
public:
   protected_virtual_derived();
   virtual int Y();
   virtual int X();
};

class private_virtual_derived : private virtual non_virtual_base
{
public:
   private_virtual_derived();
   virtual int Y();
   virtual int X();
};

TT_TEST_BEGIN(is_virtual_base_of)

BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Derived,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Derived,Derived>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,Derived>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,MultiBase>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Derived,MultiBase>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Derived2,MultiBase>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,PrivateBase>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<NonDerived,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,void>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,const void>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<void,Derived>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<const void,Derived>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<int, int>::value), false);  // really it is!!!!!
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<const int, int>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<VB,VD>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<VD,VB>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<test_abc1,test_abc3>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<test_abc3,test_abc1>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,virtual_inherit1>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit1,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,virtual_inherit2>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit2,Base>::value), false);
#ifndef BOOST_BROKEN_IS_BASE_AND_DERIVED
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,virtual_inherit3>::value), true);
#endif
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit3,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<boost::noncopyable,virtual_inherit4>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit4,boost::noncopyable>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<int_convertible,virtual_inherit5>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit5,int_convertible>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,virtual_inherit6>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit6,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<B,D>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<non_virtual_base,non_virtual_derived>::value), false);
//
// Bug cases:
//
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<bug11309_A, bug11309_C>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<bug11309_A, bug11309_B>::value), true);
#if !defined(BOOST_NO_SFINAE_EXPR) && !defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS) && !defined(BOOST_NO_CXX11_NULLPTR) && !BOOST_WORKAROUND(BOOST_GCC, < 40800)
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<bug11323_B, bug11323_C>::value), false);
#ifndef BOOST_NO_CXX11_FINAL
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<bug11323_2B, bug11323_2C>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<non_virtual_base, final_non_virtual_derived>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<non_virtual_base, final_virtual_derived>::value), true);
#endif
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<non_virtual_base, protected_virtual_derived>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<non_virtual_base, private_virtual_derived>::value), true);
#endif

TT_TEST_END
