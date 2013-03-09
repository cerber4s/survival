#include <boost/test/unit_test.hpp>

#include "vector2d.h"

BOOST_AUTO_TEST_SUITE( vector2d_tests )

BOOST_AUTO_TEST_CASE( empty_ctor_test )
{
  Vector2d a;

  BOOST_REQUIRE_EQUAL(a.x, 0);
  BOOST_REQUIRE_EQUAL(a.y, 0);
}

BOOST_AUTO_TEST_CASE( value_ctor_test )
{
  double x = 10.0;
  double y = 10.0;

  Vector2d a(x, y);

  BOOST_REQUIRE_EQUAL(a.x, x);
  BOOST_REQUIRE_EQUAL(a.y, y);
}

BOOST_AUTO_TEST_CASE( copy_ctor_test )
{
  double x = 10.0;
  double y = 10.0;

  Vector2d a(x, y);
  Vector2d b(a);

  BOOST_REQUIRE_EQUAL(b.x, x);
  BOOST_REQUIRE_EQUAL(b.y, y);
}

BOOST_AUTO_TEST_CASE( subtract_operator_test )
{
  Vector2d a(10, 10);
  Vector2d b(5, 5);

  Vector2d c = a - b;
  
  Vector2d d(a);
  d -= b;

  BOOST_REQUIRE_EQUAL(c.x, 5);
  BOOST_REQUIRE_EQUAL(c.y, 5);

  BOOST_REQUIRE_EQUAL(d.x, 5);
  BOOST_REQUIRE_EQUAL(d.y, 5);
}

BOOST_AUTO_TEST_CASE( add_operator_test )
{
  Vector2d(5, 5);

  Vector2d c = Vector2d(10, 10) + Vector2d(5, 5);
  
  Vector2d d(10, 10);
  d += Vector2d(5, 5);
  
  BOOST_REQUIRE_EQUAL(c.x, 15);
  BOOST_REQUIRE_EQUAL(c.y, 15);

  BOOST_REQUIRE_EQUAL(d.x, 15);
  BOOST_REQUIRE_EQUAL(d.y, 15);
}

BOOST_AUTO_TEST_CASE( multiply_operator_test )
{
  Vector2d p = Vector2d(10, 10) * 5.0;
  
  Vector2d q(10, 10);
  q *= 5.0;
  
  BOOST_REQUIRE_EQUAL(p.x, 50);
  BOOST_REQUIRE_EQUAL(p.y, 50);

  BOOST_REQUIRE_EQUAL(q.x, 50);
  BOOST_REQUIRE_EQUAL(q.y, 50);
}

BOOST_AUTO_TEST_CASE( equal_operator_test )
{
  Vector2d a;
  Vector2d b;
  Vector2d c(1, 1);

  BOOST_CHECK( a == a );
  BOOST_CHECK( a == b );
  BOOST_CHECK( !(a == c) );
  BOOST_CHECK( c == c );
}

BOOST_AUTO_TEST_CASE( not_equal_operator_test )
{
  Vector2d a;
  Vector2d b;
  Vector2d c(1, 1);

  BOOST_CHECK( !(a != a) );
  BOOST_CHECK( !(a != b) );
  BOOST_CHECK( a != c );
  BOOST_CHECK( !(c != c) );
}

BOOST_AUTO_TEST_CASE( is_zero_test )
{
  Vector2d a;
  Vector2d b(0, 0);
  Vector2d c(1, 1);

  BOOST_REQUIRE( a.IsZero() );
  BOOST_REQUIRE( b.IsZero() );
  BOOST_REQUIRE( !c.IsZero() );
}

BOOST_AUTO_TEST_CASE( length_test )
{
  Vector2d a(10, 0);
  Vector2d b(0, -5);

  BOOST_REQUIRE_EQUAL(a.Length(), 10);
  BOOST_REQUIRE_EQUAL(b.Length(), 5);
}

BOOST_AUTO_TEST_CASE( length_sqr_test )
{
  Vector2d a(10, 0);
  Vector2d b(0, -5);

  BOOST_REQUIRE_EQUAL(a.LengthSqr(), 100);
  BOOST_REQUIRE_EQUAL(b.LengthSqr(), 25);
}

BOOST_AUTO_TEST_CASE( normal_test )
{
  Vector2d n(0, -10);
  Vector2d e(10, 0);
  Vector2d s(0, 10);
  Vector2d w(-10, 0);

  BOOST_REQUIRE_EQUAL(n.Normal(), Vector2d(0, -1));
  BOOST_REQUIRE_EQUAL(e.Normal(), Vector2d(1, 0));
  BOOST_REQUIRE_EQUAL(s.Normal(), Vector2d(0, 1));
  BOOST_REQUIRE_EQUAL(w.Normal(), Vector2d(-1, 0));
}

BOOST_AUTO_TEST_CASE( distance_to_test )
{
  Vector2d a(0, 0);
  Vector2d b(0, 10);

  BOOST_REQUIRE_EQUAL(a.DistanceTo(b), 10);
}

BOOST_AUTO_TEST_CASE( distance_to_sqr_test )
{
  Vector2d a(0, 0);
  Vector2d b(0, 10);

  BOOST_REQUIRE_EQUAL(a.DistanceToSqr(b), 100);
}

BOOST_AUTO_TEST_CASE( angle_test )
{
  BOOST_FAIL( "not implemented" );
}

BOOST_AUTO_TEST_CASE( angle_sqr_test )
{
  BOOST_FAIL( "not implemented" );
}

BOOST_AUTO_TEST_CASE( rotate_test )
{
  BOOST_FAIL( "not implemented" );
}

BOOST_AUTO_TEST_CASE( rotate_by_degrees_test )
{
  BOOST_FAIL( "not implemented" );
}

BOOST_AUTO_TEST_SUITE_END()
