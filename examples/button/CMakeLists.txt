# set headers
set(header_path "${QMIDI_SOURCE_DIR}/include")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
#qt_standard_project_setup()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(button
    main.cpp
    mainwindow.h mainwindow.cpp mainwindow.ui
)
add_dependencies(button QMidi)

set_target_properties(button PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(
    button
    PUBLIC
    QMidi
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)
