# - Try to find Eina
# Once done this will define
#  EINA_FOUND - System has Eina
#  EINA_INCLUDE_DIRS - The Eina include directories
#  EINA_LIBRARIES - The libraries needed to use Eina
#  EINA_DEFINITIONS - Compiler switches required for using Eina

find_package(PkgConfig)
pkg_check_modules(EINA QUIET eina)
