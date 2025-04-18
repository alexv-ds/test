#include "MapImpl.hpp"
#include <format>
#include <stdexcept>


namespace engine {

  // //////////////////////////// //
  // ///////// INSTANCE ///////// //
  // //////////////////////////// //

  InstanceImpl::InstanceImpl(const std::string_view name, const std::uint32_t id) :
      Instance(id), instance_name(name.begin(), name.end()) {}

  std::string_view InstanceImpl::name() const { return instance_name; }

  // //////////////////////////// //
  // //////////// MAP /////////// //
  // //////////////////////////// //

  MapImpl::MapImpl() {
    // first (idx 0) instance always nullptr
    this->instances.push_back(nullptr);
  }

  const Instance& MapImpl::create_instance(std::string_view name) {
    // find empty cell
    auto instance_it = this->instances.end();
    for (auto it = this->instances.begin() + 1; it != this->instances.end(); ++it) {
      if (*it == nullptr) {
        instance_it = it;
      }
    }
    if (instance_it == this->instances.end()) {
      instances.emplace_back(nullptr);
      instance_it = instances.end() - 1;
    }

    *instance_it =
      std::make_shared<InstanceImpl>(name, std::distance(instances.begin(), instance_it));
    return **instance_it;
  }

  void MapImpl::delete_instance(const std::uint32_t) {
    throw std::runtime_error("MapImpl::delete_instance() is not implemented");
  }

  const Instance& MapImpl::get_instance(std::uint32_t id) const {
    auto* instance = try_get_instance(id);
    if (instance == nullptr) {
      throw std::runtime_error(std::format("instance id {} not exists", id));
    }
    return *instance;
  }

  const Instance* MapImpl::try_get_instance(const std::uint32_t id) const noexcept {
    return id < this->instances.size() ? this->instances[id].get() : nullptr;
  }
} // namespace engine
