project(SpaceInvasion)

add_executable(${PROJECT_NAME} 
    main.cpp 
    openglwindow.cpp 
    bullets.cpp 
    playership.cpp     
    alienship.cpp 
    randomizer.cpp
    starlayers.cpp
)

enable_abcg(${PROJECT_NAME})