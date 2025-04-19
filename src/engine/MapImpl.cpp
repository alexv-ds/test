#include "MapImpl.hpp"
#include <format>
#include <loose_quadtree/loose_quadtree.hpp>
#include <stdexcept>

namespace engine {
  // //////////////////////////// //
  // ///////// QUADTREE ///////// //
  // //////////////////////////// //

  using TreeBBox = loose_quadtree::bounding_box<float>;

  struct BBoxExtractor {
    static void ExtractBoundingBox(const TreeObject* obj, TreeBBox* bbox) {
      bbox->left = obj->position.x - obj->bbox.width * 0.5f;
      bbox->top = obj->position.y - obj->bbox.height * 0.5f;
      bbox->width = obj->bbox.width;
      bbox->height = obj->bbox.height;
    }
  };

  using QuadTree = loose_quadtree::quad_tree<float, TreeObject, BBoxExtractor>;

  // //////////////////////////// //
  // ///////// INSTANCE ///////// //
  // //////////////////////////// //

  class InstanceImpl final : public Instance, public noncopyable {
  public:
    explicit InstanceImpl(std::string_view name, std::uint32_t id) :
        Instance(id), instance_name(name.begin(), name.end()) {}
    std::string_view name() const override { return instance_name; }

  private:

    const std::string instance_name;
  };

  // //////////////////////////// //
  // //////////// MAP /////////// //
  // //////////////////////////// //

  MapImpl::MapImpl() {
    // first (idx 0) instance always nullptr
    this->instances.push_back(nullptr);
  }

  MapImpl::~MapImpl() = default;

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
    return try_get_impl_instance(id);
  }

  InstanceImpl* MapImpl::try_get_impl_instance(const std::uint32_t id) const noexcept {
    return id < this->instances.size() ? this->instances[id].get() : nullptr;
  }
} // namespace engine
