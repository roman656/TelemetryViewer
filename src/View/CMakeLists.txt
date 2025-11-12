# Собрать GUI как отдельную библиотеку не вышло
# Для сборки вместе с основным приложением нужно к цели exe-шника добавить вот эти ресурсы
# Чтобы ресурсы превратились в исходники надо у цели exe-шника установить свойство AUTORCC
# Также необходимо слинковать цель exe-шника с QML
set(VIEW_RESOURCES_FILEPATH "${CMAKE_CURRENT_SOURCE_DIR}/View/qml.qrc" PARENT_SCOPE)
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/View/" CACHE STRING "" FORCE)