CPMAddPackage(
  NAME EnTT
  URL "https://github.com/skypjack/entt/archive/refs/tags/v3.14.0.tar.gz"
)

add_library(third_party::entt ALIAS EnTT)
