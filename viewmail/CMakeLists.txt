set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories (BEFORE ${MUSTACHE_SOURCE_DIR})
include_directories (BEFORE ${QT_INCLUDES})
include_directories (BEFORE ${GMIME_INCLUDE_DIRS})
include_directories (BEFORE ${EXTRA_INCLUDE_DIRS})

link_directories (${GMIME_LIBRARY_DIRS} ${QT_LIBRARY_DIRS})

set(src main.cpp mailview.cpp htmlmail.cpp mailnetworkmanager.cpp statichttpreply.cpp)

QT4_WRAP_UI(src mailview.ui)
add_executable (viewmail ${src})

target_link_libraries(viewmail Qt4::QtGui Qt4::QtNetwork Qt4::QtWebKit ${GMIME_LIBRARIES})

install (TARGETS viewmail DESTINATION bin)
