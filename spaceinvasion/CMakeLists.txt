project(SpaceInvasion)

add_executable(${PROJECT_NAME} 
    main.cpp 
    openglwindow.cpp 
    bullets.cpp 
    ship.cpp     
    enemy_ship.cpp 
    util.cpp 
    base_ship.cpp
)

enable_abcg(${PROJECT_NAME})