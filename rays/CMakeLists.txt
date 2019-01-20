cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11) 

project(rays)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    /Users/cemil/g3d/G3D10/external/assimp.lib/include
    /Users/cemil/g3d/G3D10/external/glew.lib/include
    /Users/cemil/g3d/G3D10/external/enet.lib/include
    /Users/cemil/g3d/G3D10/external/glfw.lib/include
    /Users/cemil/g3d/G3D10/external/nfd.lib/include
    /Users/cemil/g3d/G3D10/external/zip.lib/include
    /Users/cemil/g3d/G3D10/external/zlib.lib/include
    /Users/cemil/g3d/G3D10/external/civetweb.lib/include
    /Users/cemil/g3d/G3D10/external/tbb/include
    /Users/cemil/g3d/G3D10/external/fmod/include
    /Users/cemil/g3d/G3D10/external/ffmpeg/include
    /Users/cemil/g3d/G3D10/external/embree/include
    /Users/cemil/g3d/G3D10/external/openvr/include
    /Users/cemil/g3d/G3D10/G3D-base.lib/include
    /Users/cemil/g3d/G3D10/G3D-gfx.lib/include
    /Users/cemil/g3d/G3D10/G3D-app.lib/include 
)

file(GLOB SOURCE_FILES "source/*")
add_executable(rays ${SOURCE_FILES})

set(LIBRARIES_DIR "/Users/cemil/g3d/G3D10/build/lib")
find_library(LIB_AVFILTER   NAMES avfilter   PATHS ${LIBRARIES_DIR})
find_library(LIB_SWRESAMPLE NAMES swresample PATHS ${LIBRARIES_DIR})
find_library(LIB_SWSCALE    NAMES swscale    PATHS ${LIBRARIES_DIR})
find_library(LIB_FMOD       NAMES fmod       PATHS ${LIBRARIES_DIR})
find_library(LIB_CIVETWEB   NAMES civetweb   PATHS ${LIBRARIES_DIR})
find_library(LIB_NFDD       NAMES nfdd       PATHS ${LIBRARIES_DIR})
find_library(LIB_PYTHON36   NAMES python36   PATHS ${LIBRARIES_DIR})
find_library(LIB_TBBMALLOC  NAMES tbbmalloc  PATHS ${LIBRARIES_DIR})
find_library(LIB_EMBREE     NAMES embree     PATHS ${LIBRARIES_DIR})
find_library(LIB_TBB        NAMES tbb        PATHS ${LIBRARIES_DIR})
find_library(LIB_G3DAPPD    NAMES G3D-appd   PATHS ${LIBRARIES_DIR})
find_library(LIB_AVFORMAT   NAMES avformat   PATHS ${LIBRARIES_DIR})
find_library(LIB_AVCODEC    NAMES avcodec    PATHS ${LIBRARIES_DIR})
find_library(LIB_AVUTIL     NAMES avutil     PATHS ${LIBRARIES_DIR})
find_library(LIB_G3DGFXD    NAMES G3D-gfxd   PATHS ${LIBRARIES_DIR})
find_library(LIB_GLFWD      NAMES glfwd      PATHS ${LIBRARIES_DIR})
find_library(LIB_G3DBASED   NAMES G3D-based  PATHS ${LIBRARIES_DIR})
find_library(LIB_ENETD      NAMES enetd      PATHS ${LIBRARIES_DIR})
find_library(LIB_FREEIMAGED NAMES freeimaged PATHS ${LIBRARIES_DIR})
find_library(LIB_ZIP        NAMES zip        PATHS ${LIBRARIES_DIR})
find_library(LIB_Z          NAMES z          PATHS ${LIBRARIES_DIR})
find_library(LIB_GLEWD      NAMES glewd      PATHS ${LIBRARIES_DIR})
find_library(LIB_PTHREAD    NAMES pthread    PATHS ${LIBRARIES_DIR})
find_library(LIB_ASSIMPD    NAMES assimpd    PATHS ${LIBRARIES_DIR})

target_link_libraries(rays
    "-framework AGL"
    "-framework CoreVideo"
    ${LIB_AVFILTER}
    ${LIB_SWRESAMPLE}
    ${LIB_SWSCALE}
    ${LIB_FMOD}
    "-framework IOKit"
    ${LIB_CIVETWEB}
    ${LIB_NFDD}
    ${LIB_PYTHON36}
    ${LIB_TBBMALLOC}
    ${LIB_EMBREE}
    ${LIB_TBB}
    ${LIB_G3DAPPD}
    ${LIB_AVFORMAT}
    ${LIB_AVCODEC}
    ${LIB_AVUTIL}
    ${LIB_G3DGFXD}
    ${LIB_GLFWD}
    ${LIB_G3DBASED}
    ${LIB_ENETD}
    ${LIB_FREEIMAGED}
    ${LIB_ZIP}
    "-framework Cocoa"
    ${LIB_Z}
    ${LIB_GLEWD}
    "-framework OpenGL"
    ${LIB_ZIP}
    ${LIB_PTHREAD}
    ${LIB_ASSIMPD}
)

