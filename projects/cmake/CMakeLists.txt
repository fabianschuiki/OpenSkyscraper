cmake_minimum_required(VERSION 2.8)
project(OpenSkyscraper)

if(NOT CMAKE_BUILD_TYPE) # By default the build type should be Debug.
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING
	"The type of build. Options: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# find ../../source/* -name "*.cpp"
set(source
../../source/base/managedmemory.cpp
../../source/base/object.cpp
../../source/base/autoreleasequeue.cpp
../../source/base/responder.cpp
../../source/classic/tower/environment.cpp
../../source/classic/tower/funds.cpp
../../source/classic/tower/tower.cpp
../../source/classic/tower/route.cpp
../../source/classic/tower/structure.cpp
../../source/classic/tower/background.cpp
../../source/classic/tower/time.cpp
../../source/classic/ui/game.cpp
../../source/classic/ui/control/rating.cpp
../../source/classic/ui/control/watch.cpp
../../source/classic/ui/control/window.cpp
../../source/classic/items/transports/elevators/standardelevator.cpp
../../source/classic/items/transports/elevators/elevatorbackground.cpp
../../source/classic/items/transports/elevators/elevator.cpp
../../source/classic/items/transports/stairslike/escalator.cpp
../../source/classic/items/transports/stairslike/stairslike.cpp
../../source/classic/items/transports/stairslike/stairs.cpp
../../source/classic/items/transports/transport.cpp
../../source/classic/items/facilities/hotel/suite.cpp
../../source/classic/items/facilities/hotel/housekeeping.cpp
../../source/classic/items/facilities/hotel/singleroom.cpp
../../source/classic/items/facilities/hotel/doubleroom.cpp
../../source/classic/items/facilities/hotel/hotel.cpp
../../source/classic/items/facilities/facility.cpp
../../source/classic/items/facilities/occupiable.cpp
../../source/classic/items/facilities/structure/lobby.cpp
../../source/classic/items/facilities/office/office.cpp
../../source/classic/items/item.cpp
../../source/classic/people/person.cpp
../../source/classic/people/individuals/janitor.cpp
../../source/classic/people/individuals/hotelguest.cpp
../../source/classic/people/timedperson.cpp
../../source/classic/people/scheduledperson.cpp
../../source/classic/responder.cpp
../../source/classic/scenes/game.cpp
../../source/core/application.cpp
../../source/core/responder.cpp
../../source/engine/engine.cpp
../../source/engine/application.cpp
../../source/engine/eventpump.cpp
../../source/engine/audio/audio.cpp
../../source/engine/audio/soundeffect.cpp
../../source/engine/resources/sound.cpp
../../source/engine/resources/texture.cpp
../../source/engine/cruisecontrol.cpp
../../source/engine/video/sprite.cpp
../../source/engine/video/interpolatedtexturequad.cpp
../../source/engine/video/texturedquad.cpp
../../source/engine/video/video.cpp
../../source/engine/scene.cpp
../../source/engine/platformspecific.cpp
../../source/engine/responder.cpp
../../source/game/application.cpp
../../source/game/simtower-names.cpp
../../source/game/simtower.cpp
../../source/gui/view.cpp
../../source/main.cpp
../../source/math/clamp.cpp
../../source/math/rand.cpp)

find_package(SDL)
find_package(OpenGL)
find_package(OpenAL)
find_package(DevIL)

add_executable(OpenSkyscraper ${source})
target_link_libraries(OpenSkyscraper ${SDL_LIBRARY} ${OPENGL_LIBRARY} ${OPENAL_LIBRARY} ${IL_LIBRARIES})
