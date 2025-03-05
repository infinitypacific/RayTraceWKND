# CMakeList.txt : CMake project for RayTraceWKND, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("RayTraceWKND")

# Add source to this project's executable.
add_executable (RayTraceWKND "vec3.h" "vec3.cpp" "ray.h" "ray.cpp" "RayTraceWKND.cpp" "RayTraceWKND.h" "datatool.cpp" "datatool.h" "frostimg.cpp" "frostimg.h" "solid.h" "sphere.h" "plane.h" "plane.cpp" "sphere.cpp" "solidlist.h" "solidlist.cpp" "camera.h" "randengine.cpp" "material.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RayTraceWKND PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
