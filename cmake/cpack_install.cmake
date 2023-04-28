# Reset the planet texture directory to the default local Textures directory
# before packing

# get the source directory
list(GET CPACK_BUILD_SOURCE_DIRS 0 src_dir)

# Note: ${CMAKE_INSTALL_PREFIX} doesn't seem to be properly set here
# so the following doesn't currently work
set(inst_dir ${CMAKE_INSTALL_PREFIX}/SpaceXpanse)
set(SPACEXPANSE_PLANET_TEXTURE_INSTALL_DIR_W ".\\Textures")
configure_file(${src_dir}/Src/SpaceXpanse/SpaceXpanse.cfg.in ${inst_dir}/SpaceXpanse.cfg)
configure_file(${src_dir}/Src/SpaceXpanse/SpaceXpanse.cfg.in ${inst_dir}/SpaceXpanse_NG.cfg)
