add_custom_target(
  astern_data ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${astern_SOURCE_DIR}/data/daten
          ${astern_BINARY_DIR}/daten
)

add_custom_target(
  astern_font ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${astern_SOURCE_DIR}/data/font
          ${astern_BINARY_DIR}/font
)
