#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include "cell.h"
#include "entity.h"

class Application;

BOOST_AUTO_TEST_SUITE( cell_tests )

BOOST_AUTO_TEST_CASE( add_test )
{
  int c = 32;

  double size = c;
  double halfSize = size * 0.5;
  Cell root(-halfSize, -halfSize, halfSize, halfSize, 1);

  std::set<Entity*> entities;

  for (int ix = 0; ix < c; ix++)
  {
    for (int iy = 0; iy < c; iy++)
    {
      Entity* e = new Entity(nullptr);
      e->SetPosition(Vector2d(-halfSize, -halfSize) + Vector2d(ix, iy));
      e->SetBoundingRadius(10.0);

      entities.insert(e);
      root.Add(e);
    }
  }

  BOOST_CHECK( root.GetEntities().size() == (size * size) );

  BOOST_FOREACH(Entity* entity, entities)
  {
    delete entity;
  }
  /*

  Entity a(nullptr);
  a.SetPosition(Vector2d(-halfSize, -halfSize));

  Entity b(nullptr);
  b.SetPosition(Vector2d(halfSize, -halfSize));
  
  Entity c(nullptr);
  c.SetPosition(Vector2d(-halfSize, halfSize));
  
  Entity d(nullptr);
  d.SetPosition(Vector2d(halfSize, halfSize));
  
  root.Add(&a);
  root.Add(&b);
  root.Add(&c);
  root.Add(&d);

  auto entities = root.GetEntities();
  BOOST_CHECK( entities.size() == 4 );
  */
}

BOOST_AUTO_TEST_CASE( get_intities_in_range_test )
{
  Entity a(nullptr);
  a.SetPosition(Vector2d(0, 0));
  a.SetBoundingRadius(10.0);

  Entity b(nullptr);
  b.SetPosition(Vector2d(0, 5.0));
  b.SetBoundingRadius(5.0);

  Entity c(nullptr);
  c.SetPosition(Vector2d(0, 10.0));
  c.SetBoundingRadius(0.0);

  Cell cell(-1024, -1024, 1024, 1024, 1);
  cell.Add(&a);
  cell.Add(&b);
  cell.Add(&c);

  std::set<Entity*> entitiesInRange;

  entitiesInRange = cell.GetEntitiesInRange(Vector2d(), 0.0);
  BOOST_TEST_MESSAGE( "Entities in range count: " << entitiesInRange.size() );
  BOOST_CHECK( entitiesInRange.size() == 0 );

  entitiesInRange = cell.GetEntitiesInRange(Vector2d(), 1.0);
  BOOST_TEST_MESSAGE( "Entities in range count: " << entitiesInRange.size() );
  BOOST_CHECK( entitiesInRange.size() == 1 );

  entitiesInRange = cell.GetEntitiesInRange(Vector2d(), 5.0);
  BOOST_TEST_MESSAGE( "Entities in range count: " << entitiesInRange.size() );
  BOOST_CHECK( entitiesInRange.size() == 2 );

  entitiesInRange = cell.GetEntitiesInRange(Vector2d(), 10.0);
  BOOST_TEST_MESSAGE( "Entities in range count: " << entitiesInRange.size() );
  BOOST_CHECK( entitiesInRange.size() == 3 );

  entitiesInRange = cell.GetEntitiesInRange(0, 0, 256.0);
  BOOST_TEST_MESSAGE( "Entities in range count: " << entitiesInRange.size() );
  BOOST_CHECK( entitiesInRange.size() == 3 );
}

BOOST_AUTO_TEST_SUITE_END()
