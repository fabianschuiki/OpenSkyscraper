cmake_minimum_required(VERSION 2.8)
project(OpenSkyscraper)

if(NOT CMAKE_BUILD_TYPE) # By default the build type should be Debug.
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING
	"The type of build. Options: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(source
	engine/resources/sound.cpp
	engine/resources/texture.cpp
	engine/events/responder.cpp
	engine/events/basicresponder.cpp
	engine/scene.cpp
	engine/math/rand.cpp
	engine/math/clamp.cpp
	engine/platform/platformspecific.cpp
	engine/video/sprite.cpp
	engine/video/texturedquad.cpp
	engine/video/interpolatedtexturequad.cpp
	engine/video/video.cpp
	engine/subsystems/cruisecontrol.cpp
	engine/engine.cpp
	engine/base/autoreleasequeue.cpp
	engine/base/managedmemory.cpp
	engine/base/object.cpp
	engine/application.cpp
	engine/audio/audio.cpp
	engine/audio/soundeffect.cpp
	game/simtower.cpp
	game/classic/events/responder.cpp
	game/classic/tower/decoration.cpp
	game/classic/tower/environment.cpp
	game/classic/tower/time.cpp
	game/classic/tower/tower.cpp
	game/classic/tower/structure.cpp
	game/classic/tower/funds.cpp
	game/classic/tower/background.cpp
	game/classic/tower/route.cpp
	game/classic/items/transports/stairslike/stairs.cpp
	game/classic/items/transports/stairslike/escalator.cpp
	game/classic/items/transports/stairslike/stairslike.cpp
	game/classic/items/transports/transport.cpp
	game/classic/items/transports/elevators/elevator.cpp
	game/classic/items/transports/elevators/service.cpp
	game/classic/items/transports/elevators/express.cpp
	game/classic/items/transports/elevators/standard.cpp
	game/classic/items/transports/elevators/car.cpp
	game/classic/items/transports/elevators/queue.cpp
	game/classic/items/item.cpp
	game/classic/items/facilities/office/office.cpp
	game/classic/items/facilities/occupiable.cpp
	game/classic/items/facilities/hotel/hotel.cpp
	game/classic/items/facilities/hotel/suite.cpp
	game/classic/items/facilities/hotel/doubleroom.cpp
	game/classic/items/facilities/hotel/singleroom.cpp
	game/classic/items/facilities/hotel/housekeeping.cpp
	game/classic/items/facilities/structure/lobby.cpp
	game/classic/items/facilities/facility.cpp
	game/classic/items/facilities/entertainment/partyhall.cpp
	game/classic/items/facilities/entertainment/restaurant.cpp
	game/classic/items/facilities/entertainment/fastfood.cpp
	game/classic/items/facilities/entertainment/shop.cpp
	game/classic/people/timedperson.cpp
	game/classic/people/scheduledperson.cpp
	game/classic/people/person.cpp
	game/classic/people/individuals/janitor.cpp
	game/classic/people/individuals/hotelguest.cpp
	game/classic/people/individuals/officeworker.cpp
	game/classic/game.cpp
	game/classic/ui/control/rating.cpp
	game/classic/ui/control/window.cpp
	game/classic/ui/control/watch.cpp
	game/classic/ui/tools/construction.cpp
	game/classic/ui/tools/tools.cpp
	game/classic/ui/button.cpp
	game/classic/ui/toolbox/window.cpp
	game/classic/ui/toolbox/groupbutton.cpp
	game/classic/ui/game.cpp
	game/application.cpp
	game/simtower-names.cpp
	game/ui/view.cpp
	game/ui/window.cpp
	main.cpp
)

# Add the PkgConfig package to find packages that CMake doesn't.
find_package(PkgConfig)

# libmspack for decompressing KWAJ files (SIMTOWER.EX_)
# http://www.cabextract.org.uk/libmspack/
pkg_check_modules(MSPACK libmspack)
include_directories(${MSPACK_INCLUDE_DIRS})
link_directories(${MSPACK_LIBRARY_DIRS})

# other packages
find_package(SDL)
find_package(OpenGL)
find_package(OpenAL)
find_package(DevIL)

# setup the executable
add_executable(OpenSkyscraper ${source})
target_link_libraries(OpenSkyscraper
	${SDL_LIBRARY}
	${OPENGL_LIBRARY}
	${OPENAL_LIBRARY}
	${IL_LIBRARIES}
	${MSPACK_LIBRARIES}
)
