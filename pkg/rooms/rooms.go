package rooms

import (
	"github.com/crazy2be/OpenSkyScraper/pkg/location"
	"github.com/crazy2be/OpenSkyScraper/pkg/people"
)

type Room interface {
	Location() location.2D
	Inhabitants() []people.Person
	AddInhabatent(person.Person) os.Error
}