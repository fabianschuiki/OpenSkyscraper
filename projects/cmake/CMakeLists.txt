cmake_minimum_required(VERSION 2.8)
project(OpenSkyscraper)

if(NOT CMAKE_BUILD_TYPE) # By default the build type should be Debug.
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING
	"The type of build. Options: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# find ../../source/* -name "*.cpp"
set(source
../../source/engine/resources/sound.cpp
../../source/engine/resources/texture.cpp
../../source/engine/events/responder.cpp
../../source/engine/events/basicresponder.cpp
../../source/engine/scene.cpp
../../source/engine/math/rand.cpp
../../source/engine/math/clamp.cpp
../../source/engine/platform/platformspecific.cpp
../../source/engine/video/sprite.cpp
../../source/engine/video/texturedquad.cpp
../../source/engine/video/interpolatedtexturequad.cpp
../../source/engine/video/video.cpp
../../source/engine/subsystems/cruisecontrol.cpp
../../source/engine/engine.cpp
../../source/engine/base/autoreleasequeue.cpp
../../source/engine/base/managedmemory.cpp
../../source/engine/base/object.cpp
../../source/engine/application.cpp
../../source/engine/audio/audio.cpp
../../source/engine/audio/soundeffect.cpp
../../source/game/simtower.cpp
../../source/game/classic/events/responder.cpp
../../source/game/classic/tower/decoration.cpp
../../source/game/classic/tower/environment.cpp
../../source/game/classic/tower/time.cpp
../../source/game/classic/tower/tower.cpp
../../source/game/classic/tower/structure.cpp
../../source/game/classic/tower/funds.cpp
../../source/game/classic/tower/background.cpp
../../source/game/classic/tower/route.cpp
../../source/game/classic/items/transports/stairslike/stairs.cpp
../../source/game/classic/items/transports/stairslike/escalator.cpp
../../source/game/classic/items/transports/stairslike/stairslike.cpp
../../source/game/classic/items/transports/transport.cpp
../../source/game/classic/items/transports/elevators/elevator.cpp
../../source/game/classic/items/transports/elevators/service.cpp
../../source/game/classic/items/transports/elevators/express.cpp
../../source/game/classic/items/transports/elevators/standard.cpp
../../source/game/classic/items/transports/elevators/car.cpp
../../source/game/classic/items/transports/elevators/queue.cpp
../../source/game/classic/items/item.cpp
#../../source/game/classic/items/facilities/office/office.cpp # Broken
../../source/game/classic/items/facilities/occupiable.cpp
../../source/game/classic/items/facilities/hotel/hotel.cpp
../../source/game/classic/items/facilities/hotel/suite.cpp
../../source/game/classic/items/facilities/hotel/doubleroom.cpp
../../source/game/classic/items/facilities/hotel/singleroom.cpp
../../source/game/classic/items/facilities/hotel/housekeeping.cpp
../../source/game/classic/items/facilities/structure/lobby.cpp
../../source/game/classic/items/facilities/facility.cpp
../../source/game/classic/items/facilities/entertainment/partyhall.cpp
../../source/game/classic/items/facilities/entertainment/restaurant.cpp
../../source/game/classic/items/facilities/entertainment/fastfood.cpp
../../source/game/classic/items/facilities/entertainment/shop.cpp
../../source/game/classic/people/timedperson.cpp
../../source/game/classic/people/scheduledperson.cpp
../../source/game/classic/people/person.cpp
../../source/game/classic/people/individuals/janitor.cpp
../../source/game/classic/people/individuals/hotelguest.cpp
../../source/game/classic/game.cpp
../../source/game/classic/ui/control/rating.cpp
../../source/game/classic/ui/control/window.cpp
../../source/game/classic/ui/control/watch.cpp
../../source/game/classic/ui/tools/construction.cpp
../../source/game/classic/ui/tools/tools.cpp
../../source/game/classic/ui/button.cpp
../../source/game/classic/ui/toolbox/window.cpp
../../source/game/classic/ui/toolbox/groupbutton.cpp
../../source/game/classic/ui/game.cpp
../../source/game/application.cpp
../../source/game/simtower-names.cpp
../../source/game/ui/view.cpp
../../source/game/ui/window.cpp
../../source/main.cpp)

find_package(SDL)
find_package(OpenGL)
find_package(OpenAL)
find_package(DevIL)

add_executable(OpenSkyscraper ${source})
target_link_libraries(OpenSkyscraper ${SDL_LIBRARY} ${OPENGL_LIBRARY}
	${OPENAL_LIBRARY} ${IL_LIBRARIES})
