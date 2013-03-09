#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include <luabind/luabind.hpp>

#include "entitymanager.h"

BOOST_AUTO_TEST_SUITE( entitymanager_tests )

BOOST_AUTO_TEST_CASE( add_test )
{
  int entityCount = 8;
  EntityManager entityManager(nullptr, entityCount);

  Entity* entity;
  for (int i = 0; i < entityCount; i++)
  {
    BOOST_REQUIRE( entityManager.TrySpawn(&entity, luabind::object()) );
  }
  
  BOOST_REQUIRE( !entityManager.TrySpawn(&entity, luabind::object()) );

  entityManager.ActivateSpawnedEntities();

}

BOOST_AUTO_TEST_SUITE_END()
