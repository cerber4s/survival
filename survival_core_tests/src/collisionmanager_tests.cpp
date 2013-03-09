#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include "collisionmanager.h"
#include "common.h"
#include "entity.h"

BOOST_AUTO_TEST_SUITE( collisionmanager_tests )

BOOST_AUTO_TEST_CASE( get_entities_in_range_test )
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

  CollisionManager cm(-1024, -1024, 1024, 1024, 8);
  cm.Add(&a);
  cm.Add(&b);
  cm.Add(&c);

  std::set<Entity*> entitiesInRange;

  entitiesInRange = cm.GetEntitiesInRange(0, 0, 0.0);
  BOOST_CHECK( entitiesInRange.size() == 0 );

  entitiesInRange = cm.GetEntitiesInRange(0, 0, 1.0);
  BOOST_CHECK( entitiesInRange.size() == 1 );

  entitiesInRange = cm.GetEntitiesInRange(0, 0, 5.0);
  BOOST_CHECK( entitiesInRange.size() == 2 );
  
  entitiesInRange = cm.GetEntitiesInRange(0, 0, 10.0);
  BOOST_CHECK( entitiesInRange.size() == 3 );
  
  entitiesInRange = cm.GetEntitiesInRange(0, 0, 256.0);
  BOOST_CHECK( entitiesInRange.size() == 3 );
  }

BOOST_AUTO_TEST_CASE( large_bounds_test )
{
  CollisionManager cm(-262144, -262144, 262144, 262144, 1);
  
  // 2048
  std::set<Entity*> entities;
  int c = 256;//524288 / 2048;
  for (int i = 0; i < c; i++)
  {
    Entity* e = new Entity(nullptr);
    e->SetPosition(Vector2d(-262144 + (i * 2048.0), -262144 + (i * 2048.0)));
    e->SetBoundingRadius(10.0);

    entities.insert(e);

    cm.Add(e);
  }

  int entityCount = cm.GetEntitiesInRange(0, 0, 262144 * 2).size();
  BOOST_TEST_MESSAGE( "Entity count: " << entityCount );
  BOOST_CHECK( entityCount == c, entityCount );

  BOOST_FOREACH(Entity* entity, entities)
  {
    delete entity;
  }
}

BOOST_AUTO_TEST_SUITE_END()
