#include <boost/test/unit_test.hpp>

#include "boundingbox.h"
#include "vector2d.h"

BOOST_AUTO_TEST_SUITE( boundingbox_tests )

BOOST_AUTO_TEST_CASE( left_right_bottom_top_test )
{
  BoundingBox boundingBox(0, 0, 100, 100);

  BOOST_CHECK( boundingBox.GetLeft() == 0 );
  BOOST_CHECK( boundingBox.GetRight() == 100 );
  BOOST_CHECK( boundingBox.GetTop() == 0 );
  BOOST_CHECK( boundingBox.GetBottom() == 100 );
}

BOOST_AUTO_TEST_CASE( contains_test )
{
  BoundingBox boundingBox(0, 0, 100, 100);

  BOOST_CHECK( boundingBox.Contains(0, 0) );
  BOOST_CHECK( boundingBox.Contains(100, 100) );
  BOOST_CHECK( !boundingBox.Contains(-1, -1) );
  BOOST_CHECK( boundingBox.Contains(Vector2d()) );
  BOOST_CHECK( boundingBox.Contains(Vector2d(100, 100)) );
  BOOST_CHECK( boundingBox.Contains(BoundingBox(0, 0, 10, 10)) );

  BOOST_CHECK( !boundingBox.Contains(BoundingBox(-10, 0, 10, 20)) );
  BOOST_CHECK( !boundingBox.Contains(BoundingBox(90, 0, 110, 20)) );
  BOOST_CHECK( !boundingBox.Contains(BoundingBox(0, -10, 20, 10)) );
  BOOST_CHECK( !boundingBox.Contains(BoundingBox(0, 90, 20, 110)) );

  BOOST_CHECK( BoundingBox(-256, -256, 0, 0).Contains(Vector2d(-5.1907, -21.0872)) );
}

BOOST_AUTO_TEST_CASE( center_test )
{
  BOOST_CHECK( BoundingBox(-1024, -1024, 1024, 1024).GetCenter() == Vector2d() );

  BOOST_CHECK( BoundingBox(-1024, -1024, 0, 0).GetCenter() == Vector2d(-512, -512) );
  BOOST_CHECK( BoundingBox(0, -1024, 1024, 0).GetCenter() == Vector2d(512, -512) );
  BOOST_CHECK( BoundingBox(-1024, 0, 0, 1024).GetCenter() == Vector2d(-512, 512) );
  BOOST_CHECK( BoundingBox(0, 0, 1024, 1024).GetCenter() == Vector2d(512, 512) );
}

BOOST_AUTO_TEST_SUITE_END()
