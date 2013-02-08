#include <boost/test/unit_test.hpp>

#include "cell.h"
#include "entity.h"

class Application;

BOOST_AUTO_TEST_SUITE( cell_tests )

BOOST_AUTO_TEST_CASE( empty_test )
{
  Application* application = nullptr;

  Entity a(nullptr);
  Entity b(application);
  Entity c(application);
  Entity d(application);
  Entity e(application);

  a.SetPosition(Vector2d(25, 25));
  b.SetPosition(Vector2d(75, 25));
  c.SetPosition(Vector2d(25, 75));
  d.SetPosition(Vector2d(75, 75));
  e.SetPosition(Vector2d(80, 80));
  
  Cell root(4, Vector2d(0, 0), Vector2d(100, 100));

  root.Add(&a);
  root.Add(&b);
  root.Add(&c);
  root.Add(&d);
  root.Add(&e);
}

BOOST_AUTO_TEST_SUITE_END()
