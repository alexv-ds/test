#include "MapImpl.hpp"
#include <format>
#include <loose_quadtree/loose_quadtree.hpp>
#include <stdexcept>
#include "log.hpp"

namespace engine {
  // //////////////////////////// //
  // ///////// QUADTREE ///////// //
  // //////////////////////////// //

  using TreeBBox = loose_quadtree::bounding_box<float>;

  struct BBoxExtractor {
    static TreeBBox make_bbox(const components::world::Position& position,
                              const components::world::BoundingBox& bbox) {
      const float left = position.x - bbox.width * 0.5f;
      const float top = position.y - bbox.height * 0.5f;
      const float width = bbox.width;
      const float height = bbox.height;
      return {left, top, width, height};
    }

    static void ExtractBoundingBox(const TreeObject* obj, TreeBBox* bbox) {
      *bbox = make_bbox(obj->position, obj->bbox);
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
    QuadTree& tree() { return tree_; }

    void query(std::vector<EntityData>& out, const Position& pos,
               const BoundingBox& bbox) const override {
      auto q = this->tree_.query_intersects_region(BBoxExtractor::make_bbox(pos, bbox));
      while (!q.end_of_query()) {
        TreeObject* obj = q.get_current();
        out.emplace_back(obj->position, obj->bbox, obj->entity);
        q.next();
      }
    }

    void query(std::vector<entt::entity>& out, const Position& pos,
               const BoundingBox& bbox) const override {
      auto q = this->tree_.query_intersects_region(BBoxExtractor::make_bbox(pos, bbox));
      while (!q.end_of_query()) {
        TreeObject* obj = q.get_current();
        out.emplace_back(obj->entity);
        q.next();
      }
    }

  private:
    mutable QuadTree tree_;
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

  bool MapImpl::update_tree_object(TreeObject* obj) {
    if (!obj) {
      throw std::runtime_error("cannot update tree object: obj is nullptr");
    }
    auto* instance = this->try_get_impl_instance(obj->instance.id);
    if (!instance) {
      return false;
    }
    instance->tree().update(obj);
    return true;
  }

  void MapImpl::remove_tree_object(TreeObject* obj) {
    if (!obj) {
      throw std::runtime_error("cannot remove tree object: obj is nullptr");
    }
    auto* instance = this->try_get_impl_instance(obj->instance.id);
    if (!instance) {
      return;
    }
    instance->tree().remove(obj);
  }

  void MapImpl::clean_tree_objects() {
    for (auto& instance : this->instances) {
      if (instance) {
        instance->tree().clear();
      }
    }
  }

  InstanceImpl* MapImpl::try_get_impl_instance(const std::uint32_t id) const noexcept {
    return id < this->instances.size() ? this->instances[id].get() : nullptr;
  }
} // namespace engine
