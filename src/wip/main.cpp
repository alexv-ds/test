#include <entt/entt.hpp>
#include <entt/locator/locator.hpp>
#include <print>


class ServiceDescription
{
public:
private:
  std::string name;
};

class ServiceRegistry
{
public:

private:

};

namespace omega
{
  class Gigga
  {
    int keks;
  };

  class Gogga
  {
    int keks;
  };
} // namespace omega
int main()
{
  using namespace omega;
  std::println("{}", entt::type_id<Gigga>().name());
  std::println("{}", entt::type_id<Gogga>().name());
}
