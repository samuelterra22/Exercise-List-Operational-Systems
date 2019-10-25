cmake_minimum_required(VERSION 3.14)
project(Exercise_List_Operational_Systems C)

set(CMAKE_C_STANDARD 99)

add_executable(Exercise_List_Operational_Systems
        List_01/exercise_01/source/main.c
        List_01/exercise_02/source/main.c

        List_02/exercise_01/source/main.c
        List_02/exercise_02/source/main.c
        List_02/exercise_02/source/main.c

        List_03/exercise_01/source/main.c
        List_03/exercise_02/source/main.c
        List_03/exercise_03/source/main.c
        List_03/exercise_04/source/main.c

        List_04/exercise_01/source/main.c
        List_04/exercise_02/source/main.c
        List_04/exercise_03/source/main.c
        List_04/exercise_04/source/main.c

        List_05/exercise_01/source/producer.c
        List_05/exercise_01/source/consumer.c
        List_05/exercise_01/source/shared.c
        List_05/exercise_01/source/shared.h

        List_05/exercise_02/source/main.c
        List_05/exercise_03/source/main.c
        )