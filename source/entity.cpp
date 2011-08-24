#include "entity.h"


Entity::Entity()
{
	owner = NULL;
}

Item * Entity::getOwner() const
{
	return owner;
}
