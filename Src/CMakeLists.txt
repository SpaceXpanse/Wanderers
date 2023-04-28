# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_subdirectory(SpaceXpanse)
add_subdirectory(SpaceXpansesdk)
add_subdirectory(DlgCtrl)
add_subdirectory(Celbody)
add_subdirectory(Vessel)
add_subdirectory(Module)
add_subdirectory(Plugin)

if(BUILD_SPACEXPANSE_SERVER)
	add_subdirectory(SpaceXpanse_ng)
endif()
