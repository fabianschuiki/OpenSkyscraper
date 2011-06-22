package rooms

import (
	"github.com/crazy2be/OpenSkyscraper/server/pkg/location"
	"github.com/crazy2be/OpenSkyscraper/server/pkg/person"
)

type Room interface {
	Location() location.2D
	Inhabitants() []person.Person
	AddInhabatent(person.Person) os.Error
}