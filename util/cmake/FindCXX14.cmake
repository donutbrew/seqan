if(__FIND_CXX14_CMAKE__)
  return()
endif()
set(__FIND_CXX14_CMAKE__ TRUE)

# # Visual Studio 2008 (vs9) doesn't seem to support C++11 directly (only as TR1)
# if (MSVC AND MSVC_VERSION GREATER 1500)
#   set(CXX14_FOUND 1)
#   return ()
# endif (MSVC AND MSVC_VERSION GREATER 1500)

include(CheckCXXCompilerFlag)
enable_language(CXX)

check_cxx_compiler_flag("-std=c++14" CXX14_FOUND)
if (CXX14_FOUND)
  set (CXX14_CXX_FLAGS "-std=c++14")
else (CXX14_FOUND)
  check_cxx_compiler_flag("-std=c++1y" CXX14_FOUND)
  if (CXX14_FOUND)
    set (CXX14_CXX_FLAGS "-std=c++1y")
  endif (CXX14_FOUND)
endif (CXX14_FOUND)

if (CXX14_FOUND AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set (CXX14_CXX_FLAGS "${CXX14_CXX_FLAGS} -stdlib=libc++")
endif (CXX14_FOUND AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))